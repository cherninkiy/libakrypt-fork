/* --------------------------------------------------------------------------------- */
/* Пример example-g06n06.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_wpoint_double()                          */
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

  /* объявляем точку Q равную образующей точке эллиптической кривой*/
  struct wpoint Q = mycurve.point;
  
  /* применяем функцию удвоения точки Q, то есть получаем 2Q */
  ak_wpoint_double(&Q, &mycurve);
 
  /* выводим полученную точку */  
  printf("Точка 2Q\n");
  printf("X = %s\n", ak_mpzn_to_hexstr(Q.x, mycurve.size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(Q.y, mycurve.size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(Q.z, mycurve.size));
  /* проверяем что полученная точка принадлежит кривой*/
  printf("2Q принадлежит: %d\n", ak_wpoint_is_ok(&Q, &mycurve));   

  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
