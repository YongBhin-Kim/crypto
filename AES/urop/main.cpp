/*
    Chow's Whitebox AES encryption test
        - encrypt data with WBAES and decrypt with AES
        - performance benchmark (speed, look-up counts)
*/

#include <iostream>
#include <fstream>
#include <iomanip>

#include "aes.h"
#include "wbaes.h"
#include "wbaes_tables.h"
#include "debug.h"


int main(void) {
    // uint8_t   u8_aes_key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    uint8_t   u8_aes_key[16] = {0x00,};
    for ( int i=0; i<16; i++ ) {
        u8_aes_key[i] = 0xff;
    }

    uint32_t  u32_round_key[11][4], u32_inv_round_key[11][4];
    
    WBAES_ENCRYPTION_TABLE *et = new WBAES_ENCRYPTION_TABLE();


    AES32_Enc_KeySchedule(u8_aes_key, u32_round_key);
    WBAES_NONLINEAR_ENCODING *en = new WBAES_NONLINEAR_ENCODING();
    gen_encryption_table(*et, *en, (uint32_t *)u32_round_key);


    et->write("et.bin");
    
    // uint8_t pt1[16] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a}, 
    uint8_t pt1[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
            pt2[16]{}, ct[16]{}, recovered[16]{};
    memcpy(pt2, pt1, 16);

    printf("PT: \n");
    dump_bytes(pt1 , 16);
    std::cout << std::endl;
    printf("KEY: \n");
    dump_bytes(u8_aes_key , 16);
    std::cout << std::endl;

    uint8_t aesavs_kat_ct[16] = {0xa1, 0xf6, 0x25, 0x8c, 0x87, 0x7d, 0x5f, 0xcd, 0x89, 0x64, 0x48, 0x45, 0x38, 0xbf, 0xc9, 0x2c};

    encode_ext_x(en->ext_f, pt2);
    
    std::cout << std::endl;
    printf("AES32 INPUT: \n");
    dump_bytes(pt1 , 16);
    
    AES32_Encrypt(pt1, u32_round_key, ct);
    printf("AES32 OUTPUT: \n");
    dump_bytes(ct , 16);
    std::cout << std::endl;

    printf("WBAES INPUT: \n");
    dump_bytes(pt2 , 16);
    wbaes_encrypt(*et, pt2);
    printf("WBAES OUTPUT: \n");
    dump_bytes(pt2, 16);


    if ( !std::memcmp(ct, pt2, sizeof(ct)) )
        std::cout << "(AES vs WBAES ENCRYPT TEST : SUCCESS)" << std::endl;
    else {
        std::cout << "(AES vs WBAES ENCRYPT TEST : FAIL)" << std::endl;
    }
    if ( !std::memcmp(aesavs_kat_ct, pt2, sizeof(aesavs_kat_ct)) )
        std::cout << "(WBAES KAT TEST : SUCCESS)" << std::endl;
    else {
        std::cout << "(WBAES KAT TEST : FAIL)" << std::endl;
    }

    std::cout << std::endl;
    AES32_Dec_KeySchedule(u8_aes_key, u32_inv_round_key);
    AES32_EqDecrypt(pt2, u32_inv_round_key, recovered);
    printf("WBAES DECRYPT OUTPUT: \n");
    dump_bytes(recovered, 16);

    if ( !std::memcmp(pt1, recovered, sizeof(pt1)) )
        std::cout << "(WBAES DECRYPT TEST : SUCCESS)" << std::endl;
    else {
        std::cout << "(WBAES DECRYPT TEST : FAIL)" << std::endl;
    }
    std::cout << std::endl;
    delete et;
    delete en;
    return 0;
}
