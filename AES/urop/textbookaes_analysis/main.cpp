/*
    Textbook AES encryption test
        - encrypt with AES and decrypt with AES
        - for memory dump! (Analysis of Basic White-Box attack)
*/

#include <iostream>
#include <fstream>
#include <iomanip>

#include "aes.h"
#include "debug.h"


int main(void) {

    uint8_t   u8_aes_key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};


    uint32_t  u32_round_key[11][4], u32_inv_round_key[11][4];

    AES32_Enc_KeySchedule(u8_aes_key, u32_round_key);

    // uint8_t pt[16] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a}, 
    uint8_t pt[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
            ct[16]{}, recovered[16]{};

    printf("PT: \n");
    dump_bytes(pt , 16);
    std::cout << std::endl;
    printf("KEY: \n");
    dump_bytes(u8_aes_key , 16);
    std::cout << std::endl;

    // uint8_t aesavs_kat_ct[16] = {0xa1, 0xf6, 0x25, 0x8c, 0x87, 0x7d, 0x5f, 0xcd, 0x89, 0x64, 0x48, 0x45, 0x38, 0xbf, 0xc9, 0x2c};
    
    std::cout << std::endl;
    printf("AES32 INPUT: \n");
    dump_bytes(pt , 16);
    
    AES32_Encrypt(pt, u32_round_key, ct);
    printf("AES32 OUTPUT: \n");
    dump_bytes(ct , 16);
    std::cout << std::endl;

    std::cout << std::endl;
    AES32_Dec_KeySchedule(u8_aes_key, u32_inv_round_key);
    AES32_EqDecrypt(ct, u32_inv_round_key, recovered);
    printf("AES DECRYPT OUTPUT: \n");
    dump_bytes(recovered, 16);

    if ( !std::memcmp(pt, recovered, sizeof(pt)) )
        std::cout << "(AES DECRYPT TEST : SUCCESS)" << std::endl;
    else {
        std::cout << "(AES DECRYPT TEST : FAIL)" << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
