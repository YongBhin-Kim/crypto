/*
 ========================================================================================================================
                                Pre-computated 테이블을 이용한 AES 32비트 구현
 ========================================================================================================================
*/

#include <iostream>
// #include "AES32_Table/AES32_Table.h"
#include "AES32.h"


// byte array -> u32 
#define GETU32(pt) ( ( (u32)(pt)[0] << 24) ^ ( (u32)(pt)[1] << 16) ^ ( (u32)(pt)[2] << 8) ^ ( (u32)(pt)[3]) );

// u32 -> byte array, st = ct[0] || ct[1] || ct[2] || ct[3]
// @param : (byte array, u32)
#define PUTU32(ct, st) \
{ (ct)[0] = (u8)(st>>24); (ct)[1] = (u8)(st>>16); (ct)[2] = (u8)(st>>8); (ct)[3] = (u8)(st); }

void byte2state(u8 pt[16], u32 state[4]) {
    state[0] = GETU32(pt);
    state[1] = GETU32(pt+4);
    state[2] = GETU32(pt+8);
    state[3] = GETU32(pt+12);
}

void state2byte(u32 state[4], u8 ct[16]) {
    PUTU32(ct, state[0]);
    PUTU32(ct+4, state[1]);
    PUTU32(ct+8, state[2]);
    PUTU32(ct+12, state[3]);
}

void printstate(u32 state[4]) {
    for (int i=0; i<4; i++) printf("%08x ", state[i]); printf("\n");
}

u32 RotWord(u32 w32) {
    return (w32 << 8) ^ (w32 >> 24);
}

u32 SubWord(u32 w32) {
    return (SBox[w32 >> 24] << 24) ^ (SBox[(w32 >> 16) & 0xff] << 16) ^ (SBox[(w32 >> 8) & 0xff] << 8) ^ SBox[w32 & 0xff];
}

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

// Te는 이미 SBox까지 처리한 것
// output[0] = input[0]>>24 ^ input[1]>>16 ^ input[2]>>8 ^ input[3]
void AES32_Round(u32 state[4], u32 rk[4]) {
    u32 tmp[4];
    tmp[0] = Te0[ (u8)(state[0]>>24)] ^ Te1[ (u8)(state[1]>>16)] ^ Te2[ (u8)(state[2]>>8)] ^ Te3[ (u8)(state[3])] ^ rk[0];
    tmp[1] = Te0[ (u8)(state[1]>>24)] ^ Te1[ (u8)(state[2]>>16)] ^ Te2[ (u8)(state[3]>>8)] ^ Te3[ (u8)(state[0])] ^ rk[1];
    tmp[2] = Te0[ (u8)(state[2]>>24)] ^ Te1[ (u8)(state[3]>>16)] ^ Te2[ (u8)(state[0]>>8)] ^ Te3[ (u8)(state[1])] ^ rk[2];
    tmp[3] = Te0[ (u8)(state[3]>>24)] ^ Te1[ (u8)(state[0]>>16)] ^ Te2[ (u8)(state[1]>>8)] ^ Te3[ (u8)(state[2])] ^ rk[3];

    for (int i=0; i<4; i++) state[i] = tmp[i];
}

void AES32_Enc(u8 pt[16], u32 rk[11][4], u8 ct[16]) {
    u32 state[4], tmp[4];
    // u8 [16] --> u32 [4]
    byte2state(pt, state);

    // AddRoundKey
    for (int i=0; i<4; i++) state[i] ^= rk[0][i];

    // 1~9 Round
    for (int r=1; r<=9; r++)
        AES32_Round(state, rk[r]);

    // 10 Round : SubBytes --- ShiftRow --- AddRoundKey
    // Te4에서 단위행렬에 해당하는 부분만 가져옴
#if 1
    tmp[0] = ( Te4[(u8)(state[0]>>24)] & 0xff000000) ^ ( Te4[(u8)(state[1]>>16)] & 0x00ff0000) ^ ( Te4[(u8)(state[2]>>8)] & 0x0000ff00) ^ ( Te4[(u8)(state[3])] & 0x000000ff) ^ rk[10][0];
    tmp[1] = ( Te4[(u8)(state[1]>>24)] & 0xff000000) ^ ( Te4[(u8)(state[2]>>16)] & 0x00ff0000) ^ ( Te4[(u8)(state[3]>>8)] & 0x0000ff00) ^ ( Te4[(u8)(state[0])] & 0x000000ff) ^ rk[10][1];
    tmp[2] = ( Te4[(u8)(state[2]>>24)] & 0xff000000) ^ ( Te4[(u8)(state[3]>>16)] & 0x00ff0000) ^ ( Te4[(u8)(state[0]>>8)] & 0x0000ff00) ^ ( Te4[(u8)(state[1])] & 0x000000ff) ^ rk[10][2];
    tmp[3] = ( Te4[(u8)(state[3]>>24)] & 0xff000000) ^ ( Te4[(u8)(state[0]>>16)] & 0x00ff0000) ^ ( Te4[(u8)(state[1]>>8)] & 0x0000ff00) ^ ( Te4[(u8)(state[2])] & 0x000000ff) ^ rk[10][3];
#endif

    for (int i=0; i<4; i++) state[i] = tmp[i];

    // u32 [4] --> u8 [16]
    state2byte(state, ct);
}

// AES8 AddRoundKey
void AES8_AddRoundKey(u8 state[16], u8 rk[16]) {
    for (int i=0; i<16; i++)
        state[i] ^= rk[i];
}

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

// AES8 KeySchedule
void AES8_KeySchedule(u8 k[16], u8 rk[11][16]) {
    u32 rk32[11][4];

    AES32_KeySchedule(k, rk32);

    for (int i=0; i<11; i++)
        state2byte(rk32[i], rk[i]);
}

// AES8 DECRYPTION
void AES8_Dec(u8 ct[16], u8 k[16], u8 dec[16]) {
    u8 state[16];
    for (int i=0; i<16; i++)
        state[i] = ct[i];
    
    u8 rk[11][16];
    AES8_KeySchedule(k, rk);

    AES8_AddRoundKey(state, rk[10]);

    for (int r = 9; r >= 1; r--) {
        AES8_InvSubbytes(state);
        AES8_InvShiftRows(state);
        AES8_InvMixColumns(state);
        AES8_InvMixColumns(rk[r]);
        AES8_AddRoundKey(state, rk[r]);
    }

    AES8_InvSubbytes(state);
    AES8_InvShiftRows(state);
    AES8_AddRoundKey(state, rk[0]);

    for (int i=0; i<16; i++)
        dec[i] = state[i];
}
void AES32_Test() {

    u8 pt[16] = {   0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    u8 k[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    u8 ct[16]; 
    u8 rk[11][16];
    u32 roundKey[11][4];
    u8 ciphertext[16] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};
    u8 dec[16];
    int count;

    // k[16] -> roundKey[11][4]
    AES32_KeySchedule(k, roundKey);

    // AES Encryption
    AES32_Enc(pt, roundKey, ct);

    // Print Plaintext
    printf("PT = ");
    for (int i=0; i<16; i++) 
        printf("%02x ", pt[i]); 
    printf("\n");
    
    // Print Ciphertext
    printf("CT = "); 
    for (int i=0; i<16; i++) {
        printf("%02x ", ct[i]);
        if (ct[i] == ciphertext[i]) 
            count++;
    } printf("\n");

    // Test vector veridation
    count = 0;
    for (int i=0; i<16; i++)
        if (ct[i] == ciphertext[i]) 
            count++;

    if (count == 16) 
        printf("** 암호화 성공 **\n");
    else 
        printf("** 암호화 실패 **\n");

    // AES Decryption
    AES8_Dec(ct, k, dec);

    // Decrypted text  출력
    printf("DEC = ");
    for (int i=0; i<16; i++) 
        printf("%02x ", dec[i]); 
    printf("\n");

    // Test vector veridation
    count = 0;
    for (int i=0; i<16; i++)
        if (dec[i] == pt[i]) 
            count++;

    if (count == 16) 
        printf("** 복호화 성공 **\n");
    else 
        printf("** 복호화 실패 **\n");


}

#if 0
int main() {
    AES32_Test();
}
#endif