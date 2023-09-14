
/*************************************************
* Monte Carlo Test - CBC
* (AESAVS - 8 page)
**************************************************/

#include "CBCMCT.h"
/*************************************************
 * req
    KEY       = 9dc2c84a37850c11699818605f47958c // secret key
    IV        = 256953b2feab2a04ae0180d8335bbed6 // public parameter
    PLAINTEXT = 2e586692e647f5028ec6fa47a55a2aab // secret parameter

 * pseudo code
    Key[0] = Key 
    IV[0] = IV 
    PT[0] = PT
    For i = 0 to 99
        Output Key[i]
        Output IV[i]
        Output PT[0]
        For j = 0 to 999
            If ( j=0 )
                CT[j] = AES(Key[i], IV[i], PT[j])
                PT[j+1] = IV[i]         // MMT에서만 적용 : PT 설정
            Else
                CT[j] = AES(Key[i], PT[j])
                PT[j+1] = CT[j-1]       // MMT에서만 적용 : PT 설정
        Output CT[j]
        If ( keylen = 128 )
            Key[i+1] = Key[i] xor CT[j]
        IV[i+1] = CT[j]
        PT[0] = CT[j-1]

 * rsp
    KEY         = 9dc2c84a37850c11699818605f47958c
    IV          = 256953b2feab2a04ae0180d8335bbed6 
    PLAINTEXT   = 2e586692e647f5028ec6fa47a55a2aab 
    CIPHERTEXT  = 1b1ebd1fc45ec43037fd4844241a437f
    KEY         = 86dc7555f3dbc8215e6550247b5dd6f3
    IV          = 1b1ebd1fc45ec43037fd4844241a437f 
    PLAINTEXT   = c1b77ed52521525f0a4ba341bdaf51d9 
    CIPHERTEXT  = bf43583a665fa45fdee831243a16ea8f
    KEY         = 399f2d6f95846c7e808d6100414b3c7c
    IV          = bf43583a665fa45fdee831243a16ea8f 
    PLAINTEXT   = 7cbeea19157ec7bbf6289e2dff5e8ee4 
    CIPHERTEXT  = 5464e1900f81e06f67139456da25fc09
*************************************************/
void CBCMCT() {
    const char* hex_key   = "9dc2c84a37850c11699818605f47958c";
    const char* hex_iv    = "256953b2feab2a04ae0180d8335bbed6";
    const char* hex_pt    = "2e586692e647f5028ec6fa47a55a2aab";

    byte key[16], iv[16], pt[16], ct[16], prev_ct[16];

    cout << "MCT-CBC Test" << endl;
    Hex2Array(hex_key, 32, key);                             // Key[0] <- Key 
    Hex2Array(hex_iv, 32, iv);                               // IV[0] <- IV 
    Hex2Array(hex_pt, 32, pt);                               // PT[0] <- PT 

    for (int i=0; i<3; i++) {
        print_b_array(key, 16, "KEY");                       // Output Key[i]
        print_b_array(iv, 16, "IV ");                        // Output IV[i]
        print_b_array(pt, 16, "PT ");                        // Output PT[0]
        for (int j=0; j<1000; j++) {                         // For j = 0 to 999
            if (j==0) {                                      // If ( j=0 )
                xor_b_array(pt, 16, iv);                     // CT[j] = AES(Key[i], IV[i], PT[j])
                AES32_Enc(pt, key, ct);                      // CT[j] = AES(Key[i], IV[i], PT[j])
                copy_b_array(iv, 16, pt);                    // PT[j+1] <- IV[i]
            }
            else {                                           // Else
                copy_b_array(ct, 16, prev_ct);
                AES32_Enc(pt, key, ct);                      // CT[j] = AES(Key[i], PT[j])
                copy_b_array(prev_ct, 16, pt);               // PT[j+1] = CT[j-1]
            }                                                // End Else
        }                                                    // End For
        print_b_array(ct, 16, "CT ");                        // Output CT[j]
        xor_b_array(key, 16, ct);                            // Key[i+1] = Key[i] xor CT[j]
        copy_b_array(ct, 16, iv);                            // IV[i+1] = CT[j]
        copy_b_array(prev_ct, 16, pt);                       // PT[0] = CT[j-1]

        cout << endl;
    }

}

int main() {
    CBCMCT();

    return 0;
}