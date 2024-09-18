/**
 * @file hmac_sha256.h
 * @author Yongbhin Kim (coji67@gmail.com)
 * @brief HMAC_SHA256 implementations
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) Yongbhin Kim 2024
 */

#ifndef HMAC_SHA256_H
#define HMAC_SHA256_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "../../HASH/SHA/SHA256/sha256.h"

// #ifndef OUT                             // data output
// #define OUT 
// #endif
// 
// #ifndef IN                              // data input
// #define IN
// #endif
// 
// #ifndef INOUT                           // data input and output
// #define INOUT
// #endif
// 
// #define SHA256_DIGEST_SIZE 32  
// #define SHA512_DIGEST_SIZE 64

/**************************** DATA TYPES ****************************/
// typedef unsigned char  u8;             // 8-bit byte
// typedef unsigned int  u32;             // 32-bit word, change to "long" for 16-bit machines

/*********************** FUNCTION DECLARATIONS **********************/
void hmac_sha256_encrypt( IN const *u8 data, IN size_t datalen, IN const *u8 key, IN size_t keylen, OUT u8 digest[] );

#ifdef  __cplusplus
}
#endif

#endif   // SHA256_H