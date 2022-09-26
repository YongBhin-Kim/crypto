#include <stdio.h>
#include "GF_Math.h"

// Polynomial over GF(2^8) --> unsigned char == byte

byte GF_xtime_1(byte f);
byte GF_xtime_2(byte f);
byte GF_add(byte f, byte g);
byte GF_sub(byte f, byte g);
byte GF_mul(byte f, byte g);
byte GF_div(byte f, byte g);
int gcd(int a, int b);
int gcd_2(int a, int b);
int xgcd(int a, int b);
int xgcd_2(int a, int b);
byte xgcd_poly(byte f, byte g);
byte GF_inv(byte f);

// GF_print  : GF(2^8)의 원소를 출력하는 함수
void GF_print(byte f) {
    printf("%3d = %02x =", f, f);
    int coef;
    for (int i=7; i>=0; i--) {
        coef = (f >> i) & 0x01; // lsb
        if (coef == 1) printf("+ x^%1d ", i);
    }
    printf("\n");
}

// GF_print_Matrix : GF(2^8)의 원소(다항식)로 이루어진 행렬을 출력하는 함수
void GF_print_Matrix(byte Matrix[256]) {
    printf(" --------------------------------------------------\n");
    for (int i=0; i<256; i++) {
        if (i % 16 == 0) printf("| ");
        printf("%02x ", Matrix[i]);
        if (i % 16 == 15) printf(" |\n");
    }
    printf(" --------------------------------------------------\n\n");
}

// GF_xtime    : GF(2^8)의 원소(다항식)에 x를 곱하는 함수
byte GF_xtime_1(byte f) {
    int msb;
    byte result;
    msb = (f >> 7) & 0x01;
    if (msb == 1)
        result = (f << 1) ^ 0x1b;
    else
        result = f << 1;
    return result;
}

// GF_xtime    : GF(2^8)의 원소(다항식)에 x를 곱하는 함수
byte GF_xtime_2(byte f) {
    return ( ((f >> 7) & 0x01) == 1 ? (f << 1) ^ 0x1b : f << 1 );
}

// GF_add      : GF(2^8)의 두 원소(다항식)의 합
byte GF_add(byte f, byte g) {
    return (f ^ g);
}

// GF_add      : GF(2^8)의 두 원소(다항식)의 차
byte GF_sub(byte f, byte g) {
    return (f ^ g);
}

// GF_add      : GF(2^8)의 두 원소(다항식)의 곱
byte GF_mul(byte f, byte g) {
    byte h;
    int coef;
    h = 0x00;
    for(int i=7; i>=0; i--) {
        coef = (f>>i) & 0x01;
        if (h != 0)
            h = GF_xtime_2(h);
        if (coef == 1)
            h = GF_add(h, g);
    }

    return h;
}

// GF(2^8) f / g 
// GF_add      : GF(2^8)의 두 원소(다항식)의 나눗셈
byte GF_div(byte f, byte g) {
    byte q = 0;
    byte tmp;
    int coef_g, coef_f, flag;
    for (int i=7; i>=0; i--) {
        if ( ((g>>i) & 0x01) == 1 ) {
            coef_g = i;
            break;
        }
    }
    for (int i=7; i>=0; i--) {
        if ( ((f>>i) & 0x01) == 1 ) {
            coef_f = i;
            break;
        }
    }
    //printf("coef of g = %d\n", coef_g);
    //printf("coef of f = %d\n", coef_f);

    if (coef_g > coef_f)
        return 0;
    
    for (int i=coef_f; i>=coef_g; i--) {

        tmp = g << (i - coef_g);
        for (int k=7; k>=0; k--) { // update coef_f <--- f
            if ( ((f>>k) & 0x01) == 1 ) {
                coef_f = k;
                break;
            }
        }

        if (i <= coef_f) {
            // printf("tmp = %d\n", tmp);
            q += (1<<(i - coef_g)); // q += sqrt(2, coef_f - coef_g)
            f ^= tmp;
        }
        // printf("q = %d\n", q);
        for (int j=7; j>=0; j--) {
            if ( ((f>>j) & 0x01) == 1 ) {
                flag = j;
                break;
            }
        }
        if (coef_g > flag)
            return q;
    }

    return q;
}

// gcd         : Euclidean Algorithm을 이용하여 두 정수의 최대공약수를 구하는 함수
int gcd(int a, int b) {
    int r;
    while (b != 0) {
        // printf("%d = %d * %d + %d\n", a, b, a / b, r);
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

// gcd         : Euclidean Algorithm을 이용하여 두 정수의 최대공약수를 구하는 함수
int gcd_2(int a, int b) {
    if (b == 0) return a;
    return gcd_2(b, a%b);
}

// xgcd        : Extended Euclidean Algorithm을 이용하여 두 정수의 최대공약수 및 역원 계산에 필요한 함수
int xgcd(int a, int b) {
    int an, bn, new_an, new_bn;
    int ub, vb, q, new_ua, new_va, new_ub, new_vb;

    an = a; bn = b;
    int ua = 1; int va = 0; ub = 0; vb = 1; 

    while (bn != 0) {
        printf("%d = (%d)*%d + (%d)*%d\n", bn, ub, 37, vb, 41);
        q = an / bn;
        new_an = bn;
        new_bn = an - bn * q;
        new_ua = ub;
        new_va = vb;
        new_ub = ua = ub * q;
        new_vb = va - vb * q;

        an = new_an; bn = new_bn;
        ua = new_ua; va = new_va; ub = new_ub; vb = new_vb;
    }
    return an;
}

// xgcd        : Extended Euclidean Algorithm을 이용하여 두 정수의 최대공약수 및 역원 계산에 필요한 함수
int xgcd_2(int a, int b) {

    int q = 0;
    int s0,s1,t0,t1;
    s0 = 1; s1 = 0; t0 = 0; t1 = 1;
    int tmp;
    int fixedA = a; int fixedB = b;

    while(b != 0) {
        printf("%d = (%d)*%d + (%d)*%d\n", b,s1,fixedA, t1, fixedB); // 마지막께 업데이트 되기 전이므로 b, s1, t1으로 출력하면 됨
        q = a / b;
        tmp = a;
        a = b;
        b = tmp - q*b;
        tmp = s0;
        s0 = s1;
        s1 = tmp - s1*q;
        tmp = t0;
        t0 = t1;
        t1 = tmp - t1*q;
    }
    printf("A : %d^(-1) = %d mod %d\n", fixedA, s0, fixedB);
    printf("B : %d^(-1) = %d mod %d\n", fixedB, t0, fixedA);
    return a;
}


// 구현중... 혼자 해보기
// xgcd_poly   : Extended Euclidean Algorithm을 이용하여 GF(2^8)의 두 원소(다항식)의 최대공약수 및 역원 계산에 필요한 함수
byte xgcd_poly(byte f, byte g) {

    byte q = 0;
    byte s0, s1, t0, t1;
    s0 = 1; s1 = 0; t0 = 0; t1 = 1;
    byte tmp;

    do {
        q = GF_div(f, g);
        tmp = f;
        f = g;
        g = GF_sub(tmp, GF_mul(q,g));
        tmp = s0;
        s0 = s1;
        s1 = GF_sub(tmp, GF_mul(s1, q));
        tmp = t0;
        t0 = t1;
        t1 = GF_sub(tmp, GF_mul(t1, q));
        printf("f = ");
        GF_print(g);
    } while(g != 0);

    return s0;
}

/** GF_inv : Little Fermat Thm을 이용하여 GF2^8 원소의 역원을 구한다.
 * f^255 = 1 mod m(x)
 * => inverse of f = f^254 mod m(x)
 * 254 = 2 + 4 + 8 + 16 + 32 + 64 + 128
 */
// GF_inv      : Little Fermat Theorem을 이용하여 GF(2^8)의 원소(다항식)의 역원을 계산하는 함수
byte GF_inv(byte f) {
    byte f_inv, term;
    f_inv = 1;
    term = f;
    for (int i = 0; i < 7; i++) {    // 7번
        term = GF_mul(term, term);   // f^2  f^4      f^8         ...
        f_inv = GF_mul(f_inv, term); // f^2  f^2*f^4  f^2*f^4*f^8 ...
    }

    return f_inv;
}



// int main() {
    /**
     * GF_mul function 
    printf("%02x ", GF_mul(0xd7, 0x52));
    */

   /**
    * GCD - Euclidean Algorithm
    * a = b*q + r
    * a = b
    * b = r 
    printf("%d \n", gcd(63, 90));
    printf("%d \n", gcd_2(63, 90));
    printf("gcd(%d, %d) = %d \n\n", 37, 41, xgcd(37, 41));
    printf("gcd(%d, %d) = %d \n", 37, 41, xgcd_2(37, 41));
    // poly div
    printf("91(x^6 + x^4 + x^3 + x + 1) / 6(x^2 + x) = ");
    byte q = GF_div(91, 6);
    print_poly(q);

    // xgcd_poly - 혼자 구현
    // 확장 유클리드 알고리즘(EEA)을 이용하여 GF2^8 원소의 역원을 구한다.
    printf("inverse of %d = ", f); // 256 16 8 2 1
    print_poly(xgcd_poly(f,g));
    // GF_inv
    // Little Fermat Thm을 이용하여 GF2^8 원소의 역원을 구한다.
    */

   /**
    byte f = 4;
    byte g = 0; // m(x)
    print_poly(GF_inv(f));
*/
    


    // return 0;
// }