/*
Define: 

Block size: 64, 128
Key size: 128, 256

CHAM-64/128, CHAM-128/128 and CHAM 128/256

Block length:= n
Key length:= k
Rounds:= r
Word size:= w-bit

Cham parameters:
====================================
Cipher        n    k    r   w   k/w
------------------------------------
CHAM-64/128   64   128  80  16  8
CHAM-128/128  128  128  80  32  4
CHAM-128/256  128  256  96  32  8
====================================
*/

#include <stdio.h>

// Debug mode: 0 or 1
#define _DEBUG 1

// CHAM 1: n = 64,  k = 128
// CHAM 2: n = 128, k = 128
// CHAM 3: n = 128, k = 256
#define CHAM 1	

#if CHAM == 1   // n = 64, k = 128
#define N 64
#define K 128
#define R 80
#define W 16
#elif CHAM == 2 // n = 128, k = 128
#define N 128
#define K 128
#define R 80
#define W 32
#elif CHAM == 3 // n = 128, k = 256
#define N 128
#define K 256
#define R 96
#define W 32
#else
#endif

#define u16 unsigned short
#define u32 unsigned int


void cham_keyschedule_64_128(u16 *rk, u16 *k)
{
	u16 rol1, rol8, rol11;	 // Rotation to Left ?-bits

    // Do 8-rounds
	for (int i=0; i<K/W; i++) { 
		rol1  = (k[i] << 1 ) ^ (k[i] >> (W - 1));
		rol8  = (k[i] << 8 ) ^ (k[i] >> (W - 8));
		rol11 = (k[i] << 11) ^ (k[i] >> (W - 11));

		rk[i] = k[i] ^ rol1 ^ rol8;
		rk[(i + K/W) ^ 0x0001] = k[i] ^ rol1 ^ rol11;
	}
}

void cham_encrypt_64_128(u16 *ct, u16 *pt, u16 *rk) {
	u16 rol1, rol8, temp, state[4];
    int i, j;

    for (i=0; i<4; i++) {
        state[i] = pt[i];
    }

	for (i=0; i<R; i++) {
		ct[0] = state[1];
		ct[1] = state[2];
		ct[2] = state[3];

        // i is even
		if ( !(i % 2) ) {
			rol1 = (state[1] << 1) | (state[1] >> (W - 1));
			temp = rol1 ^ rk[i % (2 * K / W)]; 
			ct[3] = ((state[0] ^ i) + temp) % 0x10000;
			ct[3] = (ct[3] << 8) | (ct[3] >> (W - 8));
		}
        // i is odd
		else {
			rol8 = (state[1] << 8) | (state[1] >> (W - 8));
			temp = rol8 ^ rk[i % (2 * K / W)];
			ct[3] = ((state[0] ^ i) + temp) % 0x10000;
			ct[3] = (ct[3] << 1) | (ct[3] >> (W - 1));
		}

		for (j=0; j<4; j++) {
            state[j] = ct[j];
        }

#if _DEBUG == 1
		printf("Round %2d : %4x:%4x:%4x:%4x\n", i, state[0], state[1], state[2], state[3]);
#else
#endif
	}

	for (i=0; i<4; i++) {
        ct[i] = state[i];
    }

}

void cham_decrypt_64_128(u16 *pt, u16 *ct, u16 *rk) {
	u16 ror_1, ror_8, temp, state[4];
    int i, j;

    for ( i=0; i<4; i++ ) {
        state[i] = ct[i];
    }

	for ( i=R-1; i>=0; i-- ) {
		pt[3] = state[2];
		pt[2] = state[1];
		pt[1] = state[0];

		if ( !(i%2) ) {
			ror_8 = (state[3] >> 8) | (state[3] << (W - 8));
			temp = ((state[0] << 1) | (state[0] >> (W - 1))) ^ rk[i % (2 * K / W)];
			pt[0] = (ror_8 - temp) % 0x10000;
			pt[0] = pt[0] ^ i;
		}
		else {
			ror_1 = (state[3] >> 1) | (state[3] << (W - 1));
			temp = ((state[0] << 8) | (state[0] >> (W - 8))) ^ rk[i % (2 * K / W)];
			pt[0] = (ror_1 - temp) % 0x10000;
			pt[0] = pt[0] ^ i;
		}

		for ( j=0; j<4; j++ ) {
            state[j] = pt[j];
        }

#if _DEBUG == 1
		printf("Round %2d : %4x:%4x:%4x:%4x\n", i, state[0], state[1], state[2], state[3]);
#endif
	}

	for ( i=0; i<4; i++ ) {
        pt[i] = state[i];
    }

}

// Test code for CHAM64/128
void cham64_128_test() {
    
    // Define parameters for CHAM 1 (k = 64, n = 128)
	u16 key[8] = { 0, }; // 0x0000, 0x0000, 0x0000, 0x0000
	u16 rk[16] = { 0, }; // 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
	u16 pt[4]  = { 0, }; // 0x0000, 0x0000, 0x0000, 0x0000

	u16 ct[4]  = { 0, }; // 0x6d65, 0x0d74, 0x9773, 0xd515
	u16 dec[4] = { 0, }; // 0x0000, 0x0000, 0x0000, 0x0000

    // Keygen
    cham_keyschedule_64_128(rk, key);

    // Encrypt
	cham_encrypt_64_128(ct, pt, rk);

    // Decrypt
	cham_decrypt_64_128(dec, ct, rk);

    // Debug
    printf("Plaintext:      %4x:%4x:%4x:%4x\n\n", pt[0], pt[1], pt[2], pt[3]);
	printf("Ciphertext:     %4x:%4x:%4x:%4x\n\n", ct[0], ct[1], ct[2], ct[3]);
	printf("Decrypted text: %4x:%4x:%4x:%4x\n", dec[0], dec[1], dec[2], dec[3]);
}

int main() {
#if CHAM == 1
    cham64_128_test();
#elif CHAM == 2
    cham128_128_test();
#elif CHAM == 3
    cham128_256_test();
#endif
    return 0;
}
