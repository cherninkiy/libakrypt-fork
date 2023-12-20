/* --------------------------------------------------------------------------------- */
/* Пример example-g06n04.c                                                           */
/* Вурификация сертификата                                                           */
/* --------------------------------------------------------------------------------- */
 #include <stdio.h>
 #include <libakrypt.h>

 int main( void )
{
 /* код ошибки, возвращаемый функциями библиотеки */
  int error = ak_error_ok;
 /* статус выполнения программы */
  int exitstatus = EXIT_FAILURE;

  struct certificate ca_cert, cert;
  ak_asn1 root = NULL;


 /* инициализируем библиотеку */
  if( ak_libakrypt_create( NULL ) != ak_true ) {
    ak_libakrypt_destroy();
    return EXIT_FAILURE;
  }

 /* 1. считываем корневой сертификат из файла root_cert.crt */
  ak_certificate_opts_create(&ca_cert.opts);
  if (( error =  ak_certificate_import_from_file(
	&ca_cert, NULL, "root_cert.crt" )) != ak_error_ok ) {
    goto ex;
  }

 /* 2. проверяем, что сертификат может подписывать другие сертификаты и
  * проверять подписи */
 /* расширение `Basic Constraints` (oid: 2.5.29.19) */
  if (!ca_cert.opts.ext_ca.is_present) {
    goto ex;
  }
  if (( ca_cert.opts.ext_key_usage.bits&bit_keyCertSign ) == 0 ) {
    goto ex;
  }

 /* 3. считываем asn1 дерево проверяемого сертификата из файла */
  if(( error = ak_asn1_import_from_file(
        root = ak_asn1_new(), "cert.crt", NULL )) != ak_error_ok ) {
    goto ex;
  }

 /* 4. верифицируем сертификат */
  if( !ak_asn1_is_certificate( root ) ) {
    goto ex;
  }
  ak_certificate_opts_create( &cert.opts );
 /* если данная функция вернула ak_error_ok, то сертификат валиден */
  error = ak_certificate_import_from_asn1( &cert, &ca_cert, root );
 
  ex:
  ak_certificate_destroy( &ca_cert );
  ak_certificate_destroy( &cert );

 /* завершаем работу */
  if( error == ak_error_ok ) exitstatus = EXIT_SUCCESS;
  ak_libakrypt_destroy();
 return exitstatus;
}