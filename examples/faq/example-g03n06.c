/* ----------------------------------------------------------------------------------------------- */
/* Режим простой замены с зацеплением                                                              */
/* ----------------------------------------------------------------------------------------------- */

#include <stdio.h>
#include <libakrypt.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    int inlen=16;
    unsigned char key[32]={0xe9, 0xde, 0xe7, 0x2c, 0x8f, 0x0c, 0x0f, 0xa6,
                            0x2d, 0xdb, 0x49, 0xf4, 0x6f, 0x73, 0x96, 0x47,
                            0x06, 0x07, 0x53, 0x16, 0xed, 0x24, 0x7a, 0x37, 
                            0x39, 0xcb, 0xa3, 0x83, 0x03, 0xa9, 0x8b, 0xf6};
	
    unsigned char plain_text[16]={0xb1, 0x94, 0xba, 0xc8, 0x0a, 0x08, 0xf5, 0x3b,
                                    0x36, 0x6d, 0x00, 0x8e, 0x58, 0x4a, 0x5d, 0xe4};
    unsigned char cipher_text[16]={0x10, 0x11, 0x6e, 0xfa, 0xe6, 0xad, 0x58, 0xee,
                                    0x14, 0x85, 0x2e, 0x11, 0xda, 0x1b, 0x8a, 0x74};

    unsigned char sync_p[16]={0xbe, 0x32, 0x97, 0x13, 0x43, 0xfc, 0x9a, 0x48,
                                    0xa0, 0x2a, 0x88, 0x5f, 0x19, 0x4b, 0x09, 0xa1};                                
    
    unsigned char out[16];
    unsigned char in[16];

    printf("Key:\n");
    for (int i =0;i<32;i++){
        printf("%x ",key[i]);
    }
    printf("\n\n");
    printf("Sync_p:\n");
    for (int i =0;i<16;i++){
        printf("%x ",sync_p[i]);
    }
    printf("\n\n");

    printf("Plaintext (before encryption):\n");
    for (int i =0;i<16;i++){
        printf("%x ",plain_text[i]);
    }
    printf("\n\n");
    printf("Ciphertext (expected, before encryption):\n");
    for (int i =0;i<16;i++){
        printf("%x ",cipher_text[i]);
    }
    printf("\n\n");

	struct bckey ctx;
	ak_libakrypt_create(NULL);
    ak_bckey_create_oid(&ctx,ak_oid_find_by_name("belt"));

    int re=ak_bckey_set_key(&ctx,key,32);
    ak_bckey_encrypt_cbc(&ctx,&plain_text,&out,inlen,&sync_p,16);
    printf("Ciphertext (after encryption):\n");
    for (int i = 0; i < inlen; i ++) {
        printf("%x ", out[i]);
    }
    printf("\n\n");

    ak_bckey_decrypt_cbc(&ctx,&out,&in,inlen,&sync_p,16);
    printf("Plaintext (after encryption and decryption):\n");
    for (int i = 0; i < inlen; i ++) {
        printf("%x ", in[i]);
    }
    putchar('\n');

	ak_bckey_destroy(&ctx);
	ak_libakrypt_destroy();
    
	return 0;
}
