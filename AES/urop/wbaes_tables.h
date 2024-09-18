#ifndef WBAES_TABLES_H
#define WBAES_TABLES_H

#include "aes.h"
#include "debug.h"

/*
    Whitebox AES Tables
*/
struct WBAES_ENCRYPTION_TABLE {
    uint32_t   ty_boxes[9][16][256]   ; // (sr) l^-1 [ark sbox mc] mb 
    uint8_t  xor_tables[9][96][16][16]; // 
    uint32_t mbl_tables[9][16][256]   ; // mb^-1 l 
    uint8_t    last_box[16][256]      ;

    void read(const char* file);  // should be remove
    void write(const char* file); // should be remove
};

struct WBAES_NONLINEAR_ENCODING {

    uint8_t ext_f[16];
    uint8_t inv_ext_f[16];

    // uint8_t int_f[9][16][16];
    // uint8_t inv_int_f[9][16][16];

    uint8_t int_g[9][16][16];
    uint8_t inv_int_g[9][16][16];




    uint8_t int_h[9][16][16];
    uint8_t inv_int_h[9][16][16];

    uint8_t int_z[9][16][16];
    uint8_t inv_int_z[9][16][16];

};

void shift_rows(uint8_t *x);
void inv_shift_rows(uint8_t *x);
void add_rk(uint8_t *x, uint32_t *rk);

void gen_xor_tables(uint8_t (*xor_tables)[96][16][16]);
// void gen_t_boxes(uint8_t (*t_boxes)[16][256], uint32_t *roundkeys);
void gen_tyi_tables(uint32_t (*tyi_tables)[256]);
void composite_t_tyi(uint8_t (*t_boxes)[16][256], uint32_t (*tyi_tables)[256], uint32_t (*ty_boxes)[16][256], uint8_t (*last_box)[256]);



void encode_ext_x(uint8_t *f, uint8_t *x);
void gen_t_boxes(uint8_t (*t_boxes)[16][256], uint32_t *roundkeys, WBAES_NONLINEAR_ENCODING &en);

// void gen_encryption_table(WBAES_ENCRYPTION_TABLE &et, uint32_t *roundkeys);
void gen_encryption_table(WBAES_ENCRYPTION_TABLE &et, WBAES_NONLINEAR_ENCODING &en, uint32_t *roundkeys);

#endif /* WBAES_TABLES_H */