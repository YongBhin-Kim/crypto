#pragma once

// #include "GF_Matrix.h"
// 행렬 계산 헤더
#include "../Math/GF_Math.h"
#include "../Math/GF_Matrix_Math.h"


// 선언
using namespace std;
typedef unsigned char u8;
typedef unsigned int u32;   // 32bit (= column)


// Functions
void byte2state(u8 pt[16], u32 state[4]);
void state2byte(u32 state[4], u8 ct[16]);
void printstate(u32 state[4]);

u32 RotWord(u32 w32);
u32 SubWord(u32 w32);
void AES32_KeySchedule(u8 k[16], u32 rk[11][4]);
void AES32_Round(u32 state[4], u32 rk[4]);
void AES32_Enc(u8 pt[16], u8 k[16], u8 ct[16]);


void AES8_AddRoundKey(u8 state[16], u8 rk[16]);
void AES8_InvSubbytes(u8 state[16]);
void AES8_InvShiftRows(u8 state[16]);
void AES8_InvMixColumns(u8 state[16]);
void AES8_KeySchedule(u8 k[16], u8 rk[11][16]);
void AES8_Dec(u8 ct[16], u8 k[16], u8 dec[16]);