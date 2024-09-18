/*
    Implementation of related GF2 matrix
*/

#include "gf2_mat.h"

void GF_print(byte gf) {
    int coef;

    printf("%d = %02x = ", gf, gf);
    for (int i = 7; i >= 0; i--) {
        coef = (gf >> i) & 0x01;
        if (coef == 1) {
            std::cout << " + " << "x^" << i;
        }
    }
    std::cout << std::endl;
}

byte GF_add(byte gf1, byte gf2) {
    return gf1 ^ gf2; // XOR
}

byte GF_xtime(byte gf) {
    int msb;  
    byte result; 

    msb = (gf >> 7) & 0x01;
    if (msb == 1) {
        result = (gf << 1) ^ 0x1b; 
    }
    else {
        result = gf << 1;
    }
    return result;
}

byte GF_mul(byte f, byte g) {
    byte h; 
    int coef; 
    h = 0x00;
    for (int i = 7; i >= 0; i--) {
        coef = (f >> i) & 0x01; // a7, a6, a5, ... ,a0
        h = GF_xtime(h);
        if (coef == 1) {
            h = GF_add(h, g);
        }
    }
    return h;
}

byte GF_inv(byte f) {
    byte f_inv; 
    byte temp; 
    f_inv = 1;
    temp = f;
    for (int i = 0; i < 7; i++) {
        temp = GF_mul(temp, temp);
        f_inv = GF_mul(f_inv, temp);
    }
    return f_inv;
}

byte AES_Affine(byte w) {
    byte A[8][8] = {  
    {1, 0, 0, 0, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 1, 1}
    };
    byte b_vec[8] = { 1, 1, 0, 0, 0, 1, 1, 0 };
    byte w_vec[8], y_vec[8], y;

    for (int i = 0; i < 8; i++) {
        w_vec[i] = (w >> i) & 0x01;
    }

    for (int i = 0; i < 8; i++) {
        y_vec[i] = b_vec[i];
        for (int j = 0; j < 8; j++) {
            y_vec[i] ^= A[i][j] * w_vec[j];
        }
    }

    y = 0;
    byte temp_bit;
    for (int i = 0; i < 8; i++) {
        temp_bit = y_vec[i] << i;
        y ^= temp_bit;
    }
    return y;
}

void Get_AES_Sbox(byte sbox[256]) {
    byte temp;

    sbox[0] = AES_Affine(0);
    for (int i = 1; i < 256; i++) {
        temp = GF_inv(i);
        sbox[i] = AES_Affine(temp);
    }
}

void Get_AES_Inv_Sbox(byte isbox[256]) {
    byte Sbox[256];
    Get_AES_Sbox(Sbox);
    for (int i = 0; i < 256; i++) {
        isbox[Sbox[i]] = i;
    }
}

NTL::mat_GF2 gen_gf2_rand_matrix(int dimension) {
    NTL::mat_GF2 mat(NTL::INIT_SIZE, dimension, dimension);
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            mat[i][j] = NTL::random_GF2();
        }
    }

    return mat;
}

NTL::mat_GF2 gen_gf2_rand_invertible_matrix(int dimension) {
    for (;;) {
        NTL::mat_GF2 result = gen_gf2_rand_matrix(dimension);
        if (NTL::determinant(result) != 0)
            return result;
    }
}

NTL::vec_GF2 scalar_to_vec_GF2(const uint8_t src) {
    int j;
    NTL::vec_GF2 ret;

    ret.SetLength(8);

    for (j = 0; j < 8; j++) {
        ret[7-j] = (src >> j) & 0x1;
    }

    return ret;
}

NTL::vec_GF2 scalar_to_vec_GF2(const uint32_t src) {
    int j;
    NTL::vec_GF2 ret;
    
    ret.SetLength(32);
    
    for (j = 0; j < 32; j++) {
        ret[31-j] = (src >> j) & 0x1;
    }

    return ret;
}

template <typename T>
T vec_GF2_to_scalar(const NTL::vec_GF2 &src) {
    int j;
    long len = src.length() >> 2;
    T ret = 0;

    for (j = 0; j < len; j++) {
        ret = ret << 4 | NTL::rep(src[j*4  ]) << 3 | NTL::rep(src[j*4+1]) << 2 
                       | NTL::rep(src[j*4+2]) << 2 | NTL::rep(src[j*4+3]) << 1;
    }

    return ret;
}

uint8_t mat_GF2_times_vec_GF2(const NTL::mat_GF2 &x, const uint8_t y) {
    return vec_GF2_to_scalar<uint8_t>(x * scalar_to_vec_GF2(y));
}

uint32_t mat_GF2_times_vec_GF2(const NTL::mat_GF2 &x, const uint32_t y) {
    return vec_GF2_to_scalar<uint32_t>(x * scalar_to_vec_GF2(y));
}
