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
  ak_uint8 inenc[16] =
  {
    0xB1,0x94,0xBA,0xC8,
    0x0A,0x08,0xF5,0x3B,
    0x36,0x6D,0x00,0x8E,
    0x58,0x4A,0x5D,0xE4,
  };

  /* выходной блок после зашифрования */
  ak_uint8 outenc[16] = {0};

  /* расшифрованный блок */
  ak_uint8 outdec[16] = {0};

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

  size_t i;

  // зашифровываем в режиме простой замены один блок
  if(( ak_bckey_encrypt_ecb( &ctx, inenc, outenc, 16 ))
     != ak_error_ok ) goto exlab;

  printf("Encryption:\n");
  printf("m: ");
  for (i = 0; i < 16; ++i) { printf("%02X ", inenc[i]); }
  printf("\nk: ");
  for (i = 0; i < 32; ++i) { printf("%02X ", key[i]); }
  printf("\nc: ");
  for (i = 0; i < 16; ++i) { printf("%02X ", outenc[i]); }
  printf("\n");

  printf("Expected:\n");
  printf("c: 69 CC A1 C9 35 57 C9 E3 D6 6B C3 E0 FA 88 FA 6E\n");

 /* после использования необходимо удалить контекст секретного ключа */
  exlab: ak_bckey_destroy( &ctx );

 /* завершаем работу */
  if( error == ak_error_ok ) exitstatus = EXIT_SUCCESS;
  ak_libakrypt_destroy();
 return exitstatus;
}
