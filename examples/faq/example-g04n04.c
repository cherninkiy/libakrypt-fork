/* --------------------------------------------------------------------------------- */
/* Пример example-g04n04.c                                                           */
/* Иллюстрация проверки качества двоичной последовательности, вырабатываемой ПДСЧ    */
/* на массивах, длиной 4096 байт                                                     */
/* --------------------------------------------------------------------------------- */
 #include <stdio.h>
 #include <libakrypt.h>

 int main( void )
{
 /* буффер для хранения случайных данных */
  ak_uint8 buffer[4096];

 /* определяем контекст генератора псевдослучайных значений */
  struct random generator;

 /* устанавливаем уровень аудита */
  ak_log_set_level( ak_log_maximum );
  ak_libakrypt_create( ak_function_log_stderr );

 /* вызываем конструктор генератора и вырабатываем случайные данные */
  ak_random_create_hrng( &generator );

 /* выполняем несколько попыток тестирования последовательности */
  for( int i = 0; i < 10; i++ )
  {
      ak_random_ptr( &generator, buffer, 4096 );

      for( int j = 0; j < 128; j++ ) {
         printf(" %s\n", ak_ptr_to_hexstr( buffer +32*j, 32, ak_false ));
      }
      if( ak_random_start_test( buffer, 4096 )) printf("Ok\n");
       else { /* в момент, когда тест не выполнен, производится остановка программы */
        printf("No\n [number: %d]\n", i );
        break;
       }
  }
 /* вызываем деструктор генератора */
  ak_random_destroy( &generator );
  ak_libakrypt_destroy();

 return EXIT_SUCCESS;
}
