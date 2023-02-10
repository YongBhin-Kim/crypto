/*
    GHASH (for gcm mode)
*/
//  Copyright © Yonhbhin Kim
//  Github url : https://github.com/YongBhin-Kim

#include <stdio.h>
#include "../Util/util.h"

unsigned char R0[256] = {
	0x00, 0x01, 0x03, 0x02, 0x07, 0x06, 0x04, 0x05, 0x0e, 0x0f, 0x0d, 0x0c, 0x09, 0x08, 0x0a, 0x0b, 
	0x1c, 0x1d, 0x1f, 0x1e, 0x1b, 0x1a, 0x18, 0x19, 0x12, 0x13, 0x11, 0x10, 0x15, 0x14, 0x16, 0x17, 
	0x38, 0x39, 0x3b, 0x3a, 0x3f, 0x3e, 0x3c, 0x3d, 0x36, 0x37, 0x35, 0x34, 0x31, 0x30, 0x32, 0x33, 
	0x24, 0x25, 0x27, 0x26, 0x23, 0x22, 0x20, 0x21, 0x2a, 0x2b, 0x29, 0x28, 0x2d, 0x2c, 0x2e, 0x2f, 
	0x70, 0x71, 0x73, 0x72, 0x77, 0x76, 0x74, 0x75, 0x7e, 0x7f, 0x7d, 0x7c, 0x79, 0x78, 0x7a, 0x7b, 
	0x6c, 0x6d, 0x6f, 0x6e, 0x6b, 0x6a, 0x68, 0x69, 0x62, 0x63, 0x61, 0x60, 0x65, 0x64, 0x66, 0x67, 
	0x48, 0x49, 0x4b, 0x4a, 0x4f, 0x4e, 0x4c, 0x4d, 0x46, 0x47, 0x45, 0x44, 0x41, 0x40, 0x42, 0x43, 
	0x54, 0x55, 0x57, 0x56, 0x53, 0x52, 0x50, 0x51, 0x5a, 0x5b, 0x59, 0x58, 0x5d, 0x5c, 0x5e, 0x5f, 
	0xe1, 0xe0, 0xe2, 0xe3, 0xe6, 0xe7, 0xe5, 0xe4, 0xef, 0xee, 0xec, 0xed, 0xe8, 0xe9, 0xeb, 0xea, 
	0xfd, 0xfc, 0xfe, 0xff, 0xfa, 0xfb, 0xf9, 0xf8, 0xf3, 0xf2, 0xf0, 0xf1, 0xf4, 0xf5, 0xf7, 0xf6, 
	0xd9, 0xd8, 0xda, 0xdb, 0xde, 0xdf, 0xdd, 0xdc, 0xd7, 0xd6, 0xd4, 0xd5, 0xd0, 0xd1, 0xd3, 0xd2, 
	0xc5, 0xc4, 0xc6, 0xc7, 0xc2, 0xc3, 0xc1, 0xc0, 0xcb, 0xca, 0xc8, 0xc9, 0xcc, 0xcd, 0xcf, 0xce, 
	0x91, 0x90, 0x92, 0x93, 0x96, 0x97, 0x95, 0x94, 0x9f, 0x9e, 0x9c, 0x9d, 0x98, 0x99, 0x9b, 0x9a, 
	0x8d, 0x8c, 0x8e, 0x8f, 0x8a, 0x8b, 0x89, 0x88, 0x83, 0x82, 0x80, 0x81, 0x84, 0x85, 0x87, 0x86, 
	0xa9, 0xa8, 0xaa, 0xab, 0xae, 0xaf, 0xad, 0xac, 0xa7, 0xa6, 0xa4, 0xa5, 0xa0, 0xa1, 0xa3, 0xa2, 
	0xb5, 0xb4, 0xb6, 0xb7, 0xb2, 0xb3, 0xb1, 0xb0, 0xbb, 0xba, 0xb8, 0xb9, 0xbc, 0xbd, 0xbf, 0xbe
 };
unsigned char R1[256] = {
	0x00, 0xc2, 0x84, 0x46, 0x08, 0xca, 0x8c, 0x4e, 0x10, 0xd2, 0x94, 0x56, 0x18, 0xda, 0x9c, 0x5e, 
	0x20, 0xe2, 0xa4, 0x66, 0x28, 0xea, 0xac, 0x6e, 0x30, 0xf2, 0xb4, 0x76, 0x38, 0xfa, 0xbc, 0x7e, 
	0x40, 0x82, 0xc4, 0x06, 0x48, 0x8a, 0xcc, 0x0e, 0x50, 0x92, 0xd4, 0x16, 0x58, 0x9a, 0xdc, 0x1e, 
	0x60, 0xa2, 0xe4, 0x26, 0x68, 0xaa, 0xec, 0x2e, 0x70, 0xb2, 0xf4, 0x36, 0x78, 0xba, 0xfc, 0x3e, 
	0x80, 0x42, 0x04, 0xc6, 0x88, 0x4a, 0x0c, 0xce, 0x90, 0x52, 0x14, 0xd6, 0x98, 0x5a, 0x1c, 0xde, 
	0xa0, 0x62, 0x24, 0xe6, 0xa8, 0x6a, 0x2c, 0xee, 0xb0, 0x72, 0x34, 0xf6, 0xb8, 0x7a, 0x3c, 0xfe, 
	0xc0, 0x02, 0x44, 0x86, 0xc8, 0x0a, 0x4c, 0x8e, 0xd0, 0x12, 0x54, 0x96, 0xd8, 0x1a, 0x5c, 0x9e, 
	0xe0, 0x22, 0x64, 0xa6, 0xe8, 0x2a, 0x6c, 0xae, 0xf0, 0x32, 0x74, 0xb6, 0xf8, 0x3a, 0x7c, 0xbe, 
	0x00, 0xc2, 0x84, 0x46, 0x08, 0xca, 0x8c, 0x4e, 0x10, 0xd2, 0x94, 0x56, 0x18, 0xda, 0x9c, 0x5e, 
	0x20, 0xe2, 0xa4, 0x66, 0x28, 0xea, 0xac, 0x6e, 0x30, 0xf2, 0xb4, 0x76, 0x38, 0xfa, 0xbc, 0x7e, 
	0x40, 0x82, 0xc4, 0x06, 0x48, 0x8a, 0xcc, 0x0e, 0x50, 0x92, 0xd4, 0x16, 0x58, 0x9a, 0xdc, 0x1e, 
	0x60, 0xa2, 0xe4, 0x26, 0x68, 0xaa, 0xec, 0x2e, 0x70, 0xb2, 0xf4, 0x36, 0x78, 0xba, 0xfc, 0x3e, 
	0x80, 0x42, 0x04, 0xc6, 0x88, 0x4a, 0x0c, 0xce, 0x90, 0x52, 0x14, 0xd6, 0x98, 0x5a, 0x1c, 0xde, 
	0xa0, 0x62, 0x24, 0xe6, 0xa8, 0x6a, 0x2c, 0xee, 0xb0, 0x72, 0x34, 0xf6, 0xb8, 0x7a, 0x3c, 0xfe, 
	0xc0, 0x02, 0x44, 0x86, 0xc8, 0x0a, 0x4c, 0x8e, 0xd0, 0x12, 0x54, 0x96, 0xd8, 0x1a, 0x5c, 0x9e, 
	0xe0, 0x22, 0x64, 0xa6, 0xe8, 0x2a, 0x6c, 0xae, 0xf0, 0x32, 0x74, 0xb6, 0xf8, 0x3a, 0x7c, 0xbe
 };
 

/*************************************************
* Name:        GF128_xtime
*
* Description: x times over galois field GF(2^128) 
*
* Arguments: 
*			 - unsigned char p[16]: p <- x * p (p \in gf(2^128))
*									(p(x) <- x*p(x))
**************************************************/
void GF128_xtime(unsigned char p[16]) {
	unsigned char msb; 
	msb = p[15] & 0x01;

	for (int i=15; i>0; i--) {
		p[i] = (p[i] >> 1) | ((p[i-1] & 0x01) << 7); // | or ^
	}
	p[0] >>= 1; // p[0] = p[0] >> 1;

	if (msb == 1) { // p[127] == 1
		p[0] ^= 0xE1; // p[0] = p[0] ^ 0b11100001; 
	}

}

/*************************************************
* Name:        GF128_mul
*
* Description: multiplications over GF(2^128) : p*q -> p
*			p(x) <- p(x) * q(x)
*
* 			p(x) = p0 + p1x + ... + p127x^127
* 			q(x) = q0 + q1x + ... + q127x^127
*			p(x)q(x) = p(x)(q0 + .. + q127x^127)
*					 = q0*p(x) + q1*x*p(x) + ... + q127 * x^127 * p(x)
*
*
* Arguments: 
*			 - unsigned char p[16]: poly
*			 - unsigned char q[16]: poly
**************************************************/
void GF128_mul(unsigned char p[16], unsigned char q[16]) {
	unsigned char z[16] = { 0, };
	unsigned char qi;
	for (int i=0; i<16; i++) { // 바이트
		for (int j=0; j<8; j++) { // 비트
			qi = (q[i] >> (7-j)) & 0x01; // q0 q1 ... q127
			// q1 = q[i] & (1 << (7-j)); // 1000 0000 0100 0000 ...
			if (qi) { // qi가 0이 아닌 경우 (즉 qi == 1)
				for (int k=0; k<16; k++) {
					z[k] ^= p[k];
				}
			}
			GF128_xtime(p); // p(x) <- x*p(x)
		}
	}



	for (int k = 0; k < 16; k++) p[k] = z[k];
}

// GHash 테이블 고속화에 필요
// 7차 이하 다항식 * x^128
// 한바이트 (a0+a1*x+...a7*x^7)에 x^128을 곱한 결과를 R0[]R1[] 두 바이트에 저장
/*
	(a0+a1*x+...a7*x^7)*x^128 = (a0+a1*x+...a7*x^7)*(1+x+x^2+x^7)
	 = a0 + a1*x + a2*x^2 + a3*x^3 + a4*x^4 + a5*x^5 + a6*x^6 + a7*x^7 
	      + a0*x + a1*x^2 + a2*x^3 + a3*x^4 + a4*x^5 + a5*x^6 + a6*x^7 + a7*x^8
		         + a0*x^2 + a1*x^3 + a2*x^4 + a3*x^5 + a4*x^6 + a5*x^7 + a6*x^8 + a7*x^9
				                                                a0*x^7 + a1*x^8 + a2*x^9 + ... + a7*x^14
	 = a0 + (a0+a1)*x + (a0+a1+a2)*x^2 + (a1+a2+a3)*x^3 + (a2+a3+a4)*x^4 + (a3+a4+a5)*x^5
					  + (a4+a5+a6)*x^6 + (a0+a5+a6+a7)*x^7 + (a1+a6+a7)*x^8 + (a2+a7)*x^9
					  + a3*x^10 + a4*x^11 + a5*x^12 + a6*x^13 + a7*x^14
*/
// [3] - Make_GHASH_const_R0R1() 함수를 실행하고 결과를 변수 R0[], R1[] 선언부에 넣기
void Make_GHASH_const_R0R1(unsigned char R0[256], unsigned char R1[256]) {
	unsigned char a[8]; // 각 계수 (비트) 저장

	for (int i = 0; i < 256; i++) { //초기화
		R0[i] = 0x00;
		R1[i] = 0x00;
	}
	for (int i = 0; i < 256; i++) {  // 한바이트 값을 하나씩 선택하여 7차 다항식의 계수로 간주함
		for (int j = 0; j < 8; j++) {
			a[j] = (i >> (7 - j)) & 0x01;   // 순서대로 a0, a1, a2, ... , a7
		}
		R0[i] = a[0] << 7;
		R0[i] ^= (a[0] ^ a[1]) << 6;
		R0[i] ^= (a[0] ^ a[1] ^ a[2]) << 5;
		R0[i] ^= (a[1] ^ a[2] ^ a[3]) << 4;
		R0[i] ^= (a[2] ^ a[3] ^ a[4]) << 3;
		R0[i] ^= (a[3] ^ a[4] ^ a[5]) << 2;
		R0[i] ^= (a[4] ^ a[5] ^ a[6]) << 1;
		R0[i] ^= a[0] ^ a[5] ^ a[6] ^ a[7];

		R1[i] = (a[7] ^ a[6] ^ a[1]) << 7;
		R1[i] ^= (a[7] ^ a[2]) << 6;
		R1[i] ^= a[3] << 5;
		R1[i] ^= a[4] << 4;
		R1[i] ^= a[5] << 3;
		R1[i] ^= a[6] << 2;
		R1[i] ^= a[7] << 1;
	}

	printf("R0 = {\n");
	for (int i = 0; i < 256; i++) {
		printf("0x%02x, ", R0[i]);
		if ((i % 16) == 15) printf("\n");
	}
	printf(" }\n");
	printf("R1 = {\n");
	for (int i = 0; i < 256; i++) {
		printf("0x%02x, ", R1[i]);
		if ((i % 16) == 15) printf("\n");
	}
	printf(" }\n");
}

// 상수 H(x)에 7차이하 다항식을 곱하기 위한 테이블 
// H(x) * (q0+q1*x+...q7*x^7) 테이블 만들기
// H := Enc_k(0...0) // 128비트
/*
   H(x) * (q0 + q1*x + q2*x^2 + ... + q7*x^7)
     = q0*H(x) + q1*H(x)*x + q2*H(x)*x^2 + ... + q7*H(x)*x^7
     = HT[][0], HT[][1], ... , HT[][15]
*/
void Make_GHASH_H_table(unsigned char H[16], unsigned char HT[256][16]) {
	unsigned char Z[16], H_mul[16];
	unsigned char qi_bit;

	for (int i = 0; i < 256; i++) { // 7차 이하 다항식 00000000 .... 11111111
		for (int j = 0; j < 16; j++) { // 결과 저장할 변수 초기화
			Z[j] = 0x00; 
			H_mul[j] = H[j];
		}
		for (int j = 0; j < 8; j++) { // q0, q1, ..., q7
			qi_bit = ((i >> (7 - j)) & 0x01) == 1 ? 0x01 : 0x00;
			if (qi_bit == 1) {
				xor_b_array(Z, 16, H_mul); //for (int k = 0; k < 16; k++) Z[k] ^= H_mul[k];
			}
			GF128_xtime(H_mul);
		}
		//for (int k = 0; k < 16; k++) HT[i][k] = Z[k];
		copy_b_array(Z, 16, HT[i]);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// 아래로 GHASH 함수들
////////////////////////////////////////////////////////////////////////////////////////////////////


/*************************************************
* Name:        GF128_Hmul (테이블을 이용한 곱하기)
*  HTable을 이용한 GF(2^128) 곱셈: p(x) <-- p(x)*q(x)
*  p(x), q(x) = q0 + q1*x + q2*x^2 + ... + q127*x^127
*  p(x)*q(x) = p(x)*(q0 + q1*x + q2*x^2 + ... + q127*x^127)
*      = q0*p(x) + q1*x*p(x) + q2*x^2*p(x) + ... + q127*x^127*p(x)
* 
*   H(x)*p(x) = H(x)*(P[0] + P[1]*x^8 + P[2]*x^16 + ... + P[15]*x^120)
* = H(x)*P[0] + (H(x)*P[1] + ... (H(x)*P[13] + (H(x)*P[14] + (H(x)*P[15])*x^8)*x^8)*x^8) ... )*x^8)
**************************************************/
void GF128_Hmul(unsigned char state[16], unsigned char HT[256][16], unsigned char R0[256], unsigned char R1[256]) {
	unsigned char W[16] = { 0, };
	unsigned char temp;
	unsigned char in[16]; 

	//for (int j = 0; j < 16; j++) {
	//	in[j] = state[j];
	//}
	copy_b_array(state, 16, in);
	for (int i = 0; i < 15; i++) { // 0, 1, ... , 14
		temp = in[15 - i];  // 초록색 오른쪽부터 왼쪽으로 가기
		for (int j = 0; j < 16; j++) {
			W[j] ^= HT[temp][j]; // W <- Hg <- B0H B1H ... B15H // ^= : 주황 더하기
		}
		//xor_b_array(W, 16, HT[temp]); // W ^= H(x)*in[15-i]

		// W(x) <-- W(x)*x^8
		temp = W[15]; // R0R1 input // BH[15]
		for (int j = 15; j >= 1; j--) W[j] = W[j - 1];
		W[1] ^= R1[temp];
		W[0] = R0[temp];
	}

	// H(x)*P[0]를 더한다.
	temp = in[0];
	for (int j = 0; j < 16; j++) {
		state[j] = W[j] ^ HT[temp][j]; // 맨왼쪽 BH 더하면 끝
	}
}


/*************************************************
* Name:        GHASH_mul_test (테이블을 이용한 곱하기 테스트 함수)
**************************************************/
void GHASH_mul_test() {
	unsigned char H[16], HT[256][16], P[16] = { 0, };

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 16; j++) H[j] = j;
		Make_GHASH_H_table(H, HT);
		for (int j = 0; j < 16; j++) P[j] = 0;
		P[1] = i;
		GF128_mul(H, P);
		printf("GF128_mul: H*P = ");
		for (int k = 0; k < 16; k++) { printf("%02x", H[k]); }
		for (int j = 0; j < 16; j++) H[j] = j;
		GF128_Hmul(P, HT, R0, R1);
		printf("\nGF128_Hmul: H*P = ");
		for (int k = 0; k < 16; k++) { printf("%02x", P[k]); }

		printf("\nGF128_mul_HT: H*P = ");
		for (int k = 0; k < 16; k++) { printf("%02x", HT[i][k]); }
		printf("\n\n");
	}
}

/*************************************************
* Name:        GHASH_mul_test (테이블을 이용한 곱하기 테스트 함수)

* GHASH는 일반적인 hash와 다르게 블록의 배수만 받을 수 있음
**************************************************/
void GHASH(unsigned char msg[], int msg_blocks, unsigned char H[16], unsigned char tag[16]) {
	unsigned char x[16];
	unsigned char out[16] = { 0, }; // iv = 0000000000000000
	for (int i = 0; i < msg_blocks; i++) {
		for (int j = 0; j < 16; j++) 
			x[j] = msg[i * 16 + j];
		xor_b_array(out, 16, x);
		GF128_mul(out, H); // 시간이 오래걸림
		//GF128_mul_tls(out, H);
	}
	for (int j = 0; j < 16; j++) tag[j] = out[j];
}


void GHASH_TableVersion(unsigned char msg[], int msg_blocks, unsigned char HT[256][16], unsigned char R0[256], unsigned char R1[256], unsigned char tag[16]) {
	unsigned char x[16];
	unsigned char out[16] = { 0, };
	for (int i = 0; i < msg_blocks; i++) {
		for (int j = 0; j < 16; j++) 
			x[j] = msg[i * 16 + j];
		xor_b_array(out, 16, x); // 이전거 xor
		//GF128_mul(out, H); 
		GF128_Hmul(out, HT, R0, R1); // 암호화보다 빠름
	}
	for (int j = 0; j < 16; j++) tag[j] = out[j];
}

void GHASH_test() {
	unsigned char H[16], HT[256][16], Msg[16] = { 0, };
	unsigned char Tag1[16], Tag2[16];

	for (int j = 0; j < 16; j++) H[j] = j;
	Make_GHASH_H_table(H, HT);
	for (int j = 0; j < 16; j++) Msg[j] = 0;
	Msg[1] = 0x01;
	
	GHASH(Msg, 1, H, Tag1);
	GHASH_TableVersion(Msg, 1, HT, R0, R1, Tag2);

	printf("GHASH Tag1 = ");
	for (int k = 0; k < 16; k++) { printf("%02x", Tag1[k]); }
	printf("\nGHASH Tag2 = ");
	for (int k = 0; k < 16; k++) { printf("%02x", Tag2[k]); }
	printf("\n");
	
}

//int main() {
//	//unsigned char R0[256], R1[256]; 
//	//Make_GHASH_const_R0R1(R0, R1);
//
//	//GHASH_mul_test();
//
//	GHASH_test();
//}

