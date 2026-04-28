/* --------------------------------------------------------------------------------- */
/*  Пример                                                                           */
/*  example-convert-01.с                                                             */
/*                                                                                   */
/*  Пример иллюстрирует процесс перекодировки массива данных из одного               */
/*  представления в другой, а также обработку ошибок, возникающих при кодировке      */
/* --------------------------------------------------------------------------------- */
 #include <stdio.h>
 #include <stdlib.h>
 #include <libakrypt.h>

 int main( void )
{
     char *str = NULL;
     char *hexstr = "11ac56ad5befd2ead9043ac13dr37e";
                   /*  ошибочный символ:     ----    */
     ak_uint8 data[16];
     ak_uint8 newd[16] = {
       0x11, 0xac, 0x56, 0xad, 0x5b, 0xef, 0xd2, 0xeB, 0xd9, 0x04, 0x3a, 0xc1, 0x3d, 0x03, 0x7e
     };

   /* устанавливаем функцию вывода ошибок */
     ak_log_set_function( ak_function_log_stderr );

   /* информация о строке, перекодируемой в данные */
     printf("hexstr: %s (bytes: %lld)\n", hexstr, (long long int) ak_hexstr_size( hexstr ));

   /* перекодируем данные */
     if( ak_hexstr_to_ptr( hexstr, data, sizeof( data ), direct ) != ak_error_ok ) {
         printf( "expected encode error: %d\n\n", ak_error_get_value());
     } else
             printf( "encode Ok\n\n" );

   /* выводим в консоль */
     printf("data:   %s\n", ak_ptr_to_hexstr( data, sizeof( data ), direct ));
     printf("data:   %s\n", ak_ptr_to_hexstr( data, sizeof( data ), Direct ));
     printf("data:   %s\n", ak_ptr_to_hexstr( data, sizeof( data ), BlankDirect ));
     printf("data:   %s\n", ak_ptr_to_hexstr( data, sizeof( data ), reverse ));
     printf("data:   %s\n", ak_ptr_to_hexstr( data, sizeof( data ), Reverse ));
     printf("data:   %s\n", ak_ptr_to_hexstr( data, sizeof( data ), BlankReverse ));

   /* выводим с выделением памяти */
     str = ak_ptr_to_hexstr_alloc( data, sizeof( data ), BlankDirect );
     if( str != NULL ) {
         printf("data:   %s (BlankDirect)\n\n", str );
         free( str );
     }

   /* сравниваем преобразованные данные с константными
                       и должны найти еще одну заранее подготовленную ошибку */
     ak_ptr_is_equal_with_log( data, newd, 16 ) ?
                               printf("equal is Ok\n") : printf("equal is Wrong\n");

 return EXIT_SUCCESS;
}

/* --------------------------------------------------------------------------------- */
/*                                                             example-convert-01.c  */
/* --------------------------------------------------------------------------------- */
