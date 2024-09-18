#ifndef CHAM_H
#define CHAM_H

#include <stdio.h>

#define u16 unsigned short
#define u32 unsigned int

#define CHAM 1		//n=64, k=128
//#define CHAM 2	//n=128, k=128
//#define CHAM 3	//n=128, k=256

#if CHAM==1 // n=64, k=128
#define R 80
#define N 64
#define K 128
#define W 16
#elif CHAM==2 //n=128, k=128
#define R 80
#define N 128
#define K 128
#define W 32
#else // n=128, k=256
#define R 96
#define N 128
#define K 256
#define W 32
#endif

void KeyExpansion64_128(u16 k[], u16 rk[])
{
	u16 rol_1;		// Rotation Left 1
	u16 rol_8;		// Rotationo Left 8
	u16 rol_11;		// Rotationo Left 11

	u16 tmp;

	for (int i = 0; i < K / W; i++) {
		rol_1 = (k[i] << 1) | (k[i] >> (W - 1));
		rol_8 = (k[i] << 8) | (k[i] >> (W - 8));
		rol_11 = (k[i] << 11) | (k[i] >> (W - 11));

		rk[i] = k[i] ^ rol_1 ^ rol_8;
		rk[(i + (K / W)) ^ 0x0001] = k[i] ^ rol_1 ^ rol_11;
	}
}

void Encryption64_128(u16 pt[], u16 rk[], u16 ct[])
{
	u16 rol_1;
	u16 rol_8;
	u16 tmp;
	for (int i = 0; i < R; i++) {
		ct[0] = pt[1];
		ct[1] = pt[2];
		ct[2] = pt[3];
		if (i % 2 == 0) {
			rol_1 = (pt[1] << 1) | (pt[1] >> (W - 1));
			tmp = rol_1 ^ rk[i % (2 * K / W)];
			ct[3] = ((pt[0] ^ i) + tmp) % 0x10000;
			ct[3] = (ct[3] << 8) | (ct[3] >> (W - 8));
		}
		else {
			rol_8 = (pt[1] << 8) | (pt[1] >> (W - 8));
			tmp = rol_8 ^ rk[i % (2 * K / W)];
			ct[3] = ((pt[0] ^ i) + tmp) % 0x10000;
			ct[3] = (ct[3] << 1) | (ct[3] >> (W - 1));
		}

		for (int j = 0; j < 4; j++) pt[j] = ct[j];
		// // printf("Round %d : %x %x %x %x\n", i, pt[0], pt[1], pt[2], pt[3]);
	}

	for (int i = 0; i < 4; i++) ct[i] = pt[i];
}

void Decryption64_128(u16 ct[], u16 rk[], u16 pt[])
{
	u16 ror_1;
	u16 ror_8;
	u16 tmp;

	for (int i = R-1; i >= 0; i--) {
		pt[3] = ct[2];
		pt[2] = ct[1];
		pt[1] = ct[0];

		if (i % 2 == 0) {
			ror_8 = (ct[3] >> 8) | (ct[3] << (W - 8));
			tmp = ((ct[0] << 1) | (ct[0] >> (W - 1))) ^ rk[i % (2 * K / W)];
			pt[0] = (ror_8 - tmp) % 0x10000;
			pt[0] = pt[0] ^ i;
		}
		else {
			ror_1 = (ct[3] >> 1) | (ct[3] << (W - 1));
			tmp = ((ct[0] << 8) | (ct[0] >> (W - 8))) ^ rk[i % (2 * K / W)];
			pt[0] = (ror_1 - tmp) % 0x10000;
			pt[0] = pt[0] ^ i;
		}

		for (int j = 0; j < 4; j++) ct[j] = pt[j];

		// printf("Round %d : %4x %4x %4x %4x\n", i, ct[0], ct[1], ct[2], ct[3]);
	}

	for (int i = 0; i < 4; i++) pt[i] = ct[i];

}


void KeyExpansion128_128(u32 k[], u32 rk[])
{
	u32 rol_1;		// Rotation Left 1
	u32 rol_8;		// Rotationo Left 8
	u32 rol_11;		// Rotationo Left 11

	u32 tmp;

	for (int i = 0; i < K / W; i++) {
		rol_1 = (k[i] << 1) | (k[i] >> (W - 1));
		rol_8 = (k[i] << 8) | (k[i] >> (W - 8));
		rol_11 = (k[i] << 11) | (k[i] >> (W - 11));

		rk[i] = k[i] ^ rol_1 ^ rol_8;
		rk[(i + (K / W)) ^ 0x00000001] = k[i] ^ rol_1 ^ rol_11;
	}

}

void Encryption128_128(u32 pt[], u32 rk[], u32 ct[])
{
	u32 rol_1;
	u32 rol_8;
	u32 tmp;

	for (int i = 0; i < R; i++) {
		ct[0] = pt[1];
		ct[1] = pt[2];
		ct[2] = pt[3];
		if (i % 2 == 0) {
			rol_1 = (pt[1] << 1) | (pt[1] >> (W - 1));
			tmp = rol_1 ^ rk[i % (2 * K / W)];
			ct[3] = ((pt[0] ^ i) + tmp) % 0x100000000; // Addition
			ct[3] = (ct[3] << 8) | (ct[3] >> (W - 8));
		}
		else {
			rol_8 = (pt[1] << 8) | (pt[1] >> (W - 8));
			tmp = rol_8 ^ rk[i % (2 * K / W)];
			ct[3] = ((pt[0] ^ i) + tmp) % 0x100000000;
			ct[3] = (ct[3] << 1) | (ct[3] >> (W - 1));
		}

		for (int j = 0; j < 4; j++) pt[j] = ct[j];
		// printf("Round %d : %x %x %x %x\n", i, pt[0], pt[1], pt[2], pt[3]);

	}

	for (int i = 0; i < 4; i++) ct[i] = pt[i];

}

void Decryption128_128(u32 ct[], u32 rk[], u32 pt[])
{
	u32 ror_1;
	u32 ror_8;
	u32 tmp;

	for (int i = R - 1; i >= 0; i--) {
		pt[3] = ct[2];
		pt[2] = ct[1];
		pt[1] = ct[0];

		if (i % 2 == 0) {
			ror_8 = (ct[3] >> 8) | (ct[3] << (W - 8));
			tmp = ((ct[0] << 1) | (ct[0] >> (W - 1))) ^ rk[i % (2 * K / W)];
			pt[0] = (ror_8 - tmp) % 0x100000000;
			pt[0] = pt[0] ^ i;
		}
		else {
			ror_1 = (ct[3] >> 1) | (ct[3] << (W - 1));
			tmp = ((ct[0] << 8) | (ct[0] >> (W - 8))) ^ rk[i % (2 * K / W)];
			pt[0] = (ror_1 - tmp) % 0x100000000;
			pt[0] = pt[0] ^ i;
		}

		for (int j = 0; j < 4; j++) ct[j] = pt[j];

		// printf("Round %d : %4x %4x %4x %4x\n", i, ct[0], ct[1], ct[2], ct[3]);
	}

	for (int i = 0; i < 4; i++) pt[i] = ct[i];


}


void KeyExpansion128_256(u32 k[], u32 rk[])
{
	u32 rol_1;		// Rotation Left 1
	u32 rol_8;		// Rotationo Left 8
	u32 rol_11;		// Rotationo Left 11

	u32 tmp;

	for (int i = 0; i < K / W; i++) {
		rol_1 = (k[i] << 1) | (k[i] >> (W - 1));
		rol_8 = (k[i] << 8) | (k[i] >> (W - 8));
		rol_11 = (k[i] << 11) | (k[i] >> (W - 11));

		rk[i] = k[i] ^ rol_1 ^ rol_8;
		rk[(i + (K / W)) ^ 0x00000001] = k[i] ^ rol_1 ^ rol_11;
	}

}

void Encryption128_256(u32 pt[], u32 rk[], u32 ct[])
{
	u32 rol_1;
	u32 rol_8;
	u32 tmp;

	for (int i = 0; i < R; i++) {
		ct[0] = pt[1];
		ct[1] = pt[2];
		ct[2] = pt[3];
		if (i % 2 == 0) {
			rol_1 = (pt[1] << 1) | (pt[1] >> (W - 1));
			tmp = rol_1 ^ rk[i % (2 * K / W)];
			ct[3] = ((pt[0] ^ i) + tmp) % 0x100000000; // Addition
			ct[3] = (ct[3] << 8) | (ct[3] >> (W - 8));
		}
		else {
			rol_8 = (pt[1] << 8) | (pt[1] >> (W - 8));
			tmp = rol_8 ^ rk[i % (2 * K / W)];
			ct[3] = ((pt[0] ^ i) + tmp) % 0x100000000;
			ct[3] = (ct[3] << 1) | (ct[3] >> (W - 1));
		}

		for (int j = 0; j < 4; j++) pt[j] = ct[j];
		// printf("Round %d : %x %x %x %x\n", i, pt[0], pt[1], pt[2], pt[3]);

	}

	for (int i = 0; i < 4; i++) ct[i] = pt[i];

}

void Decryption128_256(u32 ct[], u32 rk[], u32 pt[])
{
	u32 ror_1;
	u32 ror_8;
	u32 tmp;

	for (int i = R - 1; i >= 0; i--) {
		pt[3] = ct[2];
		pt[2] = ct[1];
		pt[1] = ct[0];

		if (i % 2 == 0) {
			ror_8 = (ct[3] >> 8) | (ct[3] << (W - 8));
			tmp = ((ct[0] << 1) | (ct[0] >> (W - 1))) ^ rk[i % (2 * K / W)];
			pt[0] = (ror_8 - tmp) % 0x100000000;
			pt[0] = pt[0] ^ i;
		}
		else {
			ror_1 = (ct[3] >> 1) | (ct[3] << (W - 1));
			tmp = ((ct[0] << 8) | (ct[0] >> (W - 8))) ^ rk[i % (2 * K / W)];
			pt[0] = (ror_1 - tmp) % 0x100000000;
			pt[0] = pt[0] ^ i;
		}

		for (int j = 0; j < 4; j++) ct[j] = pt[j];

		// printf("Round %d : %4x %4x %4x %4x\n", i, ct[0], ct[1], ct[2], ct[3]);
	}

	for (int i = 0; i < 4; i++) pt[i] = ct[i];


}


#endif