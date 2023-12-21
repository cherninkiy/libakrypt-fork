#include "stdio.h"
#include "libakrypt.h"

static ak_uint64 h2[8] = {0x7e5534fa80104109, 0x7e6634fa80104109, 0x7e7734fa80104109, 0x7e8834fa80104109, 0x7e9934fa80104109, 0x7eaa34fa80104109, 0x7ebb34fa80104109, 0x7ecc34fa80104109};
static ak_uint64 h3[8] = {0x7e6634fa80104109, 0x7e8834fa80104109, 0x7eaa34fa80104109, 0x7ecc34fa80104109, 0x7ebb34fa80104109, 0x7e9934fa80104109, 0x7e7734fa80104109, 0x7e5534fa80104109};

int get_params_message_a(ak_wcurve E_b, ak_wpoint K_a, ak_pointer k_a);
int check_params_from_message_b(ak_pointer Id_a, ak_pointer Id_b, ak_certificate Cert_a, ak_wcurve E_a, ak_wcurve E_b, ak_wpoint K_a, ak_wpoint K_b, ak_pointer s_b, ak_pointer tag_b, ak_pointer K_ab, ak_bckey key);
int check_params_from_message_a(ak_pointer Id_a, ak_pointer Id_b, ak_certificate Cert_b, ak_wpoint K_a, ak_wpoint K_b, ak_pointer k_a, ak_pointer s_a, ak_pointer tag_b, ak_wcurve E_a, ak_wcurve E_b, ak_pointer tag_a, ak_pointer K_ba);
int auth_b(ak_pointer tag_a, ak_pointer Id_a, ak_pointer Id_b, ak_wpoint K_a, ak_wpoint K_b, ak_bckey key);


// 1 step

int get_params_message_a(ak_wcurve E_b, ak_wpoint K_a, ak_pointer k_a) {
    /*
    Выработка случайного значения K_a = [k_a]P
    E_b - кривая E_b стороны В
    K_a - точка K_a, в которую помещается результат нахождения случайной кратной точки [k_a]P
    k_a - указатель на ak_uint64 [8] размера 512 бит, в который помещается случайное число k_a
    */

    /* определяем контекст генератора псевдослучайных значений */
    struct random generator;
    /* вызываем конструктор генератора */
    ak_random_create_hrng(&generator);
    /* вырабатываем случайное значение */
    ak_mpzn_set_random_modulo(k_a, E_b->q, ak_mpzn512_size, &generator);
    /* вызываем деструктор генератора */
    ak_random_destroy(&generator);

    ak_wpoint_pow(K_a, &(E_b->point), k_a, ak_mpzn512_size, E_b); // K_a = el_point(k_a*P_b)
    
    return EXIT_SUCCESS;
}

// 2 step

int check_params_from_message_b(ak_pointer Id_a, ak_pointer Id_b, ak_certificate Cert_a, ak_wcurve E_a, ak_wcurve E_b, ak_wpoint K_a, ak_wpoint K_b, ak_pointer s_b, ak_pointer tag_b, ak_pointer K_ab, ak_bckey key) {
    /*
    Выработка случайного значения K_b = [k_b]P и tag_b
    Id_a - идентификатор стороны A
    Id_b - идентификатор стороны B
    Cert_a - сертификат ключа проверки Cert_a, содержащий ключ S_a
    E_a - кривая E_b стороны В
    E_b - кривая E_b стороны В
    K_a - точка K_a стороны A
    K_b - точка K_a, в которую помещается результат нахождения случайной кратной точки [k_b]P
    s_b - секретный ключ стороны B
    tag_b - указатель на ak_uint64 [8] размера 512 бит, в который помещается результат нахождения tag_b
    K_ab - указатель на ak_uint8 [32] размера 256 бит, в который помещается созданный ключ стороны B
    key - указатель на bckey, в который помещается ключ для вычисления MAC
    */

    if (!ak_wpoint_is_ok(K_a, E_b)) { // check_point_in_curve(K_a, E_b)
        printf("K_a point not in E_b");
        return EXIT_FAILURE;
    }

    struct wpoint mq_K_a;
    ak_wpoint_pow(&mq_K_a, K_a, (ak_uint64*)&E_b->cofactor, ak_mpzn512_size, E_b);

    if (!ak_wpoint_is_ok(&mq_K_a, E_b)) { // check_infinity_point(K_a, m, q)
        printf("[m/q]K_a point is O");
        return EXIT_FAILURE;
    }
    
    struct wpoint S_a = (Cert_a->vkey).qpoint; // S_a = key_from_cert(Cert_a)

    ak_uint64 k_b[ak_mpzn512_size];
    /* определяем контекст генератора псевдослучайных значений */
    struct random generator;
    /* вызываем конструктор генератора */
    ak_random_create_hrng(&generator);
    /* вырабатываем случайное значение */
    ak_mpzn_set_random_modulo(k_b, E_a->q, ak_mpzn512_size, &generator);
    /* вызываем деструктор генератора */
    ak_random_destroy(&generator);

    ak_wpoint_pow(K_b, &(E_a->point), k_b, ak_mpzn512_size, E_a); // K_b = el_point(k_b*P_a)

    struct wpoint Q_ab, R_ab;
    ak_wpoint_pow(&Q_ab, &S_a, k_b, ak_mpzn512_size, E_a);
    ak_wpoint_pow(&Q_ab, &Q_ab, (ak_uint64*)&E_a->cofactor, ak_mpzn512_size, E_a); // Q_ab = el_point((m_a/q_a)*k_b*S_a)
    ak_wpoint_pow(&R_ab, K_a, s_b, ak_mpzn512_size, E_b);
    ak_wpoint_pow(&R_ab, &R_ab, (ak_uint64*)&E_b->cofactor, ak_mpzn512_size, E_b); // R_ab = el_point((m_b/q_b)*s_b*K_a)
    ak_wpoint_reduce(&Q_ab, E_a);
    ak_wpoint_reduce(&R_ab, E_b);

    struct hash h;
    ak_hash_create_streebog512(&h);
    ak_uint64 T_ab[ak_hash_get_tag_size(&h)];
    char in[64*4];
    ak_snprintf(in, 64*4, "%s%s%s%s", ak_mpzn_to_hexstr(Q_ab.x, 8), ak_mpzn_to_hexstr(R_ab.x, 8), ak_mpzn_to_hexstr(Id_a, 8), ak_mpzn_to_hexstr(Id_b, 8));
    ak_hash_finalize(&h, in, 64*4, T_ab, ak_hash_get_tag_size(&h)); // T_ab = KDF(Pi_func(Q_ab)||Pi_func(R_ab)||Id_a||Id_b)
    ak_hash_destroy(&h);
    
    ak_uint8 M_ab[32];
    memcpy(K_ab, T_ab, 32);
    memcpy(M_ab, T_ab+4, 32);

    ak_bckey_create_kuznechik(key);
    ak_bckey_set_key(key, M_ab, sizeof(M_ab));

    char in2[64*5];
    ak_snprintf(in2, 64*5, "%s%s%s%s%s", ak_mpzn_to_hexstr(h2, 8), ak_mpzn_to_hexstr(K_b->x, 8), ak_mpzn_to_hexstr(K_a->x, 8), ak_mpzn_to_hexstr(Id_b, 8), ak_mpzn_to_hexstr(Id_a, 8));
    ak_bckey_cmac(key, in2, 64*5, tag_b, 64); // tag_b = MAC_M_ab(h2||Pi_func(K_b)||Pi_func(K_a)||Id_b||Id_a)

    memset(M_ab, 0, 32);

    return EXIT_SUCCESS;
}

// 3 step

int check_params_from_message_a(ak_pointer Id_a, ak_pointer Id_b, ak_certificate Cert_b, ak_wpoint K_a, ak_wpoint K_b, ak_pointer k_a, ak_pointer s_a, ak_pointer tag_b, ak_wcurve E_a, ak_wcurve E_b, ak_pointer tag_a, ak_pointer K_ba) {
    /*
    Проверка полученного значения tag_b и выработка значения tag_a
    Id_a - идентификатор стороны A
    Id_b - идентификатор стороны B
    Cert_b - сертификат ключа проверки Cert_b, содержащий ключ S_b
    K_a - точка K_a стороны A
    K_b - точка K_b стороны B
    k_a - указатель на ak_uint64 [8] размера 512 бит, с результатом вычисления случайного числа k_a
    s_a - секретный ключ стороны A
    tag_b - указатель на ak_uint64 [8] размера 512 бит, с результатом нахождения tag_b
    E_a - кривая E_a стороны A
    E_b - кривая E_b стороны В
    tag_a - указатель на ak_uint64 [8] размера 512 бит, в который помещается результат нахождения tag_a
    K_ba - указатель на ak_uint8 [32] размера 256 бит, в который помещается созданный ключ стороны А
    */

    if (!ak_wpoint_is_ok(K_b, E_a)) { // check_point_in_curve(K_b, E_a)
        printf("K_b point not in E_a");
        return EXIT_FAILURE;
    }

    struct wpoint mq_K_b;
    ak_wpoint_pow(&mq_K_b, K_b, (ak_uint64*)&E_a->cofactor, ak_mpzn512_size, E_a);

    if (!ak_wpoint_is_ok(&mq_K_b, E_a)) {
        printf("[m/q]K_b point is O");
        return EXIT_FAILURE;
    }

    struct wpoint S_b = (Cert_b->vkey).qpoint; // S_b = key_from_cert(Cert_b)

    struct wpoint Q_ba, R_ba;
    ak_wpoint_pow(&Q_ba, K_b, s_a, ak_mpzn512_size, E_a);
    ak_wpoint_pow(&Q_ba, &Q_ba, (ak_uint64*)&E_a->cofactor, ak_mpzn512_size, E_a); // Q_ba = el_point((m_a/q_a)*s_a*K_b)
    ak_wpoint_pow(&R_ba, &S_b, k_a, ak_mpzn512_size, E_b);
    ak_wpoint_pow(&R_ba, &R_ba, (ak_uint64*)&E_b->cofactor, ak_mpzn512_size, E_b); // R_ba = el_point((m_b/q_b)*k_a*S_b)
    ak_wpoint_reduce(&Q_ba, E_a);
    ak_wpoint_reduce(&R_ba, E_b);

    struct hash h;
    ak_hash_create_streebog512(&h);
    ak_uint64 T_ba[ak_hash_get_tag_size(&h)];
    char in[64*4];
    ak_snprintf(in, 64*4, "%s%s%s%s", ak_mpzn_to_hexstr(Q_ba.x, 8), ak_mpzn_to_hexstr(R_ba.x, 8), ak_mpzn_to_hexstr(Id_a, 8), ak_mpzn_to_hexstr(Id_b, 8));
    ak_hash_finalize(&h, in, 64*4, T_ba, ak_hash_get_tag_size(&h)); // T_ba = KDF(Pi_func(Q_ba)||Pi_func(R_ba)||Id_a||Id_b)
    ak_hash_destroy(&h);
    
    ak_uint8 M_ba[32];
    memcpy(K_ba, T_ba, 32);
    memcpy(M_ba, T_ba+4, 32);

    struct bckey key;
    ak_bckey_create_kuznechik(&key);
    ak_bckey_set_key(&key, M_ba, sizeof(M_ba));

    char in2[64*5];
    ak_uint64 tag_b_[8];
    ak_snprintf(in2, 64*5, "%s%s%s%s%s", ak_mpzn_to_hexstr(h2, 8), ak_mpzn_to_hexstr(K_b->x, 8), ak_mpzn_to_hexstr(K_a->x, 8), ak_mpzn_to_hexstr(Id_b, 8), ak_mpzn_to_hexstr(Id_a, 8));
    ak_bckey_cmac(&key, in2, 64*5, tag_b_, 64); // tag_b_ = MAC_M_ba(h2||Pi_func(K_b)||Pi_func(K_a)||Id_b||Id_a)

    if (!ak_ptr_is_equal(tag_b, tag_b_, 8)) {
        printf("tag_b is not valid");
        return EXIT_FAILURE;
    } else {
        memset(in2, 0, 64*5);
        ak_snprintf(in2, 64*5, "%s%s%s%s%s", ak_mpzn_to_hexstr(h3, 8), ak_mpzn_to_hexstr(K_a->x, 8), ak_mpzn_to_hexstr(K_b->x, 8), ak_mpzn_to_hexstr(Id_a, 8), ak_mpzn_to_hexstr(Id_b, 8));
        ak_bckey_cmac(&key, in2, 64*5, tag_a, 64); // tag_a = MAC_M_ba(h3||Pi_func(K_a)||Pi_func(K_b)||Id_a||Id_b)

        memset(M_ba, 0, 32);

        return EXIT_SUCCESS;
    }
}

// 4 step

int auth_b(ak_pointer tag_a, ak_pointer Id_a, ak_pointer Id_b, ak_wpoint K_a, ak_wpoint K_b, ak_bckey key) {
    /*
    Проверка значения tag_a и подтверждение успешного обмена ключами
    tag_a - указатель на ak_uint64 [8] размера 512 бит, с результатом нахождения tag_a
    Id_a - идентификатор стороны A
    Id_b - идентификатор стороны B
    K_a - точка K_a стороны A
    K_b - точка K_b стороны B
    key - ключ для вычисления MAC
    */
    char in[64*5];
    ak_uint64 tag_a_[8];
    ak_snprintf(in, 64*5, "%s%s%s%s%s", ak_mpzn_to_hexstr(h3, 8), ak_mpzn_to_hexstr(K_a->x, 8), ak_mpzn_to_hexstr(K_b->x, 8), ak_mpzn_to_hexstr(Id_a, 8), ak_mpzn_to_hexstr(Id_b, 8));
    ak_bckey_cmac(key, in, 64*5, tag_a_, 64); // tag_a_ = MAC_M_ab(h3||Pi_func(K_a)||Pi_func(K_b)||Id_a||Id_b)
    ak_bckey_destroy(key);

    if (!ak_ptr_is_equal(tag_a, tag_a_, 8)) {
        printf("tag_a is not valid");
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}


int main() {
    if(ak_libakrypt_create(NULL) != ak_true) {
        /* инициализация выполнена не успешно, следовательно, выходим из программы */
        ak_libakrypt_destroy();
        return EXIT_FAILURE;
    }

    ak_uint64 Id_a[8] = {0x7e5534fa80104109, 0x7e6634fa80104109, 0x7e7734fa80104109, 0x7e8834fa80104109, 0x7e9934fa80104109, 0x7eaa34fa80104109, 0x7ebb34fa80104109, 0x7ecc34fa80104109};
    ak_uint64 Id_b[8] = {0x7e6634fa80104109, 0x7e8834fa80104109, 0x7eaa34fa80104109, 0x7ecc34fa80104109, 0x7ebb34fa80104109, 0x7e9934fa80104109, 0x7e7734fa80104109, 0x7e5534fa80104109};

    struct wcurve E_a = id_tc26_gost_3410_2012_512_paramSetA;
    struct wcurve E_b = id_tc26_gost_3410_2012_512_paramSetB;

    ak_uint64 s_a[ak_mpzn512_size], s_b[ak_mpzn512_size];
    /* определяем контекст генератора псевдослучайных значений */
    struct random generator;
    /* вызываем конструктор генератора */
    ak_random_create_hrng(&generator);
    /* вырабатываем случайное значение */
    ak_mpzn_set_random_modulo(s_a, E_a.q, ak_mpzn512_size, &generator);
    ak_mpzn_set_random_modulo(s_b, E_b.q, ak_mpzn512_size, &generator);
    /* вызываем деструктор генератора */
    ak_random_destroy(&generator);

    struct wpoint S_a;
    struct wpoint S_b;
    ak_wpoint_pow(&S_a, &(E_a.point), s_a, ak_mpzn512_size, &E_a);
    ak_wpoint_pow(&S_b, &(E_b.point), s_b, ak_mpzn512_size, &E_b);

    struct certificate Cert_a;
    struct certificate Cert_b;
    ak_certificate_opts_create(&(Cert_a.opts));
    ak_certificate_opts_create(&(Cert_b.opts));
    Cert_a.vkey.qpoint = S_a;
    Cert_b.vkey.qpoint = S_b;

    ak_uint64 k_a[8];
    struct wpoint K_a, K_b;
    ak_uint64 tag_b[8], tag_a[8];
    struct bckey key;
    ak_uint8 K_ab[32], K_ba[32];

    if (get_params_message_a(&E_b, &K_a, k_a) == EXIT_SUCCESS) {
        if (check_params_from_message_b(Id_a, Id_b, &Cert_a, &E_a, &E_b, &K_a, &K_b, s_b, tag_b, K_ab, &key) == EXIT_SUCCESS) {
            if (check_params_from_message_a(Id_a, Id_b, &Cert_b, &K_a, &K_b, k_a, s_a, tag_b, &E_a, &E_b, tag_a, K_ba) == EXIT_SUCCESS) {
                if (auth_b(tag_a, Id_a, Id_b, &K_a, &K_b, &key) == EXIT_SUCCESS) {
                    printf("%s\n", ak_ptr_to_hexstr(K_ab, 32, 0));
                    printf("%s\n", ak_ptr_to_hexstr(K_ba, 32, 0));
                }
            }
        }
    }

    ak_libakrypt_destroy();
    return 0;
}
