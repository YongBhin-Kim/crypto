#pragma once

#include <iostream>

typedef unsigned char byte;
typedef unsigned char u8;
using namespace std;


bool isHex(char ch);
byte Hex2Digit(char ch);
byte Hex2Byte(const char h[2]);
void Hex2Array(const char hex_str[], int hex_len, byte barr[]);
void print_b_array(byte b_arr[], int len, const char* pStr = nullptr);
void copy_b_array(byte src[], int len, byte dest[]);
void xor_b_array(byte data[], int len, byte xor_arr[]);

void hex_test();
