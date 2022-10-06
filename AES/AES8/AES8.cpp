#include <iostream>
#include "AES8.h"

/**
 * AES 암호화 8비트 구현
 * AES - FIPS 197 표준문서
 * 5. Algorithm Specification
*/

// Generate SBox
// void gen_SBox() {
//     byte SBox[256];
//     Get_AES_Sbox(SBox);
//     cout << "SBox[256] = { " << endl;
//     for (int i=0; i<256; i++) {
//         printf(" 0x%02x, ", SBox[i]);
//         if ((i % 16) == 15) printf("\n");
//     }
//     cout << "}" << endl;
// }

// SubBytes : 입력 state[16]의 각 바이트에 SBox를 적용하여 업데이트
void SubBytes(byte state[16]) {
    for (int i=0; i<16; i++) state[i] = SBox[state[i]];
}

// ShiftRows : 각 행을 왼쪽으로 0, 1, 2, 3칸 로테이트 한다.
//
//     0   4   8   12                      0   4   8   12
//     1   5   9   13      -------->       5   9   13  1
//     2   6   10  14                      10  14  2   6
//     3   7   11  15                      15  3   7   11
// 
void ShiftRows(byte state[16]) {
    byte temp;
    temp = state[1]; state[1] = state[5]; state[5] = state[9]; state[9] = state[13]; state[13] = temp;

    temp = state[2];
    state[2] = state[10]; state[10] = temp;
    temp = state[6];
    state[6] = state[14]; state[14] = temp;

    temp = state[15]; state[15] = state[11]; state[11] = state[7]; state[7] = state[3]; state[3] = temp;
}

// MixColumns
void MixClomuns(byte state[16]) {
    // y = MC x         // 행렬과 벡터의 곱으로 연산
    // given MC Matrix : x*4 + 1
    byte MixCol[4][4] = { {0x02, 0x03, 0x01, 0x01}, {0x01, 0x02, 0x03, 0x01}, {0x01, 0x01, 0x02, 0x03} , {0x03, 0x01, 0x01, 0x02} };

    GF_Matrix MC;
    MC.row = 4; MC.col = 4;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            MC.M[i][j] = MixCol[i][j];

    GF_Matrix X, Y; // Column vector - 4 x 1 Matrix
    X.col = 1; X.row = 4;
    Y.col = 1; Y.row = 4;

    // Y = MC * X
    for (int col=0; col<4; col++) {
        for (int i=0; i<4; i++) X.M[i][0] = state[col*4+i]; // col=0 -> 0,1,2,3; col=1 -> 4,5,6,7 ...
        Y = GF_Mat_Mul(MC, X);

        for (int i=0; i<4; i++) state[col*4+i] = Y.M[i][0];
    }
}

// AddRoundKey
void AddRoundKey(byte state[16], byte roundkey[16]) {
    for (int i=0; i<16; i++) state[i] ^= roundkey[i];
}

// AES8 Encrypt
void AES_Encrypt(byte PT[16], byte RK[11][16], byte CT[16]) {
    byte state[16];
    for (int i=0; i<16; i++) state[i] = PT[i];

    // AddRoundKey
    AddRoundKey(state, RK[0]);

    // 1~9 Round
    for (int r=1; r<=9; r++) {
        SubBytes(state);
        ShiftRows(state);
        MixClomuns(state);
        AddRoundKey(state, RK[r]);
    }

    // 10 Round : SubBytes --- ShiftRow --- AddRoundKey
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, RK[10]);

    for (int i=0; i<16; i++) CT[i] = state[i];
}

// AES8 TESTCODE
void Run_AES8_test() {

    byte pt[16] = {   0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    byte rk[11][16]    = { { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f },  
                            { 0xd6, 0xaa, 0x74, 0xfd, 0xd2, 0xaf, 0x72, 0xfa, 0xda, 0xa6, 0x78, 0xf1, 0xd6, 0xab, 0x76, 0xfe },  
                            { 0xb6, 0x92, 0xcf, 0x0b, 0x64, 0x3d, 0xbd, 0xf1, 0xbe, 0x9b, 0xc5, 0x00, 0x68, 0x30, 0xb3, 0xfe },  
                            { 0xb6, 0xff, 0x74, 0x4e, 0xd2, 0xc2, 0xc9, 0xbf, 0x6c, 0x59, 0x0c, 0xbf, 0x04, 0x69, 0xbf, 0x41 },  
                            { 0x47, 0xf7, 0xf7, 0xbc, 0x95, 0x35, 0x3e, 0x03, 0xf9, 0x6c, 0x32, 0xbc, 0xfd, 0x05, 0x8d, 0xfd },  
                            { 0x3c, 0xaa, 0xa3, 0xe8, 0xa9, 0x9f, 0x9d, 0xeb, 0x50, 0xf3, 0xaf, 0x57, 0xad, 0xf6, 0x22, 0xaa },  
                            { 0x5e, 0x39, 0x0f, 0x7d, 0xf7, 0xa6, 0x92, 0x96, 0xa7, 0x55, 0x3d, 0xc1, 0x0a, 0xa3, 0x1f, 0x6b }, 
                            { 0x14, 0xf9, 0x70, 0x1a, 0xe3, 0x5f, 0xe2, 0x8c, 0x44, 0x0a, 0xdf, 0x4d, 0x4e, 0xa9, 0xc0, 0x26 }, 
                            { 0x47, 0x43, 0x87, 0x35, 0xa4, 0x1c, 0x65, 0xb9, 0xe0, 0x16, 0xba, 0xf4, 0xae, 0xbf, 0x7a, 0xd2 }, 
                            { 0x54, 0x99, 0x32, 0xd1, 0xf0, 0x85, 0x57, 0x68, 0x10, 0x93, 0xed, 0x9c, 0xbe, 0x2c, 0x97, 0x4e }, 
                            { 0x13, 0x11, 0x1d, 0x7f, 0xe3, 0x94, 0x4a, 0x17, 0xf3, 0x07, 0xa7, 0x8b, 0x4d, 0x2b, 0x30, 0xc5 }  };
    byte ct[16]; int count = 0;
    byte ciphertext[16] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};
    
    AES_Encrypt(pt, rk, ct);

    // Plaintext  출력
    printf("PT = ");
    for (int i=0; i<16; i++) printf("%02x ", pt[i]); printf("\n");
    
    // Ciphertext 출력
    printf("CT = ");
    for (int i=0; i<16; i++) {
        printf("%02x ", ct[i]);
        if (ct[i] == ciphertext[i]) count++;
    } printf("\n");
    if (count == 16) printf("** 암호화 성공 **\n");
    else printf("** 암호화 실패 **\n");

}

// main function
int main()
{
    Run_AES8_test();
}
