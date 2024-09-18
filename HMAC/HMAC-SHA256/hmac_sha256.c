
/**
 * @file hmac_sha256.c
 * @author Yongbhin Kim (coji67@gmail.com)
 * @brief HMAC_SHA256 implementations
 * @version 0.1
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) Yongbhin Kim 2024
 */

/*************************** HEADER FILES ***************************/
#include <memory.h>
#include "hmac_sha256.h"

#define ZEROIZE(data, len) {for (size_t i=0; i<len; i++) data[i] = 0;}

void hmac_sha256_encrypt( IN const *u8 data, IN size_t datalen, IN const *u8 key, IN size_t keylen, OUT u8 digest[] ) {
    size_t i;
    u8 kp[SHA256_DIGEST_SIZE], y[SHA256_DIGEST_SIZE];
    u8 *x = (u8 *)malloc((sizeof)u8 * (SHA256_DIGEST_SIZE + SHA256_DIGEST_SIZE + datalen));

    // keylength > SHAlength
    if ( keylen > (size_t) SHA256_DIGEST_SIZE ) {
        sha256_encrypt(key, keylen, kp); // k' <- H(key)
        // x <- (k' ^ opad || k' ^ ipad) (64-bytes) 
        for ( i=0; i<SHA256_DIGEST_SIZE; i++ ) {
            x[i]                      = kp[i] ^ 0x5c;
            x[SHA256_DIGEST_SIZE + i] = kp[i] ^ 0x36;
        }
    }
    // keylength <= SHAlength
    else {
        // x <- (k ^ opad || k ^ ipad) (64-bytes)
        for ( i=0; i<keylen; i++ ) {
            x[i] = key[i] ^ 0x5c;
            x[SHA256_DIGEST_SIZE + i] = 0x36;
        }
        for ( ; i<SHA256_DIGEST_SIZE; i++ ) {
            x[i] = key[i] ^ 0x5c;
            x[SHA256_DIGEST_SIZE + i] = 0x36;
        }
    }
    // x <- (k ^ opad || k ^ ipad || m) (64 + datalen-bytes) 
    for ( i=0; i<datalen; i++)
        x[SHA256_DIGEST_SIZE + SHA256_DIGEST_SIZE + i] = data[i];

    // x <- ( k^opad || H(k^ipad||m) ) (64 + datalen-bytes) 
    // (y <- H(k^ipad||m))
    sha256_encrypt(x + SHA256_DIGEST_SIZE, SHA256_DIGEST_SIZE + datalen, y);
    for ( i=0; i<SHA256_DIGEST_SIZE; i++ )
        x[SHA256_DIGEST_SIZE + i] = y[i];
    
    // x <- H (k^opad || H(k^ipad||m)) (64-bytes) 
    sha256_encrypt(x, SHA256_DIGEST_SIZE + SHA256_DIGEST_SIZE, digest);

    ZEROIZE(x, SHA256_DIGEST_SIZE + SHA256_DIGEST_SIZE + datalen);
    free(x);
}