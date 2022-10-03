/**
 * AES - FIPS 197 표준문서
 * 4. Mathematical Prelimaries - MixColumns
 */

#include <iostream>
#include "GF_Matrix_Math.h"

//=========== GF(2^8) 행렬(4x4 한정) 연산들 ======================
/**
 * 
 *  [ GF(2^8) GF(2^8) GF(2^8) GF(2^8) ]
 *  [ GF(2^8) GF(2^8) GF(2^8) GF(2^8) ]
 *  [ GF(2^8) GF(2^8) GF(2^8) GF(2^8) ]
 *  [ GF(2^8) GF(2^8) GF(2^8) GF(2^8) ]
 * 
 */


/*
 * MixColumns 행렬의 3 1 1 2은 확산 효과가 좋다. 
 * a(x) = 3x^3 + x^2 + x + 2
 * 다항식 : x^4 + 1
*/

// GF_Matrix 인스턴스 생성&초기화
GF_Matrix GF_Mat_init() {
    GF_Matrix M;
    M.row = 1;
    M.col = 1;
    M.M[0][0] = 0x01;

    return M;
}

// GF_Matrix 출력
void GF_Mat_print(GF_Matrix Mat) {
    for (int i = 0; i < Mat.row; i++) { // 다항식, 이진수 형태로 출력하기
        for (int j = 0; j < Mat.col; j++)
            GF_print(Mat.M[i][j]); 
    }
    for (int i = 0; i < Mat.row; i++) {
        printf("[");
        for (int j = 0; j < Mat.col; j++)
            printf(" %02x", Mat.M[i][j]);
        printf(" ]\n");
    }
    printf("\n");
}

// GF_Matrix 곱셈
GF_Matrix GF_Mat_Mul(GF_Matrix A, GF_Matrix B) {       // 행렬들의 곱은 GF(2^8)^16 들의 곱셈

    GF_Matrix AB;
    AB = GF_Mat_init();

    if (A.col != B.row) {
        cout << "Matrix size error! \n";
        return AB;
    }
    byte temp;

    AB.row = A.row;
    AB.col = B.col;
    for (int i = 0; i < AB.row; i++)
        for (int j = 0; j < AB.col; j++) {
            AB.M[i][j] = 0x00;                         // initialize
            for (int k = 0; k < A.col; k++) {          // A.col == B.row
                temp = GF_mul(A.M[i][k], B.M[k][j]);   // 각 원소의 곱은 GF(2^8)에서의 곱셈
                AB.M[i][j] = GF_add(AB.M[i][j], temp); // 각 원소의 합은 GF(2^8)에서의 합 (ex) 37 + 28 = 100101 + 011100 = 100001 = 33 
            }
        }
    return AB;
}

// (피봇 행 찾기)
// 역행렬 구하기 - 행 교환 
void GF_Mat_Exchange_Row(GF_Matrix& M, int row1, int row2) {
    byte tmp;
    for (int idx = 0; idx < M.col; idx++) {
        tmp = M.M[row1][idx];
        M.M[row1][idx] = M.M[row2][idx];
        M.M[row2][idx] = tmp;
    }
}

// (피봇을 1로 만들기)
// 역행렬 구하기 - 한 행에 상수배 하기 : Matrix의 해당 row에 scalar배 한다. (여기서 scalar는 GF(2^8)의 원소)
void GF_Mat_Scalar_Mul(GF_Matrix& M, byte scalar, int row) {
    for (int idx=0; idx<M.col; idx++) {
        M.M[row][idx] = GF_mul(scalar, M.M[row][idx]); // scalar 은 GF(2^8)의 원소 이므로 즉, 두 다항식의 곱이라고 보면 됨
    }
}

// (피봇 제외 행을 0으로 만들기)
// 역행렬 구하기 - targetRow에 (srcRow * scalar)를 더한다.
void GF_Mat_Row_Add(GF_Matrix& M, byte scalar, int srcRow, int targetRow) { // row2 <- scalar * row1 + row2
    byte temp;
    for (int idx=0; idx<M.col; idx++) {
        temp = GF_mul(scalar, M.M[srcRow][idx]);
        M.M[targetRow][idx] = GF_add(M.M[targetRow][idx], temp);
    }
}

// GF(2^8)원소로 이루어진 Matrix의 역행렬을 구한다.
// MI = [ MC | I ] -----> [ I | InvMC ]
GF_Matrix GF_Mat_inverse(GF_Matrix M) {
    GF_Matrix MI;
    int pivot_row;
    // 형렬 M과 단위행렬 I를 옆으로 붙여 행렬 MM을 만든다. MM = [ M | I ]
    MI.row = M.row;
    MI.col = M.col * 2;
    for (int i=0; i<MI.row; i++) {
        for (int j=0; j<M.col; j++) {
            MI.M[i][j] = M.M[i][j];
            MI.M[i][M.col + j] = (i == j) ? 0x01 : 0x00;
        }
    }

    // Reduced REF(Row Echelon Form) 사다리꼴을 만들어 역행렬을 구한다.
    for (int j=0; j<M.col; j++) { 
        // GF_Mat_print(MI);
        pivot_row = -1;  // 핵심 행(피봇 행)을 찾는다. (초기값 -1로 설정)
        for (int i=j; i<MI.row; i++) {
            if (MI.M[i][j] != 0x00) { // [j][j], [j+1][j], [j+2][j] 중 0이 아닌 것, 실수는 abs
                pivot_row = i;
                break;
            }
        } // ------ after break
        if (pivot_row == -1) {  // 피봇 행이 없으면 (모든 행이 0인 경우)
            cout << "Not invertible !" << endl;  // 역행렬 없음
            return M;
        }
        if (pivot_row > j) {
            GF_Mat_Exchange_Row(MI, j, pivot_row); // j행을 pivot row로 만든다
        }
        // printf("(%d열의 %d행을 1로 만들기)\n", j, j);
        GF_Mat_Scalar_Mul(MI, GF_inv(MI.M[j][j]), j); // 피봇값이 1이 되도록 j행을 조정한다 -> 곱셈의 역원을 구하기 위해 GF_inv를 사용

        for (int i=0; i<MI.row; i++) {
            if ((i != j) && ((MI.M[i][j]) != 0)) { // abs
                // printf("(%d열의 %d행을 0으로 만들기)\n", j, i);
                GF_Mat_Row_Add(MI, MI.M[i][j], j, i); // i!=j 인 i행의 값을 모두 0으로 만들기, GF_inv, MI.M[i][j]의 덧셈의 역원은 자기자신 (GF(2^8)의 원소의 덧셈의 역원이므로 자기 자신임, (ex) 8 = 1000, 8 + 8 = 1000 + 1000 = 0 over coef : GF(2))
            }
        }
    }

    GF_Matrix Inv; // 역행렬을 저장할 구조체
    Inv.row = M.row;
    Inv.col = M.col;
    for (int i=0; i<M.row; i++) {
        for (int j=0; j<M.col; j++) {
            Inv.M[i][j] = MI.M[i][M.col + j];
        }
    }
    return Inv;
}


void Run_GF_matrix_test() {
    byte MixCol[4][4] = { {0x02, 0x03, 0x01, 0x01}, {0x01, 0x02, 0x03, 0x01},
                          {0x01, 0x01, 0x02, 0x03} , {0x03, 0x01, 0x01, 0x02} };
    //byte MixCol[4][4] = { {0x01, 0x00, 0x00, 0x00}, {0x00, 0x01, 0x00, 0x00},
    //                      {0x00, 0x00, 0x01, 0x00} , {0x00, 0x00, 0x00, 0x01} };
    GF_Matrix MC, MC2, MC4, InvMC;
    MC.row = 4;
    MC.col = 4;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            MC.M[i][j] = MixCol[i][j];
    cout << "MixColumn Matrix MC = " << endl;
    GF_Mat_print(MC);

    MC2 = GF_Mat_Mul(MC, MC);
    cout << "MC^2 = " << endl;
    GF_Mat_print(MC2);
    MC4 = GF_Mat_Mul(MC2, MC2);
    cout << "MC^4 = " << endl;
    GF_Mat_print(MC4);
/*
    InvMC = GF_Mat_inverse(MC);
    cout << "Inv MC" << endl;
    GF_Mat_print(InvMC);
*/
}

/*
int main()
{
    Run_GF_matrix_test();
}
*/

