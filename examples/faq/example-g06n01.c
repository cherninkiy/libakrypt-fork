/* --------------------------------------------------------------------------------- */
/* Пример example-g06n01.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_wpoint_set()                             */
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

  /* выводим образующую точку эллиптический кривой*/
  printf("Образующая точка из кривой rfc4357_gost_3410_2001_paramSetA\n");
  printf("X = %s\n", ak_mpzn_to_hexstr(mycurve.point.x, mycurve.size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(mycurve.point.y, mycurve.size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(mycurve.point.z, mycurve.size));

  /* объявляем точку с какими-либо параметрами*/
  struct wpoint newpoint = {
    {0x11L, 0x0, 0x0, 0x0},
    {0x22L, 0x0, 0x0, 0x0},
    {0x33L, 0x0, 0x0, 0x0}
  };
   
  /* выводим точку, которая была объявлена*/
  printf("Точка newpoint, которую мы объявили\n"); 
  printf("X = %s\n", ak_mpzn_to_hexstr(newpoint.x, ak_mpzn256_size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(newpoint.y, ak_mpzn256_size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(newpoint.z, ak_mpzn256_size));

  /* присваиваем контекст образующей точки эллиптической кривой*/
  ak_wpoint_set(&newpoint, &mycurve);

  /* выводим полученную точку после применения функции ak_wpoint_set() */
  printf("Точка newpoint после вызова функции\n");
  printf("X = %s\n", ak_mpzn_to_hexstr(newpoint.x, ak_mpzn256_size));
  printf("Y = %s\n", ak_mpzn_to_hexstr(newpoint.y, ak_mpzn256_size));
  printf("Z = %s\n", ak_mpzn_to_hexstr(newpoint.z, ak_mpzn256_size));

  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
