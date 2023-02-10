#ifndef UTIL_H
#define UTIL_H

int isHex(char ch);
unsigned char Hex2Digit(char ch);
unsigned char Hex2Byte(const char h[2]);
void byte_fromhex(unsigned char barr[], const char hex_str[], int hex_len);
void byte_fromstr(unsigned char barr[], unsigned char str[], int str_len);
void copy_b_array(unsigned char src[], int len, unsigned char dest[]);
void xor_b_array(unsigned char data[], int len, unsigned char xor_arr[]);

#endif

// EOF