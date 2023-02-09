/*
    ARIA - C Parallel implementation
*/
//  Copyright © Yonhbhin Kim
//  Github url : https://github.com/YongBhin-Kim

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "aria_parallel.h"

#define FAIL   -1
#define SUCCESS 0

/* Parallel */
pthread_mutex_t mutx;

/* Constant for key expansion */
unsigned char C1[16] = { 0x51, 0x7c, 0xc1, 0xb7, 0x27, 0x22, 0x0a, 0x94, 0xfe, 0x13, 0xab, 0xe8, 0xfa, 0x9a, 0x6e, 0xe0 };
unsigned char C2[16] = { 0x6d, 0xb1, 0x4a, 0xcc, 0x9e, 0x21, 0xc8, 0x20, 0xff, 0x28, 0xb1, 0xd5, 0xef, 0x5d, 0xe2, 0xb0 };
unsigned char C3[16] = { 0xdb, 0x92, 0x37, 0x1d, 0x21, 0x26, 0xe9, 0x70, 0x03, 0x24, 0x97, 0x75, 0x04, 0xe8, 0xc9, 0x0e };

/* Sbox1, Sbox2, Inverse of Sbox1, Inverse of Sbox2 for substlayer */
unsigned char sbox[2][256] = {{
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
},
{ 0xe2, 0x4e, 0x54, 0xfc, 0x94, 0xc2, 0x4a, 0xcc, 0x62, 0x0d, 0x6a, 0x46, 0x3c, 0x4d, 0x8b, 0xd1,
  0x5e, 0xfa, 0x64, 0xcb, 0xb4, 0x97, 0xbe, 0x2b, 0xbc, 0x77, 0x2e, 0x03, 0xd3, 0x19, 0x59, 0xc1,
  0x1d, 0x06, 0x41, 0x6b, 0x55, 0xf0, 0x99, 0x69, 0xea, 0x9c, 0x18, 0xae, 0x63, 0xdf, 0xe7, 0xbb,
  0x00, 0x73, 0x66, 0xfb, 0x96, 0x4c, 0x85, 0xe4, 0x3a, 0x09, 0x45, 0xaa, 0x0f, 0xee, 0x10, 0xeb,
  0x2d, 0x7f, 0xf4, 0x29, 0xac, 0xcf, 0xad, 0x91, 0x8d, 0x78, 0xc8, 0x95, 0xf9, 0x2f, 0xce, 0xcd,
  0x08, 0x7a, 0x88, 0x38, 0x5c, 0x83, 0x2a, 0x28, 0x47, 0xdb, 0xb8, 0xc7, 0x93, 0xa4, 0x12, 0x53,
  0xff, 0x87, 0x0e, 0x31, 0x36, 0x21, 0x58, 0x48, 0x01, 0x8e, 0x37, 0x74, 0x32, 0xca, 0xe9, 0xb1,
  0xb7, 0xab, 0x0c, 0xd7, 0xc4, 0x56, 0x42, 0x26, 0x07, 0x98, 0x60, 0xd9, 0xb6, 0xb9, 0x11, 0x40,
  0xec, 0x20, 0x8c, 0xbd, 0xa0, 0xc9, 0x84, 0x04, 0x49, 0x23, 0xf1, 0x4f, 0x50, 0x1f, 0x13, 0xdc,
  0xd8, 0xc0, 0x9e, 0x57, 0xe3, 0xc3, 0x7b, 0x65, 0x3b, 0x02, 0x8f, 0x3e, 0xe8, 0x25, 0x92, 0xe5,
  0x15, 0xdd, 0xfd, 0x17, 0xa9, 0xbf, 0xd4, 0x9a, 0x7e, 0xc5, 0x39, 0x67, 0xfe, 0x76, 0x9d, 0x43,
  0xa7, 0xe1, 0xd0, 0xf5, 0x68, 0xf2, 0x1b, 0x34, 0x70, 0x05, 0xa3, 0x8a, 0xd5, 0x79, 0x86, 0xa8,
  0x30, 0xc6, 0x51, 0x4b, 0x1e, 0xa6, 0x27, 0xf6, 0x35, 0xd2, 0x6e, 0x24, 0x16, 0x82, 0x5f, 0xda,
  0xe6, 0x75, 0xa2, 0xef, 0x2c, 0xb2, 0x1c, 0x9f, 0x5d, 0x6f, 0x80, 0x0a, 0x72, 0x44, 0x9b, 0x6c,
  0x90, 0x0b, 0x5b, 0x33, 0x7d, 0x5a, 0x52, 0xf3, 0x61, 0xa1, 0xf7, 0xb0, 0xd6, 0x3f, 0x7c, 0x6d,
  0xed, 0x14, 0xe0, 0xa5, 0x3d, 0x22, 0xb3, 0xf8, 0x89, 0xde, 0x71, 0x1a, 0xaf, 0xba, 0xb5, 0x81
}};

unsigned char isbox[2][256] = {{
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
},
{
  0x30, 0x68, 0x99, 0x1b, 0x87, 0xb9, 0x21, 0x78, 0x50, 0x39, 0xdb, 0xe1, 0x72, 0x09, 0x62, 0x3c,
  0x3e, 0x7e, 0x5e, 0x8e, 0xf1, 0xa0, 0xcc, 0xa3, 0x2a, 0x1d, 0xfb, 0xb6, 0xd6, 0x20, 0xc4, 0x8d,
  0x81, 0x65, 0xf5, 0x89, 0xcb, 0x9d, 0x77, 0xc6, 0x57, 0x43, 0x56, 0x17, 0xd4, 0x40, 0x1a, 0x4d,
  0xc0, 0x63, 0x6c, 0xe3, 0xb7, 0xc8, 0x64, 0x6a, 0x53, 0xaa, 0x38, 0x98, 0x0c, 0xf4, 0x9b, 0xed,
  0x7f, 0x22, 0x76, 0xaf, 0xdd, 0x3a, 0x0b, 0x58, 0x67, 0x88, 0x06, 0xc3, 0x35, 0x0d, 0x01, 0x8b,
  0x8c, 0xc2, 0xe6, 0x5f, 0x02, 0x24, 0x75, 0x93, 0x66, 0x1e, 0xe5, 0xe2, 0x54, 0xd8, 0x10, 0xce,
  0x7a, 0xe8, 0x08, 0x2c, 0x12, 0x97, 0x32, 0xab, 0xb4, 0x27, 0x0a, 0x23, 0xdf, 0xef, 0xca, 0xd9,
  0xb8, 0xfa, 0xdc, 0x31, 0x6b, 0xd1, 0xad, 0x19, 0x49, 0xbd, 0x51, 0x96, 0xee, 0xe4, 0xa8, 0x41,
  0xda, 0xff, 0xcd, 0x55, 0x86, 0x36, 0xbe, 0x61, 0x52, 0xf8, 0xbb, 0x0e, 0x82, 0x48, 0x69, 0x9a,
  0xe0, 0x47, 0x9e, 0x5c, 0x04, 0x4b, 0x34, 0x15, 0x79, 0x26, 0xa7, 0xde, 0x29, 0xae, 0x92, 0xd7,
  0x84, 0xe9, 0xd2, 0xba, 0x5d, 0xf3, 0xc5, 0xb0, 0xbf, 0xa4, 0x3b, 0x71, 0x44, 0x46, 0x2b, 0xfc,
  0xeb, 0x6f, 0xd5, 0xf6, 0x14, 0xfe, 0x7c, 0x70, 0x5a, 0x7d, 0xfd, 0x2f, 0x18, 0x83, 0x16, 0xa5,
  0x91, 0x1f, 0x05, 0x95, 0x74, 0xa9, 0xc1, 0x5b, 0x4a, 0x85, 0x6d, 0x13, 0x07, 0x4f, 0x4e, 0x45,
  0xb2, 0x0f, 0xc9, 0x1c, 0xa6, 0xbc, 0xec, 0x73, 0x90, 0x7b, 0xcf, 0x59, 0x8f, 0xa1, 0xf9, 0x2d,
  0xf2, 0xb1, 0x00, 0x94, 0x37, 0x9f, 0xd0, 0x2e, 0x9c, 0x6e, 0x28, 0x3f, 0x80, 0xf0, 0x3d, 0xd3,
  0x25, 0x8a, 0xb5, 0xe7, 0x42, 0xb3, 0xc7, 0xea, 0xf7, 0x4c, 0x11, 0x33, 0x03, 0xa2, 0xac, 0x60
}};


/* Odd round F function */
void fo(unsigned char state[16], unsigned char key[16]) {

    // AddRoundKey
    for (int i=0; i<16; i++)
        state[i] ^= key[i];

    // SubstLayer type1 (LT||LT||LT||LT)
    for (int i=0; i<4; i++) {
        state[4*i] = sbox[0][state[4*i]];
        state[4*i+1] = sbox[1][state[4*i+1]];
        state[4*i+2] = isbox[0][state[4*i+2]];
        state[4*i+3] = isbox[1][state[4*i+3]];
    }

    // Difflayer
    difflayer(state, state);
}

/* Even round F function */
void fe(unsigned char state[16], unsigned char key[16]) {

    // AddRoundKey
    for (int i=0; i<16; i++)
        state[i] ^= key[i];
    
    // SubstLayer type2 (invLT||invLT||invLT||invLT)
    for (int i=0; i<4; i++) {
        state[4*i] = isbox[0][state[4*i]];
        state[4*i+1] = isbox[1][state[4*i+1]];
        state[4*i+2] = sbox[0][state[4*i+2]];
        state[4*i+3] = sbox[1][state[4*i+3]];
    }

    // Difflayer
    difflayer(state, state);
}

/* Final round F function */
void ff(unsigned char state[16], unsigned char key[16], unsigned char whiteningkey[16]) {

    // AddRoundKey
    for (int i=0; i<16; i++)
        state[i] ^= key[i];

    // SubstLayer type2 (invLT||invLT||invLT||invLT)
    for (int i=0; i<4; i++) {
        state[4*i] = isbox[0][state[4*i]];
        state[4*i+1] = isbox[1][state[4*i+1]];
        state[4*i+2] = sbox[0][state[4*i+2]];
        state[4*i+3] = sbox[1][state[4*i+3]];
    }

    // AddRoundKey - Whitening key
    for (int i=0; i<16; i++)
        state[i] ^= whiteningkey[i];
}

/* Diffusion Layer function */
void difflayer(unsigned char src[16], unsigned char dst[16]) {

    unsigned char temp[16];

    for (int i=0; i<16; i++)
        temp[i] = src[i];

    dst[0]=temp[3]^temp[4]^temp[6]^temp[8]^temp[9]^temp[13]^temp[14];
    dst[1]=temp[2]^temp[5]^temp[7]^temp[8]^temp[9]^temp[12]^temp[15];
    dst[2]=temp[1]^temp[4]^temp[6]^temp[10]^temp[11]^temp[12]^temp[15];
    dst[3]=temp[0]^temp[5]^temp[7]^temp[10]^temp[11]^temp[13]^temp[14];

    dst[4]=temp[0]^temp[2]^temp[5]^temp[8]^temp[11]^temp[14]^temp[15];
    dst[5]=temp[1]^temp[3]^temp[4]^temp[9]^temp[10]^temp[14]^temp[15];
    dst[6]=temp[0]^temp[2]^temp[7]^temp[9]^temp[10]^temp[12]^temp[13];
    dst[7]=temp[1]^temp[3]^temp[6]^temp[8]^temp[11]^temp[12]^temp[13];

    dst[8]=temp[0]^temp[1]^temp[4]^temp[7]^temp[10]^temp[13]^temp[15];
    dst[9]=temp[0]^temp[1]^temp[5]^temp[6]^temp[11]^temp[12]^temp[14];
    dst[10]=temp[2]^temp[3]^temp[5]^temp[6]^temp[8]^temp[13]^temp[15];
    dst[11]=temp[2]^temp[3]^temp[4]^temp[7]^temp[9]^temp[12]^temp[14];
	
    dst[12]=temp[1]^temp[2]^temp[6]^temp[7]^temp[9]^temp[11]^temp[12];
    dst[13]=temp[0]^temp[3]^temp[6]^temp[7]^temp[8]^temp[10]^temp[13];
    dst[14]=temp[0]^temp[3]^temp[4]^temp[5]^temp[9]^temp[11]^temp[14];
    dst[15]=temp[1]^temp[2]^temp[4]^temp[5]^temp[8]^temp[10]^temp[15];
        
}

/************************************************ 
 * Bits rotations function for key expansion 
 * 
 * Pseudo code of rotatuion function
    q <- bits / 8
    r <- bits % 8
    for i 0 to 15
        output[(q + i) % 16] <- output[(q + i) % 16] ^ (input >> r)
        if ( r != 0 )
            output[(q + i + 1) % 16] <- output[(q + i + 1) % 16] ^ ((input[i] << (8 - r)) & 0xfe)
 ************************************************/
void rot(unsigned char input[16], int bits, unsigned char output[16]) {
    int q = bits / 8;
    int r = bits % 8;
    for (int i=0; i<16; i++) {
        output[(q+i) % 16] ^= input[i] >> r;
        if ( r != 0 )
            output[(q+i+1) % 16] ^= (input[i] << (8-r) & 0xfe);
    }
}

/* Key expansion function for generate round key or whitening key */
void keyexpansion(unsigned char *mk, unsigned char (*enc_w)[16], unsigned char (*dec_w)[16], int round) { // mk is 16/24/32 bytes for ARIA-128/192/256
    
    /* Key expansion initial part (ARIA128/ ARIA192/ ARIA256) */ 
    // Feistal 암호의 입력인 KL, KR은 mk||0..0 으로 적절히 만들것
    // (MK로부터 W0 W1 W2 만들기)
    unsigned char W0[16], W1[16], W2[16], W3[16];             // output of feistal crypto 
    unsigned char KL[16], KR[16], CK1[16], CK2[16], CK3[16];  // input of feistal crypto 
    memset(&W0, 0, sizeof(W0)); memset(&W1, 0, sizeof(W1)); memset(&W2, 0, sizeof(W2)); memset(&W3, 0, sizeof(W3));
    memset(&KL, 0, sizeof(KL)); memset(&KR, 0, sizeof(KR)); memset(&CK1, 0, sizeof(CK1)); memset(&CK2, 0, sizeof(CK2)); memset(&CK3, 0, sizeof(CK3));

    // KL <- mk[0~15] for all case of ARIA-128/192/256
    for (int i=0; i<16; i++) 
        KL[i] = mk[i]; // Allocations KL which is msb-16-bytes of master key
    
    // KR <- mk||0..0 case ARIA-128/192/256
    if ( round == 12 ) {
        for (int i=0; i<16; i++) { // select CK1, CK2, CK3
            CK1[i] = C1[i];
            CK2[i] = C2[i];
            CK3[i] = C3[i];
        }
    }
    else if ( round == 14 ) {      // KR <- mk[16~23] || 0[24~31] 
        for (int i=0; i<8; i++)
            KR[i] = mk[16+i]; 
        // zero value is already initialized
        for (int i=0; i<16; i++) {
            CK1[i] = C2[i];
            CK2[i] = C3[i];
            CK3[i] = C1[i];
        }
    }
    else if ( round == 16 ) {      // KR <- mk[16~31]
        for (int i=0; i<16; i++) {
            KR[i] = mk[16+i];
            CK1[i] = C3[i];
            CK2[i] = C1[i];
            CK3[i] = C2[i];
        }
    }
    else
        return;

    // W0 <- KL
    for (int i=0; i<16; i++)
        W0[i] = KL[i];
    
    // W1 <- fo(W0, CK1) ^ KR
    unsigned char temp0[16];
    memset(&temp0, 0, sizeof(temp0));
    for (int i=0; i<16; i++)
        temp0[i] = W0[i];
    fo(temp0, CK1);
    for (int i=0; i<16; i++)
        W1[i] = temp0[i] ^ KR[i];

    // W2 <- fe(W1, CK2) ^ W0
    memset(&temp0, 0, sizeof(temp0));
    for (int i=0; i<16; i++)
        temp0[i] = W1[i];
    fe(temp0, CK2);
    for (int i=0; i<16; i++)
        W2[i] = temp0[i] ^ W0[i];

    // W3 <- fo(W2, CK3) ^ W1
    memset(temp0, 0, sizeof(temp0));
    for (int i=0; i<16; i++)
        temp0[i] = W2[i];
    fo(temp0, CK3);
    for (int i=0; i<16; i++)
        W3[i] = temp0[i] ^ W1[i];

    /* Round key generation part (ARIA-128/192/256 : 12/14/16 round, 16 bytes) */
    unsigned char rotW0[16], rotW1[16], rotW2[16], rotW3[16];
    for (int i=0; i<REPLY; i++) {
        memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));

        // Generate encrypt round key
        rot(W0, 19, rotW0); rot(W1, 19, rotW1); rot(W2, 19, rotW2); rot(W3, 19, rotW3); // rotation >>>19 W0,W1,W2,W3
        for (int i=0; i<16; i++) {
            enc_w[0][i] = W0[i] ^ rotW1[i];      // ek1 <- W0 ^ (W1 >>> 19)
            enc_w[1][i] = W1[i] ^ rotW2[i];      // ek2 <- W1 ^ (W2 >>> 19)
            enc_w[2][i] = W2[i] ^ rotW3[i];      // ek3 <- W2 ^ (W3 >>> 19)
            enc_w[3][i] = W3[i] ^ rotW0[i];      // ek4 <- (W0 >>> 19) ^ W3
        }

        memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));
        rot(W0, 31, rotW0); rot(W1, 31, rotW1); rot(W2, 31, rotW2); rot(W3, 31, rotW3); // rotation >>>31 W0,W1,W2,W3
        for (int i=0; i<16; i++) {
            enc_w[4][i] = W0[i] ^ rotW1[i];      // ek5 <- W0 ^ (W1>>>31) 
            enc_w[5][i] = W1[i] ^ rotW2[i];      // ek6 <- W1 ^ (W2>>>31)
            enc_w[6][i] = W2[i] ^ rotW3[i];      // ek7 <- W2 ^ (W3>>>31)
            enc_w[7][i] = W3[i] ^ rotW0[i];      // ek8 <- (W0>>>31) ^ W3
        }

        memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));
        rot(W0, 67, rotW0); rot(W1, 67, rotW1); rot(W2, 67, rotW2); rot(W3, 67, rotW3); // rotation <<<61 W0,W1,W2,W3
        for (int i=0; i<16; i++) {
            enc_w[8][i] = W0[i] ^ rotW1[i];      // ek9  <- W0 ^ (W1<<<61) 
            enc_w[9][i] = W1[i] ^ rotW2[i];      // ek10 <- W1 ^ (W2<<<61)
            enc_w[10][i] = W2[i] ^ rotW3[i];     // ek11 <- W2 ^ (W3<<<61)
            enc_w[11][i] = W3[i] ^ rotW0[i];     // ek12 <- (W0<<<61) ^ W3
        }
    }








    // Generate encrypt round key or whitening key for ARIA128/192/256
    memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));
    rot(W1, 97, rotW1);
    for (int i=0; i<16; i++)
        enc_w[12][i] = W0[i] ^ rotW1[i];     // ek13 <- W0 ^ (W1<<<31) (ARIA-128 whiteningkey)
    if ( (round == 14) || (round == 16) ) {
        rot(W2, 97, rotW2);
        rot(W3, 97, rotW3);
        for (int i=0; i<16; i++) {
            enc_w[13][i] = W1[i] ^ rotW2[i]; // ek14 <- W1 ^ (W2<<<31)
            enc_w[14][i] = W2[i] ^ rotW3[i]; // ek15 <- W2 ^ (W3<<<31) (ARIA-192 whiteningkey)
        }
    }
    if ( round == 16 ) {
        memset(&rotW1, 0, sizeof(rotW1));
        rot(W0, 97, rotW0);
        rot(W1, 109, rotW1);
        for (int i=0; i<16; i++) {
            enc_w[15][i] = W3[i] ^ rotW0[i]; // ek16 <- (W0<<<31) ^ W3
            enc_w[16][i] = W0[i] ^ rotW1[i]; // ek17 <- W0 ^ (W1<<<19) (ARIA-256 whiteningkey)
        }
    }

    //  Generate decrypt round key (Parallel for dec_w[0, 1, 2, ...])
    for (int i=0; i<16; i++) {
        dec_w[0][i] = enc_w[round][i];       // dec_w[0] <- enc_w[round]
        dec_w[round][i] = enc_w[0][i];       // dec_w[round] <- enc_w[0];
    }
    for (int r=1; r<=round-1; r++) {
        difflayer(enc_w[round-r], dec_w[r]); // dec_w[r] <- A(enc_w[round-r]) for r 1 to round-1
    }
}

unsigned char enc_temp_w[12][16];

/* Key expansion - parallel implementation */
void parallel_keyexpansion(unsigned char *mk, unsigned char (*enc_w)[16], unsigned char (*dec_w)[16], int round) { // mk is 16/24/32 bytes for ARIA-128/192/256

    /* Key expansion initial part (ARIA128/ ARIA192/ ARIA256) */ 
    // Feistal 암호의 입력인 KL, KR은 mk||0..0 으로 적절히 만들것
    // (MK로부터 W0 W1 W2 만들기)
    unsigned char W[16*4];
    unsigned char *W0, *W1, *W2, *W3;             // output of feistal crypto 
    unsigned char KL[16], KR[16], CK1[16], CK2[16], CK3[16];  // input of feistal crypto 
    memset(&W, 0, sizeof(W));
    memset(&KL, 0, sizeof(KL)); memset(&KR, 0, sizeof(KR)); memset(&CK1, 0, sizeof(CK1)); memset(&CK2, 0, sizeof(CK2)); memset(&CK3, 0, sizeof(CK3));
    W0 = W; W1 = W + 16; W2 = W + 32; W3 = W + 48;

    // KL <- mk[0~15] for all case of ARIA-128/192/256
    for (int i=0; i<16; i++) 
        KL[i] = mk[i]; // Allocations KL which is msb-16-bytes of master key
    
    // KR <- mk||0..0 case ARIA-128/192/256
    if ( round == 12 ) {
        for (int i=0; i<16; i++) { // select CK1, CK2, CK3
            CK1[i] = C1[i];
            CK2[i] = C2[i];
            CK3[i] = C3[i];
        }
    }
    else if ( round == 14 ) {      // KR <- mk[16~23] || 0[24~31] 
        for (int i=0; i<8; i++)
            KR[i] = mk[16+i]; 
        // zero value is already initialized
        for (int i=0; i<16; i++) {
            CK1[i] = C2[i];
            CK2[i] = C3[i];
            CK3[i] = C1[i];
        }
    }
    else if ( round == 16 ) {      // KR <- mk[16~31]
        for (int i=0; i<16; i++) {
            KR[i] = mk[16+i];
            CK1[i] = C3[i];
            CK2[i] = C1[i];
            CK3[i] = C2[i];
        }
    }
    else
        return;

    // W0 <- KL
    for (int i=0; i<16; i++)
        W0[i] = KL[i];
    
    // W1 <- fo(W0, CK1) ^ KR
    unsigned char temp0[16];
    memset(&temp0, 0, sizeof(temp0));
    for (int i=0; i<16; i++)
        temp0[i] = W0[i];
    fo(temp0, CK1);
    for (int i=0; i<16; i++)
        W1[i] = temp0[i] ^ KR[i];

    // W2 <- fe(W1, CK2) ^ W0
    memset(&temp0, 0, sizeof(temp0));
    for (int i=0; i<16; i++)
        temp0[i] = W1[i];
    fe(temp0, CK2);
    for (int i=0; i<16; i++)
        W2[i] = temp0[i] ^ W0[i];

    // W3 <- fo(W2, CK3) ^ W1
    memset(temp0, 0, sizeof(temp0));
    for (int i=0; i<16; i++)
        temp0[i] = W2[i];
    fo(temp0, CK3);
    for (int i=0; i<16; i++)
        W3[i] = temp0[i] ^ W1[i];



    // Thread (23.2.9 발표)
    pthread_t t_id[4]; // thread id
    pthread_mutex_init(&mutx, NULL);

    pthread_create(&t_id[0], NULL, key_expansion1, (void*)W);
    pthread_create(&t_id[1], NULL, key_expansion2, (void*)W);
    pthread_create(&t_id[2], NULL, key_expansion3, (void*)W);
    // pthread_detach(t_id[0]);
    // pthread_detach(t_id[1]);
    // pthread_detach(t_id[2]);
    int ret;
    pthread_join(t_id[0], (void *)&ret);
    pthread_join(t_id[1], (void *)&ret);
    pthread_join(t_id[2], (void *)&ret);
    
    // Get 1~12 Round key from thread
    for (int i=0; i<12; i++)
        memcpy(enc_w[i], enc_temp_w[i], 16);
        

    // Generate encrypt round key or whitening key for ARIA128/192/256
    unsigned char rotW0[16], rotW1[16], rotW2[16], rotW3[16];
    memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));
    rot(W1, 97, rotW1);
    for (int i=0; i<16; i++)
        enc_w[12][i] = W0[i] ^ rotW1[i];     // ek13 <- W0 ^ (W1<<<31) (ARIA-128 whiteningkey)
    if ( (round == 14) || (round == 16) ) {
        rot(W2, 97, rotW2);
        rot(W3, 97, rotW3);
        for (int i=0; i<16; i++) {
            enc_w[13][i] = W1[i] ^ rotW2[i]; // ek14 <- W1 ^ (W2<<<31)
            enc_w[14][i] = W2[i] ^ rotW3[i]; // ek15 <- W2 ^ (W3<<<31) (ARIA-192 whiteningkey)
        }
    }
    if ( round == 16 ) {
        memset(&rotW1, 0, sizeof(rotW1));
        rot(W0, 97, rotW0);
        rot(W1, 109, rotW1);
        for (int i=0; i<16; i++) {
            enc_w[15][i] = W3[i] ^ rotW0[i]; // ek16 <- (W0<<<31) ^ W3
            enc_w[16][i] = W0[i] ^ rotW1[i]; // ek17 <- W0 ^ (W1<<<19) (ARIA-256 whiteningkey)
        }
    }

    //  Generate decrypt round key (Parallel for dec_w[0, 1, 2, ...])
    for (int i=0; i<16; i++) {
        dec_w[0][i] = enc_w[round][i];       // dec_w[0] <- enc_w[round]
        dec_w[round][i] = enc_w[0][i];       // dec_w[round] <- enc_w[0];
    }
    for (int r=1; r<=round-1; r++) {
        difflayer(enc_w[round-r], dec_w[r]); // dec_w[r] <- A(enc_w[round-r]) for r 1 to round-1
    }


}

// Generate round key[0~3]
void *key_expansion1(void *arg) {

    unsigned char *W = (unsigned char *) arg;
    unsigned char *W0, *W1, *W2, *W3;
    W0 = W;
    W1 = W + 16;
    W2 = W + 32;
    W3 = W + 48;

    /* Round key generation part */
    unsigned char rotW0[16], rotW1[16], rotW2[16], rotW3[16];
        




    for (int i=0; i<REPLY; i++) {

        memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));

        // Generate encrypt round key
        rot(W0, 19, rotW0); rot(W1, 19, rotW1); rot(W2, 19, rotW2); rot(W3, 19, rotW3); // rotation >>>19 W0,W1,W2,W3
        for (int i=0; i<16; i++) {
            enc_temp_w[0][i] = W0[i] ^ rotW1[i];      // ek1 <- W0 ^ (W1 >>> 19)
            enc_temp_w[1][i] = W1[i] ^ rotW2[i];      // ek2 <- W1 ^ (W2 >>> 19)
            enc_temp_w[2][i] = W2[i] ^ rotW3[i];      // ek3 <- W2 ^ (W3 >>> 19)
            enc_temp_w[3][i] = W3[i] ^ rotW0[i];      // ek4 <- (W0 >>> 19) ^ W3
        }
    }





    int *ret = SUCCESS;
    return (void*) ret;
}

// Generate round key[4~7]
void *key_expansion2(void *arg) {
    unsigned char *W = (unsigned char *) arg;
    unsigned char *W0, *W1, *W2, *W3;
    W0 = W;
    W1 = W + 16;
    W2 = W + 32;
    W3 = W + 48;




    for (int i=0; i<REPLY; i++) {

        /* Round key generation part */
        unsigned char rotW0[16], rotW1[16], rotW2[16], rotW3[16];
        memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));

        // Generate encrypt round key
        rot(W0, 31, rotW0); rot(W1, 31, rotW1); rot(W2, 31, rotW2); rot(W3, 31, rotW3); // rotation >>>31 W0,W1,W2,W3
        for (int i=0; i<16; i++) {
            enc_temp_w[4][i] = W0[i] ^ rotW1[i];      // ek5 <- W0 ^ (W1>>>31) 
            enc_temp_w[5][i] = W1[i] ^ rotW2[i];      // ek6 <- W1 ^ (W2>>>31)
            enc_temp_w[6][i] = W2[i] ^ rotW3[i];      // ek7 <- W2 ^ (W3>>>31)
            enc_temp_w[7][i] = W3[i] ^ rotW0[i];      // ek8 <- (W0>>>31) ^ W3
        }
    }





    int *ret = SUCCESS;
    return (void*) ret;
}

// Generate round key[8~11]
void *key_expansion3(void *arg) {

    unsigned char *W = (unsigned char *) arg;
    unsigned char *W0, *W1, *W2, *W3;
    W0 = W;
    W1 = W + 16;
    W2 = W + 32;
    W3 = W + 48;
    unsigned char rotW0[16], rotW1[16], rotW2[16], rotW3[16];





    for (int i=0; i<REPLY; i++) {

        /* Round key generation part (ARIA-128/192/256 : 12/14/16 round, 16 bytes) */
        memset(&rotW0, 0, sizeof(rotW0)); memset(&rotW1, 0, sizeof(rotW1)); memset(&rotW2, 0, sizeof(rotW2)); memset(&rotW3, 0, sizeof(rotW3));

        // Generate encrypt round key
        rot(W0, 67, rotW0); rot(W1, 67, rotW1); rot(W2, 67, rotW2); rot(W3, 67, rotW3); // rotation <<<61 W0,W1,W2,W3
        for (int i=0; i<16; i++) {
            enc_temp_w[8][i]  = W0[i] ^ rotW1[i];      // ek9  <- W0 ^ (W1<<<61) 
            enc_temp_w[9][i]  = W1[i] ^ rotW2[i];      // ek10 <- W1 ^ (W2<<<61)
            enc_temp_w[10][i] = W2[i] ^ rotW3[i];     // ek11 <- W2 ^ (W3<<<61)
            enc_temp_w[11][i] = W3[i] ^ rotW0[i];     // ek12 <- (W0<<<61) ^ W3
        }
    }




    int *ret = SUCCESS;
    return (void*) ret;
}


/* ARIA-128/192/256 Encrypt function */
void aria_enc(unsigned char plaintext[16], unsigned char ciphertext[16], unsigned char (*w)[16], int round) {

    unsigned char state[16] = { 0x00, };
    for (int i=0; i<16; i++)
        state[i] = plaintext[i];
    
    // 1 ~ 11/13/15 round (ARIA-128/192/256)
    for (int r=1; r<round; r++) {
        if ( (r%2) == 1 )
            fo(state, w[r-1]);
        else
            fe(state, w[r-1]);
    }

    // Final round : 12/14/16 (ARIA-128/192/256)
    ff(state, w[round-1], w[round]);

    // deep copy
    for (int i=0; i<16; i++)
        ciphertext[i] = state[i];
}

/* ARIA-128/192/256 Decrypt function */
void aria_dec(unsigned char ciphertext[16], unsigned char recovered[16], unsigned char (*w)[16], int round) {

    unsigned char state[16] = { 0x00, };
    for (int i=0; i<16; i++)
        state[i] = ciphertext[i];
    
    // 1 ~ 11/13/15 round (ARIA-128/192/256)
    for (int r=1; r<round; r++) {
        if ( (r%2) == 1 )
            fo(state, w[r-1]);
        else 
            fe(state, w[r-1]);
    }

    // Final round : 12/14/16 (ARIA-128/192/256)
    ff(state, w[round-1], w[round]);

    // deep copy
    for (int i=0; i<16; i++)
        recovered[i] = state[i];

}

/* Verification of plaintext versus recovered */
int verify(unsigned char plaintext[16], unsigned char recovered[16]) {
    int ret;
    ret = memcmp(plaintext, recovered, 16);
    return ret;
}

/* Print list */
void printstate(char *msg, unsigned char *state, int bytelen) {
    printf("%s ", msg);
    for (int i=0; i<bytelen; i++) {
        printf("%02x ", state[i]);
    }
    printf("\n");
}

// EOF