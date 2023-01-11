#include <stdio.h>
#include <string.h> 
#include "../ARIA-C/aria.h"

int isHex(char ch) {  
	return ((ch >= '0') && (ch <= '9')) ||
		((ch >= 'A') && (ch <= 'F')) ||
		((ch >= 'a') && (ch <= 'f'));
}

unsigned char Hex2Digit(char ch) {
	if (!isHex(ch)) {
		printf("%c is not a hex value.\n", ch);
		return -1; //�ǹ̾��� ���
	}
	if ((ch >= '0') && (ch <= '9')) {
		return  ch - '0'; // ��: '7' - '4' = 3
	}
	else if ((ch >= 'A') && (ch <= 'F')) {
		return ch - 'A' + 10;
	}
	else if ((ch >= 'a') && (ch <= 'f')) {
		return ch - 'a' + 10;
	}
    printf("Unknown error.\n");
	return -1;
}

unsigned char Hex2Byte(const char h[2]) { // h[0], h[1]
	unsigned char upper, lower; //����(x), ���� ����Ʈ(y) -->  ����Ʈ(xy) 
	upper = h[0];
	lower = h[1];
	if ((!isHex(upper)) || (!isHex(lower))) {
        printf("Hex Error\n");
		return -1;
	}
	return Hex2Digit(upper) * 16 + Hex2Digit(lower);
}


/* byte array <- hex string */
void byte_fromhex(unsigned char barr[], const char hex_str[], int hex_len) {
//void Hex2Array(const char* hex_str, int hex_len, byte* barr) {
	char h[2];
	unsigned char b_value;
	for (int i = 0; i < hex_len / 2; i++) {
		h[0] = hex_str[2 * i];
		h[1] = hex_str[2 * i + 1];
		b_value = Hex2Byte(h); // {h[0], h[1]} --> h[0]h[1]
		barr[i] = b_value;
	}
}
void byte_fromstr(unsigned char barr[], unsigned char str[], int str_len) {
	for (int i=0; i<str_len; i++) {
		barr[i] = str[i];
	}
}
void copy_b_array(unsigned char src[], int len, unsigned char dest[]) {
	for (int i = 0; i < len; i++) {
		dest[i] = src[i];
	}
}

void xor_b_array(unsigned char data[], int len, unsigned char xor_arr[]) {
	for (int i = 0; i < len; i++) {
		data[i] ^= xor_arr[i]; // data[]�� ������Ʈ
	}
}

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