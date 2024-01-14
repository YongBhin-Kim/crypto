/**
 * @file main.c
 * @author Yongbhin Kim (coji67@gmail.com)
 * @brief SHA224 test codes
 * @version 0.1
 * @date 2024-01-09
 * 
 * @copyright Copyright (c) Yongbhin Kim 2024
 */


#include "sha224.h"
#include <stdio.h>
#include <string.h>

int sha224_test(void) {
    
    char *ptstr = "abc";
    size_t ptlen = strlen(ptstr);
    u8 *digest = (u8 *)malloc(SHA224_DIGEST_SIZE * sizeof(u8));

    SHA224_CTX ctx;

    // SHA224 encrypt - 1
    sha224_init(&ctx);
    sha224_update(&ctx, (u8 *)ptstr, ptlen);
    sha224_final(&ctx, digest);

    printf(" ********** SHA224 **********\n");
    printf(" pt:      %s\n", ptstr);
    printf(" digest1: ");
    for (int i=0; i<SHA224_DIGEST_SIZE; i++)
        printf("%02x ", digest[i]); printf("\n");

    // zeroisation
    volatile u8 *p = digest;
    for (int i=0; i<SHA224_DIGEST_SIZE; ++i) p[i] = 0;

    // SHA224 encrypt - 2
    sha224_encrypt((u8 *)ptstr, ptlen, digest);
    printf(" digest2: ");
    for (int i=0; i<SHA224_DIGEST_SIZE; i++)
        printf("%02x ", digest[i]); printf("\n");

    return 0;
}

int sha224_test_stdin(void) {

    char ptstr[100000];
    size_t ptlen;
    u8 *digest = (u8 *)malloc(SHA224_DIGEST_SIZE * sizeof(u8));
    
    // Get input string from standart inputs.
    printf(" ********** SHA224 **********\n");
    printf(" Input your data: "); 
    fgets(ptstr, 100000, stdin);
    ptlen = strlen(ptstr);
    ptstr[ptlen] = '\0';
    ptlen--;

    // Declare SHA-224 ctx
    SHA224_CTX ctx;

    // SHA224 encrypt - 1
    sha224_init(&ctx);
    sha224_update(&ctx, (u8 *)ptstr, ptlen);
    sha224_final(&ctx, digest);

    printf(" pt:      %s\n", ptstr);
    printf(" digest1: ");
    for (int i=0; i<SHA224_DIGEST_SIZE; i++)
        printf("%02x ", digest[i]); printf("\n");

    // zeroisation
    volatile u8 *p = digest;
    for (int i=0; i<SHA224_DIGEST_SIZE; ++i) p[i] = 0;

    // SHA224 encrypt - 2
    sha224_encrypt((u8 *)ptstr, ptlen, digest);
    printf(" digest2: ");
    for (int i=0; i<SHA224_DIGEST_SIZE; i++)
        printf("%02x ", digest[i]); printf("\n");

    return 0;
}

int main(void) {
    int ret;
    ret = sha224_test();
    ret = sha224_test_stdin();

    return 0;
}