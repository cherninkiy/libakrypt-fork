/* --------------------------------------------------------------------------------- */
/* Пример example-g06n05.c                                                           */
/* Добавление сертификата в хранилище                                                     */
/* --------------------------------------------------------------------------------- */
 #include <stdio.h>
 #include <libakrypt.h>

int main(void){
    
    ak_asn1 root = ak_asn1_new();
    char certname[FILENAME_MAX]="cert.crt";
    int exitstatus = 0;

    // сохраняем сертификат в der-формате
    if( ak_asn1_export_to_derfile(root, certname) != ak_error_ok ) {
      perror("Error\n");
      exitstatus = EXIT_FAILURE;
      goto ex;
    }

  printf("Ok\n");
  ex:
  return exitstatus;
}