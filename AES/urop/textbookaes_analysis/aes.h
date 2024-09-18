#ifndef AES_H
#define AES_H

#include <stdint.h>

typedef unsigned char byte;
typedef unsigned char u8;
typedef unsigned int u32;


//==== from (rijndael-alg-fst.h)
#define GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ ((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]))
#define PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16); (ct)[2] = (u8)((st) >>  8); (ct)[3] = (u8)(st); }


void byte2state(byte b[16], u32 state[4]);
void state2byte(u32 state[4], byte b[16]);
// void AES32_print_state(u32 state[4]);
void AES32_round(u32 state[4], u32 rk[4]);
void AES32_EqInv_round(u32 state[4], u32 rk[4]);
void AES32_Encrypt(byte pt[16], u32 rk[11][4], byte ct[16]);
void AES32_EqDecrypt(byte ct[16], u32 rk[11][4], byte pt[16]);


//=== Key schedule ��
u32 RotWord(u32 w32);
u32 SubWord(u32 w32);
void AES32_Enc_KeySchedule(byte k[16], u32 rk[11][4]);
void AES32_Dec_KeySchedule(byte k[16], u32 rk[11][4]);
void AES8_KeySchedule(byte k[16], byte rk[11][16]);

#endif /* AES_H */