#include <libakrypt-internal.h>

static inline void sm4_4uint8_to_uint32(ak_uint32 *n,ak_uint8* b,ak_uint8 i)
{
    #ifdef AK_LITTLE_ENDIAN
        *n = bswap_32(*((ak_uint32 *)(b+i)));
    #else
        *n = *((ak_uint32 *)(b+i));
    #endif
}

static inline void sm4_uint32_to_4uint8(ak_uint32 n,ak_uint8* b,ak_uint8 i)
{
    #ifdef AK_LITTLE_ENDIAN
        *((ak_uint32 *)(b+i))=bswap_32(n);
    #else
        *((ak_uint32 *)(b+i))=n;
    #endif
}

#define  SM4_SHL(x,n) (((x) & 0xFFFFFFFF) << n)
#define SM4_ROTL(x,n) (SM4_SHL((x),n) | ((x) >> (32 - n)))

#define SM4_SWAP(a,b) { ak_uint32 t = a; a = b; b = t; t = 0; }

struct sm4_encrypted_keys
{
    ak_uint64 sm4_expanded_key[32];
};

static const ak_uint8 sm4_box[16][16] =
        {
                {0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05},
                {0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99},
                {0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62},
                {0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6},
                {0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8},
                {0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35},
                {0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87},
                {0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e},
                {0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1},
                {0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3},
                {0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f},
                {0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51},
                {0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8},
                {0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0},
                {0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84},
                {0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48}
        };

static const ak_uint32 sm4_system_param[4] = {0xa3b1bac6,0x56aa3350,0x677d9197,0xb27022dc};

static const ak_uint32 sm4_fixed_param[32] =
        {
                0x00070e15,0x1c232a31,0x383f464d,0x545b6269,
                0x70777e85,0x8c939aa1,0xa8afb6bd,0xc4cbd2d9,
                0xe0e7eef5,0xfc030a11,0x181f262d,0x343b4249,
                0x50575e65,0x6c737a81,0x888f969d,0xa4abb2b9,
                0xc0c7ced5,0xdce3eaf1,0xf8ff060d,0x141b2229,
                0x30373e45,0x4c535a61,0x686f767d,0x848b9299,
                0xa0a7aeb5,0xbcc3cad1,0xd8dfe6ed,0xf4fb0209,
                0x10171e25,0x2c333a41,0x484f565d,0x646b7279
        };

static inline ak_uint8 sm4_sbox_get(ak_uint8 order)
{
    return ((ak_uint8 *)sm4_box)[order];
}


static ak_uint32 sm4_round_encrypt(ak_uint32 x0, ak_uint32 x1, ak_uint32 x2, ak_uint32 x3, ak_uint32 round_key)
{
    ak_uint32 val = 0;
    ak_uint32 c = 0;
    ak_uint8 a[4];
    ak_uint8 b[4];
    sm4_uint32_to_4uint8((x1^x2^x3^round_key),a,0);
    b[0] = sm4_sbox_get(a[0]);
    b[1] = sm4_sbox_get(a[1]);
    b[2] = sm4_sbox_get(a[2]);
    b[3] = sm4_sbox_get(a[3]);
    sm4_4uint8_to_uint32(&val,b,0);
    c =val^(SM4_ROTL(val, 2))^(SM4_ROTL(val, 10))^(SM4_ROTL(val, 18))^(SM4_ROTL(val, 24));
    return (x0^c);
}

static ak_uint32 sm4_round_key(ak_uint32 t)
{
    ak_uint32 val = 0;
    ak_uint8 a[4];
    ak_uint8 b[4];
    sm4_uint32_to_4uint8(t,a,0);
    b[0] = sm4_sbox_get(a[0]);
    b[1] = sm4_sbox_get(a[1]);
    b[2] = sm4_sbox_get(a[2]);
    b[3] = sm4_sbox_get(a[3]);
    sm4_4uint8_to_uint32(&val,b,0);
    return val^(SM4_ROTL(val, 13))^(SM4_ROTL(val, 23));
}
/* ----------------------------------------------------------------------------------------------- */
/*! \brief Функция уничтожения развернутых ключей для sm4

    В данной функции освобождается память выделенная под ключи

    @param skey Указатель на контекст секретного ключа

    @return В случае успеха функция возвращает ak_error_ok. В противном случае,
    возвращается код ошибки.                                                                       */
/* ----------------------------------------------------------------------------------------------- */
static int ak_sm4_delete_keys (ak_skey skey)
{
    if( skey == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                "using a null pointer to secret key" );
    /* если ключ был создан, но ему не было присвоено значение, здесь возникнет ошибка */
    if( skey->data != NULL ) {
        ak_ptr_wipe( skey->data, sizeof( struct sm4_encrypted_keys), &skey->generator );
        ak_aligned_free( skey->data );
        skey->data = NULL;
    }
    return ak_error_ok;
}
/* ----------------------------------------------------------------------------------------------- */
/*! \brief Функция выработки ранудовых ключей для sm4.

    @param skey Указатель на контекст секретного ключа

    @return В случае успеха функция возвращает \ref ak_error_ok. В противном случае,
    возвращается код ошибки.                                                                       */
/* ----------------------------------------------------------------------------------------------- */
static int ak_sm4_schedule_keys(ak_skey skey)
{
    struct sm4_encrypted_keys *data = NULL;

    if( skey == NULL ) return ak_error_message( ak_error_null_pointer, __func__ ,
                                                "using a null pointer to secret key" );
    /* проверяем целостность ключа */
    if( skey->check_icode( skey ) != ak_true ) return ak_error_message( ak_error_wrong_key_icode,
                                                                        __func__ , "using key with wrong integrity code" );
    /* удаляем былое */
    if( skey->data != NULL ) ak_sm4_delete_keys( skey );

    if(( data = ak_aligned_malloc( sizeof( struct sm4_encrypted_keys ))) == NULL )
        return ak_error_message( ak_error_out_of_memory, __func__, "incorrect memory allocation" );

    /* выставляем флаги того, что память выделена */
    memset( data, 0, sizeof( struct sm4_encrypted_keys ));
    skey->data = ( ak_pointer )data;
    skey->flags |= key_flag_data_not_free;
    ak_uint32 MK[4];
    ak_uint32 mMK[4];
    ak_uint32 k[36];
    sm4_4uint8_to_uint32( &MK[0], (ak_uint8*)skey->key, 0 );
    sm4_4uint8_to_uint32( &MK[1], (ak_uint8*)skey->key, 4 );
    sm4_4uint8_to_uint32( &MK[2], (ak_uint8*)skey->key, 8 );
    sm4_4uint8_to_uint32( &MK[3], (ak_uint8*)skey->key, 12 );
    sm4_4uint8_to_uint32( &mMK[0], (ak_uint8*)skey->key+ skey->key_size, 0 );
    sm4_4uint8_to_uint32( &mMK[1], (ak_uint8*)skey->key+ skey->key_size, 4 );
    sm4_4uint8_to_uint32( &mMK[2], (ak_uint8*)skey->key+ skey->key_size, 8 );
    sm4_4uint8_to_uint32( &mMK[3], (ak_uint8*)skey->key+ skey->key_size, 12 );
    k[0] = mMK[0]^MK[0]^sm4_system_param[0];
    k[1] = mMK[1]^MK[1]^sm4_system_param[1];
    k[2] = mMK[2]^MK[2]^sm4_system_param[2];
    k[3] = mMK[3]^MK[3]^sm4_system_param[3];
    for(size_t i = 0; i<32; i++)
    {
        k[i+4] = k[i] ^ (sm4_round_key(k[i+1]^k[i+2]^k[i+3]^sm4_fixed_param[i]));
        data->sm4_expanded_key[i] = k[i+4];
    }
    return ak_error_ok;
}
/* ----------------------------------------------------------------------------------------------- */
/*! \brief Функция зашифрования одного блока информации алгоритмом sm4.

    @param skey Контекст секретного ключа.
    @param in Блок входной информации (открытый текст).
    @param out Блок выходной информации (шифртекст).                                               */
/* ----------------------------------------------------------------------------------------------- */
static void ak_sm4_encrypt( ak_skey skey, ak_pointer in, ak_pointer out )
{
    ak_uint32 i = 0;
    ak_uint32 buf[36];
    memset(buf, 0, sizeof(buf));
    sm4_4uint8_to_uint32( &buf[0], (ak_uint8*)in, 0 );
    sm4_4uint8_to_uint32( &buf[1], (ak_uint8*)in, 4 );
    sm4_4uint8_to_uint32( &buf[2], (ak_uint8*)in, 8 );
    sm4_4uint8_to_uint32( &buf[3], (ak_uint8*)in, 12 );
    while(i<32)
    {
        buf[i+4] = sm4_round_encrypt(buf[i], buf[i+1], buf[i+2], buf[i+3], ((struct sm4_encrypted_keys*)(skey->data))->sm4_expanded_key[i]);
        i++;
    }
    sm4_uint32_to_4uint8(buf[35],(ak_uint8*)out,0);
    sm4_uint32_to_4uint8(buf[34],(ak_uint8*)out,4);
    sm4_uint32_to_4uint8(buf[33],(ak_uint8*)out,8);
    sm4_uint32_to_4uint8(buf[32],(ak_uint8*)out,12);
}
/* ----------------------------------------------------------------------------------------------- */
/*! \brief Функция расшифрования одного блока информации алгоритмом sm4.

    @param skey Контекст секретного ключа.
    @param in Блок входной информации (открытый текст).
    @param out Блок выходной информации (шифртекст).                                               */
/* ----------------------------------------------------------------------------------------------- */
static void ak_sm4_decrypt( ak_skey skey, ak_pointer in, ak_pointer out )
{
    ak_uint32 i = 0;
    ak_uint32 buf[36];
    memset(buf, 0, sizeof(buf));
    sm4_4uint8_to_uint32( &buf[0], (ak_uint8*)in, 0 );
    sm4_4uint8_to_uint32( &buf[1], (ak_uint8*)in, 4 );
    sm4_4uint8_to_uint32( &buf[2], (ak_uint8*)in, 8 );
    sm4_4uint8_to_uint32( &buf[3], (ak_uint8*)in, 12 );
    while(i<32)
    {
        buf[i+4] = sm4_round_encrypt(buf[i], buf[i+1], buf[i+2], buf[i+3], ((struct sm4_encrypted_keys*)(skey->data))->sm4_expanded_key[31-i]);
        i++;
    }
    sm4_uint32_to_4uint8(buf[35],(ak_uint8*)out,0);
    sm4_uint32_to_4uint8(buf[34],(ak_uint8*)out,4);
    sm4_uint32_to_4uint8(buf[33],(ak_uint8*)out,8);
    sm4_uint32_to_4uint8(buf[32],(ak_uint8*)out,12);
}

/* ----------------------------------------------------------------------------------------------- */
/*! Функция инициализируете контекст ключа алгоритма блочного шифрования sm4.
    После инициализации устанавливаются обработчики (функции класса). Однако само значение
    ключу не присваивается - поле `bkey->key` остается неопределенным.

    @param bkey Контекст секретного ключа алгоритма блочного шифрования.

    @return Функция возвращает код ошибки. В случаее успеха возвращается \ref ak_error_ok.         */
/* ----------------------------------------------------------------------------------------------- */
int ak_bckey_create_sm4( ak_bckey bkey )
{
    int error = ak_error_ok;
    if( bkey == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                "using null pointer to block cipher key context" );

    /* создаем ключ алгоритма шифрования и определяем его методы */
    if(( error = ak_bckey_create( bkey, 16, 16 )) != ak_error_ok )
        return ak_error_message( error, __func__, "wrong initalization of block cipher key context" );

    /* устанавливаем OID алгоритма шифрования */
    if(( bkey->key.oid = ak_oid_find_by_name( "sm4" )) == NULL ) {
        error = ak_error_get_value();
        ak_error_message( error, __func__, "wrong search of predefined sm4 block cipher OID" );
        ak_bckey_destroy( bkey );
        return error;
    }
    /* ресурс ключа устанавливается в момент присвоения ключа */

    /* устанавливаем методы */
    bkey->schedule_keys = ak_sm4_schedule_keys;
    bkey->delete_keys = ak_sm4_delete_keys;
    bkey->encrypt = ak_sm4_encrypt;
    bkey->decrypt = ak_sm4_decrypt;
    return error;
}
