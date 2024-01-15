/* --------------------------------------------------------------------------------- */
/* Пример example-g06n05.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_wpoint_check_order()                     */
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

  /* объявляем точку R*/
  struct wpoint R;
  /* объявляем точку Q равную образующей точки эллиптической кривой */
  struct wpoint Q = mycurve.point;

  /* проверяем порядок точек */
  printf("порядок R верный: %d\n", ak_wpoint_check_order(&R, &mycurve));  
  printf("порядок Q верный: %d\n", ak_wpoint_check_order(&Q, &mycurve));  

  /* применяем функцию умножения точки Q, то есть получаем 2Q */
  ak_wpoint_double(&Q, &mycurve);

  /* проверяем порядок точки */
  printf("порядок 2Q верный: %d\n", ak_wpoint_check_order(&Q, &mycurve));  

  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
