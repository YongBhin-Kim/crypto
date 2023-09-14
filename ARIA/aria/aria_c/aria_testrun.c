/*
    ARIA - C Implementation run test
*/
//  Copyright © Yonhbhin Kim
//  Github url : https://github.com/YongBhin-Kim

#include <stdio.h>
#include <string.h>
#include "aria.h"

#define FAIL   -1
#define SUCCESS 0
#define _DEBUG  1

// main function
int main() {

    /* Test vector for ARIA-128/192/256 */
    unsigned char pt[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    unsigned char mk[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    // unsigned char mk[24] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
    // unsigned char mk[32] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
    unsigned char ct[16] = { 0x00, };
    unsigned char dec[16] = { 0x00, };

    // ARIA-bits
    int bits = sizeof(mk)*8;

    /* Key expansion part */
    // Key length must be 128/192/256
    int round;
    if ( bits == 128 )
        round = 12;
    else if ( bits == 192 )
        round = 14;
    else if ( bits == 256 )
        round = 16;
    else {
        printf("Key length must be 128/192/256bits \n");
        return 0;
    }
        
    
    unsigned char enc_w[round+1][16], dec_w[round+1][16];
    memset(&enc_w, 0, sizeof(enc_w)); // 2차원 배열의 초기화가 이게 맞는지? 
    memset(&dec_w, 0, sizeof(dec_w));
    keyexpansion(mk, enc_w, dec_w, round);

    /* ARIA-128/192/256 Encryption part */
    aria_enc(pt, ct, enc_w, round);

    /* ARIA-128/192/256 Decryption part */
    aria_dec(ct, dec, dec_w, round);

    /* Varifications part */
    if ( verify(pt, dec) != SUCCESS) {
        printf("Failed to recover\n");
        return 0;
    }


#ifdef _DEBUG
    printf("\n ======================================================================================\n");
    printf(" * Verification success \n");
    printf(" ======================================================================================\n");
    printf("                                     Encrypt key                                       \n");
    printf(" --------------------------------------------------------------------------------------\n");
    for (int r=0; r<=round-1; r++) {
        printf(" * %d ", r+1);
        printstate("round key = ", enc_w[r], 16);
    }
    printstate(" * whiteningkey = ", enc_w[round], 16);

    printf("\n ======================================================================================\n");
    printf("                                     Decrypt key                                       \n");
    printf(" --------------------------------------------------------------------------------------\n");
    for (int r=0; r<=round-1; r++) {
        printf(" * %d ", r+1);
        printstate("round key = ", dec_w[r], 16);
    }
    printstate(" * whiteningkey = ", dec_w[round], 16);


    printf(" ======================================================================================\n");
    printf("                                      Test Vector                                      \n");
    printf(" --------------------------------------------------------------------------------------\n");
    printf(" [ ARIA-%d ]\n", bits);
    printstate(" * plaintext  = ", pt, 16);
    printstate(" * masterkey  = ", mk, sizeof(mk));
    printstate(" * ciphertext = ", ct, 16);

    printf(" --------------------------------------------------------------------------------------\n");
    printf(" [ ARIA-%d ]\n", bits);
    printstate(" * ciphertext = ", ct, 16);
    printstate(" * masterkey  = ", mk, 16);
    printstate(" * recovered  = ", dec, 16);
    printf(" ======================================================================================\n");
#else
#endif

    return 0;
}


// EOF