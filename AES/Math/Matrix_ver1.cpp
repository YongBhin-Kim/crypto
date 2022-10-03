#include <iostream>
#include <stdio.h>
#include "Matrix_ver1.h"

// Matrix_ver1_template.cpp : 2022.9.22
// 고정된 size만 사용되는 기본 행렬 연산
//-------------------------------------

/*
   실습 내용
   [실습1] 행렬의 덧셈 코드 
   [실습2] 행렬의 곱셈 코드
   [실습3] 행렬의 연산 함수 활용하기
*/

void Mat_print(double A[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], int row, int col) {
    for (int i = 0; i < row; i++) {
        printf("[");
        for (int j = 0; j < col; j++)
            printf("%7.3f", A[i][j]);
        printf(" ]\n");
    }
    printf("\n");
}


void Mat_Add(double A[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], double B[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], double C[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], int mat_size) {
    for(int i=0; i<mat_size; i++)
        for(int j=0; j<mat_size; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void Mat_Mul(double A[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], double B[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], double C[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE], int mat_size) {
    for (int i=0; i<mat_size; i++)
        for (int j=0; j<mat_size; j++) {
            C[i][j] = 0.0;
            for (int k=0; k<mat_size; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
}


void run_matrix_test() {
    const int N = 4;
    double A[N][N] = { {1, 1, 1, 0}, {0, 1, 1, 1}, {1, 0, 1, 1}, {1, 1, 0, 1} };
    double B[N][N] = { {1, 2, 3, 0}, {0, 1, 2, 3}, {3, 0, 1, 2}, {2, 3, 0, 1} };
    double C[N][N], D[N][N];

    cout << "A = " << endl;
    Mat_print(A, 4, 4);
    cout << "B = " << endl; 
    Mat_print(B, 4, 4);

    //[실습3] 행렬의 연산 함수 활용하기
    // C = A + B
    Mat_Add(A,B,C,4);
    cout << "C = A + B = " << endl; 
    Mat_print(C, 4, 4);

    // D = A * B
    Mat_Mul(A,B,D,4);
    cout << "D = A * B = " << endl; 
    Mat_print(D, 4, 4);
}

/*
int main()
{
    run_matrix_test();
}
*/
