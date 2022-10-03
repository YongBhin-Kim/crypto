#pragma once

using namespace std;
typedef unsigned char u8;
typedef unsigned int u32;   // 32bit (= column)

// Functions
u8 GF_xtime(u8 x);
void Gen_AES32_EncTables();


// 전역변수
/*
u8 SBox[256];

u32 Te0[256];
u32 Te1[256];
u32 Te2[256];
u32 Te3[256];
u32 Te4[256];
*/