/*
    Block Cipher Mode - GCM(Galois Counter Mode) (CIPH is ARIA)
*/
//  Copyright © Yonhbhin Kim
//  Github url : https://github.com/YongBhin-Kim

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "../Util/util.h"
#include "../../ARIA-C/aria.h"
#include "../ARIA-CTR/aria_ctr.h"
#include "ghash.h"

extern unsigned char R0[256], R1[256]; // in ghash.c

/* **********************************************
 input 
    : key, iv, aad, pt 
    ( pt, pt_len, ctr, mk, a, a_len, (ct), (tag) )
 output 
    : (ct, tag)

 H <- ENC(zero)
 J0 <- iv
 inc(j0)
 GCTR( p, iv )
 GHASH_H( A || C || A,Clen ) 
 Tag <- msb(t)
 
 ***********************************************/

/*************************************************
* Name:        aria_gcm_enc
*
* Description: ARIA gcm mode encryption function
*
* Arguments: 
* [Input]
*			 - unsigned char pt[]:          pointer to input plaintext array
*			 - int pt_len:					input plaintext length (bytes)
*			 - unsigned char iv[]:			pointer to input initialization vector
*			 - int iv_len:					input iv length (bytes)
*			 - unsigned char add[16]:		pointer to input additional authenticated data
*			 - unsigned int add_len:		input add length (bytes)
* [Prerequisites]
*			 - unsigned char mk[]:			pointer to input master key (128/192/256 bits)
*			 - int BCM:						information of blockcipher mode
* [Output]
*			 - unsigned char ct[]:			pointer to output ciphertext array
*			 - int *ct_len:					pointer to output ciphertext length (bytes)
*			 - unsigned char tag[]:			pointer to output tag
* Steps:
* 1. Let h <- aria_enc(enc_key, zero_128)
* 2. Define j0
	if iv_bit_len == 96 then let j0 = ( iv || 0^31 || 1 )
	if iv_bit_len != 96 then let s = 128ceil(iv_bit_len/128) - iv_bit_len
							 let j0 = GHASH( ( iv || 0^{s+64} || [iv_bit_len]_64 ) )
* 3. Let ct = GCTR_k( inc32(j0), pt )
* 4. Let u = 128*ceil(ct_bit_len/128) - ct_bit_len
	 let v = 128*ceil(aad_bit_len/128) - aad_bit_len
* 5. Define s = GHASH( aad || 0^v || ct || 0^u || [aad_bit_len]_64 || [ct_bit_len]_64 )
* 6. Let tag = MSB_t( GCTR_k(j0, s) )
* 7. Return (ct, tag)
**************************************************/
int aria_gcm_enc(unsigned char pt[], int pt_len, unsigned char iv[16], unsigned char mk[16], unsigned char aad[], int aad_len, unsigned char ct[], unsigned char tag[16]) {

    /* define AIRA-round */
    int round = 12;


    /* gen round key (using mk) */
    unsigned char enc_w[round+1][16], dec_w[round+1][16];
    memset(&enc_w, 0, sizeof(enc_w));
    memset(&dec_w, 0, sizeof(dec_w));
    keyexpansion(mk, enc_w, dec_w, 12);


	/* Step 1. H <- CIPH_k(0^128) */
    unsigned char H[16] = { 0x00, }; // for GHASH
	unsigned char zero[16] = { 0x00, };
    aria_enc(zero, H, enc_w, round);


    /* Step 2. Let iv length is 96, define iv and j0 */
    unsigned char inc32j0[16] = { 0x00, }; // 암호화에 쓰이는 ctr 변수
    unsigned char j0[16] = { 0x00, }; // 암호화에 쓰이는 ctr 변수
	for (int i = 0; i < 12; i++) { 
        j0[i] = iv[i];
        inc32j0[i] = iv[i];
    }
    j0[15] = 0x01;
    inc32j0[15] = 0x01;

    /* Step 3. C <- GCTR_k( inc32(j0), P ) */
	counter_inc(inc32j0); 
	aria_ctr(pt, pt_len, mk, inc32j0, ct, round);

    /* Step 5. S <- GHASH_H ( aad || C || aad_bit_len, ct_bit_len ) */

    /* Given len(A), len(C). */
	long long int aad_bit_len, ct_bit_len;
	aad_bit_len = (long long int)aad_len * 8; 	// aad 길이(비트)
	ct_bit_len = (long long int)pt_len * 8; 	// pt  길이(비트)

	unsigned char first_block[16] = { 0, };
	unsigned char last_block[16];
	if (aad_len > 0) { // aad_len = 0,1,2,..., 16 (바이트)
		for (int j = 0; j < aad_len; j++) first_block[j] = aad[j];
	}

	for (int j = 0; j < 8; j++) { // length = aad_bit_len | ct_bit_len
		last_block[j] = (aad_bit_len >> (8 * (7 - j))) & 0xff;
		last_block[8 + j] = (ct_bit_len >> (8 * (7 - j))) & 0xff;
	}


	int msg_len, remainder;
    // ct_len == pt_len
	int ct_len = pt_len;
	msg_len = (ct_len % 16) == 0 ? ct_len + 2 * 16 : (ct_len / 16) * 16 + 3 * 16; // 지금은 전부다 전자의 경우
	remainder = (ct_len % 16) == 0 ? 0 : 16 - (ct_len % 16);

	unsigned char* MSG;
	MSG = (unsigned char*)malloc(msg_len);
	for (int i = 0; i < 16; i++) MSG[i] = first_block[i]; // GHASH 입력 맨앞에 붙는것 (aad가 있으면)
	for (int i = 0; i < ct_len; i++) MSG[16 + i] = ct[i]; // GHASH 중간 CT (ct_len==0인경우 안함)
	for (int i = 0; i < remainder; i++) MSG[16 + ct_len + i] = 0x00;  // GHASH 중간 CT 뒤에 zero패딩
	for (int i = 0; i < 16; i++) MSG[16 + ct_len + remainder + i] = last_block[i]; // GHASH 맨뒤 길이정보 (aad_bit_len | ct_bit_len) (필수)

	/* Step 7. Tag <- MSB_t(GCTR_k(J0, s)) */
	// GHASH(MSG, msg_len / 16, H, tag);
	unsigned char HT[256][16];
	Make_GHASH_H_table(H, HT);
	GHASH_TableVersion(MSG, msg_len / 16, HT, R0, R1, tag); // GHASH(MSG, msg_len / 16, H, tag);

    unsigned char Y[16]; 
    aria_enc(j0, Y, enc_w, round);
	xor_b_array(tag, 16, Y); // tag <- tag^Y

	return SUCCESS;

}

/* **********************************************
 input 
    : ct, iv, aad, tag 
    ( ct, ct_len, ctr, mk, a, aad_len, (pt), tag )
 output 
    : (pt)

 H <- ENC(zero)
 J0 <- iv
 inc(j0)
 GCTR( p, iv )
 GHASH_H( A || C || A,Clen ) 
 Tag <- msb(t)
 
 ***********************************************/
int aria_gcm_dec(unsigned char ct[], int ct_len, unsigned char iv[16], unsigned char mk[16], unsigned char aad[], int aad_len, unsigned char pt[], unsigned char tag[16]) {  

    /* define AIRA-round */
    int round = 12;


    /* gen round key (using mk) */
    unsigned char enc_w[round+1][16], dec_w[round+1][16];
    memset(&enc_w, 0, sizeof(enc_w));
    memset(&dec_w, 0, sizeof(dec_w));
    keyexpansion(mk, enc_w, dec_w, 12);

    
    /* Step 2. H <- CIPH_k(0^128) */
    unsigned char H[16] = { 0x00, }; // for GHASH
	unsigned char zero[16] = { 0x00, };
    aria_enc(zero, H, enc_w, round);
	
    /* Step 3. Let iv length is 96, define iv and j0 */
    unsigned char inc32j0[16] = { 0x00, }; // 암호화에 쓰이는 ctr 변수
    unsigned char j0[16] = { 0x00, }; // 암호화에 쓰이는 ctr 변수
	for (int i = 0; i < 12; i++) { 
        j0[i] = iv[i];
        inc32j0[i] = iv[i];
    }
    j0[15] = 0x01;
    inc32j0[15] = 0x01;


    /* Step 4. P <- GCTR_k( inc32(j0), C ) */
	counter_inc(inc32j0);
	aria_ctr(ct, ct_len, mk, inc32j0, pt, round);
    

    /* Given len(A), len(C). */
	long long int aad_bit_len, ct_bit_len;
	aad_bit_len = (long long int)aad_len * 8; 	// A 길이(비트)
	ct_bit_len = (long long int)ct_len * 8; 	// 평문 길이(비트)

    /* Step 6. S <- GHASH_H ( aad || C || aad_bit_len, ct_bit_len ) */
	unsigned char first_block[16] = { 0, };
	unsigned char last_block[16];
	if (aad_len > 0) { // aad_len = 0,1,2,..., 16 (바이트)
		for (int j = 0; j < aad_len; j++) first_block[j] = aad[j];
	}
	for (int j = 0; j < 8; j++) { // length = aad_bit_len | ct_bit_len
		last_block[j] = (aad_bit_len >> (8 * (7 - j))) & 0xff;
		last_block[8 + j] = (ct_bit_len >> (8 * (7 - j))) & 0xff;
	}


	int msg_len, remainder;
    // ct_len == pt_len
	msg_len = (ct_len % 16) == 0 ? ct_len + 2 * 16 : (ct_len / 16) * 16 + 3 * 16; // 지금은 전부다 전자의 경우
	remainder = (ct_len % 16) == 0 ? 0 : 16 - (ct_len % 16);

	unsigned char* MSG;
	MSG = (unsigned char*)malloc(msg_len);
	for (int i = 0; i < 16; i++) MSG[i] = first_block[i]; // GHASH 입력 맨앞에 붙는것 (aad가 있으면)
	for (int i = 0; i < ct_len; i++) MSG[16 + i] = ct[i]; // GHASH 중간 CT (ct_len==0인경우 안함)
	for (int i = 0; i < remainder; i++) MSG[16 + ct_len + i] = 0x00;  // GHASH 중간 CT 뒤에 zero패딩
	for (int i = 0; i < 16; i++) MSG[16 + ct_len + remainder + i] = last_block[i]; // GHASH 맨뒤 길이정보 (aad_bit_len | ct_bit_len) (필수)

	//(debug) print MSG
	// printf("\n Input for GHASH =");
	// for (int i = 0; i < msg_len; i++) {
	// 	printf("%02x", MSG[i]);
	// 	if ((i % 16) == 15) printf(" ");
	// }
	// printf("\n");
	unsigned char HT[256][16];
    unsigned char tag_prime[16] = { 0x00, };
	Make_GHASH_H_table(H, HT);
	GHASH_TableVersion(MSG, msg_len / 16, HT, R0, R1, tag_prime); // GHASH(MSG, msg_len / 16, H, tag);

    /* Step 7-1. Tag' <- MSBt(GCTRk(J0, S)) */
    unsigned char Y[16]; 
    aria_enc(j0, Y, enc_w, round);

    /* Step 7-2. Tag' <- MSBt(GCTRk(J0, S)) */
	xor_b_array(tag_prime, 16, Y); // tag <- tag^Y


    /* Step 8. compare Tag and Tag' */
    // printstate("tag1(기존) = ", tag, 16);
    printstate("tag_prime = ", tag_prime, 16);
	
	int ret;
	ret = memcmp(tag, tag_prime, 16);
	if ( ret != SUCCESS ) {
		return FAIL;
	}
	return SUCCESS;

}



// void aria_gcm_enc(unsigned char pt[], int pt_len, unsigned char CTR[16], unsigned char mk[16], unsigned char aad[], int aad_len, unsigned char CT[], unsigned char tag[16]) {  

// 	/* Step 6. */
// 	long long int aad_bit_len, ct_bit_len;
// 	aad_bit_len = (long long int)aad_len * 8; 	// aad 길이(비트)
// 	ct_bit_len = (long long int)pt_len * 8; 	// pt 길이(비트)


// 	unsigned char first_block[16] = { 0, };
// 	unsigned char last_block[16];
// 	if (aad_len > 0) { // aad_len = 0,1,2,..., 16 (바이트)
// 		for (int j = 0; j < aad_len; j++) first_block[j] = aad[j];
// 	}
// 	// printf("%lld", aad_bit_len);
// 	// printf(" %lld", ct_bit_len);
// 	for (int j = 0; j < 8; j++) { // length = aad_bit_len | ct_bit_len
// 		last_block[j] = (aad_bit_len >> (8 * (7 - j))) & 0xff;
// 		last_block[8 + j] = (ct_bit_len >> (8 * (7 - j))) & 0xff;
// 	}
// 	// for (int i=0; i<16; i++) {
//     //     printf("%02x ", last_block[i]);
//     // }

// 	unsigned char CTR1[16]; // CTR모드 암호화를 위한 변수추가
// 	unsigned char Y[16]; // 태그만들때 XOR할 벡터 Y
// 	unsigned char H[16]; // GHASH용 H
// 	unsigned char Zero[16] = { 0, }; // 제로 벡터 평문 --> H := Enc_k(0...0)
//     int round = 12; // for only ARIA128
//     unsigned char enc_w[round+1][16], dec_w[round+1][16];
//     memset(&enc_w, 0, sizeof(enc_w));
//     memset(&dec_w, 0, sizeof(dec_w));
//     keyexpansion(mk, enc_w, dec_w, 12);

// 	// AES32_Encrypt(CTR, rk, Y); // Y <- E(CTR)
// 	// AES32_Encrypt(Zero, rk, H); // H <- Enc_k(Zero)
//     aria_enc(CTR, Y, enc_w, round);
//     aria_enc(Zero, H, enc_w, round);

// 	copy_b_array(CTR, 16, CTR1);
// 	counter_inc(CTR1);

// 	aria_ctr(pt, pt_len, mk, CTR1, CT, round); /* Step 3. C <- GCTRk(inc32(J0), P) */

// 	int msg_len, remainder;
// 	msg_len = (pt_len % 16) == 0 ? pt_len + 2 * 16 : (pt_len / 16) * 16 + 3 * 16; // 지금은 전부다 전자의 경우, 128bit || 128bit || 128bit VS 128|| pad ||128
// 	remainder = (pt_len % 16) == 0 ? 0 : 16 - (pt_len % 16);

// 	unsigned char* MSG;
// 	MSG = (unsigned char*)malloc(msg_len);
// 	for (int i = 0; i < 16; i++) MSG[i] = first_block[i]; // GHASH 입력 맨앞에 붙는것 (aad가 있으면)
// 	for (int i = 0; i < pt_len; i++) MSG[16 + i] = CT[i]; // GHASH 중간 CT (pt_len==0인경우 안함)
// 	for (int i = 0; i < remainder; i++) MSG[16 + pt_len + i] = 0x00;  // GHASH 중간 CT 뒤에 zero패딩
// 	for (int i = 0; i < 16; i++) MSG[16 + pt_len + remainder + i] = last_block[i]; // GHASH 맨뒤 길이정보 (aad_bit_len | ct_bit_len) (필수)

// 	//(debug) print MSG
// 	printf("\n Input for GHASH =");
// 	for (int i = 0; i < msg_len; i++) {
// 		printf("%02x", MSG[i]);
// 		if ((i % 16) == 15) printf(" ");
// 	}
// 	printf("\n");

// 	// GHASH(MSG, msg_len / 16, H, tag);


// 	unsigned char HT[256][16];
// 	Make_GHASH_H_table(H, HT);
// 	GHASH_TableVersion(MSG, msg_len / 16, HT, R0, R1, tag);

// 	xor_b_array(tag, 16, Y); // tag <- tag^Y

// }

// EOF