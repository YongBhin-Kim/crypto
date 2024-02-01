/**
 * @file sha2_test.c
 * @author Yongbhin Kim (coji67@gmail.com)
 * @brief SHA-224/256/384/512 test codes
 * @version 0.2
 * @date 2024-02-02
 * 
 * @copyright Copyright (c) Yongbhin Kim 2024
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HASH_API.h"

int main() {

    char ptstr[100000];
    size_t ptlen;
    u8 *digest = (u8 *)malloc(SHA512_DIGEST_SIZE * sizeof(u8));
    
    // SHA256 encrypt
    // Get input string from standart inputs.
    printf(" ********** SHA-256 **********\n");
    printf(" Input your data: "); 
    fgets(ptstr, 100000, stdin);
    ptlen = strlen(ptstr);
    ptstr[ptlen] = '\0';
    ptlen--;

    HashFunction hashFunction = sha256Function;
    H((u8 *)ptstr, ptlen, digest, hashFunction);
    printf(" digest: ");
    for (int i=0; i<SHA256_DIGEST_SIZE; i++)
        printf("%02x ", digest[i]); printf("\n");

    // zeroisation
    volatile u8 *p = digest;
    for (int i=0; i<SHA256_DIGEST_SIZE; ++i) p[i] = 0;
    volatile char *q = ptstr;
    for (int i=0; i<100000; ++i) q[i] = 0;

    // SHA512 encrypt
    // Get input string from standart inputs.
    printf(" ********** SHA-512 **********\n");
    printf(" Input your data: "); 
    fgets(ptstr, 100000, stdin);
    ptlen = strlen(ptstr);
    ptstr[ptlen] = '\0';
    ptlen--;
    hashFunction = sha512Function;
    H((u8 *)ptstr, ptlen, digest, hashFunction);
    printf(" digest: ");
    for (int i=0; i<SHA512_DIGEST_SIZE; i++)
        printf("%02x ", digest[i]); printf("\n");

    // zeroisation
    for (int i=0; i<SHA512_DIGEST_SIZE; ++i) p[i] = 0;

    return 0;
}