#ifndef ARIA_CTR
#define ARIA_CTR

#include "../ARIA-C/aria.h"

int isHex(char ch);
unsigned char Hex2Digit(char ch);
unsigned char Hex2Byte(const char h[2]);
void byte_fromhex(unsigned char barr[], const char hex_str[], int hex_len);
void byte_fromstr(unsigned char barr[], unsigned char str[], int str_len);
void copy_b_array(unsigned char src[], int len, unsigned char dest[]);
void xor_b_array(unsigned char data[], int len, unsigned char xor_arr[]);
void counter_inc(unsigned char counter[16]);
void aria_ctr(unsigned char PT[], int pt_len, unsigned char mk[16], unsigned char CTR[16], unsigned char CT[], int round);

#endif

// EOF