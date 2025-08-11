/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2021, 2025 by Axel Kenzo, axelkenzo@mail.ru                               */
/*                                                                                                 */
/*  Файл ak_options.с                                                                              */
/*  - содержит реализацию функций для работы с опциями библиотеки                                  */
/* ----------------------------------------------------------------------------------------------- */
 #include <libakrypt.h>

/* ----------------------------------------------------------------------------------------------- */
/*! @brief Тип данных для хранения одной опции библиотеки */
 typedef struct option {
  /*! @brief Человекочитаемое имя опции, используется для поиска и установки значения */
   char *name;
  /*! @brief Численное значение опции */
   ak_int64 value;
  /*! @brief Минимально возможное значение */
   ak_int64 min;
  /*! @brief Максимально возможное значение */
   ak_int64 max;
 } *ak_option;

/* ----------------------------------------------------------------------------------------------- */
/*! Константные значения опций (значения по-умолчанию) */
 static struct option options[] = {

     { "pbkdf2_iteration_count",
     #ifdef AK_OPTION_PDKDF2_ITERATION_COUNT
       AK_OPTION_PDKDF2_ITERATION_COUNT
     #else
       2000
     #endif
       , 1000, 65536 },

     { "hmac_key_count_resource",
     #ifdef AK_OPTION_HMAC_KEY_COUNT_RESOURCE
       AK_OPTION_HMAC_KEY_COUNT_RESOURCE
     #else
       1048576
     #endif
       , 1024, 2147483648 },

     { "magma_cipher_resource",
     #ifdef AK_OPTION_MAGMA_CIPHER_RESOURCE
       AK_OPTION_MAGMA_CIPHER_RESOURCE
     #else
       33554432
     #endif
       , 1024, 2147483648 },

     { "kuznechik_cipher_resource",
     #ifdef AK_OPTION_KUZNECHIK_CIPHER_RESOURCE
       AK_OPTION_KUZNECHIK_CIPHER_RESOURCE
     #else
       268435456
     #endif
       , 8196, 2147483648 },

     { "acpkm_message_count",
     #ifdef AK_OPTION_ACPKM_MESSAGE_COUNT
       AK_OPTION_ACPKM_MESSAGE_COUNT
     #else
       4096
     #endif
       , 128, 65536 },

     { "acpkm_section_magma_block_count",
     #ifdef AK_OPTION_ACPKM_SECTION_MAGMA_BLOCK_COUNT
       AK_OPTION_ACPKM_SECTION_MAGMA_BLOCK_COUNT
     #else
       128
     #endif
       , 128, 16777216 },

     { "acpkm_section_kuznechik_block_count",
     #ifdef AK_OPTION_ACPKM_SECTION_KUZNECHIK_BLOCK_COUNT
       AK_OPTION_ACPKM_SECTION_KUZNECHIK_BLOCK_COUNT
     #else
       512
     #endif
       , 512, 16777216 },

     { "digital_signature_count_resource",
     #ifdef AK_OPTION_DIGITAL_SIGNATURE_COUNT_RESOURCE
       AK_OPTION_DIGITAL_SIGNATURE_COUNT_RESOURCE
     #else
       65536
     #endif
       , 1024, 2147483648 },

     { "openssl_compability",
     #ifdef AK_OPTION_OPENSSL_COMPABILITY
       AK_OPTION_OPENSSL_COMPABILITY
     #else
       0
     #endif
       , 0, 1 },

     { "use_additional_algorithm_check_context",
     #ifdef AK_OPTION_USE_ADDITIONAL_ALGORITHM_CHECK_CONTEXT
       AK_OPTION_USE_ADDITIONAL_ALGORITHM_CHECK_CONTEXT
     #else
       0
     #endif
       , 0, 1 },

     { "use_color_output",
     #ifdef AK_OPTION_USE_COLOR_OUTPUT
       AK_OPTION_USE_COLOR_OUTPUT
     #else
       #ifdef AK_HAVE_WINDOWS_H
         0
       #else
         1
       #endif
     #endif
       , 0, 1 },

   /* завершающая константа, должна всегда принимать нулевые значения */
     { NULL, 0, 0, 0 }
 };

/* ----------------------------------------------------------------------------------------------- */
 size_t ak_libakrypt_options_count( void )
{
  return ( sizeof( options )/( sizeof( struct option ))-1 );
}

/* ----------------------------------------------------------------------------------------------- */
 const char *ak_libakrypt_get_option_name( const size_t index )
{
     if( index >= ak_libakrypt_options_count() ) {
         ak_error_set_value( ak_error_wrong_index );
         return ak_null_string;
     }

  return options[index].name;
}

/* ----------------------------------------------------------------------------------------------- */
 ak_int64 ak_libakrypt_get_option_by_name( const char *name )
{
     size_t i = 0;
     ak_int64 result = ak_error_wrong_option;

     for( i = 0; i < ak_libakrypt_options_count(); i++ ) {
         if( strncmp( name, options[i].name, strlen( options[i].name )) == 0 ) {
             result = options[i].value;
         }
     }

  return result;
}

/* ----------------------------------------------------------------------------------------------- */
 ak_int64 ak_libakrypt_get_option_by_index( const size_t index )
{
     if( index >= ak_libakrypt_options_count() ) {
         ak_error_set_value( ak_error_wrong_index );
         return ak_error_wrong_option;
     }

  return options[index].value;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_libakrypt_set_option_by_name( const char *name, const ak_int64 value )
{
     size_t i = 0;
     int result = ak_error_wrong_option;

     for( i = 0; i < ak_libakrypt_options_count(); i++ ) {
         if( strncmp( name, options[i].name, strlen( options[i].name )) == 0 ) {
             options[i].value = value;
             result = ak_error_ok;
         }
     }

  return result;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_libakrypt_set_option_by_index( const size_t index, const ak_int64 value )
{
     if( index >= ak_libakrypt_options_count() ) {
         ak_error_set_value( ak_error_wrong_index );
         return ak_error_wrong_option;
     }
     options[index].value = value;

  return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
 void ak_libakrypt_log_options( void )
{
     size_t i = 0;

   /* выводим сообщение об установленных параметрах библиотеки */
   /* мы пропускаем вывод информации об архитектуре,
      поскольку она будет далее тестироваться отдельно         */
    for( i = 0; i < ak_libakrypt_options_count(); i++ ) {
       switch( options[i].value ) {

         case  0:  ak_error_message_str( ak_error_ok, __func__, options[i].name, "false" );
                   break;

         case  1:  ak_error_message_str( ak_error_ok, __func__, options[i].name, "true" );
                   break;

         default:  ak_error_message_int( ak_error_ok, __func__, options[i].name, options[i].value );
       }
    }
 /* выводим сообщение об установленных каталогах доступа к криптографическим ключам */
/*   ak_error_message_fmt( ak_error_ok, __func__,
                                      "certificate's repo: %s", ak_certificate_get_repository( )); */
}

/* ----------------------------------------------------------------------------------------------- */
/*! @example faq/example-options-01.c                                                              */
/* ----------------------------------------------------------------------------------------------- */
/*                                                                                   ak_options.c  */
/* ----------------------------------------------------------------------------------------------- */
