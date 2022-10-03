#include <iostream>
#include "Matrix_ver2.h"

// Matrix_ver2_template.cpp : 2022.9.22
// 구조체를 이용한 행렬 구현
// 역행렬 구하기
//-------------------------------------

/*
   실습 내용
   [실습1] 전달받은 A[][]로 구조체에 행렬을 설정하기
   [실습2] Matrix_ver1.cpp의 형렬 출력함수를 수정하여 완성하기
   [실습3] Matrix_ver1.cpp의 형렬곱 함수를 수정하여 완성하기
   [실습4] 구조체를 리턴하는 행렬곱 함수 만들기
   [실습5] 행렬의 두행을 바꾸기
   [실습6] 한행에 상수배 하기
   [실습7] 역행력 구하기
*/

/** 
 ========================================================================================================================
                                                선언문 --> 헤더
 ========================================================================================================================
 */

/** 
 * Mat_init 함수
 * 배열로 Matrix 설정  ---> max size는 20이지만 col 크기가 4로 고정되어야 하는 문제
 * 열의 크기는 실제 함수호출시 전달하는 크기와 같아야 함 --> class로 구현할 때 해결할 예정임
 */
Matrix Mat_init(double A[][4], int row, int col) { 
    Matrix Result;
    Result.row = row;
    Result.col = col;

    //[실습1] 전달받은 A[][]로 구조체에 행렬을 설정하기 == 초기화
    for (int i=0; i<Result.row; i++)
        for (int j=0; j<Result.col; j++)
            Result.M[i][j] = A[i][j];
    return Result;
}

// 같은 이름의 함수라도 파라미터가 다르면 구분된다. 
Matrix Mat_init() {
    Matrix M;
    M.row = 1;
    M.col = 1;
    M.M[0][0] = 1;

    return M;
}

// Matrix 출력
void Mat_print(Matrix Mat) {
    for (int i=0; i < Mat.row; i++) {
        printf("[");
        for (int j = 0; j < Mat.col; j++)
            printf("%7.3f", Mat.M[i][j]); // really zero 보다 절댓값이 작으면 0으로 찍어라!
        printf(" ]\n");
    } printf("\n");
}

// Matrix 곱셈
void Mat_Mul(Matrix A, Matrix B, Matrix& AB) {

    if (A.col != B.row) {
        cout << "Matrix size error! \n";
        return;
    }
    AB.row = A.row;
    AB.col = B.col; // call by reference이므로 바로 넘어감

    for (int i=0; i<AB.row; i++) {
        for (int j=0; j<AB.col; j++) {
            AB.M[i][j] = 0.0;
            for (int k=0; k<A.col; k++) // A.col = B.row
                AB.M[i][j] += A.M[i][k] * B.M[k][j];
        }
    }
}

// Matrix 곱셈 --  구조체를 리턴하는 함수
Matrix Mat_Mul(Matrix A, Matrix B) {
    Matrix AB;
    // AB = Mat_init();
    if (A.col != B.row) {
        cout << "Matrix size error! \n";
        return AB;
    }

    AB.row = A.row;
    AB.col = B.col; // call by reference이므로 바로 넘어감

    for (int i=0; i<AB.row; i++) {
        for (int j=0; j<AB.col; j++) {
            AB.M[i][j] = 0.0;
            for (int k=0; k<A.col; k++) // A.col = B.row
                AB.M[i][j] += A.M[i][k] * B.M[k][j];
        }
    }

    return AB;
}

// Matrix의 두 행을 바꾸는 함수
void Mat_Exchange_Row(Matrix& M, int row1, int row2) { // M[row1][] <----> M[row2][]
    double tmp;
    for (int j=0; j<M.col; j++) {
        tmp = M.M[row1][j];
        M.M[row1][j] = M.M[row2][j];
        M.M[row2][j] = tmp;
    }

}

// Matrix의 한 행에 상수배 하기
void Mat_Scalar_Mul(Matrix& M, double scalar, int row) {
    for (int j=0; j<M.col; j++)
        M.M[row][j] *= scalar;
}

// Matrix의 한 행의 상수배를 다른 행에 더하기
void Mat_Row_Add(Matrix& M, int tragetRow, int srcRow, double scalar) {
    for (int j=0; j<M.col; j++) {
        printf(" %.2f --> ", M.M[tragetRow][j]);
        M.M[tragetRow][j] += M.M[srcRow][j] * scalar;
        printf(" %.2f\n", M.M[tragetRow][j]);
    }
}

// Inverse Matrix 구하는 함수 -- 구조체 리턴
Matrix Mat_inverse(Matrix M) {
    Matrix MM;
    int pivot_row;

    // 형렬 A와 단위행렬 I를 옆으로 붙여 행렬 AA를 만든다. AA = [ A | I ]
    MM.row = M.row;
    MM.col = M.col * 2;
    for (int i=0; i<MM.row; i++)
        for (int j=0; j<M.col; j++) {
            MM.M[i][j] = M.M[i][j];
            MM.M[i][M.col+j] = (i == j) ? 1.0 : 0.0;
        }

    // Reduced REF(Row Echelon Form) 사다리꼴을 만들어 역행렬을 구한다.
    for (int j=0; j<M.col; j++) { 
        pivot_row = -1;  // 핵심 행(피봇 행)을 찾는다. (초기값 -1로 설정)
        for (int i=j; i<MM.row; i++) {
            if (abs(MM.M[i][j]) > NEARLY_ZERO) { // [j][j], [j+1][j], [j+2][j] 중 0이 아닌 것
                pivot_row = i;
                break;
            }
        }
        if (pivot_row == -1) {  // 피봇 행이 없으면 (모두 0인 경우)
            cout << "Not invertible !" << endl;  // 역행렬 없음
            return M;  // 의미없는 리턴값
        }
        if (pivot_row > j) { // [실습7-c] -- [j][j]가 피봇이 되도록 행을 교환한다
            Mat_Exchange_Row(MM, j, pivot_row);
        }
        Mat_Scalar_Mul(MM, 1. / MM.M[j][j], j); // 피봇값이 1이 되도록 j행을 조정한다
        for (int i=0; i<MM.row; i++) {
            if ((i != j) && (abs(MM.M[i][j]) > NEARLY_ZERO)) {
                printf("(addidion) src = %d, target = %d scalar = %.2f \n", j, i, -MM.M[i][j]);
                Mat_print(MM);
                Mat_Row_Add(MM, i, j, -MM.M[i][j]);
                Mat_print(MM); printf("---------------------------\n");
            }
        }
        //[실습7-e] 단계별 결과를 출력하기
        Mat_print(MM);
    }
    Matrix Inv; // 역행렬을 저장할 구조체
    Inv.row = M.row;
    Inv.col = M.col;
    for (int i=0; i<M.row; i++) {
        for (int j=0; j<M.col; j++) {
            Inv.M[i][j] = MM.M[i][M.col + j];
        }
    }
    return Inv;
}

// C언어 방식의 행렬곱 테스트
void run_matrix_mul_test1() {
    const int N = 4;
    double A[N][N] = { {1, 1, 1, 0}, {0, 1, 1, 1}, {1, 0, 1, 1}, {1, 1, 0, 1} };
    double B[N][N] = { {1, 2, 3, 0}, {0, 1, 2, 3}, {3, 0, 1, 2}, {2, 3, 0, 1} };
    double C[N][N];

    Matrix MA, MB, MC, MAB;
    MA = Mat_init(A, 4, 4);
    MB = Mat_init(B, 4, 4);
    MC = Mat_init(B, 4, 4);

    //[실습1,2,3] 테스트 하기
    /*==
    cout << "== run matrix mul test 1 ===" << endl << endl;
    Mat_Mul(MA, MB, MC);
    cout << "A = " << endl; 
    Mat_print(MA);
    cout << "B = " << endl; 
    Mat_print(MB);
    cout << "C = A * B =" << endl; 
    Mat_print(MC);
    */
}

//구조체를 리턴하는 행렬곱 테스트
void run_matrix_mul_test2() {
    const int N = 4;
    double A[N][N] = { {1, 1, 1, 0}, {0, 1, 1, 1}, {1, 0, 1, 1}, {1, 1, 0, 1} };
    double B[N][N] = { {1, 2, 3, 0}, {0, 1, 2, 3}, {3, 0, 1, 2}, {2, 3, 0, 1} };

    Matrix MA, MB, MC, MAB;
    MA = Mat_init(A, 4, 4);
    MB = Mat_init(B, 4, 4);

    //[실습4] 테스트 하기
    /*==
    cout << "== run matrix mul test 2 ===" << endl << endl;
    MAB = Mat_Mul(MA, MB);
    cout << "A = " << endl;
    Mat_print(MA);
    cout << "B = " << endl;
    Mat_print(MB);
    cout << "C = A * B =" << endl;
    Mat_print(MAB);
    */
}

void run_matrix_inv_test() {
    const int N = 3;
    double A[N][N] = { {0.5, -0.25, 0}, {-0.25, 0.5, -0.25}, {0, -0.25, 0.5} };


    Matrix MA, InvMA;
    MA.row = 3;
    MA.col = 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            MA.M[i][j] = A[i][j];

    //[실습7] 테스트하기
    // /*

    cout << "== run matrix inv test ===" << endl << endl;
    cout << "A = " << endl;
    Mat_print(MA);
    InvMA = Mat_inverse(MA);
    cout << "Inverse of A = " << endl;
    Mat_print(InvMA);

    cout << "check A * invA = I" << endl;
    Matrix MI;
    MI = Mat_Mul(MA, InvMA);
    Mat_print(MI);
    // */
}

/*
int main()
{
    run_matrix_mul_test1();
    run_matrix_mul_test2();
    run_matrix_inv_test();
}

*/