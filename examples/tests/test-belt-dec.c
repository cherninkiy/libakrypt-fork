/* --------------------------------------------------------------------------------- */
/* Контрольный пример расшифрования одного блока из стандарта (STB 34.101.31-2020)   */
/* --------------------------------------------------------------------------------- */
#include <stdio.h>
#include <libakrypt.h>

 int main( void )
{
 /* код ошибки, возвращаемый функциями библиотеки */
  int error = ak_error_ok;
 /* статус выполнения программы */
  int exitstatus = EXIT_FAILURE;

  /* входной блок (уже зашифрован)*/
  ak_uint8 indec[16] =
  {
    0xE1,0x2B,0xDC,0x1A,
    0xE2,0x82,0x57,0xEC,
    0x70,0x3F,0xCC,0xF0,
    0x95,0xEE,0x8D,0xF1
  };

  /* расшифрованный блок */
  ak_uint8 out[16] = {0};

 /* константное значение ключа */  
  ak_uint8 keydec[32] =
  {
    0x92,0xBD,0x9B,0x1C,
    0xE5,0xD1,0x41,0x01,
    0x54,0x45,0xFB,0xC9,
    0x5E,0x4D,0x0E,0xF2,
    0x68,0x20,0x80,0xAA,
    0x22,0x7D,0x64,0x2F,
    0x26,0x87,0xF9,0x34,
    0x90,0x40,0x55,0x11
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
  ak_bckey_set_key( &ctx, keydec, 32 );

  size_t i;

  // расшифровываем в режиме простой замены один блок
  if(( ak_bckey_decrypt_ecb( &ctx, indec, out, 16 ))
     != ak_error_ok ) goto exlab;

  printf("Decryption:\n");
  printf("m: ");
  for (i = 0; i < 16; ++i) { printf("%02X ", indec[i]); }
  printf("\nk: ");
  for (i = 0; i < 32; ++i) { printf("%02X ", keydec[i]); }
  printf("\nc: ");
  for (i = 0; i < 16; ++i) { printf("%02X ", out[i]); }
  printf("\n");

  printf("Expected:\n");
  printf("c: 0D C5 30 06 00 CA B8 40 B3 84 48 E5 E9 93 F4 21\n");

 /* после использования необходимо удалить контекст секретного ключа */
  exlab: ak_bckey_destroy( &ctx );

 /* завершаем работу */
  if( error == ak_error_ok ) exitstatus = EXIT_SUCCESS;
  ak_libakrypt_destroy();
 return exitstatus;
}
