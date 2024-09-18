/*
    Implementation of Chow's Whitebox AES
        - Encrypt on the whiteboxing algorithm
*/

#include <iostream>

#include "wbaes.h"
#include "debug.h"

/*
    A Tutorial on Whitebox AES
            J.A. Muir
     
     - 3.6 pseudo algo.
        state = plaintext
        for r = 1 ... 9
            ShiftRows(state)
            TBoxesTyiTables(state)  ----- ty_boxes   & xor_tables
            XORTables(state)        ----- mbl_tables & xor_tables
        ShiftRows(state)
        TBoxes(state, 10)           ----- last_box
        chipertext = state
*/

static void ref_table(uint32_t uint32_tables[16][256], uint8_t xor_tables[96][16][16], uint8_t *in) {
    int i;
    uint32_t a, b, c, d;

    for (i = 0; i < 4; i++) {
        a = uint32_tables[i*4  ][in[i*4  ]];
        b = uint32_tables[i*4+1][in[i*4+1]];
        c = uint32_tables[i*4+2][in[i*4+2]];
        d = uint32_tables[i*4+3][in[i*4+3]];

        in[i*4  ] = (
            (xor_tables[i*24+4 ][xor_tables[i*24   ][(a >> 28) & 0xf][(b >> 28) & 0xf]][xor_tables[i*24+1 ][(c >> 28) & 0xf][(d >> 28) & 0xf]]) << 4 |
            (xor_tables[i*24+5 ][xor_tables[i*24+2 ][(a >> 24) & 0xf][(b >> 24) & 0xf]][xor_tables[i*24+3 ][(c >> 24) & 0xf][(d >> 24) & 0xf]])
        );
        in[i*4+1] = (
            (xor_tables[i*24+10][xor_tables[i*24+6 ][(a >> 20) & 0xf][(b >> 20) & 0xf]][xor_tables[i*24+7 ][(c >> 20) & 0xf][(d >> 20) & 0xf]]) << 4 |
            (xor_tables[i*24+11][xor_tables[i*24+8 ][(a >> 16) & 0xf][(b >> 16) & 0xf]][xor_tables[i*24+9 ][(c >> 16) & 0xf][(d >> 16) & 0xf]])
        );
        in[i*4+2] = (
            (xor_tables[i*24+16][xor_tables[i*24+12][(a >> 12) & 0xf][(b >> 12) & 0xf]][xor_tables[i*24+13][(c >> 12) & 0xf][(d >> 12) & 0xf]]) << 4 |
            (xor_tables[i*24+17][xor_tables[i*24+14][(a >>  8) & 0xf][(b >>  8) & 0xf]][xor_tables[i*24+15][(c >>  8) & 0xf][(d >>  8) & 0xf]])
        );
        in[i*4+3] = (
            (xor_tables[i*24+22][xor_tables[i*24+18][(a >>  4) & 0xf][(b >>  4) & 0xf]][xor_tables[i*24+19][(c >>  4) & 0xf][(d >>  4) & 0xf]]) << 4 |
            (xor_tables[i*24+23][xor_tables[i*24+20][(a      ) & 0xf][(b      ) & 0xf]][xor_tables[i*24+21][(c      ) & 0xf][(d      ) & 0xf]])
        );
    }
}










void wbaes_encrypt(WBAES_ENCRYPTION_TABLE &et, uint8_t *pt) {
    int r;

    for (r = 0; r < 9; r++) {
        shift_rows(pt);
        ref_table(et.ty_boxes[r]  , et.xor_tables[r], pt);
        ref_table(et.mbl_tables[r], et.xor_tables[r], pt);
    }
    
























    
    shift_rows(pt);
    pt[0 ] = et.last_box[0 ][pt[0 ]];
    pt[1 ] = et.last_box[1 ][pt[1 ]];
    pt[2 ] = et.last_box[2 ][pt[2 ]];
    pt[3 ] = et.last_box[3 ][pt[3 ]];
    pt[4 ] = et.last_box[4 ][pt[4 ]];
    pt[5 ] = et.last_box[5 ][pt[5 ]];
    pt[6 ] = et.last_box[6 ][pt[6 ]];
    pt[7 ] = et.last_box[7 ][pt[7 ]];
    pt[8 ] = et.last_box[8 ][pt[8 ]];
    pt[9 ] = et.last_box[9 ][pt[9 ]];
    pt[10] = et.last_box[10][pt[10]];
    pt[11] = et.last_box[11][pt[11]];
    pt[12] = et.last_box[12][pt[12]];
    pt[13] = et.last_box[13][pt[13]];
    pt[14] = et.last_box[14][pt[14]];
    pt[15] = et.last_box[15][pt[15]];
}