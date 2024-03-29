/*
    ARIA - C Parallel implementation
*/
//  Copyright © Yonhbhin Kim
//  Github url : https://github.com/YongBhin-Kim

#ifndef ARIA_PARALLEL_H
#define ARIA_PARALLEL_H

#define FAIL   -1
#define SUCCESS 0

// #define REPLY   7000000
// #define REPLY   1000000
#define REPLY   500000
// #define REPLY   100000
// #define REPLY   10000


void fo(unsigned char state[16], unsigned char key[16]);
void fe(unsigned char state[16], unsigned char key[16]);
void ff(unsigned char state[16], unsigned char key[16], unsigned char whiteningkey[16]);
void difflayer(unsigned char src[16], unsigned char dst[16]);
void rot(unsigned char input[16], int bits, unsigned char output[16]);
void keyexpansion(unsigned char *mk, unsigned char (*enc_w)[16], unsigned char (*dec_w)[16], int round);
void parallel_keyexpansion(unsigned char *mk, unsigned char (*enc_w)[16], unsigned char (*dec_w)[16], int round);
void *key_expansion1(void *arg);
void *key_expansion2(void *arg);
void *key_expansion3(void *arg);
void aria_enc(unsigned char plaintext[16], unsigned char ciphertext[16], unsigned char (*w)[16], int round);
void aria_dec(unsigned char ciphertext[16], unsigned char recovered[16], unsigned char (*w)[16], int round);
int verify(unsigned char plaintext[16], unsigned char recoveredy[16]);
void printstate(char *msg, unsigned char *state, int bytelen);


#endif