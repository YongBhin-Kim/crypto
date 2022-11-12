#include <iostream>
#include "AES8.h"

/**
 * AES 암호화 8비트 구현
 * AES 복호화 8비트 구현
 * AES - FIPS 197 표준문서
 * 5. Algorithm Specification
*/

// 8bit array[4] -> 32bit integer
#define GETU32(pt) ( ( (u32)(pt)[0] << 24) ^ ( (u32)(pt)[1] << 16) ^ ( (u32)(pt)[2] << 8) ^ ( (u32)(pt)[3]) );

// 32bit integer -> 8bit array[4]
#define PUTU32(ct, st) \
{ (ct)[0] = (u8)(st>>24); (ct)[1] = (u8)(st>>16); (ct)[2] = (u8)(st>>8); (ct)[3] = (u8)(st); }

// 8bit array[16] -> 32bit array[4]
void byte2state(u8 pt[16], u32 state[4]) {
    state[0] = GETU32(pt);
    state[1] = GETU32(pt+4);
    state[2] = GETU32(pt+8);
    state[3] = GETU32(pt+12);
}

// 32bit array[4] -> 8bit array[16]
void state2byte(u32 state[4], u8 ct[16]) {
    PUTU32(ct, state[0]);
    PUTU32(ct+4, state[1]);
    PUTU32(ct+8, state[2]);
    PUTU32(ct+12, state[3]);
}

// For KeySchedule
u32 RotWord(u32 w32) {
    return (w32 << 8) ^ (w32 >> 24);
}

// For KeySchedule
u32 SubWord(u32 w32) {
    return (SBox[w32 >> 24] << 24) ^ (SBox[(w32 >> 16) & 0xff] << 16) ^ (SBox[(w32 >> 8) & 0xff] << 8) ^ SBox[w32 & 0xff];
}

// AES32 KeySchedule
void AES32_KeySchedule(u8 k[16], u32 rk[11][4]) {
    u32 tmp;
    int rcon_counter;

    for (int i=0; i<4; i++)
        rk[0][i] = GETU32(k+4*i);
    
    rcon_counter=0;
    for (int i=1; i<11; i++) {
        tmp = rk[i-1][3];
        for (int k=0; k<4; k++) {
            if (k == 0) {
                tmp = SubWord(RotWord(tmp)) ^ Rcon[rcon_counter];
                rcon_counter++;
            }
            rk[i][k] = rk[i-1][k] ^ tmp;
            tmp = rk[i][k];
        }
    }
}

// AES8 KeySchedule
void AES8_KeySchedule(u8 k[16], u8 rk[11][16]) {
    u32 rk32[11][4];

    AES32_KeySchedule(k, rk32);

    for (int i=0; i<11; i++)
        state2byte(rk32[i], rk[i]);
}

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
void AES_Encrypt(byte PT[16], byte K[16], byte CT[16]) {
    byte state[16];
    for (int i=0; i<16; i++) 
        state[i] = PT[i];

    byte RK[11][16];
    AES8_KeySchedule(K,RK);

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


// DEC
// AES8 InvSubBytes
void AES8_InvSubbytes(u8 state[16]) {
    for (int i=0; i<16; i++)
        state[i] = ISBox[state[i]];
}

// AES8_InvShiftRows : 각 행을 오른쪽으로 0, 1, 2, 3칸 로테이트 한다.
//     0   4   8   12                      0   4   8   12
//     1   5   9   13      -------->       13  1   5   9
//     2   6   10  14                      10  14  2   6
//     3   7   11  15                      7   11  15  3
void AES8_InvShiftRows(u8 state[16]) {
    u8 tmp;
    tmp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = tmp;

    tmp = state[2];
    state[2] = state[10];
    state[10] = tmp;
    tmp = state[6];
    state[6] = state[14];
    state[14] = tmp;

    tmp = state[3];
    state[3] = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = tmp;

}

// AES8_InvMixColumns : 다항식이 3x^3 + x^2 + x + 1 ----> bx^3 + dx^2 + 9x + e 로 바뀐다. ( GF(2^8)^4에서의 역원, 혹은 행렬에서의 역행렬, 링에서의 다항식은 x^4 + 1 )
void AES8_InvMixColumns(u8 state[16]) {
    // y = MC x         // 행렬과 벡터의 곱으로 연산
    u8 InvMixCol[4][4] = { {0x0e, 0x0b, 0x0d, 0x09}, {0x09, 0x0e, 0x0b, 0x0d}, {0x0d, 0x09, 0x0e, 0x0b} , {0x0b, 0x0d, 0x09, 0x0e} };

    GF_Matrix MC;
    MC.row = 4; MC.col = 4;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            MC.M[i][j] = InvMixCol[i][j];

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

// AES8 DECRYPTION
void AES8_Dec(u8 ct[16], u8 k[16], u8 dec[16]) {
    u8 state[16];
    for (int i=0; i<16; i++)
        state[i] = ct[i];
    
    u8 rk[11][16];
    AES8_KeySchedule(k, rk);
    AddRoundKey(state, rk[10]);

    for (int r = 9; r >= 1; r--) {
        AES8_InvSubbytes(state);
        AES8_InvShiftRows(state);
        AES8_InvMixColumns(state);
        AES8_InvMixColumns(rk[r]);
        AddRoundKey(state, rk[r]);
    }

    AES8_InvSubbytes(state);
    AES8_InvShiftRows(state);
    AddRoundKey(state, rk[0]);

    for (int i=0; i<16; i++)
        dec[i] = state[i];
}


// AES8 TESTCODE
void Run_AES8_test() {

    byte pt[16] = {   0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    byte k[16]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    byte ct[16]; 
    byte ciphertext[16] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};
    
    // Print Plaintext
    printf("PT = ");
    for (int i=0; i<16; i++) 
        printf("%02x ", pt[i]); 
    printf("\n\n");
    
    // AES Encryption
    AES_Encrypt(pt, k, ct);

    // Print Ciphertext
    printf("CT = ");
    for (int i=0; i<16; i++)
        printf("%02x ", ct[i]);
    printf("\n");

    // Varification - Ciphertext
    int count;
    count = 0;
    for (int i=0; i<16; i++)
        if (ct[i] == ciphertext[i]) 
            count++;

    if (count == 16) 
        printf("** Success to Encryprtion **\n\n");
    else {
        printf("** Failed to Encryprtion **\n");
        return;
    }

    byte dec[16];
    AES8_Dec(ct, k, dec);

    // AES Decryption
    printf("DEC = ");
    for (int i=0; i<16; i++)
        printf("%02x ", dec[i]);
    printf("\n");

    // Varification - Decrypton
    count = 0;
    for (int i=0; i<16; i++)
        if (dec[i] == pt[i]) 
            count++;

    if (count == 16) 
        printf("** Success to Decryption **\n\n");
    else {
        printf("** Failed to Decryption **\n");
        return;
    }


}

// main function
int main()
{
    Run_AES8_test();
}
