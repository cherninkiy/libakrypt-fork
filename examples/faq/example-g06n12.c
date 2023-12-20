/* --------------------------------------------------------------------------------- */
/* Пример example-g06n12.c                                                           */ 
/*                                                                                   */
/* Простейшая иллюстрация вызова функции ak_wcurve_check_order_parameters()          */
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
  
  /* проверяем корректность параметров необходимых для вычисления по модулю q*/
  printf("Проверка параметров: %d\n", ak_wcurve_check_order_parameters(&mycurve));
  
  /* завершаем работу */
  ak_libakrypt_destroy();

  return EXIT_SUCCESS;
}
