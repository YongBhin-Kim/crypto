/*
    Implementation of Chow's Whitebox AES
        - Generate a encryption table of WBAES-128
*/

#include <iostream>
#include <fstream>

#include "wbaes_tables.h"
#include "gf2_mat.h"

/*
    Constants Variables
*/
constexpr uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

constexpr uint8_t shift_map[16] = {
    0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11
};

constexpr uint8_t inv_shift_map[16] = {
    0, 13, 10, 7, 4, 1, 14, 11, 8, 5, 2, 15, 12, 9, 6, 3
};

void WBAES_ENCRYPTION_TABLE::read(const char *file) {
    std::ifstream in(file, std::ios::in | std::ios::binary);
    
    if ( in.is_open() ) {
        in.read(reinterpret_cast<char*>(this->xor_tables), sizeof(this->xor_tables));
        in.read(reinterpret_cast<char*>(this->last_box  ), sizeof(this->last_box  ));
        in.read(reinterpret_cast<char*>(this->mbl_tables), sizeof(this->mbl_tables));
        in.read(reinterpret_cast<char*>(this->ty_boxes  ), sizeof(this->ty_boxes  ));
        in.close();
    }
}

void WBAES_ENCRYPTION_TABLE::write(const char *file) {
    std::ofstream out(file, std::ios::out | std::ios::binary);

    if ( out.is_open() ) {
        out.write(reinterpret_cast<char*>(this->xor_tables), sizeof(this->xor_tables));
        out.write(reinterpret_cast<char*>(this->last_box  ), sizeof(this->last_box  ));
        out.write(reinterpret_cast<char*>(this->mbl_tables), sizeof(this->mbl_tables));
        out.write(reinterpret_cast<char*>(this->ty_boxes  ), sizeof(this->ty_boxes  ));
        out.close();

        std::cout << "WBAES-128 Encryption table has been saved." << std::endl;
    }
}

void knuth_shuffle(uint8_t *x) {
    int i,j;
    uint8_t temp;

    std::srand(std::time(nullptr));
    for ( i=0; i<16; i++ ) {
        j = std::rand() % 16;

        temp = x[i];
        x[i] = x[j];
        x[j] = temp;
    }
}

void get_inv(const uint8_t *x, uint8_t *inv_x) {
    int i;

    for ( i=0; i<16; i++ ) {
        inv_x[x[i]] = i;
    }
}

void gen_rand(uint8_t *x, uint8_t *inv_x) {
    int i;
    for ( i=0; i<16; i++ ) {
        x[i] = i;
    }

    knuth_shuffle(x);
    if ( inv_x ) {
        get_inv(x, inv_x);
    }
}

void shift_rows(uint8_t *x) {
    int i;
    uint8_t temp[16];

    memcpy(temp, x, 16);

    for (i = 0; i < 16; i++) {
        x[i] = temp[shift_map[i]];
    }
}

void inv_shift_rows(uint8_t *x) {
    int i;
    uint8_t temp[16];

    memcpy(temp, x, 16);

    for (i = 0; i < 16; i++) {
        x[i] = temp[inv_shift_map[i]];
    }
}

void add_rk(uint8_t *x, uint32_t *rk) {
    int i;
    uint8_t u8_rk[16];

    /* 
        Sync Endian
    */    

#if 1
    PUTU32(u8_rk     , rk[0]);
    PUTU32(u8_rk +  4, rk[1]);
    PUTU32(u8_rk +  8, rk[2]);
    PUTU32(u8_rk + 12, rk[3]);
#endif

    for (i = 0; i < 16; i++) {
        x[i] ^= u8_rk[shift_map[i]];
    }
}

void encode_ext_x(uint8_t *f, uint8_t *x) {
    int i;

    for( i=0; i<16; i++ ) {
        x[i] = (f[(x[i] >> 4) & 0xf] << 4) | f[x[i] & 0xf];
    }
}

void decode_ext_x(uint8_t *inv_f, uint8_t *x) {
    int i = 0;
    for ( i=0; i<16; i++ ) {
        x[i] = (inv_f[(x[i] >> 4) & 0xf] << 4) | inv_f[x[i] & 0xf];
    }
}

void gen_nonlinear_encoding(WBAES_NONLINEAR_ENCODING &en) {
    int j;

    gen_rand(en.ext_f, en.inv_ext_f);

    for ( j=0; j<16; j++ ) {
        memcpy(en.int_g[0][j], en.inv_ext_f, 16);
        memcpy(en.inv_int_g[0][j], en.ext_f, 16);
    }
}


void gen_xor_tables(uint8_t (*xor_tables)[96][16][16]) {
    int r, n, x, y;



    for (r = 0; r < 9; r++) {
        for (n = 0; n < 96; n++) {
            for (x = 0; x < 16; x++) {
                for (y = 0; y < 16; y++) {
                    xor_tables[r][n][x][y] = x ^ y;
                }
            }
        }
    }
}

// ark, sbox, shiftrow
void gen_t_boxes(uint8_t (*t_boxes)[16][256], uint32_t *roundkeys, WBAES_NONLINEAR_ENCODING &en) {
    int r, x, n;
    uint8_t temp[16], y;

    for (r = 0; r < 10; r++) {
        for (x = 0; x < 256; x++) { // 0
            // 00 -> aa
            // 0 -> a 0 -> a 
            y = (uint8_t) x;
            if ( r == 0 )
                y = ((uint8_t) en.int_g[0][15][(x >> 4) & 0x0f] << 4) | (((uint8_t) en.int_g[0][15][x & 0x0f]) & 0x0f);

            memset(temp, y, 16);
            add_rk(temp, &roundkeys[4*r]);          // temp ^ shift_rows(RK)
            for (n = 0; n < 16; n++) {
                t_boxes[r][n][x] = sbox[temp[n]];   // sbox(temp ^ shift_rows(RK))
            }
        }
    }

    /*
        Final Round
         sbox(temp ^ shift_rows(RK_10)) ^ RK_11
    */
    for (n = 0; n < 4; n++) {
        for (x = 0; x < 256; x++) { 
            t_boxes[9][n*4  ][x] ^= roundkeys[40+n] >> 24;
            t_boxes[9][n*4+1][x] ^= roundkeys[40+n] >> 16;
            t_boxes[9][n*4+2][x] ^= roundkeys[40+n] >>  8;
            t_boxes[9][n*4+3][x] ^= roundkeys[40+n]      ;
        }
    }
}

void gen_tyi_tables(uint32_t (*tyi_tables)[256]) {
    int x;

    for (x = 0; x < 256; x++) {
        tyi_tables[0][x] = (GF_mul(2, x) << 24) |           (x  << 16) |           (x  << 8) | GF_mul(3, x);
        tyi_tables[1][x] = (GF_mul(3, x) << 24) | (GF_mul(2, x) << 16) |           (x  << 8) |          (x);
        tyi_tables[2][x] =           (x  << 24) | (GF_mul(3, x) << 16) | (GF_mul(2, x) << 8) |          (x);
        tyi_tables[3][x] =           (x  << 24) |           (x  << 16) | (GF_mul(3, x) << 8) | GF_mul(2, x);
    }
}

void composite_t_tyi(uint8_t (*t_boxes)[16][256], uint32_t (*tyi_tables)[256], uint32_t (*ty_boxes)[16][256], uint8_t (*last_box)[256]) {
    int r, n, x;

    /* Round-1-9 */
    for (r = 0; r < 9; r++) {
        for (x = 0; x < 256; x++) {
            for (n = 0; n < 16; n++) {
                ty_boxes[r][n][x] = tyi_tables[n%4][t_boxes[r][n][x]];
            }
        }
    }

    /* Round-10 */
    memcpy(last_box, t_boxes[9], 16 * 256);
}

void apply_linear_encoding(uint8_t (*last_box)[256], uint32_t (*ty_boxes)[16][256], uint32_t (*mbl_tables)[16][256]) {
    uint8_t   u8_temp[256];
    uint32_t u32_temp[256];
    NTL::mat_GF2 mb[9][4], l[9][16];

    /*
        Linear Encoding
         - (MB) Inverible 32x32 matrix
         - (L)  Inverible 8x8 matrix
    */
    int r, n, x;

    for (r = 0; r < 9; r++) {
        /*
            Initializes Invertible Matrix (MB)
             - size       : 32 x 32
             - components : GF2
             - determinant: !0
        */
        mb[r][0] = gen_gf2_rand_invertible_matrix(32);
        mb[r][1] = gen_gf2_rand_invertible_matrix(32);
        mb[r][2] = gen_gf2_rand_invertible_matrix(32);
        mb[r][3] = gen_gf2_rand_invertible_matrix(32);

        /*
            Applies Mixing Bijection
        */
        for (x = 0; x < 256; x++) {
            for (n = 0; n < 16; n++) {
                ty_boxes[r][n][x] = mul<uint32_t>(mb[r][n/4], ty_boxes[r][n][x]);
                mbl_tables[r][n][x] = mul<uint32_t>(NTL::inv(mb[r][n/4]), (uint32_t)(x << (24 - (8 * (n % 4)))));
            }
        }
    }

    /*
        Applies L at each round
    */
    for (r = 0; r < 9; r++) {
        /*
            Initializes Invertible Matrix (L)
                - size       : 8 x 8
                - components : GF2
                - determinant: !0
        */
        l[r][0 ] = gen_gf2_rand_invertible_matrix(8); l[r][1 ] = gen_gf2_rand_invertible_matrix(8);
        l[r][2 ] = gen_gf2_rand_invertible_matrix(8); l[r][3 ] = gen_gf2_rand_invertible_matrix(8);
        l[r][4 ] = gen_gf2_rand_invertible_matrix(8); l[r][5 ] = gen_gf2_rand_invertible_matrix(8);
        l[r][6 ] = gen_gf2_rand_invertible_matrix(8); l[r][7 ] = gen_gf2_rand_invertible_matrix(8);
        l[r][8 ] = gen_gf2_rand_invertible_matrix(8); l[r][9 ] = gen_gf2_rand_invertible_matrix(8);
        l[r][10] = gen_gf2_rand_invertible_matrix(8); l[r][11] = gen_gf2_rand_invertible_matrix(8);
        l[r][12] = gen_gf2_rand_invertible_matrix(8); l[r][13] = gen_gf2_rand_invertible_matrix(8);
        l[r][14] = gen_gf2_rand_invertible_matrix(8); l[r][15] = gen_gf2_rand_invertible_matrix(8);
        
        for (n = 0; n < 16; ++n) {
            for (x = 0; x < 256; x++) {
                mbl_tables[r][n][x] = (
                    mul<uint8_t>(l[r][inv_shift_map[(4*(n/4))  ]], (uint8_t)(mbl_tables[r][n][x] >> 24)) << 24 |
                    mul<uint8_t>(l[r][inv_shift_map[(4*(n/4))+1]], (uint8_t)(mbl_tables[r][n][x] >> 16)) << 16 |
                    mul<uint8_t>(l[r][inv_shift_map[(4*(n/4))+2]], (uint8_t)(mbl_tables[r][n][x] >>  8)) <<  8 |
                    mul<uint8_t>(l[r][inv_shift_map[(4*(n/4))+3]], (uint8_t)(mbl_tables[r][n][x]      ))       
                );
            }
        }
    }

    /*
        Applies L to inverse of L at previous round
    */
    for (r = 1; r < 9; r++) {
        for (n = 0; n < 16; n++) {
            memcpy(u32_temp, ty_boxes[r][n], 1024);
            for (x = 0; x < 256; x++) {
                ty_boxes[r][n][x] = u32_temp[mul<uint8_t>(NTL::inv(l[r-1][n]), (uint8_t)x)];
            }
        }
    }

    /*
        Final Round
    */
    for (n = 0; n < 16; n++) {
        memcpy(u8_temp, last_box[n], 256);
        for (x = 0; x < 256; x++) {
            last_box[n][x] = u8_temp[mul<uint8_t>(NTL::inv(l[8][n]), (uint8_t)x)];
        }
    }
}

void gen_encryption_table(WBAES_ENCRYPTION_TABLE &et, WBAES_NONLINEAR_ENCODING &en, uint32_t *roundkeys) {
    uint8_t  t_boxes[10][16][256];
    uint32_t    tyi_table[4][256];

    /*
        Generates T-boxes depend on round keys, 
            Tyi-table and complex them. 
    */
    gen_t_boxes(t_boxes, roundkeys, en); // tbox (sr, rk, sbox)
    gen_tyi_tables(tyi_table); // mc
    composite_t_tyi(t_boxes, tyi_table, et.ty_boxes, et.last_box);

    gen_xor_tables(et.xor_tables); // xor

    /*
        Applies linear encoding to tables
    */
    apply_linear_encoding(et.last_box, et.ty_boxes, et.mbl_tables);
}
