/* --------------------------------------------------------------------------------- */
/* Пример example-g06n09.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_wpoint_pow()                             */
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

  /* объявляем точку R равную образующей точке эллиптической кривой*/
  struct wpoint Q;

  /* задаем степень кратности 10 */
  ak_mpzn256 k = {0x10, 0x0, 0x0, 0x0};

  /* вычисляем кратную точку эллиптической кривой k * mycurve.point и 
   * помещаем результат в Q */
  ak_wpoint_pow(&Q, &mycurve.point, k, mycurve.size, &mycurve);

  /* выводим полученную точку [k]P */  
  printf("Точка 10Q\n");
  printf("X = %s\n", ak_mpzn_to_hexstr(Q.x, mycurve.size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(Q.y, mycurve.size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(Q.z, mycurve.size));
  /* проверяем что полученная точка принадлежит кривой*/
  printf("10Q принадлежит: %d\n", ak_wpoint_is_ok(&Q, &mycurve));   

  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
