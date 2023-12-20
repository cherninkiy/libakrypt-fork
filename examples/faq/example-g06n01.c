/* --------------------------------------------------------------------------------- */
/* Пример example-g06n01.c                                                           */
/* Создание запроса на подпись сертификата                                           */
/* --------------------------------------------------------------------------------- */
 #include <stdio.h>
 #include <libakrypt.h>

 int main( void )
{
 /* код ошибки, возвращаемый функциями библиотеки */
  int error = ak_error_ok;
 /* статус выполнения программы */
  int exitstatus = EXIT_FAILURE;

  struct certificate cert;
  time_interval_t tm;
  ak_oid oid_of_target = NULL;
  ak_pointer key = NULL;
  ak_oid curve = NULL;
  ak_random generator = NULL;
  ak_oid oid_of_generator = NULL;
  ak_tlv subject = NULL;

 /* инициализируем библиотеку */
  if( ak_libakrypt_create( NULL ) != ak_true ) {
    ak_libakrypt_destroy();
    return EXIT_FAILURE;
  }

 /* 1. создаем ключ */
  if(( oid_of_target = ak_oid_find_by_ni( "sign256" )) == NULL ) goto ex;
  if(( key = ak_oid_new_object( oid_of_target )) == NULL ) goto ex;

 /* 2. задаем кривую для ассиметричных ключей */
  curve = ak_oid_find_by_name( "cspa" );
  if(( error = ak_signkey_set_curve( key, curve->data )) != ak_error_ok ) {
    goto ex;
  }

 /* 3. вырабатываем случайный секретный ключ */
  oid_of_generator = ak_oid_find_by_index(1);
  if( (generator = ak_oid_new_object( oid_of_generator )) == NULL ) {
    goto ex;
  }
  if( oid_of_target->func.first.set_key_random( key, generator ) != ak_error_ok ) {
    goto ex;
  }

 /* 4. устанавливаем срок действия, в сутках, начиная с текущего момента */
  tm.not_before = time( NULL );
  tm.not_after = tm.not_before + 10*86400;
  if( ak_skey_set_validity( key, tm.not_before, tm.not_after ) != ak_error_ok ) {
    goto ex;
  }

 /* 5. вырабатываем открытый ключ, соответствующий ранее сгенерированному ключу */
  if(( error = ak_verifykey_create_from_signkey( &cert.vkey, key )) != ak_error_ok ) {
    goto ex;
  }

 /* 6.  Заполняем информацию о владельце сертификата (в данном случае только CN) */
  if(( subject = ak_tlv_new_sequence()) == NULL ) {
    goto ex;
  }
  if(( error = ak_tlv_add_string_to_global_name(
	 subject, "common-name", "Example Name")) != ak_error_ok ) {
    goto ex;
  }
  cert.opts.subject = subject;

 /* 7. создаем запрос на подпись сертификата */
  if(( error = ak_request_export_to_file( (ak_request)( &cert ), key,
	 generator, "req.csr", 0, asn1_pem_format )) != ak_error_ok ) {
          goto ex;
  }

 /* 8. сохраняем созданный ключ в файле */
  if(( error = ak_skey_export_to_file_with_password(
          key,
          NULL,          /* пароль */
          0,             /* длина пароля */
          "private.key", /* имя файла, куда будет записан секретный ключ */
          0,
          asn1_pem_format
     )) != ak_error_ok ) {
     goto ex;
  }
 
  ex: ak_oid_delete_object( oid_of_target, key  );
  ak_oid_delete_object( oid_of_generator, generator );
  ak_request_destroy( (ak_request)( &cert ) );

 /* завершаем работу */
  if( error == ak_error_ok ) exitstatus = EXIT_SUCCESS;
  ak_libakrypt_destroy();
 return exitstatus;
}
