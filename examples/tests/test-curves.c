#include <libakrypt.h>
void test_ak_wcurve_discriminant_is_ok() {
  struct wcurve paramSetA256_1 = {
    ak_mpzn256_size,
    4, /* cofactor */
    { 0x6d0078e62fc81048LL, 0x94db4f98bfb73698LL, 0x75e9b60631449efdLL, 0xca0709cc398e1cd1LL }, /* a */
    { 0xacd1216d5cc63966LL, 0x534b728e6773c810LL, 0xfb4e95d31a5032feLL, 0xb76e3775f6a4aee7LL }, /* b */
    { 0xfffffffffffffd97LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* p */
    { 0x000000000005cf11LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
    { 0xc115af556c360c67LL, 0x0fd8cddfc87b6635LL, 0x0000000000000000LL, 0x4000000000000000LL }, /* q */
    { 0x57cb446240dd1710LL, 0x7556091c4805caa4LL, 0xd0593365f9384bcdLL, 0x0fb1fbc48b0f0eb4LL }, /* r2q */
    {
      { 0x8b2582fe742daa28LL, 0x658b9196932e02c7LL, 0x880923425712b2bbLL, 0x91e38443a5e82c0dLL }, /* px */
      { 0xaf268adb32322e5cLL, 0x5fde0b5344766740LL, 0x895786c4bb46e956LL, 0x32879423ab1a0375LL }, /* py */
      { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
    },
    0x46f3234475d5add9LL, /* n */
    0x035bdd1aeafdb0a9LL, /* nq */
    "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd97"
  };
  /* Ошибка, возникающая если точка не принадлежит заданной кривой (121)*/
  struct wcurve paramSetA256_2 = {
    ak_mpzn256_size,
    4, /* cofactor */
    { 0x00000000000000000LL, 0x94db4f98bfb73698LL, 0x75e9b60631449efdLL, 0xca0709cc398e1cd1LL }, /* a */
    { 0x00000000000000000LL, 0x534b728e6773c810LL, 0xfb4e95d31a5032feLL, 0xb76e3775f6a4aee7LL }, /* b */
    { 0x000000000000000LL, 0x000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* p */
    { 0x00000000000000000LL, 0x11111111111LL, 0x000000000000000LL, 0x0000000000000000LL }, /* r2 */
    { 0x00000000000000000LL, 0x111111111111LL, 0x00000000000000LL, 0x0000000000000000LL }, /* q */
    {0x0000000000000LL}, /* r2q */
    {
      { 0x8b2582fe742daa28LL, 0x658b9196932e02c7LL, 0x880923425712b2bbLL, 0x91e38443a5e82c0dLL }, /* px */
      { 0xaf268adb32322e5cLL, 0x5fde0b5344766740LL, 0x895786c4bb46e956LL, 0x32879423ab1a0375LL }, /* py */
      { 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
    },
    0x46f3234475d5add9LL, /* n */
    0x46f3234475d5add9LL, /* nq */
    "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd97"
  };
  /* Ошибка, возникающая когда парметры кривой не соответсвуют алгоритму, в котором они используются (120)*/
  struct wcurve paramSetA256_3 = {
    ak_mpzn256_size,
    4, /* cofactor */
    { 0x6d0078e62fc81048LL, 0x94db4f98bfb73698LL, 0x75e9b60631449efdLL, 0xca0709cc398e1cd1LL }, /* a */
    { 0xacd1216d5cc63966LL, 0x534b728e6773c810LL, 0xfb4e95d31a5032feLL, 0xb76e3775f6a4aee7LL }, /* b */
    { 0xfffffffffffffd97LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* p */
    { 0x000000000005cf11LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
    { 0xc115af556c360c67LL, 0x0fd8cddfc87b6635LL, 0x0000000000000000LL, 0x4000000000000000LL }, /* q */
    { 0x57cb446240dd1710LL, 0x7556091c4805caa4LL, 0xd0593365f9384bcdLL, 0x0fb1fbc48b0f0eb4LL }, /* r2q */
    {
      { 0x8b2582fe742daa28LL, 0x658b9196932e02c7LL, 0x880923425712b2bbLL, 0x91e38443a5e82c0dLL }, /* px */
      { 0xaf268adb32322e5cLL, 0x5fde0b5344766740LL, 0x895786c4bb46e956LL, 0x32879423ab1a0375LL }, /* py */
      { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
    },
    0x46f3234475d5add9LL, /* n */
    0x035bdd1aeafdb0a9LL, /* nq */
    "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd97"
  };
  /* Ошибка, возникающая когда порядок точки неверен (122)*/
  struct wcurve paramSetA256_4 = {
    ak_mpzn256_size,
    4, /* cofactor */
    { 0x6d0078e62fc81048LL, 0x94db4f98bfb73698LL, 0x75e9b60631449efdLL, 0xca0709cc398e1cd1LL }, /* a */
    { 0xacd1216d5cc63966LL, 0x534b728e6773c810LL, 0xfb4e95d31a5032feLL, 0xb76e3775f6a4aee7LL }, /* b */
    { 0xfffffffffffffd97LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* p */
    { 0x000000000005cf11LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
    { 0xc115af556c360c67LL, 0x0fd8cddfc87b6635LL, 0x0000000000000000LL, 0x4000000000000000LL }, /* q */
    { 0x57cb446240dd1710LL, 0x7556091c4805caa4LL, 0xd0593365f9384bcdLL, 0x0fb1fbc48b0f0eb4LL }, /* r2q */
    {
      { 0x8b2582fe742daa28LL, 0x658b9196932e02c7LL, 0x880923425712b2bbLL, 0x91e38443a5e82c0dLL }, /* px */
      { 0xaf268adb32322e5cLL, 0x5fde0b5344766740LL, 0x895786c4bb46e956LL, 0x32879423ab1a0375LL }, /* py */
      { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
    },
    0x46f3234475d5add9LL, /* n */
    0x035bdd1aeafdb0a9LL, /* nq */
    "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd97"
  };
   /* Ошибка, возникающая когда дискриминант равен нулю (123)*/
  struct wcurve paramSetA256_5 = {
    ak_mpzn256_size,
    4, /* cofactor */
    { 0x0111111111181048LL }, /* a */
    { 0xacd1216d5cc63966LL, }, /* b */
    { 0xfffffffffffffd97LL, 0xffffffffffffffffLL }, /* p */
    { 0x000000000005cf11LL, 0x0001111231231100LL }, /* r2 */
    { 0x0000000000000000LL }, /* q */
    { 0x0000000000000000LL }, /* r2q */
    {
      { 0x8b2582fe742daa28LL, 0x9a5e82c0dLL }, /* px */
      { 0xaf268adb32322e5cLL, 0x895786c4bb46e956LL, 0x32879423ab1a0375LL }, /* py */
      { 0x0000000000000001LL, 0x0111111111111111LL, 0x0000000000000000LL }  /* pz */
    },
    0x0111111111111111LL, /* n */
    0x0000000000000000LL, /* nq */
    "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd97"
  };
    /* Ошибка, возникающая когда неверно определены вспомогательный параметры эллиптической кривой (124) */
  struct wcurve paramSetA256_6 = {
    ak_mpzn256_size,
    4, /* cofactor */
    { 0x6d0078e62fc81048LL, 0x94db4f98bfb73698LL, 0x75e9b60631449efdLL, 0xca0709cc398e1cd1LL }, /* a */
    { 0xacd1216d5cc63966LL, 0x534b728e6773c810LL, 0xfb4e95d31a5032feLL, 0xb76e3775f6a4aee7LL }, /* b */
    { 0xfffffffffffffd97LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* p */
    { 0x000000000005cf11LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
    { 0xc115af556c360c67LL, 0x0fd8cddfc87b6635LL, 0x0000000000000000LL, 0x4000000000000000LL }, /* q */
    { 0x57cb446240dd1710LL, 0x7556091c4805caa4LL, 0xd0593365f9384bcdLL, 0x0fb1fbc48b0f0eb4LL }, /* r2q */
    {
      { 0x8b2582fe742daa28LL, 0x658b9196932e02c7LL, 0x880923425712b2bbLL, 0x91e38443a5e82c0dLL }, /* px */
      { 0xaf268adb32322e5cLL, 0x5fde0b5344766740LL, 0x895786c4bb46e956LL, 0x32879423ab1a0375LL }, /* py */
      { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
    },
    0x46f3234475d5add9LL, /* n */
    0x035bdd1aeafdb0a9LL, /* nq */
    "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd97"
  };

  /* Ошибка, возникающая когда простой модуль кривой задан неверно (125) */
  struct wcurve paramSetA256_7 = {
    ak_mpzn256_size,
    4, /* cofactor */
    { 0x6d0078e62fc81048LL, 0x94db4f98bfb73698LL, 0x75e9b60631449efdLL, 0xca0709cc398e1cd1LL }, /* a */
    { 0xacd1216d5cc63966LL, 0x534b728e6773c810LL, 0xfb4e95d31a5032feLL, 0xb76e3775f6a4aee7LL }, /* b */
    { 0xfffffffffffffd97LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* p */
    { 0x000000000005cf11LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
    { 0xc115af556c360c67LL, 0x0fd8cddfc87b6635LL, 0x0000000000000000LL, 0x4000000000000000LL }, /* q */
    { 0x57cb446240dd1710LL, 0x7556091c4805caa4LL, 0xd0593365f9384bcdLL, 0x0fb1fbc48b0f0eb4LL }, /* r2q */
    {
      { 0x8b2582fe742daa28LL, 0x658b9196932e02c7LL, 0x880923425712b2bbLL, 0x91e38443a5e82c0dLL }, /* px */
      { 0xaf268adb32322e5cLL, 0x5fde0b5344766740LL, 0x895786c4bb46e956LL, 0x32879423ab1a0375LL }, /* py */
      { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
    },
    0x46f3234475d5add9LL, /* n */
    0x035bdd1aeafdb0a9LL, /* nq */
    "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd97"
  };

  struct wcurve tests[] = {paramSetA256_1, paramSetA256_2, paramSetA256_3, paramSetA256_4, paramSetA256_5, paramSetA256_6, paramSetA256_7};

  printf("%s\n", "--------------TESTING DESCRIMINANT IS OK----------------");
  for(int i = 0; i < 7; i++) {
    printf("%d\n", ak_wcurve_discriminant_is_ok(&tests[i]));
  }

  printf("%s\n", "----------------TESTING ORDER PARAMETERS-----------------");
  for(int i = 0; i < 7; i++) {
    printf("%d\n", ak_wcurve_check_order_parameters(&tests[i]));
  }

  printf("%s\n", "-------------------TESTING CURVE IS OK-------------------");
  for(int i = 0; i< 7; i++) {
    printf("%d\n", ak_wcurve_is_ok(&tests[i]));
  }
}

int main() {
  test_ak_wcurve_discriminant_is_ok();
}
