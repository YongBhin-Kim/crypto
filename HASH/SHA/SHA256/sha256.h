/**
 * @file sha256.h
 * @author Yongbhin Kim (coji67@gmail.com)
 * @brief SHA256 implementations
 * @version 0.1
 * @date 2024-01-09
 * 
 * @copyright Copyright (c) Yongbhin Kim 2024
 */

#ifndef SHA256_H
#define SHA256_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#ifndef OUT                             // data output
#define OUT 
#endif

#ifndef IN                              // data input
#define IN
#endif

#ifndef INOUT                           // data input and output
#define INOUT
#endif

#define SHA256_DIGEST_SIZE 32  
#define SHA512_DIGEST_SIZE 64

/**************************** DATA TYPES ****************************/
typedef unsigned char  u8;             // 8-bit byte
typedef unsigned int  u32;             // 32-bit word, change to "long" for 16-bit machines

typedef struct {
	u8  data[64];
	u32 datalen;
	unsigned long long bitlen;
	u32 state[8];
} SHA256_CTX;

/*********************** FUNCTION DECLARATIONS **********************/
void sha256_init( OUT SHA256_CTX *ctx );
void sha256_update( OUT SHA256_CTX *ctx, IN const u8 data[], IN size_t len );
void sha256_final( OUT SHA256_CTX *ctx, OUT u8 digest[] );
void sha256_encrypt( IN const u8 *data, IN size_t len, OUT u8 digest[] );

#ifdef  __cplusplus
}
#endif

#endif   // SHA256_H