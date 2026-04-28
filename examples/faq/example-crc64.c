/* --------------------------------------------------------------------------------- */
/*  Пример                                                                           */
/*  example-crc64.с                                                                  */
/*                                                                                   */
/*  Пример иллюстрирует процесс вычисления контрольной суммы crc64                   */
/* --------------------------------------------------------------------------------- */
 #include <stdio.h>
 #include <stdlib.h>
 #include <libakrypt.h>

 int main( void )
{
  struct hash ctx;
  ak_uint8 buffer[9] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
  long long unsigned int crc64, mustbe = 0x6C40DF5F0B497347;

 /* устанавливаем уровень максимального аудита в стандартный поток ошибок */
  ak_log_set_level( ak_log_maximum );
  if( ak_libakrypt_create( ak_function_log_stderr ) != ak_true ) {
   /* инициализация выполнена не успешно, следовательно, выходим из программы */
    ak_libakrypt_destroy();
    return EXIT_FAILURE;
  }

 /* создаем контекст */
  ak_hash_create_crc64( &ctx );

 /* вычисляем контрольную сумму */
  ak_hash_ptr( &ctx, buffer, sizeof( buffer ), &crc64, sizeof( crc64 ));

 /* выводим результат */
  printf("input:   %s\n", ak_ptr_to_hexstr( buffer, sizeof( buffer ), direct ));
  printf("crc64:   %s [as array, direct]\n",
                           ak_ptr_to_hexstr( &crc64, sizeof( crc64 ), direct ));
  printf("crc64:   %s [as array, reverse]\n",
                          ak_ptr_to_hexstr( &crc64, sizeof( crc64 ), reverse ));
  printf("crc64:   %016llx [as integer]\nmust be: 6C40DF5F0B497347\n", crc64 );

 /* освобождаем память */
  ak_hash_destroy( &ctx );
  ak_libakrypt_destroy();

 return ( crc64 == mustbe ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* --------------------------------------------------------------------------------- */
/*                                                                  example-crc64.c  */
/* --------------------------------------------------------------------------------- */
