#include <stdio.h>
#include <libakrypt.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void point_coord(ak_wcurve curve, const char *name) {
  printf("----------------%s----------------\n", name);
  printf("Точка: %d\n", ak_wpoint_is_ok(&curve->point, curve));
  printf("Порядок точки: %d\n", ak_wpoint_check_order(&curve->point, curve));
  printf("X: %s\n", ak_mpzn_to_hexstr(curve->point.x, curve->size));
  printf("Y: %s\n", ak_mpzn_to_hexstr(curve->point.y, curve->size));
  printf("Z: %s\n", ak_mpzn_to_hexstr(curve->point.z, curve->size));
  printf("\n");
}

ak_uint32 divide_by_3(ak_uint64* x){

	ak_uint32 counter = 0;
	ak_uint64 tmp = *x;
	while (ak_mpzn_rem_uint32(&tmp, ak_mpzn256_size, 3) == 0x0)
	{
		tmp /= 3;
		counter++;
	}
	printf("x: %llu\n", tmp);
	return counter;
}


int main() {
	printf("Test_triple_point\n");

 	//Эллиптическая кривая из ak_parameters id_rfc4357_gost_3410_2001_paramSetA.
	struct wcurve paramSetA256 = {
	ak_mpzn256_size,
	1,
	{ 0xfffffffffffff65cLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* a */
	{ 0x0000000000019016LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* b */
	{ 0xfffffffffffffd97LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* p */
	{ 0x000000000005cf11LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
	{ 0x45841b09b761b893LL, 0x6c611070995ad100LL, 0xffffffffffffffffLL, 0xffffffffffffffffLL }, /* q */
	{ 0x9ac2d7858e79a469LL, 0xfb07f8222e76dd52LL, 0xf74885d08a3714c6LL, 0x551fe9cb451179dbLL }, /* r2q */
	{
	{ 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* px */
	{ 0x22acc99c9e9f1e14LL, 0x35294f2ddf23e3b1LL, 0x27df505a453f2b76LL, 0x8d91e471e0989cdaLL }, /* py */
	{ 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }  /* pz */
	},
	0x46f3234475d5add9LL, /* n */
	0x9ee6ea0b57c7da65LL, /* nq */
	"fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd97",
	};
	struct wcurve paramSetA256_1 = paramSetA256;
	struct wcurve paramSetA256_2 = paramSetA256;


	printf("------Сравнение ak_wpoint_pow и ak_wpoint_pow_modified------\n");
	printf("-------------по времени и по значениям точек----------------\n");

	// point_coord(&paramSetA256_1, "1P");

	ak_mpzn256 k[4] = {
		{0xc546562aa3}, // 3^25 
		{0x1ea391a4d9c77058, 0x1ea391a4d9c77058, 0x1ea391a4d9c77058, 0x1ea391a4d9c77058},
		{0xc250f372614b9926, 0xc250f372614b9926, 0xc250f372614b9926, 0xc250f372614b9926},
		{0x6c54bd89fab91640, 0x6c54bd89fab91640, 0x6c54bd89fab91640, 0x6c54bd89fab91640}
	};

	struct timeval stop, start;
	gettimeofday(&start, NULL);
	ak_wpoint_pow(&paramSetA256_1.point, &paramSetA256.point, k[0], paramSetA256.size, &paramSetA256);
	gettimeofday(&stop, NULL);
	point_coord(&paramSetA256_1, "6P ak_wpoint_pow");
	printf("Заняло времени в микросекундах: %lu\n", stop.tv_usec - start.tv_usec);
	

	gettimeofday(&start, NULL);
	ak_wpoint_pow_modified(&paramSetA256_1.point, &paramSetA256.point, k[0], paramSetA256.size, &paramSetA256);
	gettimeofday(&stop, NULL);
	point_coord(&paramSetA256_1, "6P ak_wpoint_pow_modified");
	printf("Заняло времени в микросекундах: %lu\n", stop.tv_usec - start.tv_usec);



	gettimeofday(&start, NULL);
	ak_wpoint_pow(&paramSetA256_1.point, &paramSetA256.point, k[1], paramSetA256.size, &paramSetA256);
	gettimeofday(&stop, NULL);
	point_coord(&paramSetA256_1, "0x1ea391a4d9c77058 P ak_wpoint_pow");
	printf("Заняло времени в микросекундах: %lu\n", stop.tv_usec - start.tv_usec);
	
	gettimeofday(&start, NULL);
	ak_wpoint_pow_modified(&paramSetA256_1.point, &paramSetA256.point, k[1], paramSetA256.size, &paramSetA256);
	gettimeofday(&stop, NULL);
	point_coord(&paramSetA256_1, "0x1ea391a4d9c77058 P ak_wpoint_pow_modified");
	printf("Заняло времени в микросекундах: %lu\n", stop.tv_usec - start.tv_usec);

	gettimeofday(&start, NULL);
	ak_wpoint_pow(&paramSetA256_1.point, &paramSetA256.point, k[2], paramSetA256.size, &paramSetA256);
	gettimeofday(&stop, NULL);
	point_coord(&paramSetA256_1, "0xc250f372614b9926 P ak_wpoint_pow");
	printf("Заняло времени в микросекундах: %lu\n", stop.tv_usec - start.tv_usec);
	
	gettimeofday(&start, NULL);
	ak_wpoint_pow_modified(&paramSetA256_1.point, &paramSetA256.point, k[2], paramSetA256.size, &paramSetA256);
	gettimeofday(&stop, NULL);
	point_coord(&paramSetA256_1, "0xc250f372614b9926 P ak_wpoint_pow_modified");
	printf("Заняло времени в микросекундах: %lu\n", stop.tv_usec - start.tv_usec);

	gettimeofday(&start, NULL);
	ak_wpoint_pow(&paramSetA256_1.point, &paramSetA256.point, k[3], paramSetA256.size, &paramSetA256);
	gettimeofday(&stop, NULL);
	point_coord(&paramSetA256_1, "0x6c54bd89fab91640 P ak_wpoint_pow");
	printf("Заняло времени в микросекундах: %lu\n", stop.tv_usec - start.tv_usec);
	
	gettimeofday(&start, NULL);
	ak_wpoint_pow_modified(&paramSetA256_1.point, &paramSetA256.point, k[3], paramSetA256.size, &paramSetA256);
	gettimeofday(&stop, NULL);
	point_coord(&paramSetA256_1, "0x6c54bd89fab91640 P ak_wpoint_pow_modified");
	printf("Заняло времени в микросекундах: %lu\n", stop.tv_usec - start.tv_usec);


	// Проверка работы double и add в разных последовательностях
	paramSetA256_1 = paramSetA256;
	ak_wpoint_add( &paramSetA256_1.point, &paramSetA256_2.point, &paramSetA256_1);
	ak_wpoint_add( &paramSetA256_1.point, &paramSetA256_2.point, &paramSetA256_1);
	ak_wpoint_add( &paramSetA256_1.point, &paramSetA256_2.point, &paramSetA256_1);

	point_coord(&paramSetA256_1, "4P with add");


	paramSetA256_1 = paramSetA256;
	ak_wpoint_double( &paramSetA256_1.point, &paramSetA256_1);
	ak_wpoint_double( &paramSetA256_1.point, &paramSetA256_1);

	point_coord(&paramSetA256_1, "4P with double");

	// ak_wpoint_triple(&paramSetA256_1.point, &paramSetA256_1);
	// ak_wpoint_triple2(&paramSetA256_1.point, &paramSetA256_1);
	return 0;
}


 
  
