/* --------------------------------------------------------------------------------- */
/* Пример example-g06n11.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_wcurve_discriminant_is_ok()           */
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
  
  /* проверяем дискриминант кривой */
  printf("Проверка дискриминанта: %d\n", ak_wcurve_discriminant_is_ok(&mycurve));
  
  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
