/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2022, 2025 by Axel Kenzo, axelkenzo@mail.ru                               */
/*                                                                                                 */
/*  Файл ak_tools.с                                                                                */
/* ----------------------------------------------------------------------------------------------- */
 #include <libakrypt.h>

/* ----------------------------------------------------------------------------------------------- */
/*! @mainpage Краткая аннотация

    Библиотека **libakrypt** написана на языке Си и реализует механизмы генерации,
    хранения, экспорта и импорта ключевой информации, а также основные
    отечественные криптографические преобразования, регламентированные
    национальными стандартами и рекомендациями по стандартизации.
    Библиотека распространяется по лицензии
    [MIT](https://git.miem.hse.ru/axelkenzo/libakrypt-0.x/-/raw/master/LICENSE).

    Домашняя страница проекта: **http://libakrypt.ru**

    Цель разработки библиотеки заключается в создании программного модуля с
    открытыми исходными текстами для СКЗИ, удовлетворяющего рекомендациям
    по стандартизации Р 1323565.1.012-2017
    «[Принципы разработки и модернизации шифровальных (криптографических) средств защиты
    информации](https://tc26.ru/standarts/rekomendatsii-po-standartizatsii/r-1323565-1-012-2017-informatsionnaya-tekhnologiya-kriptograficheskaya-zashchita-informatsii-printsipy-razrabotki-i-modernizatsii-shifrovalnykh-kriptograficheskikh-sredstv-zashchity-informatsii.html)»
    по классу КС3.

    Библиотека может применяться в различных операционных системах: `Linux`,
    `Windows` и `FreeBSD`. Также были проведены успешные тестовые запуски библиотеки под
    управлением [ReactOS](https://reactos.org),
    [Sailfish OS](https://sailfishos.org/) и
    [PetaLinux](https://www.xilinx.com/products/design-tools/embedded-software/petalinux-sdk.html).


    Библиотека **libakrypt** может быть собрана различными компиляторами такими,
    как `gcc`, `clang`, `Microsoft Visual C`, `TinyCC` и `icc`.
    Система сборки: [cmake](https://cmake.org/).

    Поддерживаемая архитектура: `x86`, `x64`, `arm32v7`,
    `arm32v7eb`, `armhf`, `mips32r2` и `mips64r2`.

    Последняя версия исходный текстов: **https://git.miem.hse.ru/axelkenzo/libakrypt-0.x**         */
/* ----------------------------------------------------------------------------------------------- */
/*  @image html https://git.miem.hse.ru/axelkenzo/libakrypt-0.x/-/raw/master/logo.png width=150mm  */
/* ----------------------------------------------------------------------------------------------- */
/** @addtogroup log
 @{
    Все сообщения библиотеки могут быть разделены на три уровня.

    @li Первый уровень аудита определяется константой @ref ak_log_none. На этом уровне выводятся
    только сообщения об ошибках.

    @li Второй уровень аудита определяется константой @ref ak_log_standard. На этом уровене
    выводятся сообщения об ошибках, а также сообщения, определяемые требованиями по безопасности.

    @li Третий (максимальный) уровень аудита определяется константой @ref ak_log_maximum.
    На этом уровне выводятся все сообщения, доступные на первых двух уровнях, а также
    сообщения отладочного характера, позволяющие проследить логику работы функций библиотеки.

    Для вывода сообщений об ошибках пользователям библиотеки
    необходимо использовать функцию @ref ak_error_message(), которая формирует строку с сообщением
    специального вида и выводит сформированную строку в установленное
    устройство аудита (консоль, демон syslog и т.п.).

    Низкоуровневая функция вывода строк в устройство аудита может быть установлена пользователем
    библиотеки с помощью @ref ak_log_set_function().
    Примерами функций, осуществляющих вывод сформированных функцией @ref ak_error_message() сообщений,
    являются:

    - @ref ak_function_log_stderr(), реализующая вывод в стандартный поток вывода ошибок,
    - @ref ak_function_log_syslog(), реализующая вывод в демон аудита syslog.
 @} */

/* ----------------------------------------------------------------------------------------------- */
/*! @brief Переменная, содержащая в себе код последней ошибки                                      */
 static int ak_errno = ak_error_ok;
/*! @brief Переменная, содержащая в себе текущий уровень аудита библиотеки                         */
 static int ak_log_level = ak_log_standard;
/*! @brief Статический указатель на текущую функцию аудита                                       */
 static ak_function_log *ak_function_log_default = NULL;
/* ----------------------------------------------------------------------------------------------- */
/*! @brief Cтатическая переменная для вывода сообщений. */
 static char ak_static_buffer[AK_DEFAULT_STATIC_BUFFER_SIZE];
/* ----------------------------------------------------------------------------------------------- */
#ifdef AK_HAVE_STDIO_H
 /*! @brief Cтатическая переменная, указатель начала окрашивания кодов и выводимых сообщений. */
  static char *ak_error_code_start_string = "\x1b[31m";
 /*! @brief Cтатическая переменная, указатель окончания окрашивания кодов и выводимых сообщений. */
  static char *ak_error_code_end_string = "\x1b[0m";
#endif

/* ----------------------------------------------------------------------------------------------- */
 int ak_log_get_level( void )
{
 return ak_log_level;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_log_set_level( int level )
{
     if( level < 0 ) {
         ak_log_level = ak_log_maximum;
         return ak_error_underflow;
     }
     if( level > 32 ) {
         ak_log_level = 32;
         return ak_error_overflow;
     }
     ak_log_level = level;

 return ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_error_set_value( const int value )
{
  return ( ak_errno = ( value <= 0 ? value : ak_error_overflow ));
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_error_get_value( void )
{
  return ak_errno;
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_log_set_function( ak_function_log *function )
{
     ak_function_log_default = function;

   #ifdef AK_HAVE_STDIO_H
     if( function == ak_function_log_stderr ) {
      #ifdef _WIN32
           /* стандартный терминал Windows не поддерживает расцветку символов */
             ak_error_code_start_string = "";
             ak_error_code_end_string = "";
      #endif
     }
   #endif

  if( ak_function_log_default == NULL ) {
   /* это часть, которая пытается подобрать функцию аудита по-своему разумению,
      если пользователь так и не смог определиться */
    #ifdef AK_HAVE_SYSLOG_H
      ak_function_log_default = ak_function_log_syslog;
      return ak_error_ok;
    #else
      #ifdef AK_HAVE_STDIO_H
       ak_function_log_default = ak_function_log_stderr;
       return ak_error_ok;
      #endif
    #endif
   }
  return ( ak_function_log_default == NULL ) ? ak_error_null_pointer : ak_error_ok;
}

/* ----------------------------------------------------------------------------------------------- */
 ak_function_log *ak_log_get_function( void )
{
     return ak_function_log_default;
}

/* ----------------------------------------------------------------------------------------------- */
#ifdef AK_HAVE_SYSLOG_H
 int ak_function_log_syslog( const char *message )
{
 #ifdef __linux__
   int priority = LOG_AUTHPRIV | LOG_NOTICE;
 #else
   int priority = LOG_USER;
 #endif
  if( message != NULL ) syslog( priority, "%s", message );
 return ak_error_ok;
}
#endif

/* ----------------------------------------------------------------------------------------------- */
#ifdef AK_HAVE_STDIO_H
 int ak_function_log_stderr( const char *message )
{
  if( message != NULL ) fprintf( stderr, "%s\n", message );
  return ak_error_ok;
}
#endif

/* ----------------------------------------------------------------------------------------------- */
 int ak_error_message( const int code, const char *function, const char *format, ... )
{
  #ifdef AK_HAVE_SNPRINTF
     va_list args;
     char ak_static_buffer_fmt[512];
     const char *br0 = "", *br1 = "():", *br = NULL;
  #endif

  if( ak_function_log_default == NULL )
     return ak_error_set_value( ak_error_undefined_function );
  if( format == NULL )
     return ak_error_message( ak_error_null_pointer, function,
                                                            "using the null string for a message" );
 #ifndef AK_HAVE_SNPRINTF
  /* здесь у нас нет функций va_start(), va_end() и т.д. для обработки переменного числа
     аргументов функции, поэтому выводим то, что имеем */
     return ak_error_message_str( code, function, format, "" );
 #else
 /* начинаем с того, что формируем единую строку из переданных аргументов функции */
  va_start( args, format );
  memset( ak_static_buffer_fmt, 0, sizeof( ak_static_buffer_fmt ));

  #ifdef _MSC_VER
    _vsnprintf_s( ak_static_buffer_fmt,
                    sizeof( ak_static_buffer_fmt ), sizeof( ak_static_buffer_fmt ), format, args );
  #else
   vsnprintf( ak_static_buffer_fmt, sizeof( ak_static_buffer_fmt ), format, args );
  #endif
  va_end( args );

 /* теперь добавляем украшения и выводим с помощью установленной ранее функции аудита */
  memset( ak_static_buffer, 0, sizeof( ak_static_buffer ));
  if(( function == NULL ) || strcmp( function, "" ) == 0 ) br = br0;
    else br = br1;

  if( code < 0 ) ak_snprintf(
                              ak_static_buffer,
                              sizeof(ak_static_buffer) -1,
                            #ifdef AK_HAVE_UNISTD_H
                              "[%d] "
                            #endif
                              "%s%s %s (%scode: %d%s)",
                            #ifdef AK_HAVE_UNISTD_H
                              getpid(),
                            #endif
                              function, br, ak_static_buffer_fmt,
                              ak_error_code_start_string, code, ak_error_code_end_string
                            );
   else ak_snprintf(
                     ak_static_buffer,
                     sizeof(ak_static_buffer) -1,
                   #ifdef AK_HAVE_UNISTD_H
                     "[%d] "
                   #endif
                     "%s%s %s",
                   #ifdef AK_HAVE_UNISTD_H
                     getpid(),
                   #endif
                     function, br, ak_static_buffer_fmt
                   );
  ak_function_log_default( ak_static_buffer );
 #endif

 return ak_error_set_value( code );
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_error_message_str( const int code, const char *function,
                                                            const char *message, const char *value )
{
 #ifdef AK_HAVE_SNPRINTF
     return ak_error_message( code, function, "%s %s", message, value );
 #else
     if( ak_function_log_default == NULL )
         return ak_error_set_value( ak_error_undefined_function );

   /* выводим номер процесса */
     #ifdef AK_HAVE_UNISTD_H
         ak_function_log_default("[");
         ak_function_log_default( ak_int64_to_str( getpid() ));
         ak_function_log_default("] ");
     #endif

   /* выводим имя функции */
     if( strlen( function ) != 0 ) {
         ak_function_log_default( function );
         ak_function_log_default( "(): ");
     }
     ak_function_log_default( message );

   /* выводим второй параметр функции */
     if( strlen( value ) != 0 ) {
         ak_function_log_default( " " ); /* дополнительный пробел */
         ak_function_log_default( value );
     }

   /* в случае нетривиального значения ошибки, добавляем ее код */
     if( code != 0 ) {
       ak_function_log_default( " (code: ");
       ak_function_log_default( ak_int64_to_str( code ));
       ak_function_log_default( ")");
     }
   /* добавляем возврат каретки */
     ak_function_log_default( "\n");
     return ak_error_set_value( code );
 #endif
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_error_message_int( const int code, const char *function,
                                                         const char *message, const ak_int64 value )
{
 #ifdef AK_HAVE_SNPRINTF
     return ak_error_message( code, function, "%s %lld", message, (long long int) value );
 #else
     if( ak_function_log_default == NULL )
         return ak_error_set_value( ak_error_undefined_function );

   /* выводим номер процесса */
     #ifdef AK_HAVE_UNISTD_H
         ak_function_log_default("[");
         ak_function_log_default( ak_int64_to_decstr( getpid() ));
         ak_function_log_default("] ");
     #endif

   /* выводим имя функции */
     if( strlen( function ) != 0 ) {
         ak_function_log_default( function );
         ak_function_log_default( "(): ");
     }
     ak_function_log_default( message );

   /* выводим второй параметр функции */
     ak_function_log_default( " " ); /* дополнительный пробел */
     ak_function_log_default( ak_int64_to_str( value ));

   /* в случае нетривиального значения ошибки, добавляем ее код */
     if( code != 0 ) {
       ak_function_log_default( " (code: ");
       ak_function_log_default( ak_int64_to_str( code ));
       ak_function_log_default( ")");
     }
   /* добавляем возврат каретки */
     ak_function_log_default( "\n");
     return ak_error_set_value( code );
 #endif
}

/* ----------------------------------------------------------------------------------------------- */
/*                        Функции для работы со строками переменной длины                          */
/* ----------------------------------------------------------------------------------------------- */
#ifdef AK_HAVE_SNPRINTF
 int ak_snprintf( char *str, size_t size, const char *format, ... )
{
  int result = 0;
  va_list args;
  va_start( args, format );

 #ifdef _MSC_VER
  #if _MSC_VER > 1310
    result = _vsnprintf_s( str, size, size, format, args );
  #else
    result = _vsnprintf( str, size, format, args );
  #endif
 #else
  result = vsnprintf( str, size, format, args );
 #endif
  va_end( args );
 return result;

}

/* ----------------------------------------------------------------------------------------------- */
 int ak_printf( ak_function_log *function, const char *format, ... )
{
  int result = 0;
  va_list args;
  va_start( args, format );

 #ifdef _MSC_VER
  #if _MSC_VER > 1310
    result = _vsnprintf_s( ak_static_buffer,
                       sizeof( ak_static_buffer ) -1, sizeof( ak_static_buffer ) -1, format, args );
  #else
    result = _vsnprintf( ak_static_buffer, sizeof( ak_static_buffer ) -1, format, args );
  #endif
 #else
  result = vsnprintf( ak_static_buffer, sizeof( ak_static_buffer ) -1, format, args );
 #endif
  va_end( args );

  function( ak_static_buffer );
 return result;
}
#endif

/* ----------------------------------------------------------------------------------------------- */
/*                                    Вспомогательные функции                                      */
/* ----------------------------------------------------------------------------------------------- */
 char *ak_int64_to_str( const ak_int64 value )
{
     ak_int64 x = value;
     size_t idx = sizeof( ak_static_buffer ) -1; /* указываем на последний элемент массива */

   /* готовим память */
     memset( ak_static_buffer, 0, sizeof( ak_static_buffer ));
   /* получам абсолютное значение */
     if( x < 0 ) x = -x;
   /* получам десятичные знаки */
     while( x != 0 ) {
         ak_static_buffer[--idx] = (char)(( x%10 ) + '0' );
         x /= 10;
         if( idx == 1 ) break;
     }
   /* добавляем знак */
     if( value < 0 ) ak_static_buffer[--idx] = '-';

  return ak_static_buffer +idx;
}

/* ----------------------------------------------------------------------------------------------- */
 ssize_t ak_hexstr_size( const char *hexstr )
{
  ssize_t len = 0;
  if( hexstr == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                             "using null pointer to a hex string" );
  len = ( ssize_t ) strlen( hexstr );
  if( len&1 ) len++;
  len >>= 1;

 return len;
}

/* ----------------------------------------------------------------------------------------------- */
/*! @brief Конвертация символа в целочисленное значение                                            */
/* ----------------------------------------------------------------------------------------------- */
 inline static ak_uint32 ak_xconvert( const char c )
{
    switch( c )
   {
      case 'a' :
      case 'A' : return 10;
      case 'b' :
      case 'B' : return 11;
      case 'c' :
      case 'C' : return 12;
      case 'd' :
      case 'D' : return 13;
      case 'e' :
      case 'E' : return 14;
      case 'f' :
      case 'F' : return 15;
      case '0' : return 0;
      case '1' : return 1;
      case '2' : return 2;
      case '3' : return 3;
      case '4' : return 4;
      case '5' : return 5;
      case '6' : return 6;
      case '7' : return 7;
      case '8' : return 8;
      case '9' : return 9;
      default : ak_error_set_value( ak_error_undefined_value ); return 0;
 }
}

/* ----------------------------------------------------------------------------------------------- */
 int ak_hexstr_to_ptr( const char *hexstr, ak_pointer ptr, const size_t size, const output_t out )
{
  int64_t i = 0;
  ak_uint8 *bdata = ptr;
  size_t len = 0;

  if( hexstr == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                             "using null pointer to a hex string" );
  if( ptr == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                                 "using null pointer to a buffer" );
  if( size == 0 ) return ak_error_message( ak_error_zero_length, __func__,
                                                          "using zero value for length of buffer" );
  len = strlen( hexstr );
  if( len&1 ) len++;
  len >>= 1;
  if( size < len ) return ak_error_message( ak_error_wrong_length, __func__ ,
                                                               "using a buffer with small length" );

  memset( ptr, 0, size ); // перед конвертацией мы обнуляем исходные данные
  ak_error_set_value( ak_error_ok );
  if( out ) {
    for( i = strlen( hexstr )-2, len = 0; i >= 0 ; i -= 2, len++ ) {
       bdata[len] = (ak_xconvert( hexstr[i] ) << 4) + ak_xconvert( hexstr[i+1] );
    }
    if( i == -1 ) bdata[len] = ak_xconvert( hexstr[0] );
  } else {
        for( i = 0, len = 0; i < (int) strlen( hexstr ); i += 2, len++ ) {
           bdata[len] = (ak_xconvert( hexstr[i] ) << 4);
           if( i < (int) strlen( hexstr )-1 ) bdata[len] += ak_xconvert( hexstr[i+1] );
        }
    }
 return ak_error_get_value();
}

/* ----------------------------------------------------------------------------------------------- */
/*! @brief Конвертация числа в символ                                                              */
/* ----------------------------------------------------------------------------------------------- */
 inline static char ak_nconvert( ak_uint8 n, output_t out )
{
     if( n > 15 ) return '?';
     if( n < 10 ) return (char)( n + '0');

     switch( out ) {
       case direct:
       case reverse:
                     return n%10 + 'a';
       default:
                     return n%10 + 'A';
     }
}

/* ----------------------------------------------------------------------------------------------- */
/*! @details Функция рассматривает область памяти, на которую указывает указатель ptr, как массив
    последовательно записанных байт фиксированной длины, и
    последовательно выводит в статический буффер значения, хранящиеся в данной области памяти.
    Значения выводятся в шестнадцатеричной системе счисления.

    Пример использования.

    @code
      ak_uint8 data[5] = { 1, 2, 3, 4, 5 };
      ak_uint8 *str = ak_ptr_to_hexstr( data, 5, direct );
      if( str != NULL ) printf("%s\n", str );
    @endcode
                                                                                                   */
/* ----------------------------------------------------------------------------------------------- */
 const char *ak_ptr_to_hexstr( ak_const_pointer ptr, const size_t ptr_size, const output_t out )
{
  size_t len, idx, js, start, offset;
  ak_uint8 *data = ( ak_uint8 * ) ptr;

  if( ptr == NULL ) {
    ak_error_message( ak_error_null_pointer, __func__ , "using null pointer to data" );
    return ak_null_string;
  }
  if( ptr_size <= 0 ) {
    ak_error_message( ak_error_zero_length, __func__ , "using data with zero or negative length" );
    return ak_null_string;
  }

  switch( out ) {
     case reverse:
     case Reverse:
             len = 1 + 2*ptr_size; start = len -3; offset = -2; break;
     case BlankDirect:
             len = 1 + 3*ptr_size; start = 0; offset = 3; break;
     case BlankReverse:
             len = 1 + 3*ptr_size; start = len -4; offset = -3; break;
     case direct:
     case Direct:
     default:
             len = 1 + 2*ptr_size; start = 0; offset = 2; break;
  }

 /* если возвращаемое функцией значение обрабатывается, то вывод предупреждения об ошибке излишен */
  if( sizeof( ak_static_buffer ) < len ) return ak_null_string;

 /* очищаем память */
  memset( ak_static_buffer, 0, sizeof( ak_static_buffer ));
 /* основной цикл*/
  for( idx = 0, js = start; idx < ptr_size; idx++, js += offset ) {
     ak_uint8 n = data[idx];

     ak_static_buffer[js] = ak_nconvert( n >> 4, out );
     ak_static_buffer[js +1] = ak_nconvert( n&0xf, out );
     if( out == BlankDirect || out == BlankReverse ) ak_static_buffer[js +2] = ' ';
  }

 return ak_static_buffer;
}

#ifdef AK_HAVE_STDLIB_H
/* ----------------------------------------------------------------------------------------------- */
/*! Пример использования.
    @code
      ak_uint8 data[1000] = { 1, 2, 3, 4, 5, 6, 7, 8 };
      ak_uint8 *str = ak_ptr_to_hexstr_alloc( data, sizeof( data ), direct );
      if( str != NULL ) {
         printf("%s\n", str );
         free( str );
      }
    @endcode
                                                                                                   */
/* ----------------------------------------------------------------------------------------------- */
 char *ak_ptr_to_hexstr_alloc( ak_const_pointer ptr, const size_t ptr_size, const output_t out )
{
     char *result = NULL;
     size_t len, idx, js, start, offset;
     ak_uint8 *data = ( ak_uint8 * ) ptr;

     if( ptr == NULL ) {
         ak_error_message( ak_error_null_pointer, __func__ , "using null pointer to data" );
         return NULL;
     }
     if( ptr_size <= 0 ) {
         ak_error_message( ak_error_zero_length, __func__ , "using data with zero or negative length" );
         return NULL;
     }

     switch( out ) {
         case reverse:
         case Reverse:
             len = 1 + 2*ptr_size; start = len -3; offset = -2; break;
         case BlankDirect:
             len = 1 + 3*ptr_size; start = 0; offset = 3; break;
         case BlankReverse:
             len = 1 + 3*ptr_size; start = len -4; offset = -3; break;
         case direct:
         case Direct:
         default:
             len = 1 + 2*ptr_size; start = 0; offset = 2; break;
     }

     if(( result = malloc( len )) == NULL ) {
         ak_error_message( ak_error_out_of_memory, __func__ , "incorrect memory allocation" );
         return NULL;
     }
      else
         memset( result, 0, len );

   /* основной цикл*/
     for( idx = 0, js = start; idx < ptr_size; idx++, js += offset ) {
         ak_uint8 n = data[idx];

         result[js] = ak_nconvert( n >> 4, out );
         result[js +1] = ak_nconvert( n&0xf, out );
         if( out == BlankDirect || out == BlankReverse ) result[js +2] = ' ';
     }

 return result;
}
#endif

/* ----------------------------------------------------------------------------------------------- */
 bool_t ak_ptr_is_equal_with_log( ak_const_pointer left, ak_const_pointer right, const size_t size )
{
  size_t i = 0;
  char buffer[128];
  bool_t result = ak_true;
  const ak_uint8 *lp = left, *rp = right;

  if(( left == NULL ) || ( right == NULL )) {
    ak_error_message( ak_error_null_pointer, __func__, "using a null pointer" );
    return ak_false;
  }

  for( i = 0; i < size; i++ ) {
     if( lp[i] != rp[i] ) {
       result = ak_false;
       if( i < ( sizeof( buffer ) >> 1 )) { buffer[2*i] = buffer[2*i+1] = '^'; }
      } else {
         if( i < ( sizeof( buffer ) >> 1 )) { buffer[2*i] = buffer[2*i+1] = ' '; }
        }
  }
  buffer[ ak_min( size << 1, sizeof(buffer) -1) ] = 0;

  if( result == ak_false ) {
    ak_error_message( ak_error_ok, "", "%s", "" ); /* пустая строка */
    ak_error_message_str( ak_error_ok, "",
                                     ak_ptr_to_hexstr( left, size, direct ), "(calculated data)" );
    ak_error_message_str( ak_error_ok, "",
                                     ak_ptr_to_hexstr( right, size, direct ), "(const value)" );
    ak_error_message( ak_error_ok, "", "%s", buffer );
  }

 return result;
}

/* ----------------------------------------------------------------------------------------------- */
/*! @example faq/example-audit-01.c                                                                */
/*! @example faq/example-convert-01.c                                                              */
/* ----------------------------------------------------------------------------------------------- */
/*                                                                                      ak_tools.c */
/* ----------------------------------------------------------------------------------------------- */
