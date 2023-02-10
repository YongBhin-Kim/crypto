/*
    Block Cipher Mode - GCM(Galois Counter Mode) run test (CIPH is ARIA)
*/
//  Copyright © Yonhbhin Kim
//  Github url : https://github.com/YongBhin-Kim

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "../Util/util.h"
#include "../../ARIA-C/aria.h"
#include "../ARIA-CTR/aria_ctr.h"
#include "aria_gcm.h"
#include "ghash.h"

void aria_gcm_runtest() { // aad (o) pt (x)
	const char* hex_key = "77be63708971c4e240d1cb79e8d77feb";	//128
	const char* hex_iv 	= "e0e00f19fed7ba0136a797f3";			//96
	const char* hex_pt 	= "000000";	//0
	const char* hex_aad = "7a43ec1d9c0a5a78a0b16533a6213cab";	//128
	const char* hex_ct 	= "";	//0	
	// Tag = 209fcc8d3675ed938e9c7166709dd946

	unsigned char key[16], iv[16], pt[16], ct[16], aad[16], tag[16], recovered[16];

	byte_fromhex(key, hex_key, 32);
	byte_fromhex(iv, hex_iv, 24);
	byte_fromhex(pt, hex_pt, strlen(hex_pt));
    byte_fromhex(aad,hex_aad, 32);

	int pt_len = 6;
	int aad_len = 16;


	printf("==========================================================================\n");
	printf("=======================  [ARIA BCM - GCM run test] =======================\n");
	printf("-------------------------------[Prerequests]------------------------------\n");
	printstate("* pt 		= ", pt, pt_len);
	printstate("* key 		= ", key, 16);
	printstate("* iv 		= ", iv, 12);
	printstate("* aad 		= ", aad, aad_len);

	printf("----------------------------[Encryption(Alice)]---------------------------\n");
	aria_gcm_enc(pt, pt_len, iv, key, aad, aad_len, ct, tag);

    printstate("* ct 		= ", ct, 16); // send to Bob
    printstate("* tag 		= ", tag, 16); // send to Bob
    printf("==========================================================================\n\n");

    memset(&recovered, 0, sizeof(recovered));
	int ct_len = 6; // received from Alice
	printf("----------------------------[Decryption(Bob)]-----------------------------\n");
	int ret;
    ret = aria_gcm_dec(ct, ct_len, iv, key, aad, aad_len, recovered, tag );
	if ( ret == FAIL ) {
		printf(" Failed to recovered, exit process..\n");
		return;
	}

	printstate("* recovered     = ", recovered, ct_len);
	printf("==========================================================================\n");


}

// main function 
int main() {
    aria_gcm_runtest();
    
    return 0;
}

// EOF