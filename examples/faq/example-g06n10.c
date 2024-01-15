/* --------------------------------------------------------------------------------- */
/* Пример example-g06n10.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_mpzn_set_wcurve_discriminant()           */
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
  
  /* объявляем вычет, в который будет помещено значение*/
  ak_mpzn256 d;

  /* применяем функцию вычисления дискриминанта кривой */
  ak_mpzn_set_wcurve_discriminant(d, &mycurve);
  
  /* выводим значение дискриминанта */
  printf("d = %s\n", ak_mpzn_to_hexstr(d, ak_mpzn256_size));

  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
