/* --------------------------------------------------------------------------------- */
/*  Пример                                                                           */
/*  example-nolibc.с                                                                 */
/*                                                                                   */
/*  Пример иллюстрирует процесс вызова функций библиотеки без поддержки libc         */
/*                                                                                   */
/*  1. При компиляции библиотеки должен применяться специально подготовленный        */
/*  файл libakrypt-config.h, отключающий подключение заголовков стандартной          */
/*  библиотеки языка Си и, как следствие, оптимизирующий код                         */
/*                                                                                   */
/*  2. При сборке примера должны использоваться флаг -nostdlib                       */
/*                                                                                   */
/*  3. Ассемблерные вставки используют системные вызовы linux и могут быть корректно */
/*  скомпилированы только под x64 архитектурой.                                      */
/* --------------------------------------------------------------------------------- */
 #include <libakrypt.h>

/* Функция которая будет использоваться в качестве низкоуровневой функции аудита */
 static int ak_function_log_write( const char *message )
{
    size_t count = 0;
    const char *str = message;
    if( str == NULL ) return 0;
    for( ; *str != '\0'; ++count, ++str ); /* вычисляем длину строки */

    asm volatile (
        "movq $1, %%rax\n\t"
        "movq $1, %%rdi\n\t" /* используем системный вызов - write */
        "syscall"
        :
        : "S" (message), "d" (count)
        : "rax", "rdi", "rcx", "r11", "memory"
    );
    return 0;
}

/* Тело основной программы, реализующей необходимый пользовательский функционал */
 int main( void )
{
     ak_log_set_level( ak_log_maximum );
     ak_libakrypt_create( ak_function_log_write );

     /* .... здесь код программы ....

                      например, такой */

      ak_uint8 data[8] = { 0xab, 0x13, 0xA5, 0xE7, 0x12, 0x9D };

      ak_error_message( ak_error_ok, __func__, ak_ptr_to_hexstr( data, sizeof(data), Direct ));
      ak_error_message( ak_error_ok, __func__, ak_ptr_to_hexstr( data, sizeof(data), Reverse ));
      ak_error_message( ak_error_ok, __func__, ak_ptr_to_hexstr( data, sizeof(data), BlankDirect ));
      ak_error_message( ak_error_ok, __func__, ak_ptr_to_hexstr( data, sizeof(data), BlankReverse ));

  return ak_libakrypt_destroy();
}

/* Точка входа в программу, которая будет вызывать функцию main
                               и возвращать результат ее работы */
 void startup(void) {

    int status = main();

     asm volatile (
         "movsx %0, %%rdi\n\t"   /* Загружаем status в rdi (с знаковым расширением) */
         "movq $60, %%rax\n\t"   /* Номер системного вызова exit (60) */
         "syscall"               /* Вызов ядра */
         :
         : "r" (status)          /* Входной операнд (status) */
         : "%rax", "%rdi"        /* Регистры, которые мы изменяем */
     );
}

/* --------------------------------------------------------------------------------- */
/*                                                                 example-nolibc.c  */
/* --------------------------------------------------------------------------------- */
