/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2020, 2025 by Axel Kenzo, axelkenzo@mail.ru                               */
/*                                                                                                 */
/*  Файл ak_random.с                                                                               */
/*  - содержит реализацию генераторов псевдо-случайных чисел                                       */
/* ----------------------------------------------------------------------------------------------- */
 #include <libakrypt.h>

/* ----------------------------------------------------------------------------------------------- */
/*! @brief Инициализация генератора псевдо-случайных чисел.
    Функция используется для установки значение полей структуры @ref random в
    значения по-умолчанию. Созданный таким образом генератор не является работоспособным.

    @param generator Указатель на структуру struct random
    @return В случае успеха возвращается ak_error_ok (ноль). В случае возникновения ошибки
    возвращается ее код.                                                                           */
/* ----------------------------------------------------------------------------------------------- */
 static int ak_random_create( ak_random generator )
{
     if( generator == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                        "use a null pointer to a random generator" );
     memset( generator, 0, sizeof(struct random));

  return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_random_destroy( ak_random generator )
{
     if( generator == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                        "use a null pointer to a random generator" );

   /* Удаляем память, выделенную под внутренний контекст.
      При этом, функция получает указатель на весь контекст generator, что позволяет получить
      доступ ко всем полям до момента освобождения памяти.
      Основной контекст generator удаляться не должен.     */
     if( generator->free != NULL ) generator->free( generator );
     memset( generator, 0, sizeof( struct random ));

  return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_random_randomize( ak_random generator, const ak_pointer in, const ssize_t size )
{
     if( generator == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                          "use a null pointer to random generator" );
     if( in == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                             "use a null pointer to initializator" );
     if( size <= 0 ) return ak_error_message( ak_error_wrong_length, __func__,
                                                                "using a buffer with wrong length" );
     if( generator->randomize_ptr == NULL )
         return ak_error_message( ak_error_undefined_function,
                                   __func__, "randomize() function not defined for this generator" );
  return generator->randomize_ptr( generator, in, size );
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_random_ptr( ak_random generator, const ak_pointer out, const ssize_t size )
{
     if( generator == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                          "use a null pointer to random generator" );
     if( out == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                               "use a null pointer to output data" );
     if( size <= 0 ) return ak_error_message( ak_error_wrong_length, __func__,
                                                                "using a buffer with wrong length" );
     if( generator->random == NULL )
         return ak_error_message( ak_error_undefined_function,
                                      __func__, "random() function not defined for this generator" );

 return generator->random( generator, out, size );
}

/* ----------------------------------------------------------------------------------------------- */
/*! @brief Внутренняя статическая переменная (счетчик вызовов)                                     */
  static ak_uint64 shift_value = 0;

/* ----------------------------------------------------------------------------------------------- */
 ak_uint64 ak_random_value( void )
{
     ak_uint64 pval = 198732, uval = 1001;
     ak_uint64 vtme = 17, clk = 23, value = 1;

   #ifdef _WIN32
     pval = _getpid();
   #else
     #ifdef AK_HAVE_UNISTD_H
         pval = ( ak_uint64 ) getpid();
         uval = ( ak_uint64 ) getuid();
     #endif
   #endif

   #ifdef AK_HAVE_TIME_H
     vtme = ( ak_uint64 ) time( NULL );
     clk = ( ak_uint64 ) clock();
   #endif

     value = ( shift_value += 11 )*125643267795740073ULL + pval;
     value = ( value * 506098983240188723ULL ) + 71331*uval + vtme;

  return value ^ clk;
}

/* ----------------------------------------------------------------------------------------------- */
/*                                реализация класса random_lcg                                     */
/* ----------------------------------------------------------------------------------------------- */
 #define ak_random_lcg_next_macro   rnd->data.val *= 125643267795740073ULL; \
                                    rnd->data.val += 506098983240188723ULL;

 static int ak_random_lcg_next( ak_random rnd )
{
     if( rnd == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                      "use a null pointer to a random generator" );
     ak_random_lcg_next_macro;

  return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
 static int ak_random_lcg_randomize_ptr( ak_random rnd, const ak_pointer ptr, const ssize_t size )
{
     ssize_t idx = 0;
     ak_uint8 *value = ptr;

     if( rnd == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                        "use a null pointer to a random generator" );
     if( ptr == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                            "use a null pointer to initial vector" );
     if( size <= 0 ) return ak_error_message( ak_error_wrong_length, __func__ ,
                                                            "use initial vector with wrong length" );
   /* сначала начальное значение, потом цикл по всем элементам массива */
     rnd->data.val = value[idx];
     do {
         ak_random_lcg_next_macro;
         rnd->data.val += value[idx];
     } while( ++idx < size );

  return rnd->next( rnd );
}

/* ----------------------------------------------------------------------------------------------- */
 static int ak_random_lcg_random( ak_random rnd, const ak_pointer ptr, const ssize_t size )
{
     ssize_t idx = 0;
     ak_uint8 *value = ptr;

     if( rnd == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                        "use a null pointer to a random generator" );
     if( ptr == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                                      "use a null pointer to data" );
     if( size <= 0 ) return ak_error_message( ak_error_wrong_length, __func__ ,
                                                             "use a data vector with wrong length" );
     do {
         value[idx] = (ak_uint8) ( rnd->data.val >> 24 );
         ak_random_lcg_next_macro;
     } while( ++idx < size );

  return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
/*! @details Генератор вырабатывает последовательность внутренних состояний, удовлетворяющую
    линейному сравнению \f$ x_{n+1} \equiv a\cdot x_n + c \pmod{2^{64}}, \f$
    в котором константы a и c удовлетворяют равенствам
    \f$ a = 125643267795740073 \f$ и \f$ b = 506098983240188723. \f$

    Далее, последовательность внутренних состояний преобразуется в последовательность
    байт по следующему правилу
    \f$ \gamma_n = \displaystyle\frac{x_n - \hat x_n}{2^{24}} \pmod{256}, \f$
    где \f$\hat x_n \equiv x_n \pmod{2^{24}}. \f$                                                  */
/* ----------------------------------------------------------------------------------------------- */
 int ak_random_create_lcg( ak_random generator )
{
     int error = ak_error_ok;
     ak_uint64 qword = ak_random_value(); /* вырабатываем случайное число */

     if(( error = ak_random_create( generator )) != ak_error_ok )
             return ak_error_message( error, __func__ , "wrong initialization of random generator" );

   #ifdef AK_HAVE_OID
     generator->oid = ak_oid_find_by_name("lcg");
   #endif
     generator->next = ak_random_lcg_next;
     generator->randomize_ptr = ak_random_lcg_randomize_ptr;
     generator->random = ak_random_lcg_random;

   /* для корректной работы присваиваем какое-то случайное начальное значение */
     ak_random_lcg_randomize_ptr( generator, &qword, sizeof( ak_uint64 ));

 return error;
}

/* ----------------------------------------------------------------------------------------------- */
/*                                реализация класса random_external                                */
/* ----------------------------------------------------------------------------------------------- */
 static int ak_random_external_rng_random( ak_random generator,
                                                          const ak_pointer ptr, const ssize_t size )
{
     if( generator->random_external( ptr, size ) != ak_error_ok )
         return ak_error_external_function;

  return ak_error_ok;
}
/* ----------------------------------------------------------------------------------------------- */
 int ak_random_create_external_rng( ak_random generator,
                                            ak_function_external_rng* function )
{
     int error = ak_error_ok;

     if( function == NULL )
         return ak_error_message( ak_error_undefined_function, __func__,
                                             "null pointer to external random generator function" );

     if(( error = ak_random_create( generator )) != ak_error_ok )
             return ak_error_message( error, __func__ , "wrong initialization of random generator" );

     generator->next = NULL;
     generator->randomize_ptr = NULL;
     generator->random = ak_random_external_rng_random;
     generator->random_external = function;

  return error;
}

/* ----------------------------------------------------------------------------------------------- */
/*                                                                                    ak_random.c  */
/* ----------------------------------------------------------------------------------------------- */
