/**
 * @file hmac_sha256_test.c
 * @author Yongbhin Kim (coji67@gmail.com)
 * @brief HMAC_SHA256 test codes
 * @version 0.1
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) Yongbhin Kim 2024
 */


#include "hmac_sha256.h"
#include <stdio.h>
#include <string.h>

int hmac_sha256_test(void) {
    
    char *ptstr = "abc";
    size_t ptlen = strlen(ptstr);
    u8 *hmac_value = (u8 *)malloc(SHA256_DIGEST_SIZE * sizeof(u8));
    u8 key[SHA256_DIGEST_SIZE] = {0x00, };

    
    hmac_sha256_encrypt(ptstr, ptlen, key, SHA256_DIGEST_SIZE, digest);

    printf(" ********** HMAC-SHA256 **********\n");
    printf(" pt:      %s\n", ptstr);
    printf(" mac value: ");
    for (int i=0; i<SHA256_DIGEST_SIZE; i++)
        printf("%02x ", hmac_value[i]); printf("\n");

    // zeroisation
    volatile u8 *p = hmac_value;
    for (int i=0; i<SHA256_DIGEST_SIZE; ++i) p[i] = 0;

    return 0;
}

int main(void) {
    int ret;
    ret = hmac_sha256_test();

    return 0;
}