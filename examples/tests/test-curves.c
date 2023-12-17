/* 
Тема проекта: "Примеры для работы с эллиптическими кривыми (создание, доступ к данным, вычисление криатной точки)

Команда:
Сурков Максим Андреевич (СКБ-192)
Кондратьев Арсений Алексеевич (СКБ-192)
Киселев Дмитрий Алексеевич (СКБ-192)

Была предемонстрирована работа со следующими функциями:
- ak_wcurve_discriminant_is_ok
- ak_wcurve_check_order_parameters
- ak_wcurve_is_ok
- ak_wpoint_add
- ak_wpoint_double
- ak_wpoint_reduce
- ak_wpoint_pow
- ak_wpoint_check_order
- ak_wpoint_is_ok

*/

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
    { 0x0111111111181048LL, 0x074e9bffffffffffLL, 0xffffffff123f111fLL}, /* a */
    { 0xacd1216d5cc63966LL, 0x88092345712b2bbfLL, 0x328794231a037b15LL}, /* b */
    { 0xfffffffffffffd97LL, 0xffffffffffffffffLL, 0xab12341bffffffffLL }, /* p */
    { 0x000000000005cf11LL, 0x0001111231231100LL, 0xffffffffffffffffLL }, /* r2 */
    { 0x0000000000000000LL, 0xffffffffffffffffLL, 0x45623123ffffffffLL }, /* q */
    { 0x0000000000000000LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* r2q */
    {
    { 0x8b2582fe742daa28LL, 0x9a5e82aac0dfffffLL, 0xffffffffffffffffLL }, /* px */
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
    { 0x0000001231230123LL, 0x0000012312300005LL, 0xfb4e95d31a5032feLL, 0xffffffffffffffffLL }, /* b */
    { 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* p */
    { 0x0000000012310123LL, 0x0001231231230204LL, 0x000d59a120000000LL, 0x0000000000000000LL }, /* r2 */
    { 0xc115af556c360c67LL, 0x0fd8cddfc87b6635LL, 0x0000000000000000LL, 0x4000000000000000LL }, /* q */
    { 0x57cb446240dd1710LL, 0x7556091c4805caa4LL, 0xd0593365f9384bcdLL, 0x0fb1fbc48b0f0eb4LL }, /* r2q */
    {
      { 0x8b2582fe742daa28LL, 0x658b9196932e02c7LL, 0x880923425712b2bbLL, 0x91e38443a5e82c0dLL }, /* px */
      { 0xaf268adb32322e5cLL, 0x5fde0b5344766740LL, 0x895786c4bb46e956LL, 0x32879423ab1a0375LL }, /* py */
      { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
    },
    0x0LL, /* n */
    0x0LL, /* nq */
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
      { 0x0000000000000000LL }, /* py */
      { 0x0000000000000000LL }  /* pz */
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

void printPointInfo(ak_wcurve curve, const char *name) {
  printf("----------------%s----------------\n", name);
  printf("point is ok: %d\n", ak_wpoint_is_ok(&curve->point, curve));
  printf("order of point is ok: %d\n", ak_wpoint_check_order(&curve->point, curve));
  printf("Contests: \n"); 
  printf("X: %s\n", ak_mpzn_to_hexstr(curve->point.x, curve->size));
  printf("Y: %s\n", ak_mpzn_to_hexstr(curve->point.y, curve->size));
  printf("Z: %s\n", ak_mpzn_to_hexstr(curve->point.z, curve->size));
  printf("\n");
}

int main() {
  struct wcurve curve1, curve2, curve3, curve4, curve5, curve6, curve7; //gost curves
  //creating your own curve 
  struct wcurve paramSetA256 = {
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
 
  //geting curves from libakrypt
  memcpy(&curve1, &id_tc26_gost_3410_2012_256_paramSetA, sizeof(id_tc26_gost_3410_2012_256_paramSetA));
  memcpy(&curve2, &id_rfc4357_gost_3410_2001_paramSetA, sizeof(id_rfc4357_gost_3410_2001_paramSetA));  
  memcpy(&curve3, &id_rfc4357_gost_3410_2001_paramSetB, sizeof(id_rfc4357_gost_3410_2001_paramSetB));  
  memcpy(&curve4, &id_rfc4357_gost_3410_2001_paramSetC, sizeof(id_rfc4357_gost_3410_2001_paramSetC));  
  memcpy(&curve5, &id_tc26_gost_3410_2012_512_paramSetA, sizeof(id_tc26_gost_3410_2012_512_paramSetA));
  memcpy(&curve6, &id_tc26_gost_3410_2012_512_paramSetB, sizeof(id_tc26_gost_3410_2012_512_paramSetB));
  memcpy(&curve7, &id_tc26_gost_3410_2012_512_paramSetC, sizeof(id_tc26_gost_3410_2012_512_paramSetC));


  //output of exits in functions
  printf("ak_error_ok = %d\n", ak_error_ok);
  printf("ak_true = %d\n", ak_true);

  printf("\n");
   

  //checking if our curve is ok
  printf("CURVE PARAMETERS = %d\n", ak_wcurve_is_ok(&paramSetA256)); 
  printPointInfo(&paramSetA256, "BASE POINT");

  //POINT OPERATIONS TESTING
  //double
  ak_wpoint_double(&paramSetA256.point, &paramSetA256);
  printPointInfo(&paramSetA256, "DOUBLE POINT");
 
  //add
  ak_wpoint_add(&paramSetA256.point, &paramSetA256.point, &paramSetA256);
  printPointInfo(&paramSetA256, "ADD POINT");

  //reduce
  ak_wpoint_reduce(&paramSetA256.point, &paramSetA256);
  printPointInfo(&paramSetA256, "REDUCED POINT");

  //key generation
  //gen
  struct random gen;
  ak_mpznmax rand;
  ak_random_create_lcg(&gen);
  ak_mpzn_set_random_modulo(rand, paramSetA256.q, paramSetA256.size, &gen);
  if (ak_mpzn_cmp_ui(rand, paramSetA256.size, 0) == ak_true)
    ak_mpzn_set_ui(rand, paramSetA256.size, 1); 

  //calculating [k]P with random k
  ak_wpoint_pow(&paramSetA256.point, &paramSetA256.point, rand, paramSetA256.size, &paramSetA256);
  printPointInfo(&paramSetA256, "[k]P point");

  ak_mpzn256 k = {0x1, 0x2, 0x3, 0x4};
  //calc with set k
  ak_wpoint_pow(&paramSetA256.point, &paramSetA256.point, k, paramSetA256.size, &paramSetA256);
  printPointInfo(&paramSetA256, "[k]P point");

  test_ak_wcurve_discriminant_is_ok();


}
