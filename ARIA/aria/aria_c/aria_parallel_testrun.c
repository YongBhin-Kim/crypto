/*
    ARIA - C Parallel implementation run test
*/
//  Copyright © Yonhbhin Kim
//  Github url : https://github.com/YongBhin-Kim

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "aria_parallel.h"
// #include "aria.h"


#define FAIL       -1
#define SUCCESS     0

/* Debug mode */
#define TIMECHECK   1 // If you want to check time, then apply this value 1, else then 0
#define _DEBUG      0 

// #include <time.h>
#include <sys/time.h>
#define KEYEXPANSION_PARALLEL 1 // 1(yes) or 0(no)


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
    memset(&enc_w, 0, sizeof(enc_w)); 
    memset(&dec_w, 0, sizeof(dec_w));


#if (TIMECHECK == 1)
    // Time check
    struct timeval  tv;
	double start, end;

    // Parallel key expansion
	gettimeofday(&tv, NULL); start = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
    parallel_keyexpansion(mk, enc_w, dec_w, round);
    gettimeofday(&tv, NULL); end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; float ptime = (end - start) / 1000;

    // Normal key expansion
	gettimeofday(&tv, NULL); start = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
    keyexpansion(mk, enc_w, dec_w, round);
    gettimeofday(&tv, NULL); end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; float ntime = (end - start) / 1000;

    // Parallel versus normal
    printf(" * Parallel keyexpansion time (%d repetitions) : %.3f\n", REPLY, ptime);
    printf(" * Normal   keyexpansion time (%d repetitions) : %.3f\n", REPLY, ntime);


#else

    parallel_keyexpansion(mk, enc_w, dec_w, round);
#endif



    /* ARIA-128/192/256 Encryption part */
    aria_enc(pt, ct, enc_w, round);

    /* ARIA-128/192/256 Decryption part */
    aria_dec(ct, dec, dec_w, round);

    /* Varifications part */
    if ( verify(pt, dec) != SUCCESS) {
        printf("Failed to recover\n");
        return 0;
    }

#if (_DEBUG == 1)
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


//  * 1 round key  =  8e 3f 60 a1 d7 c8 de ae 5d e8 98 e1 8e 92 db ac 
//  * 2 round key  =  7c da cc 73 57 12 fa 0c 9f 5f 4b cc dc 21 48 e2 
//  * 3 round key  =  bd f9 a4 13 32 f4 77 18 2c ee 5b e2 26 8a 7b 7f 
//  * 4 round key  =  17 4d 37 a1 9a 56 cb 6e 30 9f 91 08 89 a8 09 28 
//  * 5 round key  =  5a 39 e1 e5 2e 28 3a da 82 9c 54 12 22 a5 27 f2 
//  * 6 round key  =  84 00 02 ab e4 93 8a 89 c7 54 94 4b 5e 3b 62 20 
//  * 7 round key  =  c1 3e 43 91 c5 19 ef 19 8d be de 34 e8 35 ae 71 
//  * 8 round key  =  ae 96 cb bf ba 14 af e8 98 55 7c 07 b8 fb 7c bf 
//  * 9 round key  =  92 eb 80 9c d1 a6 88 39 6a ab d9 c6 d4 a4 5b ee 
//  * 10 round key =  4a 24 ef 53 fc 5b c6 c0 c5 49 86 a6 f8 1c 79 f8 
//  * 11 round key =  42 e7 7c c3 9d 1d 6d 4f cf 42 13 1d ff c9 9b 1f 
//  * 12 round key =  57 8d f6 e0 db 97 0a 2f 70 5f 50 49 c8 69 c8 69 
//  * 13 round key =  62 a4 55 85 4f af 0c 78 5e 87 32 f2 86 86 41 38 
//  * 14 round key =  41 16 be 43 b9 83 6b f8 73 11 b3 cf e5 6a 38 92 
//  * 15 round key =  4d e7 c7 35 e0 2f f8 5e 2d e7 3d bd 13 cd 25 b2 
//  * 16 round key =  34 8e 54 a2 3e 12 2e eb 16 59 f7 0e 28 e9 e9 a8 
//  * whiteningkey =  f3 77 28 56 7c 61 bc a7 af fc 62 e8 83 95 a6 bb 

// EOF
