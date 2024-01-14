/**
 * @file sha256.c
 * @author Yongbhin Kim (coji67@gmail.com)
 * @brief SHA256 implementations
 * @version 0.1
 * @date 2024-01-09
 * 
 * @copyright Copyright (c) Yongbhin Kim 2024
 */

/*************************** HEADER FILES ***************************/
#include <memory.h>
#include "sha256.h"

/************************ Macros - SHA256 Functions *************************/
#define rotl(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define rotr(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define e0(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define e1(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z))) 
#define e2(x)     (rotr(x,2)  ^ rotr(x,13) ^ rotr(x,22) ) 
#define e3(x)     (rotr(x,6)  ^ rotr(x,11) ^ rotr(x,25) ) 
#define e4(x)     (rotr(x,7)  ^ rotr(x,18) ^ ((x) >> 3) ) 
#define e5(x)     (rotr(x,17) ^ rotr(x,19) ^ ((x) >> 10))

/**************************** SHA256 Constants *****************************/
static const u32 C[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

/*********************** SHA256 Function Definitions ***********************/
void _sha256_transform( SHA256_CTX *ctx, const u8 data[] ) {
	u32 a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	for ( ; i < 64; ++i)
		m[i] = e5(m[i - 2]) + m[i - 7] + e4(m[i - 15]) + m[i - 16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + e3(e) + e0(e,f,g) + C[i] + m[i];
		t2 = e2(a) + e1(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void sha256_init( OUT SHA256_CTX *ctx ) {
    ctx->datalen = ctx->bitlen = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

void sha256_update( OUT SHA256_CTX *ctx, IN const u8 data[], IN size_t len ) {
	u32 i;

	for (i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			_sha256_transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void sha256_final( OUT SHA256_CTX *ctx, OUT u8 digest[] ) {
	u32 i;

	i = ctx->datalen;

	// Pad whatever data is left in the buffer.
	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else {
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;
		_sha256_transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}

	// Append to the padding the total message's length in bits and transform.
	ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	_sha256_transform(ctx, ctx->data);

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i=0; i<4; ++i) {
		digest[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

void sha256_encrypt( IN const u8 *data, IN size_t len, OUT u8 digest[] ) {
    
    // SHA-256 encrypt
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, data, len);
    sha256_final(&ctx, digest);

    // Zeroisation
    u8 i;
    volatile u8 *p  = (u8 *)ctx.data;
    volatile u32 *q = (u32 *)ctx.state;
    for (i=0; i<64; ++i) ctx.data[i] = 0x00;
    for (i=0; i<8;  ++i) ctx.state[i] = 0x00000000;
}