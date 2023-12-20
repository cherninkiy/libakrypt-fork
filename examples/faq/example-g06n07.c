/* --------------------------------------------------------------------------------- */
/* Пример example-g06n07.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_wpoint_add()                             */
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

  /* объявляем точку R, Q равные образующей точке эллиптической кривой*/
  struct wpoint Q = mycurve.point;
  struct wpoint R = mycurve.point;
  
  /* применяем функцию сложения точек Q и R, то есть получаем 2Q (Q = R) */
  ak_wpoint_add(&Q, &R, &mycurve);
  /* так как складываются одинаковые точки, то внутри функции
   * ak_wpoint_add() вызывается функция удвоение точки */
  /* применяем функцию сложения снова. теперь точки не равны, 
   * так как результат вызова функции сложения первый раз 
   * записал результат в Q */
  ak_wpoint_add(&Q, &R, &mycurve);
  /* выводим полученную точку */  
  printf("Точка 3Q\n");
  printf("X = %s\n", ak_mpzn_to_hexstr(Q.x, mycurve.size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(Q.y, mycurve.size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(Q.z, mycurve.size));
  /* проверяем что полученная точка принадлежит кривой*/
  printf("3Q принадлежит: %d\n", ak_wpoint_is_ok(&Q, &mycurve));   

  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
