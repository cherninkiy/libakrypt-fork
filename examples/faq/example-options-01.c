/* --------------------------------------------------------------------------------- */
/*  Пример                                                                           */
/*  example-options-01.с                                                             */
/*                                                                                   */
/*  Пример иллюстрирует вызовы функций, отвечающих за доступ к                       */
/*  криптографическим опциям библиотеки                                              */
/* --------------------------------------------------------------------------------- */
 #include <stdio.h>
 #include <stdlib.h>
 #include <libakrypt.h>

 int main( void )
{
     size_t i = 0;

   /* инициализируем библиотеку */
     ak_libakrypt_create( ak_function_log_stderr );

   /* выводим в консоль список всех доступных опций */
     for( i = 0; i < ak_libakrypt_options_count(); i++ ) {

      /* получаем имя опции по ее индексу */
         const char *name = ak_libakrypt_get_option_name(i);

        /* выводим текущее состояние */
         printf("option: %s\n\told value: %lld\n", name, (long long int)
                                      /* получаем значение опции по ее индексу */
                                          ak_libakrypt_get_option_by_index(i));

        /* изменяем значение на неосознанное значение
               устанавливаем значение опции по ее имени */
         ak_libakrypt_set_option_by_name( name, 33*i + __LINE__ );

        /* выводим измененное значение */                                             
         printf("\tnew value: %lld\n", /* получаем значение опции по ее имени */
                        (long long int) ak_libakrypt_get_option_by_name( name ));
     }

     ak_libakrypt_destroy();
 return EXIT_SUCCESS;
}
