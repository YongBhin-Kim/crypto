/*************************************************
*   Monte Carlo Test - ECB
*   (AESAVS - 7 page)
**************************************************/

#include "ECBMCT.h"

/*************************************************
 * req
    KEY       = 8d2e60365f17c7df1040d7501b4a7b5a
    PLAINTEXT = 59b5088e6dadc3ad5f27a460872d5929

 * pseudo code
    Key[0] = Key 
    PT[0] = PT
    For i = 0 to 99
        Output Key[i]
        Output PT[0]
        For j = 0 to 999
            CT[j] = AES(Key[i], PT[j])
            PT[j+1] = CT[j]
        Output CT[j]
        If ( keylen = 128 )
            Key[i+1] = Key[i] xor CT[j]
        PT[0] = CT[j]

 * rsp
    KEY        = 8d2e60365f17c7df1040d7501b4a7b5a 
    PLAINTEXT  = 59b5088e6dadc3ad5f27a460872d5929 
    CIPHERTEXT = a02600ecb8ea77625bba6641ed5f5920
    KEY        = 2d0860dae7fdb0bd4bfab111f615227a 
    PLAINTEXT  = a02600ecb8ea77625bba6641ed5f5920 
    CIPHERTEXT = 5241ead9a89ca31a7147f53a5bf6d96a
    KEY        = 7f498a034f6113a73abd442bade3fb10 
    PLAINTEXT  = 5241ead9a89ca31a7147f53a5bf6d96a 
    CIPHERTEXT = 22f09171bc67d0661d1c25f181a69f33
*************************************************/
void ECBMCT() {
    const char* hex_key = "8d2e60365f17c7df1040d7501b4a7b5a";
    const char* hex_pt  = "59b5088e6dadc3ad5f27a460872d5929";

    // 암호화를 위한 바이트 배열
    byte key[16], pt[16], ct[16];

    cout << "MCT-ECB Test" << endl;

    Hex2Array(hex_key, 32, key);            // Key[0] <- Key
    Hex2Array(hex_pt, 32, pt);              // PT[0]  <- PT

    for (int i=0; i<3; i++) {               // For i = 0 to 99
        print_b_array(key, 16, "KEY");      // Output Key[i]
        print_b_array(pt, 16, "PT ");       // Output PT[0]
        for (int j=0; j<1000; j++) {        // For j = 0 to 999
            AES32_Enc(pt, key, ct);         // CT[j] <- AES(Key[i], PT[j])
            copy_b_array(ct, 16, pt);       // PT[j+1] <- CT[j]
        }                                   // End For
        print_b_array(ct, 16, "CT ");
        xor_b_array(key, 16, ct);           // Key[i+1] <- Key[i] xor CT[j]
        cout << endl;
    }                                       // End For
    copy_b_array(ct, 16, pt);               // PT[0] <- CT[j]


}
int main() {
    ECBMCT();
}