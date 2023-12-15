#include <libakrypt-internal.h>

#define RotHi(x, r)         (((x) << (r)) | ((x) >> (32 - (r))))

#define U1(x)               ( (x) >> 24 )
#define U2(x)               (((x) >> 16 ) & 0xff )
#define U3(x)               (((x) >> 8  ) & 0xff )
#define U4(x)               ( (x) & 0xff )

#define HU1(x,H)            (((ak_uint32) (H)[ U1((x)) ]) << 24)
#define HU2(x,H)            (((ak_uint32) (H)[ U2((x)) ]) << 16)
#define HU3(x,H)            (((ak_uint32) (H)[ U3((x)) ]) <<  8)
#define HU4(x,H)            (((ak_uint32) (H)[ U4((x)) ]))

#define G(x,H,r)            RotHi(HU4((x),(H)) | HU3((x),(H)) | HU2((x),(H)) | HU1((x),(H)),(r))
#define SWAP(x,y)\
    do { __typeof__((x)) __tmp = (x); (x) = (y); (y) = __tmp; } while(0);


struct belt_encrypted_keys
{
    ak_uint64 belt_expanded_key[32];
};



static const ak_uint8 H[256] =
{
	0xB1, 0x94, 0xBA, 0xC8, 0x0A, 0x08, 0xF5, 0x3B, 0x36, 0x6D, 0x00, 0x8E, 0x58, 0x4A, 0x5D, 0xE4,
	0x85, 0x04, 0xFA, 0x9D, 0x1B, 0xB6, 0xC7, 0xAC, 0x25, 0x2E, 0x72, 0xC2, 0x02, 0xFD, 0xCE, 0x0D,
	0x5B, 0xE3, 0xD6, 0x12, 0x17, 0xB9, 0x61, 0x81, 0xFE, 0x67, 0x86, 0xAD, 0x71, 0x6B, 0x89, 0x0B,
	0x5C, 0xB0, 0xC0, 0xFF, 0x33, 0xC3, 0x56, 0xB8, 0x35, 0xC4, 0x05, 0xAE, 0xD8, 0xE0, 0x7F, 0x99,
	0xE1, 0x2B, 0xDC, 0x1A, 0xE2, 0x82, 0x57, 0xEC, 0x70, 0x3F, 0xCC, 0xF0, 0x95, 0xEE, 0x8D, 0xF1,
	0xC1, 0xAB, 0x76, 0x38, 0x9F, 0xE6, 0x78, 0xCA, 0xF7, 0xC6, 0xF8, 0x60, 0xD5, 0xBB, 0x9C, 0x4F,
	0xF3, 0x3C, 0x65, 0x7B, 0x63, 0x7C, 0x30, 0x6A, 0xDD, 0x4E, 0xA7, 0x79, 0x9E, 0xB2, 0x3D, 0x31,
	0x3E, 0x98, 0xB5, 0x6E, 0x27, 0xD3, 0xBC, 0xCF, 0x59, 0x1E, 0x18, 0x1F, 0x4C, 0x5A, 0xB7, 0x93,
	0xE9, 0xDE, 0xE7, 0x2C, 0x8F, 0x0C, 0x0F, 0xA6, 0x2D, 0xDB, 0x49, 0xF4, 0x6F, 0x73, 0x96, 0x47,
	0x06, 0x07, 0x53, 0x16, 0xED, 0x24, 0x7A, 0x37, 0x39, 0xCB, 0xA3, 0x83, 0x03, 0xA9, 0x8B, 0xF6,
	0x92, 0xBD, 0x9B, 0x1C, 0xE5, 0xD1, 0x41, 0x01, 0x54, 0x45, 0xFB, 0xC9, 0x5E, 0x4D, 0x0E, 0xF2,
	0x68, 0x20, 0x80, 0xAA, 0x22, 0x7D, 0x64, 0x2F, 0x26, 0x87, 0xF9, 0x34, 0x90, 0x40, 0x55, 0x11,
	0xBE, 0x32, 0x97, 0x13, 0x43, 0xFC, 0x9A, 0x48, 0xA0, 0x2A, 0x88, 0x5F, 0x19, 0x4B, 0x09, 0xA1,
	0x7E, 0xCD, 0xA4, 0xD0, 0x15, 0x44, 0xAF, 0x8C, 0xA5, 0x84, 0x50, 0xBF, 0x66, 0xD2, 0xE8, 0x8A,
	0xA2, 0xD7, 0x46, 0x52, 0x42, 0xA8, 0xDF, 0xB3, 0x69, 0x74, 0xC5, 0x51, 0xEB, 0x23, 0x29, 0x21, 
	0xD4, 0xEF, 0xD9, 0xB4, 0x3A, 0x62, 0x28, 0x75, 0x91, 0x14, 0x10, 0xEA, 0x77, 0x6C, 0xDA, 0x1D
};

static const ak_uint32 KeyIndex[8][7] =
{
	{ 0, 1, 2, 3, 4, 5, 6 },
	{ 7, 0, 1, 2, 3, 4, 5 },
	{ 6, 7, 0, 1, 2, 3, 4 },
	{ 5, 6, 7, 0, 1, 2, 3 }, 
	{ 4, 5, 6, 7, 0, 1, 2 }, 
	{ 3, 4, 5, 6, 7, 0, 1 },
	{ 2, 3, 4, 5, 6, 7, 0 },
	{ 1, 2, 3, 4, 5, 6, 7 }
};


//static void ak_belt_encrypt(ak_uint8* ks, ak_uint8* inBlock, ak_uint8* outBlock)
static void ak_belt_encrypt( ak_skey skey, ak_pointer in, ak_pointer out )
{	
	if((( skey->flags)&key_flag_set_mask ) != 0 ) {
		/* накладываем маску на ключ */
    	 size_t jdx = skey->key_size;
    	 for(size_t idx = 0; idx < skey->key_size; idx++, jdx++ ) skey->key[idx] ^= skey->key[jdx];
	}
	

	ak_uint32 a = ((ak_uint32 *) in)[0];     //((ak_uint32 *)inBlock)[0];
	ak_uint32 b = ((ak_uint32 *) in)[1];     //((ak_uint32 *)inBlock)[1];
	ak_uint32 c = ((ak_uint32 *) in)[2];     //((ak_uint32 *)inBlock)[2];
	ak_uint32 d = ((ak_uint32 *) in)[3];     //((ak_uint32 *)inBlock)[3];
	ak_uint32 e;
	int i;
	
    //ak_uint32 (*kp)[8] = ((struct belt_encrypted_keys*)(skey->data))->belt_expanded_key[i];
    ak_uint32* key = (ak_uint32*)(skey->key);

	for(i = 0; i<8; ++i)
	{				
		b ^= G((a + key[KeyIndex[i][0]]), H, 5); 
		c ^= G((d + key[KeyIndex[i][1]]), H, 21);
		a = (ak_uint32)(a - G((b + key[KeyIndex[i][2]]), H, 13));
		e = (G((b + c + key[KeyIndex[i][3]]), H, 21) ^ (ak_uint32)(i + 1));
		b += e;
		c = (ak_uint32)(c - e);
		d += G((c + key[KeyIndex[i][4]]), H, 13);
		b ^= G((a + key[KeyIndex[i][5]]), H, 21);
		c ^= G((d + key[KeyIndex[i][6]]), H, 5);
		SWAP(a, b);
		SWAP(c, d);
		SWAP(b, c);
	}

	((ak_uint32 *)out)[0] = b;
	((ak_uint32 *)out)[1] = d;
	((ak_uint32 *)out)[2] = a;
	((ak_uint32 *)out)[3] = c;

	if((( skey->flags)&key_flag_set_mask ) != 0 ) {
		/* накладываем маску на ключ */
    	 size_t jdx = skey->key_size;
    	 for(size_t idx = 0; idx < skey->key_size; idx++, jdx++ ) skey->key[idx] ^= skey->key[jdx];
	}
}

static void ak_belt_decrypt( ak_skey skey, ak_pointer in, ak_pointer out )
{
	if((( skey->flags)&key_flag_set_mask ) != 0 ) {
		/* накладываем маску на ключ */
    	 size_t jdx = skey->key_size;
    	 for(size_t idx = 0; idx < skey->key_size; idx++, jdx++ ) skey->key[idx] ^= skey->key[jdx];
    	/* меняем значение флага */
    	// skey->flags |= key_flag_set_mask;
	}

	ak_uint32 a = ((ak_uint32 *)in)[0];
	ak_uint32 b = ((ak_uint32 *)in)[1];
	ak_uint32 c = ((ak_uint32 *)in)[2];
	ak_uint32 d = ((ak_uint32 *)in)[3];
	ak_uint32 e;
	int i;
	ak_uint32 * key = (ak_uint32*)(skey->key);

	for(i = 7; i >= 0; --i)
	{
		b ^= G((a + key[KeyIndex[i][6]]), H, 5);
		c ^= G((d + key[KeyIndex[i][5]]), H, 21);
		a = (ak_uint32)(a - G((b + key[KeyIndex[i][4]]), H, 13));
		e = (G((b + c + key[KeyIndex[i][3]]), H, 21) ^ (ak_uint32)(i + 1));
		b += e;
		c = (ak_uint32)(c - e);
		d += G((c + key[KeyIndex[i][2]]), H, 13);
		b ^= G((a + key[KeyIndex[i][1]]), H, 21);
		c ^= G((d + key[KeyIndex[i][0]]), H, 5);
		SWAP(a, b);
		SWAP(c, d);
		SWAP(a, d);
	}

	((ak_uint32 *)out)[0] = c;
	((ak_uint32 *)out)[1] = a;
	((ak_uint32 *)out)[2] = d;
	((ak_uint32 *)out)[3] = b;

	if((( skey->flags)&key_flag_set_mask ) != 0 ) {
		/* накладываем маску на ключ */
    	 size_t jdx = skey->key_size;
    	 for(size_t idx = 0; idx < skey->key_size; idx++, jdx++ ) skey->key[idx] ^= skey->key[jdx];
	}
}


int ak_bckey_create_belt( ak_bckey bkey ) {
  	int error = ak_error_ok;

  	if( bkey == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
  	                                             "using null pointer to block cipher key context" );

 	/* создаем ключ алгоритма шифрования */
  	if(( error = ak_bckey_create( bkey, 32, 16 )) != ak_error_ok )
  	  return ak_error_message( error, __func__, "wrong initalization of block cipher key context" );

 	/* устанавливаем OID алгоритма шифрования */
  	if(( bkey->key.oid = ak_oid_find_by_name( "belt" )) == NULL ) {
  	  error = ak_error_get_value();
  	  ak_error_message( error, __func__, "wrong search of predefined belt block cipher OID" );
  	  ak_bckey_destroy( bkey );
  	  return error;
  	}

 	/* устанавливаем методы */
    bkey->encrypt = ak_belt_encrypt;
    bkey->decrypt = ak_belt_decrypt;
  
	return error;
}