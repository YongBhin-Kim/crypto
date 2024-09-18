#include "SHA256_C/KISA_SHA256.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main() {
    

    BYTE ptstr[32] = "abc";
    UINT ptlen = strlen(ptstr);
    BYTE digest[32] = {0x0, };

    SHA256_Encrpyt(ptstr, ptlen, digest);

    // for (int i=0; i<32; i++)
    //     printf("%x ", digest[i]);

    return 0;
}

// e0 == CH   (x,y,z) 
// e1 == MAJ  (x,y,z)
// e2 == EP0  (x) 
// e3 == EP1  (x) 
// e4 == SIG0 (x)
// e5 == SIG1 (x)