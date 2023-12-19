/* --------------------------------------------------------------------------------- */
/* Пример example-g06n01.c                                                           */
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

  if(( oid_of_target = ak_oid_find_by_ni( "sign256" )) == NULL ) goto ex;
  /* 1. создаем ключ */
  if(( key = ak_oid_new_object( oid_of_target )) == NULL ) goto ex;

 /* 2. set curve for assymetric keys */
  curve = ak_oid_find_by_name( "cspa" );
  if( ak_signkey_set_curve( key, curve->data ) != ak_error_ok ) {
    printf("error curve");
    goto ex;
  }

  oid_of_generator = ak_oid_find_by_index(1);
  if( (generator = ak_oid_new_object( oid_of_generator )) == NULL ){
    printf("generator error");
    goto ex;
 }
 /* 3. вырабатываем случайный секретный ключ */
  if( oid_of_target->func.first.set_key_random( key, generator ) != ak_error_ok ) {
    printf("secret key error");
    goto ex;
  }

 /* 4. устанавливаем срок действия, в сутках, начиная с текущего момента */
  tm.not_before = time( NULL );
  tm.not_after = tm.not_before + 10*86400;
  if( ak_skey_set_validity( key, tm.not_before, tm.not_after ) != ak_error_ok ) {
    printf("time error");
    goto ex;
  }

 /* 7.1. вырабатываем открытый ключ,
    это позволяет выработать номер открытого ключа, а также присвоить ему имя и ресурс */
  if( ak_verifykey_create_from_signkey( &cert.vkey, key ) != ak_error_ok ) {
    printf("incorrect creation of public key\n");
    goto ex;
  }

  if(( subject = ak_tlv_new_sequence()) == NULL ) {
    printf("subject error");
    goto ex;
  }
  if( ak_tlv_add_string_to_global_name( subject, "common-name", "Name)") !=
	 ak_error_ok ) {
    printf("common-name error");
    goto ex;
  }
  cert.opts.subject = subject;

  if( ak_request_export_to_file( (ak_request)( &cert ), key, generator, "req.csr",
    0, asn1_pem_format ) != ak_error_ok ) {
          printf("cannot export public key to request\n");
          goto ex;
  }

 /* 8. сохраняем созданный ключ в файле */
  if( ak_skey_export_to_file_with_password(
          key,            /* ключ */
          NULL,     /* пароль */
          0,  /* длина пароля */
          "private.key",
          0,
          asn1_pem_format
     ) != ak_error_ok ) {
     printf("private key error\n");
     goto ex;
  }
 
/* после использования необходимо удалить контекст секретного ключа */
  ex: ak_oid_delete_object( oid_of_target, key  );
  ak_oid_delete_object( oid_of_generator, generator );
  ak_request_destroy( (ak_request)( &cert ) );

 /* завершаем работу */
  if( error == ak_error_ok ) exitstatus = EXIT_SUCCESS;
  ak_libakrypt_destroy();
 return exitstatus;
}
