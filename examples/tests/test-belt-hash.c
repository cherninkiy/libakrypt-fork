#include <libakrypt.h>
#include <string.h>

//подстановка битов
#define H16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)\
  0x##a, 0x##b, 0x##c, 0x##d, 0x##e, 0x##f, 0x##g, 0x##h,\
  0x##i, 0x##j, 0x##k, 0x##l, 0x##m, 0x##n, 0x##o, 0x##p

int main( void ) {
  const ak_uint8 X1[13] = { 
    0xB1, 0x94, 0xBA, 0xC8, 0x0A, 0x08, 0xF5, 0x3B,
    0x36, 0x6D, 0x00, 0x8E, 0x58
  };
  const ak_uint8 Y1[32] = {
    H16(AB,EF,97,25,D4,C5,A8,35,97,A3,67,D1,44,94,CC,25),
    H16(42,F2,0F,65,9D,DF,EC,C9,61,A3,EC,55,0C,BA,8C,75)
  };
  const ak_uint8 X2[32] = { 
    H16(B1,94,BA,C8,0A,08,F5,3B,36,6D,00,8E,58,4A,5D,E4),
    H16(85,04,FA,9D,1B,B6,C7,AC,25,2E,72,C2,02,FD,CE,0D)
  };
  const ak_uint8 Y2[32] = {
    H16(74,9E,4C,36,53,AE,CE,5E,48,DB,47,61,22,77,42,EB),
    H16(6D,BE,13,F4,A8,0F,7B,EF,F1,A9,CF,8D,10,EE,77,86)
  };
  const ak_uint8 X3[48] = { 
    H16(B1,94,BA,C8,0A,08,F5,3B,36,6D,00,8E,58,4A,5D,E4),
    H16(85,04,FA,9D,1B,B6,C7,AC,25,2E,72,C2,02,FD,CE,0D),
    H16(5B,E3,D6,12,17,B9,61,81,FE,67,86,AD,71,6B,89,0B)
  };
  const ak_uint8 Y3[32] = {
    H16(9D,02,EE,44,6F,B6,A2,9F,E5,C9,82,D4,B1,3A,F9,D3),
    H16(E9,08,61,BC,4C,EF,27,CF,30,6B,FB,0B,17,4A,15,4A)
  };

  ak_uint8 success = 0;
  ak_uint8 A[32] = {0};
  struct hash ctx;

  printf("--------------------Testing belt-hash----------------------\n"
    "               -----------Test 1------------\n"
    "Input:\n"
    " > X1 = %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x\n"
    "Expected output:\n"
    " > Y1 = %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n"
    "        %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n",
    X1[0] , X1[1] , X1[2] , X1[3] , X1[4] , X1[5] , X1[6] , X1[7] ,
    X1[8] , X1[9] , X1[10], X1[11], X1[12],
    Y1[0] , Y1[1] , Y1[2] , Y1[3] , Y1[4] , Y1[5] , Y1[6] , Y1[7] ,
    Y1[8] , Y1[9] , Y1[10], Y1[11], Y1[12], Y1[13], Y1[14], Y1[15],
    Y1[16], Y1[17], Y1[18], Y1[19], Y1[20], Y1[21], Y1[22], Y1[23],
    Y1[24], Y1[25], Y1[26], Y1[27], Y1[28], Y1[29], Y1[30], Y1[31]);

  ak_hash_create_belt_hash( &ctx );
  ak_hash_ptr( &ctx, X1, 13, A, 32 );
  ak_hash_destroy( &ctx );

  printf("Output:\n"
    " > Y1 = %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n"
    "        %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n",
    A[0] , A[1] , A[2] , A[3] , A[4] , A[5] , A[6] , A[7] ,
    A[8] , A[9] , A[10], A[11], A[12], A[13], A[14], A[15],
    A[16], A[17], A[18], A[19], A[20], A[21], A[22], A[23],
    A[24], A[25], A[26], A[27], A[28], A[29], A[30], A[31]);
  
  if(strncmp(A, Y1, 32) == 0) {
    printf("Test 1 was successful\n\n");
    success += 1;
    //return 0;
  }
  else {
    printf("Test 1 failed\n\n");
    //return 1;
  }

  printf("               -----------Test 2------------\n"
    "Input:\n"
    " > X2 = %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n"
    "        %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n"
    "Expected output:\n"
    " > Y2 = %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n"
    "        %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n",
    X2[0] , X2[1] , X2[2] , X2[3] , X2[4] , X2[5] , X2[6] , X2[7] ,
    X2[8] , X2[9] , X2[10], X2[11], X2[12], X2[13], X2[14], X2[15],
    X2[16], X2[17], X2[18], X2[19], X2[20], X2[21], X2[22], X2[23],
    X2[24], X2[25], X2[26], X2[27], X2[28], X2[29], X2[30], X2[31],
    Y2[0] , Y2[1] , Y2[2] , Y2[3] , Y2[4] , Y2[5] , Y2[6] , Y2[7] ,
    Y2[8] , Y2[9] , Y2[10], Y2[11], Y2[12], Y2[13], Y2[14], Y2[15],
    Y2[16], Y2[17], Y2[18], Y2[19], Y2[20], Y2[21], Y2[22], Y2[23],
    Y2[24], Y2[25], Y2[26], Y2[27], Y2[28], Y2[29], Y2[30], Y2[31]);

  ak_hash_create_belt_hash( &ctx );
  ak_hash_ptr( &ctx, X2, 32, A, 32 );
  ak_hash_destroy( &ctx );

  printf("Output:\n"
    " > Y2 = %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n"
    "        %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n",
    A[0] , A[1] , A[2] , A[3] , A[4] , A[5] , A[6] , A[7] ,
    A[8] , A[9] , A[10], A[11], A[12], A[13], A[14], A[15],
    A[16], A[17], A[18], A[19], A[20], A[21], A[22], A[23],
    A[24], A[25], A[26], A[27], A[28], A[29], A[30], A[31]);
  
  if(strncmp(A, Y2, 32) == 0) {
    printf("Test 2 was successful\n\n");
    success += 2;
    //return 0;
  }
  else {
    printf("Test 2 failed\n\n");
    //return 1;
  }

  printf("               -----------Test 3------------\n"
    "Input:\n"
    " > X3 = %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n"
    "        %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n"
    "        %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n"
    "Expected output:\n"
    " > Y3 = %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n"
    "        %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n",
    X3[0] , X3[1] , X3[2] , X3[3] , X3[4] , X3[5] , X3[6] , X3[7] ,
    X3[8] , X3[9] , X3[10], X3[11], X3[12], X3[13], X3[14], X3[15],
    X3[16], X3[17], X3[18], X3[19], X3[20], X3[21], X3[22], X3[23],
    X3[24], X3[25], X3[26], X3[27], X3[28], X3[29], X3[30], X3[31],
    X3[32], X3[33], X3[34], X3[35], X3[36], X3[37], X3[38], X3[39],
    X3[40], X3[41], X3[42], X3[43], X3[44], X3[45], X3[46], X3[47],
    Y3[0] , Y3[1] , Y3[2] , Y3[3] , Y3[4] , Y3[5] , Y3[6] , Y3[7] ,
    Y3[8] , Y3[9] , Y3[10], Y3[11], Y3[12], Y3[13], Y3[14], Y3[15],
    Y3[16], Y3[17], Y3[18], Y3[19], Y3[20], Y3[21], Y3[22], Y3[23],
    Y3[24], Y3[25], Y3[26], Y3[27], Y3[28], Y3[29], Y3[30], Y3[31]);

  ak_hash_create_belt_hash( &ctx );
  ak_hash_ptr( &ctx, X2, 48, A, 32 );
  ak_hash_destroy( &ctx );

  printf("Output:\n"
    " > Y3 = %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n"
    "        %02x%02x%02x%02x %02x%02x%02x%02x "
            "%02x%02x%02x%02x %02x%02x%02x%02x\n",
    A[0] , A[1] , A[2] , A[3] , A[4] , A[5] , A[6] , A[7] ,
    A[8] , A[9] , A[10], A[11], A[12], A[13], A[14], A[15],
    A[16], A[17], A[18], A[19], A[20], A[21], A[22], A[23],
    A[24], A[25], A[26], A[27], A[28], A[29], A[30], A[31]);
  
  if(strncmp(A, Y3, 32) == 0) {
    printf("Test 3, was successful\n");
    success += 4;
    //return 0;
  }
  else {
    printf("Test 3 failed\n");
    //return 1;
  }

  if(success == 7) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
