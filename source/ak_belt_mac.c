/* ----------------------------------------------------------------------------------------------- */
/*                                                                                                 */
/*  Реализация Иммитоставки из Белорусского стандарта СТБ 34.101.31-2020                           */
/*                                                                                                 */
/*                                                                                                 */
/* ----------------------------------------------------------------------------------------------- */
#include <string.h> 
#include <stdint.h> 
#include <stdio.h> 
#include "libakrypt-internal.h" 
 
/*  
  Преобразование φ1: φ1(u) = u2 ∥ u3 ∥ u4 ∥ (u1 ⊕ u2) 
  u - 128 бит, разбитые на 4 части по 32 бита: u1|u2|u3|u4  
*/ 
static inline void phi1(const ak_uint8 *u, ak_uint8 *result) { 
    const ak_uint8 *u1 = u; 
    const ak_uint8 *u2 = u + 4; 
    const ak_uint8 *u3 = u + 8; 
    const ak_uint8 *u4 = u + 12; 
 
    memcpy(result,     u2, 4);   // result[0..3]   = u2 
    memcpy(result + 4, u3, 4);   // result[4..7]   = u3 
    memcpy(result + 8, u4, 4);   // result[8..11]  = u4 
 
    for (size_t i = 0; i < 4; i++) { 
        result[12 + i] = u1[i] ^ u2[i]; 
    } 
} 
 
/*  
  Преобразование φ2: φ2(u) = (u1 ⊕ u4) ∥ u1 ∥ u2 ∥ u3 
*/ 
static inline void phi2(const ak_uint8 *u, ak_uint8 *result) { 
    const ak_uint8 *u1 = u; 
    const ak_uint8 *u2 = u + 4; 
    const ak_uint8 *u3 = u + 8; 
    const ak_uint8 *u4 = u + 12; 
 
    for (size_t i = 0; i < 4; i++) { 
        result[i] = u1[i] ^ u4[i]; 
    } 
 
    memcpy(result + 4,  u1, 4); 
    memcpy(result + 8,  u2, 4); 
    memcpy(result + 12, u3, 4); 
} 
 
/* 
  Отображение ψ(u) = u ∥ 1 ∥ 0^{127 - |u|} 
  Увеличиваем данные до 128 бит: копируем u, ставим бит '1', остальное нули. 
*/ 
static inline void psi(const ak_uint8 *u, size_t u_len_bytes, ak_uint8 *result) { 
    memset(result, 0, 16); 
    memcpy(result, u, u_len_bytes); 
 
    size_t total_bits = u_len_bytes * 8; 
    if (total_bits < 128) { 
        size_t bit_pos = total_bits; 
        size_t byte_pos = bit_pos / 8; 
        size_t bit_in_byte = 7 - (bit_pos % 8); 
        result[byte_pos] |= (1 << bit_in_byte); 
    } 
} 
 
/* ----------------------------------------------------------------------------------------------- */
/*! Функция вычисляет имитовставку от заданной области памяти с использованием алгоритма belt_mac, описанного в СТБ 34.101.31-2020.

   Имитовставка представляет собой значение, которое используется для проверки целостности
   и аутентификации данных. При этом вычисление производится на основе ключа блочного шифрования
   и входных данных.

   @param bkey Ключ алгоритма блочного шифрования, используемый для вычисления имитовставки.
   Ключ должен быть предварительно инициализирован и установлен.
   @param in Указатель на входные данные, для которых вычисляется имитовставка.
   @param size Размер входных данных в байтах.
   @param out Указатель на область памяти, куда будет помещен результат. Память должна быть
   заранее выделена, а её размер должен быть не менее размера блока алгоритма блочного шифрования.
   @param out_size Ожидаемый размер имитовставки в байтах. Не должен превышать размер блока
   используемого блочного шифра.

   @return В случае успеха функция возвращает \ref ak_error_ok (ноль). В случае ошибки
   возвращается соответствующий код ошибки.                                                        */
/* ----------------------------------------------------------------------------------------------- */
int ak_belt_mac( ak_bckey bkey, const ak_uint8 *in, const size_t size, ak_uint8 *out, const size_t out_size ) 
{ 
    int error = ak_error_ok; 
 
    // Проверяем корректность указателей 
    if (bkey == NULL) {
        printf("Error: bkey is NULL.\n");
        return ak_error_message( ak_error_null_pointer, __func__, "using null pointer to bkey" ); 
    }
    if (out == NULL) {
        printf("Error: out is NULL.\n");
        return ak_error_message( ak_error_null_pointer, __func__, "using null pointer to output buffer" ); 
    }
    if (out_size == 0) {
        printf("Error: out_size is 0.\n");
        return ak_error_message( ak_error_zero_length, __func__, "using zero length of output buffer" ); 
    }
 

    // Проверка контрольной суммы ключа 
    if ( bkey->key.check_icode( &bkey->key ) != ak_true ) {
        printf("Error: Incorrect integrity code of secret key.\n");
        return ak_error_message( ak_error_wrong_key_icode, __func__, 
                                 "incorrect integrity code of secret key value" ); 
    }

    // Проверяем размер блока, для BELT это 128 бит (16 байт) 
    if (bkey->bsize != 16) {
        printf("Error: Block size is not 128-bit.\n");
        return ak_error_message( ak_error_wrong_block_cipher_length, __func__, 
                                 "belt-mac requires 128-bit block cipher" ); 
    }
 

    // Подсчет ресурса ключа:  
    size_t n = (size == 0) ? 1 : ( (size + 15)/16 ); 
    if (bkey->key.resource.value.counter < (ssize_t)n) {
        printf("Error: Low resource of block cipher key.\n");
        return ak_error_message( ak_error_low_key_resource, __func__, 
                                 "low resource of block cipher key" ); 
    }
    bkey->key.resource.value.counter -= n; 
 

    // Переменные 
    ak_uint8 s[16];    // 128-битное состояние 
    ak_uint8 r[16];    // belt-block(s, K) 
    ak_uint8 temp[16]; // временный буфер 
    memset(s, 0, 16); 
 


    // r = belt-block(s, K) 
    bkey->encrypt(&bkey->key, s, r); 

    // Если X = ⊥ (пустое сообщение), то n = 1 и X1 = ⊥
    if (size == 0) {
        n = 1;
    } else {
        // Вычисляем n = количество 128-битных блоков (округление вверх)
        n = (size + 15) / 16; // 16 байт = 128 бит
    }

    // Обрабатываем блоки от X1 до X_{n-1}
    for (size_t i = 0; i < n - 1; i++) {
        // Получаем блок X_i
        const uint8_t *block = in + i * 16;

        // temp = s XOR X_i
        for (size_t j = 0; j < 16; j++) {
            temp[j] = s[j] ^ block[j];
        }

        // s = belt-block(temp, K)
        bkey->encrypt(&bkey->key, temp, s); 
    }


    // Обработка последнего блока 
    size_t last_block_len = size - (n - 1) * 16;
 
    if (last_block_len == 16) { 
        const ak_uint8 *block = in + (n-1)*16; 
        ak_uint8 phi1_r[16]; 
        phi1(r, phi1_r); 
 
        for (size_t j = 0; j < 16; j++) { 
            s[j] = s[j] ^ block[j] ^ phi1_r[j]; 
        } 
    } else { 
        ak_uint8 psi_Xn[16]; 
        psi(in + (n-1)*16, last_block_len, psi_Xn); 
        ak_uint8 phi2_r[16]; 
        phi2(r, phi2_r); 
 
        for (size_t j = 0; j < 16; j++) { 
            s[j] = s[j] ^ psi_Xn[j] ^ phi2_r[j]; 
        } 
    } 


    // Вычисляем belt-block(s, K) 
    bkey->encrypt(&bkey->key, s, temp); 


    // Копируем нужное количество байт в out 
    memcpy(out, temp, (out_size > bkey->bsize) ? bkey->bsize : out_size); 


    return ak_error_ok; 
}