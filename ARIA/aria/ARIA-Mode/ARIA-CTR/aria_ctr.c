#include <stdio.h>
#include <string.h> 
#include "../../ARIA-C/aria.h"
#include "../Util/util.h"


/* 
GCM 표준문서의 Inc_32() 함수
counter: (msb)  c[0] c[1] ... c[15] (lsb)
*/
void counter_inc(unsigned char counter[16]) {
	for (int i = 15; i >= 0; i--) { // c[15] --> c[0]
		if (counter[i] != 0xff) { //자리올림 없음
			counter[i]++;
			break; // for-loop를 벗어남
		}
		else { // 0xff --> 0x00, 자리올림
			counter[i] = 0x00;
		}
	}
}

/* ARIA CTR mode */
void aria_ctr(unsigned char PT[], int pt_len, unsigned char mk[16], unsigned char CTR[16], unsigned char CT[], int round) {
	int num_blocks, remainder;
	num_blocks = pt_len / 16;
	remainder = pt_len - num_blocks * 16;

	unsigned char pt[16], ctr_ct[16];
	unsigned int rk[11][4];
	unsigned char current_ctr[16];

    unsigned char enc_w[round+1][16], dec_w[round+1][16];
    memset(&enc_w, 0, sizeof(enc_w));
    memset(&dec_w, 0, sizeof(dec_w));
    keyexpansion(mk, enc_w, dec_w, round);

	copy_b_array(CTR, 16, current_ctr); // current_ctr <- CTR
	for (int i = 0; i < num_blocks; i++) {
		for (int j = 0; j < 16; j++) pt[j] = PT[i * 16 + j]; // pt[] <-- PT[]
		aria_enc(current_ctr, ctr_ct, enc_w, round); // E(ctr)
		xor_b_array(pt, 16, ctr_ct); // pt <- pt^ctr_ct
		for (int j = 0; j < 16; j++) CT[i * 16 + j] = pt[j];
		counter_inc(current_ctr);
	}

	// AES32_Encrypt(current_ctr, rk, ctr_ct);
    aria_enc(current_ctr, ctr_ct, enc_w, round);
	for (int i = 0; i < remainder; i++) {
		pt[i] = PT[16 * num_blocks + i];
		pt[i] ^= ctr_ct[i]; // 마지막 블록 : ctr_ct[16]에서 remainder바이트까지만 사용
		CT[16 * num_blocks + i] = pt[i];
	}
}

// EOF