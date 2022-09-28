/**
 * AES - FIPS 197 표준문서
 * 4. Mathematical Prelimaries - SBox
 */


#include <iostream>
#include <stdio.h>
#include "SBox.h"
// #include "GF_Math.h"

// #define _SBOX_
/** 
 * AES Sbox에 사용되는 Affine 변환 
 * 입력 : x
 * 출력 : Ax + b
 * x = (byte) = b7b6b5b4 b3b2b1b0
 * vec_x[8] = ( b0, b1, b2, b3, b4, b5, b6, b7 ) = ( lsb, ..., msb )
 * y = Ax + b (A: 8*8 Matrix, b = column vector)
*/

byte AES_Affine(byte x) {
    byte A[8][8] = {
    {1, 0, 0, 0, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 1, 1}
    };
    byte b_vec[8] = {1, 1, 0, 0, 0, 1, 1, 0};
    byte x_vec[8], y_vec[8], y = 0x00;
    
    // x --> x_vec 으로 삽입
    for (int i=0; i<8; i++) 
        x_vec[i] = (x>>i) & 0x01;

    // y = b + Ax
    for (int i=0; i<8; i++) {
        y_vec[i] = b_vec[i];
        for (int j=0; j<8; j++) { // here problem
            y_vec[i] ^= (A[i][j] * x_vec[j]); // GF(2)에서의 연산이므로 + --> ^ , * --> &
        }
    }
    // y_vec --> y in GF(2^8)
    byte temp;
    for (int i=0; i<8; i++) {
        temp = y_vec[i] << i;
        y ^= temp;
    }
    return y;
}

/**
 * AES Sbox
 * y = Sbox[x] = A * x^(-1) + b
 */
void Get_AES_Sbox(byte sbox[256]) {
    // 0 ^ -1 = 0 을 별도로 처리하기
    sbox[0] = AES_Affine(0);
    for (int i=1; i<256; i++) {
        sbox[i] = AES_Affine( GF_inv(i) );
    }
}

/**
 * AES ISBox
 * x = ISbox(y) = [ A^(-1)*(y + b) ]^(-1)
 * 
 * 방법 1) 각 원소는 GF(2^8)의 원소이며 GF(2^8)은 유한체 이므로 역원을 각각 계산하여 역행렬을 구하여 계산할 수 있다.
 * 방법 2) 간단히 Sbox의 결과값을 이용하여 계산
 * 2번을 사용하여 구현
 */
void Get_AES_ISbox(byte isbox[256]) {
    byte Sbox[256];
    Get_AES_Sbox(Sbox);
    for (int i=0; i<256; i++) {
        isbox[Sbox[i]] = i;
    }
}

void print_Sbox() {
    Get_AES_Sbox(SBox);   printf("\n [SBox 출력]\n");  GF_print_Matrix(SBox);
    Get_AES_ISbox(ISBox); printf("\n [ISBox 출력]\n"); GF_print_Matrix(ISBox);
}



// int main() {
    /** 
     * byte Sbox[256], ISbox[256] ---> SBox.h에 선언
     */
/*
    Get_AES_Sbox(Sbox);   printf("\n [Sbox 출력]\n");  GF_print_Matrix(Sbox);
    Get_AES_ISbox(ISbox); printf("\n [ISbox 출력]\n"); GF_print_Matrix(ISbox);

    return 0;
}
*/