#include <stdio.h>
#include <string.h> 
#include "../../ARIA-C/aria.h"
#include "../Util/util.h"
#include "aria_ctr.h"



int main() {
    // ARIA128(CTR) test vector
    int round = 12;
	const char* hex_key = "00000000000000000000000000000000"; //"2b7e151628aed2a6abf7158809cf4f3c";
	const char* hex_iv = "80000000000000000000000000000000"; //"f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";
	const char* hex_pt = "00000000000000000000000000000000"; //"6bc1bee22e409f96e93d7e117393172a";
	const char* hex_ct = "92E51E737DABB6BFD0EABC8D32224F77";// "874d6191b620e3261bef6864990db6ce";

    int pt_len;
    pt_len = strlen(hex_pt);

	unsigned char key[16], iv[16], pt[16], ct[16], ctr[16];
    

	byte_fromhex(key, hex_key, 32);
	byte_fromhex(iv, hex_iv, 32);
	byte_fromhex(pt, hex_pt, 32);
    printstate("plaintext = ", pt, 16);

    copy_b_array(iv, 16, ctr); // iv -> ctr


    // enc
    aria_ctr(pt, pt_len, key, ctr, ct, round);


    // dec
    unsigned char recovered[16] = { 0x00, };

    aria_ctr(ct, pt_len, key, ctr, recovered, round);


    printstate("ciphertext = ", ct, 16);
    printstate("recovered = ", recovered, 16);

    return 0;
}

// EOF