/* --------------------------------------------------------------------------------- */
/* Пример example-g06n14.c                                                           */ 
/*                                                                                   */
/* Пример вычисления кратной точки с использованием случайного k                     */
/* --------------------------------------------------------------------------------- */

#include <libakrypt.h>

int main( void ) {
 
  /* инициализируем библиотеку */
  if ( ak_libakrypt_create( NULL ) != ak_true) {
    /* Инициализация выполнена не успешно, следовательно, выходим из программы*/	  
    ak_libakrypt_destroy();
    return EXIT_FAILURE;
  }

  /* объявляем кривую mycurve */
  struct wcurve mycurve;
  /* копируем параметры кривой id_rfc4357_gost_3410_2001_paramSetA в mycurve */ 
  memcpy(&mycurve, &id_rfc4357_gost_3410_2001_paramSetA, 
		  sizeof(id_rfc4357_gost_3410_2001_paramSetA));

  /* объявляем точку Q, в которую будет помещен результат */
  struct wpoint Q;
  
  /* контекст генератора псевдослучайных значений */
  struct random gen;

  /* определяем вспомогательную переменную */
  ak_mpznmax rand;

  /* создаем генератор */
  ak_random_create_lcg(&gen);

  /* вычисляем случайное значение */
  ak_mpzn_set_random_modulo(rand, mycurve.q, mycurve.size, &gen);
  /* сравниваем вычет */
  if (ak_mpzn_cmp_ui(rand, mycurve.size, 0) == ak_true)
    /* устанавливаем вычету значение 1 */
    ak_mpzn_set_ui(rand, mycurve.size, 1); 

  /* вычисляем кратную точку */
  ak_wpoint_pow(&Q, &mycurve.point, rand, mycurve.size, &mycurve);

  /* выводим полученную точку */
  printf("Полученная точка Q\n");
  printf("X = %s\n", ak_mpzn_to_hexstr(Q.x, ak_mpzn256_size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(Q.y, ak_mpzn256_size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(Q.z, ak_mpzn256_size));
  /* проверяем, что точка принадлежит заданной кривой */
  printf("Q принадлежит: %d\n", ak_wpoint_is_ok(&Q, &mycurve)); 

  /* освобождаем контекст генератора */
  ak_random_destroy( &gen );
  
  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
