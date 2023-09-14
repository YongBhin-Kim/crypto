#pragma once

#include "../Util/util.h"


void GF128_xtime(unsigned char p[16]);
void GF128_mul(unsigned char p[16], unsigned char q[16]);
void Make_GHASH_const_R0R1(unsigned char R0[256], unsigned char R1[256]);
void Make_GHASH_H_table(unsigned char H[16], unsigned char HT[256][16]);

void GF128_Hmul(unsigned char state[16], unsigned char HT[256][16], unsigned char R0[256], unsigned char R1[256]);
void GHASH(unsigned char msg[], int msg_blocks, unsigned char H[16], unsigned char tag[16]);
void GHASH_TableVersion(unsigned char msg[], int msg_blocks,
	unsigned char HT[256][16], unsigned char R0[256], unsigned char R1[256], unsigned char tag[16]);


void GHASH_test();