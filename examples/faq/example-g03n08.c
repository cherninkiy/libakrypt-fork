/* ----------------------------------------------------------------------------------------------- */
/* Выработка имитовставки                                                                          */
/* ----------------------------------------------------------------------------------------------- */

#include <stdio.h>
#include <libakrypt.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    int inlen=13;
    unsigned char key[32]={0xe9, 0xde, 0xe7, 0x2c, 0x8f, 0x0c, 0x0f, 0xa6,
                            0x2d, 0xdb, 0x49, 0xf4, 0x6f, 0x73, 0x96, 0x47,
                            0x06, 0x07, 0x53, 0x16, 0xed, 0x24, 0x7a, 0x37, 
                            0x39, 0xcb, 0xa3, 0x83, 0x03, 0xa9, 0x8b, 0xf6};
	
    unsigned char plain_text[13]={0xb1, 0x94, 0xba, 0xc8, 0x0a, 0x08, 0xf5, 0x3b,
                                    0x36, 0x6d, 0x00, 0x8e, 0x58};
    unsigned char im_exp[8]={0x72, 0x60, 0xda, 0x60, 0x13, 0x8f, 0x96, 0xc9};                              
    
    unsigned char out[16];
    unsigned char in[16];

    printf("Key:\n");
    for (int i =0;i<32;i++){
        printf("%x ",key[i]);
    }
    printf("\n\n");

    printf("Plaintext:\n");
    for (int i =0;i<13;i++){
        printf("%x ",plain_text[i]);
    }
    printf("\n\n");
    printf("Imito (expected):\n");
    for (int i =0;i<8;i++){
        printf("%x ",im_exp[i]);
    }
    printf("\n\n");

	struct bckey ctx;
	ak_libakrypt_create(NULL);
    ak_bckey_create_oid(&ctx,ak_oid_find_by_name("belt"));

    int re=ak_bckey_set_key(&ctx,key,32);
    ak_bckey_cmac(&ctx,&plain_text,inlen,&out,8);
    printf("Imito (after):\n");
    for (int i = 0; i < 8; i ++) {
        printf("%x ", out[i]);
    }
    printf("\n\n");

	ak_bckey_destroy(&ctx);
	ak_libakrypt_destroy();
    
	return 0;
}
