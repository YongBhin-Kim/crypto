#pragma once
#include "GF_Math.h"

using namespace std; // 이 코드를 넣으면 std:cout을  간단히 cout으로 할 수 있다.

#define MATRIX_MAX_SIZE 20
#define NEARLY_ZERO 1e-10  // 절대값이 10^{-10}보다 작으면 0으로 간주한다 (divide_by_zero 방지)

//구조체 사용
struct Matrix {
    double M[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
    int row;
    int col;
};
