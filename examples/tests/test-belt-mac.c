/* --------------------------------------------------------------------------------- */
/* Контрольный пример зашифрования одного блока из стандарта (STB 34.101.31-2020)    */
/* --------------------------------------------------------------------------------- */
#include <stdio.h>
#include <libakrypt.h>

 int main( void )
{
 /* код ошибки, возвращаемый функциями библиотеки */
  int error = ak_error_ok;
 /* статус выполнения программы */
  int exitstatus = EXIT_FAILURE;

  /* входной блок */
  ak_uint8 data[] =
  {
        0xB1,0x94,0xBA,0xC8,0x0A,0x08,0xF5,0x3B,
        0x36,0x6D,0x00,0x8E,0x58
  };


  /* иммитоставка */
  ak_uint8 mac[16] = {0};

 /* константное значение ключа */  
  ak_uint8 key[32] = {
    0xE9,0xDE,0xE7,0x2C,
    0x8F,0x0C,0x0F,0xA6,
    0x2D,0xDB,0x49,0xF4,
    0x6F,0x73,0x96,0x47,
    0x06,0x07,0x53,0x16,
    0xED,0x24,0x7A,0x37,
    0x39,0xCB,0xA3,0x83,
    0x03,0xA9,0x8B,0xF6,
  };

  size_t i;

 /* инициализируем библиотеку */
  if( ak_libakrypt_create( NULL ) != ak_true ) {
    ak_libakrypt_destroy();
    return EXIT_FAILURE;
  }

  /* контекст алгоритма блочного шифрования */
   struct bckey ctx;

  /* выполняем последовательный вызов двух функций:
     создаем ключ алгоритма Belt и присваиваем ему константное значение */
  ak_bckey_create_belt( &ctx );
  ak_bckey_set_key( &ctx, key, 32 );
  ak_belt_mac( &ctx, data, sizeof( data ), mac, 8);



  printf("MAC:\n");
  printf("\n mac: ");
  for (i = 0; i < 8; ++i) { printf("%02X ", mac[i]); }
  printf("\n");

  printf("Expected:\n");
  printf("mac: 72 60 DA 60 13 8F 96 C9 \n");



 /* после использования необходимо удалить контекст секретного ключа */
  exlab: ak_bckey_destroy( &ctx );

  ak_uint8 data1[] = {
        0xB1,0x94,0xBA,0xC8,0x0A,0x08,0xF5,0x3B,
        0x36,0x6D,0x00,0x8E,0x58,0x4A,0x5D,0xE4,
        0x85,0x04,0xFA,0x9D,0x1B,0xB6,0xC7,0xAC,
        0x25,0x2E,0x72,0xC2,0x02,0xFD,0xCE,0x0D,
        0x5B,0xE3,0xD6,0x12,0x17,0xB9,0x61,0x81,
        0xFE,0x67,0x86,0xAD,0x71,0x6B,0x89,0x0B
    };
      ak_uint8 key1[32] = {
        0xE9,0xDE,0xE7,0x2C,
        0x8F,0x0C,0x0F,0xA6,
        0x2D,0xDB,0x49,0xF4,
        0x6F,0x73,0x96,0x47,
        0x06,0x07,0x53,0x16,
        0xED,0x24,0x7A,0x37,
        0x39,0xCB,0xA3,0x83,
        0x03,0xA9,0x8B,0xF6,
      };
      struct bckey ctx1;
      ak_uint8 mac1[16] = {0};
      ak_bckey_create_belt( &ctx1 );
      ak_bckey_set_key( &ctx1, key1, 32 );
      ak_belt_mac( &ctx1, data1, sizeof( data1 ), mac1, 8);
      
      printf("MAC:\n");
      printf("\n mac: ");
      for (i = 0; i < 8; ++i) { printf("%02X ", mac1[i]); }
        printf("\n");

      printf("Expected:\n");
      printf("mac: 2D AB 59 77 1B 4B 16 D0 \n");


 /* завершаем работу */
  if( error == ak_error_ok ) exitstatus = EXIT_SUCCESS;
  ak_libakrypt_destroy();
 return exitstatus;
}
