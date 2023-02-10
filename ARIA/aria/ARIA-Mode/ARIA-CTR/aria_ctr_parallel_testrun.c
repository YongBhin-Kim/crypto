#include <stdio.h>
#include <string.h> 
#include "../../ARIA-C/aria.h"
#include "../Util/util.h"
#include "aria_ctr_parallel.h"
#include "aria_ctr.h"

#include <unistd.h>


#define MAX_ROUND   16
#define MAX_PTSIZE  256

int main() {
    // ARIA128(CTR) test vector
    int round = 12;
	const char* hex_key = "2b7e151628aed2a6abf7158809cf4f3c"; //"2b7e151628aed2a6abf7158809cf4f3c";
	const char* hex_iv  = "80CCCC00000000000000000000000000"; //"f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";
	// const char* hex_pt  = "00112233445566778899AABBCCDDEEFF0011223300112233445566778899AABBCCDDEEFF0011223300112233445566778899AABBCCDDEEFF0011223300112233445566778899AABBCCDDEEFF00112233";
    const char* hex_pt  = "000000000000000111111111111122222222222333333333333444444444445555555555555555566666666666677777777777778888888888888888899999999999999AAAAAAAAAAAABBBBBBBBBBBBCCCCCCCCCCCC";
	const char* hex_ct  = "92E51E737DABB6BFD0EABC8D32224F77";// "874d6191b620e3261bef6864990db6ce";

    int pt_len;
    pt_len = strlen(hex_pt)/2;

	unsigned char key[16], iv[16], pt[MAX_PTSIZE], ct[MAX_PTSIZE], ctr[16];
    memset(&key, 0, sizeof(key));
    memset(&iv, 0, sizeof(iv));
    memset(&pt, 0, sizeof(pt));
    memset(&ct, 0, sizeof(ct));
    memset(&ctr, 0, sizeof(ctr));
    

	byte_fromhex(key, hex_key, 32);
	byte_fromhex(iv, hex_iv, 32);
	byte_fromhex(pt, hex_pt, pt_len*2);

    memcpy(ctr, iv, strlen(hex_iv)/2);

    printf(" \n******************** Create message ********************\n\n");
    printstate(" * Plaintext = ", pt, pt_len); printf("\n");

    // Encryption part
    printf(" ******************** Encryption part ********************\n\n");
    // aria_ctr(pt, pt_len, key, ctr, ct, round);
    aria_ctr_parallel(pt, pt_len, key, ctr, ct, round);
    printf("\n"); printstate(" * Ciphertext = ", ct, pt_len); printf("\n");

    // Decryption part
    printf(" ******************** Decryption part ********************\n\n");
    unsigned char recovered[MAX_PTSIZE] = { 0x00, };

    // aria_ctr(ct, pt_len, key, ctr, recovered, round);
    aria_ctr_parallel(ct, pt_len, key, ctr, recovered, round); 

    
    printf("\n"); printstate(" * Recovered = ", recovered, pt_len); printf("\n");

    return 0;
}

// EOF