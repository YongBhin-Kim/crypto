/**
 * @file HASH_API.c
 * @author Yongbhin Kim (coji67@gmail.com)
 * @brief Hash function: SHA-224/256/384/512 APIs
 * @version 0.1
 * @date 2024-02-02
 * 
 * @copyright Copyright (c) Yongbhin Kim 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HASH_API.h"


// typedef void* (*HashFunction)( IN const void *, IN size_t, OUT void * );

void* sha256Function( IN const void* data, IN size_t len, OUT void* digest ) {
    sha256_encrypt(data, len, digest);
    return digest;
}

void* sha512Function( IN const void* data, IN size_t len, OUT void* digest ) {
    sha256_encrypt(data, len, digest); // sha512_encrypt(data, len, digest);
    return digest;
}

void* H( IN const void* data, IN const size_t len, OUT void* digest, IN HashFunction hashFunction ) {
    return hashFunction(data, len, digest);
}
