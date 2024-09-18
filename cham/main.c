#include <stdio.h>
#include <string.h>
#include "cham.h"

// cham[ptlength]_[keylength]

void cham64_128()
{
	u16 key[8] = { 0x0100, 0x0302, 0x0504, 0x0706, 0x0908, 0x0b0a, 0x0d0c, 0x0f0e };
	u16 pt[4] = { 0x1100, 0x3322, 0x5544, 0x7766 };
	u16 pt2[4] = { 0, };
	u16 ct[4] = { 0x453c, 0x63bc, 0xdcfa, 0xbf4e };
	u16 ct2[4] = { 0, };
	u16 rk[16] = { 0, };

	KeyExpansion64_128(key, rk);

	Encryption64_128(pt, rk, ct2);
	if ( memcmp(ct, ct2, 4 * sizeof(u16)) == 0 ) {
		printf(" * CHAM64-128 Encrypt test success\n");
		printf("ct:        0x%4x %4x %4x %4x\n", ct2[0], ct2[1], ct2[2], ct2[3]);
	}
	else {
		printf(" * Encrypt test failed\n");
		return;
	}
	Decryption64_128(ct2, rk, pt2);
	if ( memcmp(pt, pt2, 4 * sizeof(u16)) == 0 ) {
		printf(" * CHAM64-128 Decrypt test success\n");
		printf("recovered: 0x%4x %4x %4x %4x\n", pt2[0], pt2[1], pt2[2], pt2[3]);
		printf("==============================================================\n");
	}
	else {
		printf(" * Decrypt test failed\n");
		return;
	}
}

void cham128_128()
{
	u32 key[4] = { 0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c };
	u32 pt[4] = { 0x33221100, 0x77665544, 0xbbaa9988, 0xffeeddcc };
	u32 pt2[4] = { 0, };
	u32 ct[4] = { 0xc3746034, 0xb55700c5, 0x8d64ec32, 0x489332f7 };
	u32 ct2[4] = { 0, };
	u32 rk[8] = { 0, };

	KeyExpansion128_128(key, rk);

	Encryption128_128(pt, rk, ct2);
	printf("%x %x %x %x\n", ct2[0], ct2[1], ct2[2], ct2[3]);
	printf("==============================================================\n");
	Decryption128_128(ct2, rk, pt2);
	printf("%x %x %x %x\n", pt2[0], pt2[1], pt2[2], pt2[3]);
}

void cham128_256()
{
	u32 key[8] = { 0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c, 0xf3f2f1f0, 0xf7f6f5f4, 0xfbfaf9f8, 0xfffefdfc };
	u32 pt[4] = { 0x33221100, 0x77665544, 0xbbaa9988, 0xffeeddcc };
	u32 pt2[4] = { 0, };
	u32 ct[4] = { 0xa899c8a0, 0xc929d55c, 0xab670d38, 0x0c4f7ac8 };
	u32 ct2[4] = { 0, };
	u32 rk[16] = { 0, };

	KeyExpansion128_256(key, rk);
	Encryption128_256(pt, rk, ct2);
	printf("%x %x %x %x\n", ct2[0], ct2[1], ct2[2], ct2[3]);
	printf("==============================================================\n");
	Decryption128_256(ct2, rk, pt2);
	printf("%x %x %x %x\n", pt2[0], pt2[1], pt2[2], pt2[3]);

}

#ifdef CHAM_TEST_IN_SOURCES
int main(void)
{
	cham64_128();
	//cham128_128();
	//cham128_256();
	return 0;
}
#endif