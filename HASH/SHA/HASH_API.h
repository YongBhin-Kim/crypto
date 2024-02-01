/**
 * @file HASH_API.c
 * @author Yongbhin Kim (coji67@gmail.com)
 * @brief Hash function: SHA-224/256/384/512 APIs
 * @version 0.1
 * @date 2024-02-02
 * 
 * @copyright Copyright (c) Yongbhin Kim 2024
 */


#ifndef HASH_API_H
#define HASH_API_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "SHA256/sha256.h"
// #include "SHA256/sha512.h"

typedef void* (*HashFunction)( IN const void *, IN size_t, OUT void * );
void* sha256Function( IN const void* data, IN size_t len, OUT void* digest );
void* sha512Function( IN const void* data, IN size_t len, OUT void* digest );
void* H( IN const void* data, IN const size_t len, OUT void* digest, IN HashFunction hashFunction );

#ifdef  __cplusplus
}
#endif

#endif   // HASH_API_H