/* --------------------------------------------------------------------------------- */
/* Пример example-g06n03.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_wpoint_set_wpoint()                      */
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

  /* объявляем точки R и Q равные образующей точке эллиптической кривой*/
  struct wpoint R = mycurve.point;
  struct wpoint Q = mycurve.point;

  /* выводим образующую точку эллиптической кривой */
  printf("Образующая точка \n");
  printf("X = %s\n", ak_mpzn_to_hexstr(mycurve.point.x, mycurve.size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(mycurve.point.y, mycurve.size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(mycurve.point.z, mycurve.size));

  /* применяем функцию умножения точки Q, то есть получаем 2Q */
  ak_wpoint_double(&Q, &mycurve);
  printf("Точка Q после удвоения (т.е 2Q)\n");
  printf("X = %s\n", ak_mpzn_to_hexstr(Q.x, mycurve.size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(Q.y, mycurve.size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(Q.z, mycurve.size));

  /* присваем контексту R значение точки Q, т.е R = 2Q */
  ak_wpoint_set_wpoint(&R, &Q, &mycurve);

  /* выводим полученную точку после применения функции ak_wpoint_set_wpoint() */
  printf("Точка R = 2Q после выполнения функции\n");
  printf("X = %s\n", ak_mpzn_to_hexstr(R.x, mycurve.size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(R.y, mycurve.size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(R.z, mycurve.size));
  
  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
