/* --------------------------------------------------------------------------------- */
/* Пример example-g06n03.c                                                           */
/* Подпись запроса на сертификат                                                     */
/* --------------------------------------------------------------------------------- */
 #include <stdio.h>
 #include <libakrypt.h>

 int main( void )
{
 /* код ошибки, возвращаемый функциями библиотеки */
  int error = ak_error_ok;
 /* статус выполнения программы */
  int exitstatus = EXIT_FAILURE;

  ak_oid oid_of_generator = NULL;
  struct signkey issuer_skey;
  struct certificate ca_cert;
  struct certificate cert;
  ak_random generator = NULL;

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

 /* 3. импортируем секретный ключ корневого сертификата из фалйа root_private.key */
  if(( error = ak_skey_import_from_file( 
	&issuer_skey, sign_function, "root_private.key" )) != ak_error_ok) {
    goto ex;
  }

 /* 4. импортируем запрос на сертификат из файла req.csr */
  if( (error = ak_request_import_from_file((ak_request)( &cert ), "req.csr" ))
       != ak_error_ok) {
    goto ex;
  }

 /* 5. устанавливаем срок действия сертификата (в данном случае 10 дней) */
  cert.opts.time.not_before = time( NULL );
  cert.opts.time.not_after = cert.opts.time.not_before + 10*86400;

 /* 6. подписываем запрос и сохраняем сертификат */
  if( (oid_of_generator = ak_oid_find_by_index(1)) == NULL ) { 
    goto ex;
  }
  if( (generator = ak_oid_new_object( oid_of_generator )) == NULL ) {
    goto ex;
  }
  if(( error = ak_certificate_export_to_file( &cert, &issuer_skey, &ca_cert,
	generator, "cert.crt", 0, asn1_pem_format )) != ak_error_ok ) {
    goto ex;      
  }


  ex:
  ak_certificate_destroy( &ca_cert );
  ak_signkey_destroy( &issuer_skey );
  ak_oid_delete_object( oid_of_generator, generator );

 /* завершаем работу */
  if( error == ak_error_ok ) exitstatus = EXIT_SUCCESS;
  ak_libakrypt_destroy();
 return exitstatus;
}
