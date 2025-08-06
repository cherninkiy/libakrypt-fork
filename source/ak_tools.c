/* ----------------------------------------------------------------------------------------------- */
/*  Copyright (c) 2014 - 2022, 2025 by Axel Kenzo, axelkenzo@mail.ru                               */
/*                                                                                                 */
/*  Файл ak_tools.с                                                                                */
/* ----------------------------------------------------------------------------------------------- */
 #include <libakrypt.h>

/* ----------------------------------------------------------------------------------------------- */
/*! @mainpage Краткая аннотация

    @image html https://git.miem.hse.ru/axelkenzo/libakrypt-0.x/-/raw/master/logo.png width=150mm

    Целью библиотеки **libakrypt** является разработка свободно распространяемого
    программного модуля, предназначенного для использования в средствах защиты информации,
    удовлетворяющих рекомендациям по стандартизации P1323565.1.012-2017.

    Библиотека разрабатывается на языке Си и распространяется под лицензией **MIT**.

    Домашняя страница проекта: **http://libakrypt.ru**

    Последняя версия исходный текстов: **https://git.miem.hse.ru/axelkenzo/libakrypt-0.x**         */
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
 char *ak_int64_to_decstr( const ak_int64 value )
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
     if( strlen( value ) != 0 ) {
         ak_function_log_default( " " ); /* дополнительный пробел */
         ak_function_log_default( value );
     }

   /* в случае нетривиального значения ошибки, добавляем ее код */
     if( code != 0 ) {
       ak_function_log_default( " (code: ");
       ak_function_log_default( ak_int64_to_decstr( code ));
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
     return ak_error_message( code, function, "%s %ld", message, value );
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
     ak_function_log_default( ak_int64_to_decstr( value ));

   /* в случае нетривиального значения ошибки, добавляем ее код */
     if( code != 0 ) {
       ak_function_log_default( " (code: ");
       ak_function_log_default( ak_int64_to_decstr( code ));
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

#ifndef AK_HAVE_STRING_H
/* ----------------------------------------------------------------------------------------------- */
/*                        Реализация функций, определяемых в string.h                              */
/* ----------------------------------------------------------------------------------------------- */
 void *memset( void *s, int c, size_t n )
{
     size_t i;
     ak_int8 *sp = (ak_int8 *)s;

     if( sp != NULL ) {
         for( i = 0; i < n; i++ ) sp[i] = (ak_int8)c;
     }

 return s;
}

/* ----------------------------------------------------------------------------------------------- */
 void *memcpy( void *dest, const void *src, size_t n )
{
     size_t i;
     ak_uint8 *dp = (ak_uint8 *)dest;
     ak_uint8 *sp = (ak_uint8 *)src;

     if( dp != NULL ) {
         for( i = 0; i < n; i++ ) dp[i] = sp[i];
     }

 return dest;
}

/* ----------------------------------------------------------------------------------------------- */
 size_t strlen( const char *s )
{
     size_t count = 0;
     char *sptr = (char *)s;

     if( sptr == NULL ) return 0;
     for( ; *sptr != '\0'; ++count, ++sptr ) {
         if( count >= AK_DEFAULT_STATIC_BUFFER_SIZE ) break;
     };

  return count;
}
#endif

/* ----------------------------------------------------------------------------------------------- */
/*! @example faq/example-audit-01.c                                                                */
/* ----------------------------------------------------------------------------------------------- */
/*                                                                                      ak_tools.c */
/* ----------------------------------------------------------------------------------------------- */
