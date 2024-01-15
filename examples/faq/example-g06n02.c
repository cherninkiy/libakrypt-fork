/* --------------------------------------------------------------------------------- */
/* Пример example-g06n02.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_wpoint_set_as_unit()                     */
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

  /* объявляем точку */
  struct wpoint newpoint;
 
  /* присваиваем контексту значение бесконечно удаленной точки эллиптичской кривой
   * id_rfc4357_gost_3410_2001_paramSetA */
  ak_wpoint_set_as_unit(&newpoint, &mycurve);

  /* выводим полученную точку после применения функции ak_wpoint_set_as_unit() */
  printf("Бесконечно удаленная точка эллиптической кривой\n");
  printf("X = %s\n", ak_mpzn_to_hexstr(newpoint.x, ak_mpzn256_size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(newpoint.y, ak_mpzn256_size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(newpoint.z, ak_mpzn256_size));

  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
