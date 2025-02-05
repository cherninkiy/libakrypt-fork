/* ----------------------------------------------------------------------------------------------- */
/*                                                                                                 */
/*  Реализация хэширования сообщения                                                               */
/*  из Белорусского стандарта СТБ 34.101.31-2020                                                   */
/*                                                                                                 */
/* ----------------------------------------------------------------------------------------------- */

#include <libakrypt-internal.h>
#include <stdio.h>

//---------------------------------------------------------
//----------------------belt-block-------------------------
//---------------------------------------------------------

//подстановка битов
#define H16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)\
  0x##a, 0x##b, 0x##c, 0x##d, 0x##e, 0x##f, 0x##g, 0x##h,\
  0x##i, 0x##j, 0x##k, 0x##l, 0x##m, 0x##n, 0x##o, 0x##p

//базовая H
static const ak_uint8 H[256] = {
  H16(B1,94,BA,C8,0A,08,F5,3B,36,6D,00,8E,58,4A,5D,E4),
  H16(85,04,FA,9D,1B,B6,C7,AC,25,2E,72,C2,02,FD,CE,0D),
  H16(5B,E3,D6,12,17,B9,61,81,FE,67,86,AD,71,6B,89,0B),
  H16(5C,B0,C0,FF,33,C3,56,B8,35,C4,05,AE,D8,E0,7F,99),
  H16(E1,2B,DC,1A,E2,82,57,EC,70,3F,CC,F0,95,EE,8D,F1),
  H16(C1,AB,76,38,9F,E6,78,CA,F7,C6,F8,60,D5,BB,9C,4F),
  H16(F3,3C,65,7B,63,7C,30,6A,DD,4E,A7,79,9E,B2,3D,31),
  H16(3E,98,B5,6E,27,D3,BC,CF,59,1E,18,1F,4C,5A,B7,93),
  H16(E9,DE,E7,2C,8F,0C,0F,A6,2D,DB,49,F4,6F,73,96,47),
  H16(06,07,53,16,ED,24,7A,37,39,CB,A3,83,03,A9,8B,F6),
  H16(92,BD,9B,1C,E5,D1,41,01,54,45,FB,C9,5E,4D,0E,F2),
  H16(68,20,80,AA,22,7D,64,2F,26,87,F9,34,90,40,55,11),
  H16(BE,32,97,13,43,FC,9A,48,A0,2A,88,5F,19,4B,09,A1),
  H16(7E,CD,A4,D0,15,44,AF,8C,A5,84,50,BF,66,D2,E8,8A),
  H16(A2,D7,46,52,42,A8,DF,B3,69,74,C5,51,EB,23,29,21),
  H16(D4,EF,D9,B4,3A,62,28,75,91,14,10,EA,77,6C,DA,1D),
};

const ak_uint8* beltH()
{
  return H;
}

//rotl
#define ROTL32(a, r) \
  ((ak_uint32)0x##a << r | (ak_uint32)0x##a >> (32 - r))

#define ROTL32x16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p, r)\
  ROTL32(a, r), ROTL32(b, r), ROTL32(c, r), ROTL32(d, r),\
  ROTL32(e, r), ROTL32(f, r), ROTL32(g, r), ROTL32(h, r),\
  ROTL32(i, r), ROTL32(j, r), ROTL32(k, r), ROTL32(l, r),\
  ROTL32(m, r), ROTL32(n, r), ROTL32(o, r), ROTL32(p, r)

//rotl(h)
static const ak_uint32 H5[256] = {
  ROTL32x16(B1,94,BA,C8,0A,08,F5,3B,36,6D,00,8E,58,4A,5D,E4, 5),
  ROTL32x16(85,04,FA,9D,1B,B6,C7,AC,25,2E,72,C2,02,FD,CE,0D, 5),
  ROTL32x16(5B,E3,D6,12,17,B9,61,81,FE,67,86,AD,71,6B,89,0B, 5),
  ROTL32x16(5C,B0,C0,FF,33,C3,56,B8,35,C4,05,AE,D8,E0,7F,99, 5),
  ROTL32x16(E1,2B,DC,1A,E2,82,57,EC,70,3F,CC,F0,95,EE,8D,F1, 5),
  ROTL32x16(C1,AB,76,38,9F,E6,78,CA,F7,C6,F8,60,D5,BB,9C,4F, 5),
  ROTL32x16(F3,3C,65,7B,63,7C,30,6A,DD,4E,A7,79,9E,B2,3D,31, 5),
  ROTL32x16(3E,98,B5,6E,27,D3,BC,CF,59,1E,18,1F,4C,5A,B7,93, 5),
  ROTL32x16(E9,DE,E7,2C,8F,0C,0F,A6,2D,DB,49,F4,6F,73,96,47, 5),
  ROTL32x16(06,07,53,16,ED,24,7A,37,39,CB,A3,83,03,A9,8B,F6, 5),
  ROTL32x16(92,BD,9B,1C,E5,D1,41,01,54,45,FB,C9,5E,4D,0E,F2, 5),
  ROTL32x16(68,20,80,AA,22,7D,64,2F,26,87,F9,34,90,40,55,11, 5),
  ROTL32x16(BE,32,97,13,43,FC,9A,48,A0,2A,88,5F,19,4B,09,A1, 5),
  ROTL32x16(7E,CD,A4,D0,15,44,AF,8C,A5,84,50,BF,66,D2,E8,8A, 5),
  ROTL32x16(A2,D7,46,52,42,A8,DF,B3,69,74,C5,51,EB,23,29,21, 5),
  ROTL32x16(D4,EF,D9,B4,3A,62,28,75,91,14,10,EA,77,6C,DA,1D, 5),
};

static const ak_uint32 H13[256] = {
  ROTL32x16(B1,94,BA,C8,0A,08,F5,3B,36,6D,00,8E,58,4A,5D,E4, 13),
  ROTL32x16(85,04,FA,9D,1B,B6,C7,AC,25,2E,72,C2,02,FD,CE,0D, 13),
  ROTL32x16(5B,E3,D6,12,17,B9,61,81,FE,67,86,AD,71,6B,89,0B, 13),
  ROTL32x16(5C,B0,C0,FF,33,C3,56,B8,35,C4,05,AE,D8,E0,7F,99, 13),
  ROTL32x16(E1,2B,DC,1A,E2,82,57,EC,70,3F,CC,F0,95,EE,8D,F1, 13),
  ROTL32x16(C1,AB,76,38,9F,E6,78,CA,F7,C6,F8,60,D5,BB,9C,4F, 13),
  ROTL32x16(F3,3C,65,7B,63,7C,30,6A,DD,4E,A7,79,9E,B2,3D,31, 13),
  ROTL32x16(3E,98,B5,6E,27,D3,BC,CF,59,1E,18,1F,4C,5A,B7,93, 13),
  ROTL32x16(E9,DE,E7,2C,8F,0C,0F,A6,2D,DB,49,F4,6F,73,96,47, 13),
  ROTL32x16(06,07,53,16,ED,24,7A,37,39,CB,A3,83,03,A9,8B,F6, 13),
  ROTL32x16(92,BD,9B,1C,E5,D1,41,01,54,45,FB,C9,5E,4D,0E,F2, 13),
  ROTL32x16(68,20,80,AA,22,7D,64,2F,26,87,F9,34,90,40,55,11, 13),
  ROTL32x16(BE,32,97,13,43,FC,9A,48,A0,2A,88,5F,19,4B,09,A1, 13),
  ROTL32x16(7E,CD,A4,D0,15,44,AF,8C,A5,84,50,BF,66,D2,E8,8A, 13),
  ROTL32x16(A2,D7,46,52,42,A8,DF,B3,69,74,C5,51,EB,23,29,21, 13),
  ROTL32x16(D4,EF,D9,B4,3A,62,28,75,91,14,10,EA,77,6C,DA,1D, 13),
};

static const ak_uint32 H21[256] = {
  ROTL32x16(B1,94,BA,C8,0A,08,F5,3B,36,6D,00,8E,58,4A,5D,E4, 21),
  ROTL32x16(85,04,FA,9D,1B,B6,C7,AC,25,2E,72,C2,02,FD,CE,0D, 21),
  ROTL32x16(5B,E3,D6,12,17,B9,61,81,FE,67,86,AD,71,6B,89,0B, 21),
  ROTL32x16(5C,B0,C0,FF,33,C3,56,B8,35,C4,05,AE,D8,E0,7F,99, 21),
  ROTL32x16(E1,2B,DC,1A,E2,82,57,EC,70,3F,CC,F0,95,EE,8D,F1, 21),
  ROTL32x16(C1,AB,76,38,9F,E6,78,CA,F7,C6,F8,60,D5,BB,9C,4F, 21),
  ROTL32x16(F3,3C,65,7B,63,7C,30,6A,DD,4E,A7,79,9E,B2,3D,31, 21),
  ROTL32x16(3E,98,B5,6E,27,D3,BC,CF,59,1E,18,1F,4C,5A,B7,93, 21),
  ROTL32x16(E9,DE,E7,2C,8F,0C,0F,A6,2D,DB,49,F4,6F,73,96,47, 21),
  ROTL32x16(06,07,53,16,ED,24,7A,37,39,CB,A3,83,03,A9,8B,F6, 21),
  ROTL32x16(92,BD,9B,1C,E5,D1,41,01,54,45,FB,C9,5E,4D,0E,F2, 21),
  ROTL32x16(68,20,80,AA,22,7D,64,2F,26,87,F9,34,90,40,55,11, 21),
  ROTL32x16(BE,32,97,13,43,FC,9A,48,A0,2A,88,5F,19,4B,09,A1, 21),
  ROTL32x16(7E,CD,A4,D0,15,44,AF,8C,A5,84,50,BF,66,D2,E8,8A, 21),
  ROTL32x16(A2,D7,46,52,42,A8,DF,B3,69,74,C5,51,EB,23,29,21, 21),
  ROTL32x16(D4,EF,D9,B4,3A,62,28,75,91,14,10,EA,77,6C,DA,1D, 21),
};

static const ak_uint32 H29[256] = {
  ROTL32x16(B1,94,BA,C8,0A,08,F5,3B,36,6D,00,8E,58,4A,5D,E4, 29),
  ROTL32x16(85,04,FA,9D,1B,B6,C7,AC,25,2E,72,C2,02,FD,CE,0D, 29),
  ROTL32x16(5B,E3,D6,12,17,B9,61,81,FE,67,86,AD,71,6B,89,0B, 29),
  ROTL32x16(5C,B0,C0,FF,33,C3,56,B8,35,C4,05,AE,D8,E0,7F,99, 29),
  ROTL32x16(E1,2B,DC,1A,E2,82,57,EC,70,3F,CC,F0,95,EE,8D,F1, 29),
  ROTL32x16(C1,AB,76,38,9F,E6,78,CA,F7,C6,F8,60,D5,BB,9C,4F, 29),
  ROTL32x16(F3,3C,65,7B,63,7C,30,6A,DD,4E,A7,79,9E,B2,3D,31, 29),
  ROTL32x16(3E,98,B5,6E,27,D3,BC,CF,59,1E,18,1F,4C,5A,B7,93, 29),
  ROTL32x16(E9,DE,E7,2C,8F,0C,0F,A6,2D,DB,49,F4,6F,73,96,47, 29),
  ROTL32x16(06,07,53,16,ED,24,7A,37,39,CB,A3,83,03,A9,8B,F6, 29),
  ROTL32x16(92,BD,9B,1C,E5,D1,41,01,54,45,FB,C9,5E,4D,0E,F2, 29),
  ROTL32x16(68,20,80,AA,22,7D,64,2F,26,87,F9,34,90,40,55,11, 29),
  ROTL32x16(BE,32,97,13,43,FC,9A,48,A0,2A,88,5F,19,4B,09,A1, 29),
  ROTL32x16(7E,CD,A4,D0,15,44,AF,8C,A5,84,50,BF,66,D2,E8,8A, 29),
  ROTL32x16(A2,D7,46,52,42,A8,DF,B3,69,74,C5,51,EB,23,29,21, 29),
  ROTL32x16(D4,EF,D9,B4,3A,62,28,75,91,14,10,EA,77,6C,DA,1D, 29),
};

//gr(x)
#define G5(x)\
  H5[(x) & 255] ^ H13[(x) >> 8 & 255] ^ \
  H21[(x) >> 16 & 255] ^ H29[(x) >> 24]

#define G13(x)\
  H13[(x) & 255] ^ H21[(x) >> 8 & 255] ^ \
  H29[(x) >> 16 & 255] ^ H5[(x) >> 24]

#define G21(x)\
  H21[(x) & 255] ^ H29[(x) >> 8 & 255] ^ \
  H5[(x) >> 16 & 255] ^ H13[(x) >> 24]

//1 раунд
#define R(a, b, c, d, K, i, subkey)\
  *b ^= G5(*a + subkey(K, i, 0));\
  *c ^= G21(*d + subkey(K, i, 1));\
  *a -= G13(*b + subkey(K, i, 2));\
  *c += *b;\
  *b += G21(*c + subkey(K, i, 3)) ^ i;\
  *c -= *b;\
  *d += G13(*c + subkey(K, i, 4));\
  *b ^= G21(*a + subkey(K, i, 5));\
  *c ^= G5(*d + subkey(K, i, 6));\

//ключевая развертка
#define subkey_e(K, i, j) K[(7 * (i) - 7 + (j)) % 8]

//зашифрование с перестановкой через аргументы
#define E(a, b, c, d, K)\
  R(a, b, c, d, K, 1, subkey_e);\
  R(b, d, a, c, K, 2, subkey_e);\
  R(d, c, b, a, K, 3, subkey_e);\
  R(c, a, d, b, K, 4, subkey_e);\
  R(a, b, c, d, K, 5, subkey_e);\
  R(b, d, a, c, K, 6, subkey_e);\
  R(d, c, b, a, K, 7, subkey_e);\
  R(c, a, d, b, K, 8, subkey_e);\
  *a ^= *b, *b ^= *a, *a ^= *b;\
  *c ^= *d, *d ^= *c, *c ^= *d;\
  *b ^= *c, *c ^= *b, *b ^= *c;\

/**
 * \brief Алгоритм зашифрования belt-block.
 * \param block [4*32 бит] Указатель на шифруемый блок данных.
 * \param key [8*32 бит] Указатель на ключ шифрования.
 */
void beltBlockEncr2(ak_uint32 block[4], const ak_uint32 key[8]) {
  E((block + 0), (block + 1), (block + 2), (block + 3), key);
}

//---------------------------------------------------------
//--------------------belt-compress------------------------
//---------------------------------------------------------

#define beltBlockNeg(dest, src)\
  ((ak_uint32*)(dest))[0] = ~((const ak_uint32*)(src))[0],\
  ((ak_uint32*)(dest))[1] = ~((const ak_uint32*)(src))[1],\
  ((ak_uint32*)(dest))[2] = ~((const ak_uint32*)(src))[2],\
  ((ak_uint32*)(dest))[3] = ~((const ak_uint32*)(src))[3]

#define beltBlockXor(dest, src1, src2)\
  ((ak_uint32*)(dest))[0] = \
    ((const ak_uint32*)(src1))[0] ^ ((const ak_uint32*)(src2))[0],\
  ((ak_uint32*)(dest))[1] = \
    ((const ak_uint32*)(src1))[1] ^ ((const ak_uint32*)(src2))[1],\
  ((ak_uint32*)(dest))[2] = \
    ((const ak_uint32*)(src1))[2] ^ ((const ak_uint32*)(src2))[2],\
  ((ak_uint32*)(dest))[3] = \
    ((const ak_uint32*)(src1))[3] ^ ((const ak_uint32*)(src2))[3]

#define beltBlockXor2(dest, src)\
  ((ak_uint32*)(dest))[0] ^= ((const ak_uint32*)(src))[0],\
  ((ak_uint32*)(dest))[1] ^= ((const ak_uint32*)(src))[1],\
  ((ak_uint32*)(dest))[2] ^= ((const ak_uint32*)(src))[2],\
  ((ak_uint32*)(dest))[3] ^= ((const ak_uint32*)(src))[3]

#define beltBlockCopy(dest, src)\
  ((ak_uint32*)(dest))[0] = ((const ak_uint32*)(src))[0],\
  ((ak_uint32*)(dest))[1] = ((const ak_uint32*)(src))[1],\
  ((ak_uint32*)(dest))[2] = ((const ak_uint32*)(src))[2],\
  ((ak_uint32*)(dest))[3] = ((const ak_uint32*)(src))[3]

/**
 * \brief Алгоритм сжатия belt-compress <b>без обработки S</b>.
 * \param h [8*32 бит] Указатель на выходное значение Y <b>и второй половины входного значения X</b>.
 * \param X [8*32 бит] Указатель на входное значение первой половины данных X.
 * \param stack [12*32 бит] Указатель на промежуточный буфер работы функции.
 */
void beltCompress(ak_uint32 h[8], const ak_uint32 X[8], void* stack) {
  // [12]buf = [4]buf0 || [4]buf1 || [4]buf2
  ak_uint32* buf = (ak_uint32*)stack;
  // буферы не пересекаются?
  //ASSERT(memIsDisjoint3(h, 32, X, 32, buf, 48));
  // buf0, buf1 <- h0 + h1
  beltBlockXor(buf, h, h + 4);
  beltBlockCopy(buf + 4, buf);
  // buf0 <- beltBlock(buf0, X) + buf1
  beltBlockEncr2(buf, X);
  beltBlockXor2(buf, buf + 4);
  // buf2 <- h0
  beltBlockCopy(buf + 8, h);
  // buf1 <- h1 [buf01 == K1]
  beltBlockCopy(buf + 4, h + 4);
  // h0 <- beltBlock(X0, buf01) + X0
  beltBlockCopy(h, X);
  beltBlockEncr2(h, buf);
  beltBlockXor2(h, X);
  // buf1 <- ~buf0 [buf12 == K2]
  beltBlockNeg(buf + 4, buf);
  // h1 <- beltBlock(X1, buf12) + X1
  beltBlockCopy(h + 4, X + 4);
  beltBlockEncr2(h + 4, buf + 4);
  beltBlockXor2(h + 4, X + 4);
}

/**
 * \brief Алгоритм сжатия belt-compress.
 * \param s [4*32 бит] Указатель на выходное значение данных S.
 * \param h [8*32 бит] Указатель на выходное значение Y <b>и второй половины входного значения X</b>.
 * \param X [8*32 бит] Указатель на входное значение первой половины данных X.
 * \param stack [12*32 бит] Указатель на промежуточный буфер работы функции.
 */
static inline void beltCompress2(ak_uint32 s[4], ak_uint32 h[8],
                   const ak_uint32 X[8], void* stack) {
  // [12]buf = [4]buf0 || [4]buf1 || [4]buf2
  ak_uint32* buf = (ak_uint32*)stack;
  // буферы не пересекаются?
  //ASSERT(memIsDisjoint4(s, 16, h, 32, X, 32, buf, 48));
  // buf0, buf1 <- h0 + h1
  beltBlockXor(buf, h, h + 4);
  beltBlockCopy(buf + 4, buf);
  // buf0 <- beltBlock(buf0, X) + buf1
  beltBlockEncr2(buf, X);
  beltBlockXor2(buf, buf + 4);
  // s <- s ^ buf0
  beltBlockXor2(s, buf);
  // buf2 <- h0
  beltBlockCopy(buf + 8, h);
  // buf1 <- h1 [buf01 == K1]
  beltBlockCopy(buf + 4, h + 4);
  // h0 <- beltBlock(X0, buf01) + X0
  beltBlockCopy(h, X);
  beltBlockEncr2(h, buf);
  beltBlockXor2(h, X);
  // buf1 <- ~buf0 [buf12 == K2]
  beltBlockNeg(buf + 4, buf);
  // h1 <- beltBlock(X1, buf12) + X1
  beltBlockCopy(h + 4, X + 4);
  beltBlockEncr2(h + 4, buf + 4);
  beltBlockXor2(h + 4, X + 4);
}

//---------------------------------------------------------
//----------------------belt-hash--------------------------
//---------------------------------------------------------

#define beltBlockSetZero(block)\
  ((ak_uint32*)(block))[0] = 0,\
  ((ak_uint32*)(block))[1] = 0,\
  ((ak_uint32*)(block))[2] = 0,\
  ((ak_uint32*)(block))[3] = 0

ak_uint32 u32Rev(ak_uint32 w) {
  return w << 24 | (w & 0xFF00) << 8 | (w >> 8 & 0xFF00) | w >> 24;
}

#define beltBlockRevU32(block)\
  ((ak_uint32*)(block))[0] = u32Rev(((ak_uint32*)(block))[0]),\
  ((ak_uint32*)(block))[1] = u32Rev(((ak_uint32*)(block))[1]),\
  ((ak_uint32*)(block))[2] = u32Rev(((ak_uint32*)(block))[2]),\
  ((ak_uint32*)(block))[3] = u32Rev(((ak_uint32*)(block))[3])

static int ak_hash_context_belt_hash_clean( ak_pointer bctx ) {
  ak_belt_hash cx = ( ak_belt_hash ) bctx;
  if( cx == NULL ) return ak_error_null_pointer;

  beltBlockSetZero(cx->ls);
  beltBlockSetZero(cx->ls + 4);
  // h <- B194...0D
  memmove(cx->h, beltH(), 32);

  return ak_error_ok;
}

static int ak_hash_context_belt_hash_update(
    ak_pointer bctx, const ak_pointer in, const size_t size ) {
  ak_belt_hash cx = ( ak_belt_hash ) bctx;
  const ak_uint8* dt = (const ak_uint8*) in;
  ak_uint64 count;
  ak_uint64 count_for_r;
  ak_uint32* carry;

  if( cx == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                               "using null pointer to internal streebog context" );
  if(( !size ) || ( in == NULL )) return ak_error_ok;
  if( size & 0x1F ) return ak_error_message( ak_error_wrong_length, __func__,
                                      "data length is not a multiple of the length of the block" );

  // обновить длину
    /*
  count = size;
  count_for_r = count << 3;
  carry = (cx->ls[0] += carry) < carry;
  carry = (cx->ls[1] += carry) < carry;
  carry = (cx->ls[2] += carry) < carry;
  cx->ls[3] += carry;
  */
  count = size;
  count_for_r = count << 3;
  carry = (ak_uint32*) &count_for_r;

  cx->ls[0] = carry[0];
  cx->ls[1] = carry[1];
  count_for_r = count >> 61;
  cx->ls[2] = carry[0];
  cx->ls[3] = 0;
  

  while (count >= 32)
  {
    beltBlockCopy(cx->block, dt);
    beltBlockCopy(cx->block + 16, dt + 16);
#ifndef AK_LITTLE_ENDIAN
    beltBlockRevU32(st->block);
    beltBlockRevU32(st->block + 16);
#endif
    beltCompress2(cx->ls + 4, cx->h, (ak_uint32*)cx->block, cx->stack);
    dt += 32;
    count -= 32;
  }

  return ak_error_ok;
}

static int ak_hash_context_belt_hash_finalize( ak_pointer bctx,
  const ak_pointer in, const size_t size, ak_pointer out,
  const size_t out_size )
{
  ak_belt_hash cx = ( ak_belt_hash ) bctx;
  struct belt_hash bx[1]; //здесь должна изменяться копия, а не оригинал
  //ak_uint32 carry = size << 3;
  ak_uint64 count_for_r;
  ak_uint32* carry;

  if( cx == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                               "using null pointer to internal streebog context" );
  if( out == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                   "using null pointer to externl result buffer" );
  if( size >= 32 ) return ak_error_message( ak_error_wrong_length, __func__,
                                                                       "input length is too huge" );
  
  memcpy( bx, cx, sizeof( struct belt_hash ));

  // обновить длину

  //count = size;
  count_for_r = size << 3;
  carry = (ak_uint32*) &count_for_r;

  printf("1: %i %li %i %i\n",
    bx->ls[0], count_for_r, carry[0], carry[1]);

  carry[0] = (bx->ls[0] += carry[0]) < carry[0];
  carry[0] = (bx->ls[1] += carry[0]) < carry[0];
  carry[0] = (bx->ls[2] += carry[0]) < carry[0];
  bx->ls[3] += carry[0];

  carry[1] = (bx->ls[1] += carry[1]) < carry[1];
  carry[1] = (bx->ls[2] += carry[1]) < carry[1];
  bx->ls[3] += carry[1];

  count_for_r = size >> 61;
  carry[0] = (bx->ls[2] += carry[0]) < carry[0];
  bx->ls[3] += carry[0];

  /*
  carry[0] = (cx->ls[0] += carry[0]) < carry[0];
  printf("2: %i\n", carry[0]);
  carry[0] = ( carry[1] += carry[0]) < carry[0];
  printf("3: %i\n", carry[0]);
  carry[1] = (cx->ls[1] += carry[1]) < carry[1];
  printf("4: %i\n", carry[1]);
  carry[0] = (cx->ls[2] += carry[0]) < carry[0];
  printf("5: %i\n", carry[0]);
  carry[1] = (cx->ls[2] += carry[1]) < carry[1];
  printf("6: %i\n", carry[1]);
  cx->ls[3] += carry[0] + carry[1];

  count_for_r = size >> 61;
  printf("7: %i %li %i %i\n",
    cx->ls[0], count_for_r, carry[0], carry[1]);
  carry[0] = (cx->ls[2] += carry[0]) < carry[0];
  printf("8: %i\n", carry[0]);
  cx->ls[3] += carry[0];
  printf("7: %i %i %i %i\n",
    cx->ls[0], cx->ls[1], cx->ls[2], cx->ls[3]);
  */
/*
  carry = (bx->ls[0] += carry) < carry;
  carry = (bx->ls[1] += carry) < carry;
  carry = (bx->ls[2] += carry) < carry;
  bx->ls[3] += carry;*/

  if(size) {
    memset(bx->block, 0, 32);
    if (in != NULL) {
      memcpy(bx->block, in, size);
    }
#ifndef AK_LITTLE_ENDIAN
    beltBlockRevU32(bx->block);
    beltBlockRevU32(bx->block + 16);
#endif
    beltCompress2(bx->ls + 4, bx->h, (ak_uint32*)bx->block, bx->stack);
  }

  beltCompress(bx->h, bx->ls, bx->stack);

  memcpy(out, bx->h, ak_min(32, out_size));

 return ak_error_ok;
}

int ak_hash_create_belt_hash( ak_hash hctx )
{
  int error = ak_error_ok;

  if( hctx == NULL ) return ak_error_message( ak_error_null_pointer, __func__,
                                                            "using null pointer to hash context" );
  if(( hctx->oid = ak_oid_find_by_name( "belt-hash" )) == NULL )
    return ak_error_message( ak_error_wrong_oid, __func__,
                                           "incorrect internal search of belt-hash identifier" );
  if(( error = ak_mac_create( &hctx->mctx, 32, &hctx->data.bctx,
                                             ak_hash_context_belt_hash_clean,
                                             ak_hash_context_belt_hash_update,
                                             ak_hash_context_belt_hash_finalize )) != ak_error_ok )
    return ak_error_message( error, __func__, "incorrect initialization of internal mac context" );

  return ak_hash_context_belt_hash_clean( &hctx->data.bctx );
}
