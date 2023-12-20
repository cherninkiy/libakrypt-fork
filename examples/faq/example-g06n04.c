/* --------------------------------------------------------------------------------- */
/* Пример example-g06n04.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_wpoint_is_ok()                           */
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
  /* объявляем точку равную образующей точки эллиптической кривой */
  struct wpoint Q = mycurve.point;

  /* проверяем принадлежность точек для заданной кривой */
  printf("R принадлежит: %d\n", ak_wpoint_is_ok(&R, &mycurve));  
  printf("Q принадлежит: %d\n", ak_wpoint_is_ok(&Q, &mycurve));  

  /* применяем функцию умножения точки Q, то есть получаем 2Q */
  ak_wpoint_double(&Q, &mycurve);

  /* проверяем принадлежность точки для заданной кривой */
  printf("2Q принадлежит: %d\n", ak_wpoint_is_ok(&Q, &mycurve));  

  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
