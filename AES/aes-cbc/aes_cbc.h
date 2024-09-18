/**
 * @file aes_cbc.h
 * @author Yongbhin Kim (coji67@gmail.com)
 * @brief api of aes cbc mode.
 * @version 0.1
 * @date 2024-04-14
 * 
 * @copyright Copyright (c) Yongbhin Kim 2024
 */


#ifndef AES_CBC_H
#define AES_CBC_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "../AES32/AES32.h"

#ifndef OUT                             // data output
#define OUT 
#endif

#ifndef IN                              // data input
#define IN
#endif

#ifndef INOUT                           // data input and output
#define INOUT
#endif

#define BLOCK_SIZE 16                   // block size is 128-bit

/**************************** DATA TYPES ****************************/
typedef unsigned char  u8;             // 8-bit byte
typedef unsigned int  u32;             // 32-bit word, change to "long" for 16-bit machines


/*********************** FUNCTION DECLARATIONS **********************/
void aes_cbc_init( OUT AES_CTX *ctx );
void aes_cbc_update( OUT SHA256_CTX *ctx, IN const u8 data[], IN size_t len );

// typedef void* (*AESFunction)( IN const void *, IN size_t, OUT void * );
// void* AESFunction( IN const void* data, IN size_t len, OUT void* ciphertext );
// void* AES( IN const void* data, IN const size_t len, OUT void* ciphertext, IN AESMode AESMode );

#ifdef  __cplusplus
}
#endif

#endif   // AES_CBC_H