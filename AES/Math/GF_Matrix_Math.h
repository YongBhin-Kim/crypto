#pragma once
#include "GF_Math.h"

using namespace std;
typedef unsigned char byte;  // 새로운 데이터 타입: byte = GF(2^8)

#define MATRIX_MAX_SIZE 8
// #define NEARLY_ZERO 1e-10

// GF_Matrix 구조체 사용
struct GF_Matrix {
    byte M[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
    int row;
    int col;
};


GF_Matrix GF_Mat_init();
void GF_Mat_print(GF_Matrix Mat);
GF_Matrix GF_Mat_Mul(GF_Matrix A, GF_Matrix B); // GF_Math의 GF_add, GF_mul 이용
void GF_Mat_Exchange_Row(GF_Matrix& M, int row1, int row2);
void GF_Mat_Scalar_Mul(GF_Matrix& M, byte scalar, int row);
void GF_Mat_Row_Add(GF_Matrix& M, byte scalar, int srcRow, int targetRow);
GF_Matrix GF_Mat_inverse(GF_Matrix M);
void Run_GF_matrix_test();
