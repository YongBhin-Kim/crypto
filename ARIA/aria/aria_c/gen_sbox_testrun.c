/*
    ARIA - C Implementation 
            - Generate sbox used in SubstLayer
*/
//  Copyright © Yonhbhin Kim
//  Github url : https://github.com/?

#include <stdio.h>

/*
  GF(2^8) is polynomial s.t. degree <= 7 (it appeared as 1 byte) 
  It's coefficient is over Z_2 and primitive polynomial is x^8 + x^4 + x^3 + x + 1 
  ((ex) 0x1A = 26 = 0001 1010 = x^4 + x^3 + x )
*/


typedef unsigned char byte;
void print_gfpoly(byte x);
void print_matrix(byte sbox[256]);
byte gf_add(byte x1, byte x2);
#define gf_xtime(f) ((f >> 7) == 1) ? (f << 1) ^ 0x1b : (f << 1) /* x time over GF */
byte gf_mul(byte f, byte g);
byte gf_inv(byte f);
byte affine1(byte x);
byte affine2(byte x);
void gen_aria_sbox(byte sbox[2][256]);
void gen_aria_isbox(byte isbox[2][256]);


/* Print entry(polynomial) in GF */
void print_gfpoly(byte x) {

    printf("%3d = %02x =", x, x);

    int coef;
    for (int i=7; i>=0; i--) {
        coef = (x >> i) & 0x01; // lsb
        if (coef == 1) printf("+ x^%1d ", i);
    }
    printf("\n");
}

/* Matrix print function */
void print_matrix(byte matrix[256]) {

    printf("  ");
    for (int i=0; i<256; i++) {
        printf("%02x ", matrix[i]);
        if (i % 16 == 15)
            printf("\n  ");
    }
    printf("\n");
}

/* Addition over GF(2^8), coefficient is 0 or 1 */
byte gf_add(byte x1, byte x2) {

    return x1 ^ x2;
}

/* Multiplicate two entry on Galois Field */
byte gf_mul(byte f, byte g) {

    byte ret = 0x00;
    int coef;

    for (int i=7; i>=0; i--) {
        ret = gf_xtime(ret);
        coef = (f >> i) & 0x01;
        if (coef == 1) {
            ret = gf_add(g, ret);
        }
    }
    return ret;
}

/* Get inverse of entry of GF(2^8) ( except is only zero, i.e. 0^(-1) = 0 ) */
byte gf_inv(byte f) {

    if (f == 0) return 0;
    byte term  = f;                   // x^2,  x^4,  x^8,  x^16,  x^32,  x^64,  x^128
    byte f_inv = 1;                   // x^2 * x^4 * x^8 * x^16 * x^32 * x^64 * x^128 = x^254
    for (int i=0; i<7; i++) {
        term = gf_mul(term,term);     // x^(2^i)
        f_inv = gf_mul(term, f_inv);  // pi(x^(2^i))
    }
    return f_inv;
}

/* Get exp(247) of entry of GF(2^8) ( except is only zero, i.e. 0^(-1) = 0 ) */
// SBox1
// out <- Bx^-1 + b
// SBox2
// out <- Cx^247 + c
byte gf_x247(byte f) {
    if (f == 0) return 0;

    byte term = f;  
    byte f_247 = 1; 
    for (int i=0; i<8; i++) {
        if (i != 3)
            f_247 = gf_mul(term, f_247); 
        term = gf_mul(term, term); // x + x^2 x^4 x^16 + x^32 + 64 + 128
    }
    return f_247;
}

/** Affine trans for sbox1
 * x = (byte) = b7b6b5b4 b3b2b1b0
 * vec_x[8] = ( b0, b1, b2, b3, b4, b5, b6, b7 ) = ( lsb, ..., msb )
 * y = Ax + b (A: 8*8 Matrix, b = column vector)
*/
byte affine1(byte x) {
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
    
    byte b_vec[8] = {1, 1, 0, 0, 0, 1, 1, 0};
    byte x_vec[8], y_vec[8], y = 0x00;
    
    // x_vec <- x
    for (int i=0; i<8; i++)
        x_vec[i] = (x>>i) & 0x01;

    // y = b + Ax
    for (int i=0; i<8; i++) {
        y_vec[i] = b_vec[i];
        for (int j=0; j<8; j++) { 
            y_vec[i] ^= (A[i][j] * x_vec[j]); 
        }
    }

    // y <- y_vec over GF(2^8)
    byte temp;
    for (int i=0; i<8; i++) {
        temp = y_vec[i] << i;
        y ^= temp;
    }
    return y;
}

/* Affine trans for sbox2 */
byte affine2(byte x) {

    byte B[8][8] = {
    {0, 1, 0, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 1},
    {1, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 0, 1, 1, 1, 0, 1},
    {0, 0, 1, 0, 1, 1, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {0, 1, 0, 1, 1, 1, 0, 1},
    {1, 1, 0, 1, 0, 0, 1, 1}
    };

    byte b_vec[8] = {0, 1, 0, 0, 0, 1, 1, 1};    // for s2box
    byte x_vec[8], y_vec[8], y = 0x00;
    

    // x_vec <- x
    for (int i=0; i<8; i++)
        x_vec[i] = (x>>i) & 0x01;

    // y = b + Bx
    for (int i=0; i<8; i++) {
        y_vec[i] = b_vec[i];
        for (int j=0; j<8; j++) { 
            y_vec[i] ^= (B[i][j] * x_vec[j]); 
        }
    }
    // y <- y_vec over GF(2^8)
    byte temp;
    for (int i=0; i<8; i++) {
        temp = y_vec[i] << i;
        y ^= temp;
    }
    return y;
}

/** Generate ARIA s1box, s2box, 
 * s1 : y = sbox(x) = B * x^(-1) + b
 * s2 : y = sbox(x) = C * x^247  + c
 */
void gen_aria_sbox(byte sbox[2][256]) {
    sbox[0][0] = affine1(0);
    sbox[1][0] = affine2(0);

    for (int i=1; i<256; i++) {
        sbox[0][i] = affine1( gf_inv(i) );  // x <- x^-1
        sbox[1][i] = affine2( gf_x247(i) ); // x <- x^247
    }
}

/* Inverse of s1box,s2box */
void gen_aria_isbox(byte isbox[2][256]) {
    byte sbox[2][256];
    gen_aria_sbox(sbox);
    for (int i=0; i<256; i++) {
        isbox[0][sbox[0][i]] = i;
        isbox[1][sbox[1][i]] = i;
    }
}


// main function
int main() {

    byte sbox[2][256], isbox[2][256];

    gen_aria_sbox(sbox);   
    gen_aria_isbox(isbox); 


    printf("\n  ===============================================\n");
    printf("  * Genereate ARIA sbox1, sbox2, isbox1, isbox2 \n");
    printf("  -----------------------------------------------\n");
    printf("  * [ sbox1 ] \n");  print_matrix(sbox[0]);
    printf("  * [ isbox1 ] \n"); print_matrix(isbox[0]);

    printf("  -----------------------------------------------\n");

    printf("  * [ sbox2 ] \n");  print_matrix(sbox[1]);
    printf("  * [ isbox2 ] \n"); print_matrix(isbox[1]);
    printf("  ===============================================\n");

    return 0;
}

// EOF