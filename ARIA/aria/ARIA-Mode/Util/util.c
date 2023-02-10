#include <stdio.h>
#include <string.h> 
#include "util.h"

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