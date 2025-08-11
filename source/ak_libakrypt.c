/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2020, 2025 by Axel Kenzo, axelkenzo@mail.ru                               */
/*                                                                                                 */
/*  Файл ak_libakrypt.с                                                                            */
/*  - содержит реализацию функций инициализации и тестирования библиотеки.                         */
/* ----------------------------------------------------------------------------------------------- */
 #include <libakrypt.h>

/* ----------------------------------------------------------------------------------------------- */
 const char *ak_libakrypt_version( void )
{
#ifdef AK_VERSION
  return AK_VERSION;
#else
  return "0.10";
#endif
}

/* ----------------------------------------------------------------------------------------------- */
/*! @brief Функция проверяет корректность определения базовых типов данных
    @return В случе успешного тестирования возвращает @ref ak_true (истина).
    В противном случае возвращается ak_false.                                                      */
/* ----------------------------------------------------------------------------------------------- */
 static bool_t ak_libakrypt_test_types( void )
{
  union {
    ak_uint8 x[4];
    ak_uint32 z;
  } val;

  if( sizeof( ak_int8 ) != 1 ) {
    ak_error_message( ak_error_undefined_value, __func__ , "wrong size of ak_int8 type" );
    return ak_false;
  }
  if( sizeof( ak_uint8 ) != 1 ) {
    ak_error_message( ak_error_undefined_value, __func__ , "wrong size of ak_uint8 type" );
    return ak_false;
  }
  if( sizeof( ak_int32 ) != 4 ) {
    ak_error_message( ak_error_undefined_value, __func__ , "wrong size of ak_int32 type" );
    return ak_false;
  }
  if( sizeof( ak_uint32 ) != 4 ) {
    ak_error_message( ak_error_undefined_value, __func__ , "wrong size of ak_uint32 type" );
    return ak_false;
  }
  if( sizeof( ak_int64 ) != 8 ) {
    ak_error_message( ak_error_undefined_value, __func__ , "wrong size of ak_int64 type" );
    return ak_false;
  }
  if( sizeof( ak_uint64 ) != 8 ) {
    ak_error_message( ak_error_undefined_value, __func__ , "wrong size of ak_uint64 type" );
    return ak_false;
  }

  if( ak_log_get_level() > ak_log_standard )
     ak_error_message_int( ak_error_ok, __func__, "size of pointer is", sizeof( ak_pointer ));

 /* определяем тип платформы: little-endian или big-endian */
  val.x[0] = 0; val.x[1] = 1; val.x[2] = 2; val.x[3] = 3;
 #ifdef AK_LITTLE_ENDIAN
     if( val.z != 50462976 ) {
         ak_error_message( ak_error_wrong_endian, __func__, "incorrect endiannnes - "
                             "library runs on big endian, but compiled for little endian platform");
         return ak_false;
     } else
         if( ak_log_get_level() > ak_log_standard ) {
           ak_error_message( ak_error_ok, __func__ , "library runs on little endian platform" );
       }
 #else
  #if AK_BIG_ENDIAN
     if( val.z != 66051 ) {
         ak_error_message( ak_error_wrong_endian, __func__, "incorrect endianness - "
                             "library runs on little endian, but compiled for big endian platform");
         return ak_false;
     } else
         if( ak_log_get_level() > ak_log_standard ) {
             ak_error_message( ak_error_ok, __func__ , "library runs on big endian platform" );
         }
  #else
     return ak_error_message( ak_error_undefined_value, __func__, "library runs "
                               "with unsupported value of endian macros, check libakrypt-config.h");
  #endif
 #endif

  #ifdef AK_HAVE_BUILTIN_MULQ_GCC
   if( ak_log_get_level() > ak_log_standard ) {
     ak_error_message( ak_error_ok, __func__ , "library applies assembler code for mulq command" );
   }
  #endif
  #ifdef AK_HAVE_BUILTIN_CLMULEPI64
   if( ak_log_get_level() > ak_log_standard ) {
     ak_error_message( ak_error_ok, __func__ , "library applies clmulepi64 instruction" );
   }
  #endif
  #ifdef AK_HAVE_BUILTIN_MM256_SLL
   if( ak_log_get_level() > ak_log_standard ) {
     ak_error_message( ak_error_ok, __func__ , "library applies __m256i integer type" );
   }
  #endif

 return ak_true;
}

/* ----------------------------------------------------------------------------------------------- */
 bool_t ak_libakrypt_create( ak_function_log *logger )
{
   int error;

 /* перед стартом все должно быть хорошо */
   ak_error_set_value( ak_error_ok );

 /* инициализируем систему аудита (вывод сообщений) */
   if(( error = ak_log_set_function( logger )) != ak_error_ok ) {
     ak_error_message( error, __func__ , "audit mechanism not started" );
     return ak_false;
   }

 /* выводим версию библиотеки */
   if( ak_log_get_level() > ak_log_standard ) {
      ak_error_message_str( ak_error_ok, __func__, "libakrypt version", ak_libakrypt_version( ));

      #ifdef AK_COMPILER_NAME
       #ifdef AK_COMPILER_VERSION
         ak_error_message( ak_error_ok, __func__, "compiled by %s (version %s)",
                                                           AK_COMPILER_NAME, AK_COMPILER_VERSION );
       #else
         ak_error_message_str( ak_error_ok, __func__, "compiled by", AK_COMPILER_NAME );
       #endif
      #endif
   }

 /* проверяем длины фиксированных типов данных */
   if( ak_libakrypt_test_types() != ak_true ) {
     ak_error_message( ak_error_get_value(), __func__ ,
                                "processor architecture or data types are not defined correctly" );
     return ak_false;
   }

 /* выводим значения установленных параметров библиотеки */
   if( ak_log_get_level() > ak_log_standard ) ak_libakrypt_log_options();

 if( ak_log_get_level() > ak_log_none )
   ak_error_message( ak_error_ok, __func__ , "creation of libakrypt is Ok" );

 return ak_true;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_libakrypt_destroy( void )
{
  int error = ak_error_get_value();
  if( error != ak_error_ok )
    ak_error_message( error, __func__ , "before destroing library holds an error(s)" );

  if( ak_log_get_level() != ak_log_none )
    ak_error_message( ak_error_ok, __func__ , "all crypto mechanisms successfully destroyed" );

 return error;
}

/* ----------------------------------------------------------------------------------------------- */
/*! @example faq/example-hello-01.c                                                                */
/* ----------------------------------------------------------------------------------------------- */
/*                                                                                 ak_libakrypt.c  */
/* ----------------------------------------------------------------------------------------------- */
