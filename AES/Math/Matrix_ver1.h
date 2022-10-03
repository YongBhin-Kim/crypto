#pragma once
#include "GF_Math.h"

// #ifdef _MATRIX_VER1_
using namespace std;   // 이 코드를 넣으면 std:cout을  간단히 cout으로 할 수 있다.
#define MATRIX_MAX_SIZE 4  //동일한 표현: constexpr auto MATRIX_MAX_SIZE = 4;
// #endif


void Mat_print(double A[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], int row, int col);
void Mat_Add(double A[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], double B[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE],
    double C[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], int mat_size);
void Mat_Mul(double A[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], double B[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE],
    double C[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], int mat_size);
void run_matrix_test();
