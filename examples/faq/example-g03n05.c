#include <stdio.h>
#include <libakrypt.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[]){
    unsigned char key[16]={0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
	int inlen=16;
    unsigned char inbuffer[16]={0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    unsigned char outbuffer[16]={0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    printf("Key:\n");
    for (int i =0;i<16;i++){
        printf("%x ",key[i]);
    }
    printf("\n");
    printf("Plaintext (before encryption):\n");
    for (int i =0;i<16;i++){
        printf("%x ",inbuffer[i]);
    }
    printf("\n");
	struct bckey ctx;
	ak_libakrypt_create(NULL);
    ak_bckey_create_oid(&ctx,ak_oid_find_by_name("sm4"));
    int re=ak_bckey_set_key(&ctx,key,16);
    ak_bckey_encrypt_ecb(&ctx,&inbuffer,&outbuffer,inlen);
    printf("Ciphertext:\n");
    for (int i = 0; i < inlen; i ++) {
        printf("%x ", outbuffer[i]);
    }
    putchar('\n');
    ak_bckey_decrypt_ecb(&ctx,&outbuffer,&inbuffer,inlen);
    printf("Palintext (after encryption and decryption):\n");
    for (int i = 0; i < inlen; i ++) {
        printf("%x ", inbuffer[i]);
    }
    putchar('\n');
	ak_bckey_destroy(&ctx);
	ak_libakrypt_destroy();
	return 0;
}
