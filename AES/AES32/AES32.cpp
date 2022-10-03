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

void AES32_Test() {

    u8 pt[16] = {   0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    u8 ct[16]; int count = 0; u8 ciphertext[16] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};
    u8 rk[11][16]    = { { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f },  
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
    u32 roundKey[11][4];

    // rk[11][16] -> roundKey[11][4]
    for (int i=0; i<11; i++) byte2state(rk[i], roundKey[i]);

    // AES Encryption
    AES32_Enc(pt, roundKey, ct);

    // Plaintext  출력
    printf("PT = ");
    for (int i=0; i<16; i++) printf("%02x ", pt[i]); printf("\n");
    
    // Ciphertext 출력
    printf("CT = "); for (int i=0; i<16; i++) {
        printf("%02x ", ct[i]);
        if (ct[i] == ciphertext[i]) count++;
    } printf("\n");

    // Test vector veridation
    if (count == 16) printf("** 암호화 성공 **\n");
    else printf("** 암호화 실패 **\n");
}

void printstate(u32 state[4]) {
    for (int i=0; i<4; i++) printf("%08x ", state[i]); printf("\n");
}



int main() {
    AES32_Test();
}