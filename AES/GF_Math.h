#pragma once
// #include "Matrix_ver3.h"
// #include "AES8.h"

/**
========================================================================================================================
 * Rijndael Field : GF(2^8) (== GF(2)[x] / <m(x)> ) (quotient field) 에서 사용되는 수학
    * 기약다항식 m(x) = x^8 + x^4 + x^3 + x + 1
        * GF_add          : GF(2^8)의 두 원소(다항식)의 합
        * GF_sub          : GF(2^8)의 두 원소(다항식)의 차
        * GF_xtime        : GF(2^8)의 원소(다항식)에 x를 곱하는 함수
        * GF_mul          : GF(2^8)의 두 원소(다항식)의 곱 
        * GF_div          : GF(2^8)의 두 원소(다항식)의 나눗셈
        * gcd             : Euclidean Algorithm을 이용하여 두 정수의 최대공약수를 구하는 함수
        * xgcd            : Extended Euclidean Algorithm을 이용하여 두 정수의 최대공약수 및 역원 계산에 필요한 함수
        * xgcd_poly       : Extended Euclidean Algorithm을 이용하여 GF(2^8)의 두 원소(다항식)의 최대공약수 및 역원 계산에 필요한 함수
        * GF_inv          : Little Fermat Theorem을 이용하여 GF(2^8)의 원소(다항식)의 역원을 계산하는 함수
        * GF_print        : GF(2^8)의 원소(다항식)를 이진수 형태로 출력하는 함수
        * GF_print_Matrix : GF(2^8)의 원소(다항식)로 이루어진 행렬을 출력하는 함수
========================================================================================================================
 */


// Polynomial over GF(2^8)
typedef unsigned char byte;

void GF_print(byte f);
void GF_print_Matrix(byte Matrix[256]);
byte GF_add(byte f, byte g);
byte GF_sub(byte f, byte g);
byte GF_xtime_1(byte f);
byte GF_xtime_2(byte f);
byte GF_mul(byte f, byte g);
byte GF_div(byte f, byte g);
int gcd(int a, int b);
int gcd_2(int a, int b);
int xgcd(int a, int b);
int xgcd_2(int a, int b);
byte xgcd_poly(byte f, byte g); // Extended Euclidean Algorithm over GF(2^8), 구현중
byte GF_inv(byte f);
void Get_AES_Sbox(byte sbox[256]);
void Get_AES_ISbox(byte isbox[256]);