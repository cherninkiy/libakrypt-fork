/* --------------------------------------------------------------------------------- */
/* Пример example-g06n08.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_wpoint_reduce()                          */
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
  struct wpoint Q = mycurve.point;

  /* выводим точку Q в проективныых координатах*/
  printf("X = %s\n", ak_mpzn_to_hexstr(Q.x, mycurve.size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(Q.y, mycurve.size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(Q.z, mycurve.size));

  /* применяем функцию удвоения точки */
  ak_wpoint_double(&Q, &mycurve);

  /* приводим проективную точку к аффиному виду */
  ak_wpoint_reduce(&Q, &mycurve);

  /* выводим полученную точку в аффинном представлении*/  
  printf("Точка Q в афинном представлении\n");
  printf("X = %s\n", ak_mpzn_to_hexstr(Q.x, mycurve.size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(Q.y, mycurve.size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(Q.z, mycurve.size));
  /* проверяем что полученная точка принадлежит кривой*/
  printf("Q принадлежит: %d\n", ak_wpoint_is_ok(&Q, &mycurve));   

  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
