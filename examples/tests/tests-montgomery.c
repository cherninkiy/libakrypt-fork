#include <stdio.h>
#include <libakrypt.h>

void print_point(ak_wpoint p, size_t size) {
  printf("X: %s\n", ak_mpzn_to_hexstr(p->x, size));
  printf("Y: %s\n", ak_mpzn_to_hexstr(p->y, size));
  printf("Z: %s\n", ak_mpzn_to_hexstr(p->z, size));
}

int main(){
    if( ak_libakrypt_create( NULL ) != ak_true ) {
        ak_libakrypt_destroy();
        return EXIT_FAILURE;
    }

    struct wcurve curve25519 = {
        ak_mpzn256_size, /* size = 4 */
        0x0000000000000008LL, /* cofactor = 8*/
        { 0xaaaaaa984914a144LL, 0xaaaaaaaaaaaaaaaaLL, 0xaaaaaaaaaaaaaaaaLL, 0x2aaaaaaaaaaaaaaaLL }, /* a in Weierstrass */
        { 0x260b5e9c7710c864LL, 0xed097b425ed097b4LL, 0x097b425ed097b425LL, 0x7b425ed097b425edLL }, /* b in Weierstrass */
        { 0xffffffffffffffedLL, 0xffffffffffffffffLL, 0xffffffffffffffffLL, 0x7fffffffffffffffLL }, /* p = 2^255-19 */
        { 0x00000000000005A4LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2 */
        { 0x5812631a5cf5d3edLL, 0x14def9dea2f79cd6LL, 0x0000000000000000LL, 0x1000000000000000LL }, /* q = 2^252-... */
        { 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* r2q */
        {
            { 0x0000000000000009LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL}, /* px = 9*/
            { 0x29e9c5a27eced3d9LL, 0x923d4d7e6d7c61b2LL, 0xe01edd2c7748d14cLL, 0x20ae19a1b8a086b4LL}, /* py */
            { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL}  /* pz = 1*/
        },
        0x86bca1af286bca1bLL, /* n */
        0x0000000000000000LL, /* nq ??? */
        "0xffffffffffffffedffffffffffffffffffffffffffffffff7fffffffffffffff", /* p as string */

        { 0x0000000000076d06LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* ma = A in Montgomery form = 486662 */
        { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* mb = B in Montgomery form = 1*/
        { 0x000000000001db41LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL }, /* a24 = (a-2)/4 = 121665 */
    };

    struct wpoint p1 = {
        { 0x0000000000000009LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL}, /* px = 9 */
        { 0x29e9c5a27eced3d9LL, 0x923d4d7e6d7c61b2LL, 0xe01edd2c7748d14cLL, 0x20ae19a1b8a086b4LL}, /* py */
        { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL}  /* pz = 1 */
    };

    printf("initial point:\n");
    print_point(&p1, ak_mpzn256_size);
    printf("\n");

    if (ak_mpoint_is_ok(&p1, &curve25519)){
        printf("%s\n", "points belongs to curve");
    } else {
        printf("%s\n", "point does not belong to curve");
    };
    printf("\n");
    
    printf("point after doubling:\n");
    ak_mpoint_double(&p1, &curve25519);
    print_point(&p1, ak_mpzn256_size);
    printf("\n");

    if (ak_mpoint_is_ok(&p1, &curve25519)){
        printf("%s\n", "points belongs to curve");
    } else {
        printf("%s\n", "point does not belong to curve");
    };
    printf("\n");

    struct wpoint p2 = {
        { 0x0000000000000009LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL}, /* px = 9 */
        { 0x29e9c5a27eced3d9LL, 0x923d4d7e6d7c61b2LL, 0xe01edd2c7748d14cLL, 0x20ae19a1b8a086b4LL}, /* py */
        { 0x0000000000000001LL, 0x0000000000000000LL, 0x0000000000000000LL, 0x0000000000000000LL}  /* pz = 1 */
    };

    printf("point after x5:\n");
    ak_uint64 pw = 5;
    ak_mpoint_pow(&p2, &p2, &pw, ak_mpzn256_size, &curve25519);

    print_point(&p2, ak_mpzn256_size);
    printf("\n");

    if (ak_mpoint_is_ok(&p2, &curve25519)){
        printf("%s\n", "points belongs to curve");
    } else {
        printf("%s\n", "point does not belong to curve");
    };
    printf("\n");

    ak_libakrypt_destroy();
    return EXIT_SUCCESS;
}