// gift - 64
// Round function : SubCells - PermBits - AddRoundKeys

#include <stdio.h> 
#include <memory.h> 
#include <string.h>

const unsigned char GS[16] = { // Gift Substitute cells
    0x1, 0xa, 0x4, 0xc, 0x6, 0xf, 0x3, 0x9, 0x2, 0xd, 0xb, 0x7, 0x5, 0x0, 0x8, 0xe
};
const unsigned char InvGS[16] = { // Inverse of Gift Substitute cells
    0xd, 0x0, 0x8, 0x6, 0x2, 0xc, 0x4, 0xb, 0xe, 0x7, 0x1, 0xa, 0x3, 0x9, 0xf, 0x5
};

unsigned char RC(char i) { // Round Constant 제조 함수 (AddRoundKeys 에서 사용)
    unsigned char dst = 0x01;

    for (char count = 0; count < i; count++) {
        dst = ((dst<<1)&0x3f)^((dst>>5)^((dst>>4)&0x01)^0x01);
    }
    return dst;
}


void SubCells(unsigned char state[16]); // Substitute Cells
void PermBits(unsigned char state[16]); // Permutation Bits
void UpdateRoundKey(unsigned char RoundKey[16]); // 라운드키 갱신 함수
void AddRoundKey(unsigned char state[16], unsigned char RoundKey[16], unsigned char RoundConstant);

void InvSubCells(unsigned char state[16]); // SubCells 의 역함수
void InvPermBits(unsigned char state[16]); // PermBits 의 역함수
void InvUpdateRoundKey(unsigned char RoundKey[16]); 
void InvAddRoundKey(unsigned char state[16], unsigned char RoundKey[16], unsigned char RoundConstant);

void gift(const unsigned char Plaintext[8], unsigned char Ciphertext[8], const unsigned char masterKey[16]);
void ShowState(unsigned char state[16]); // Text 보여주는 함수

int main() {

    const unsigned char Plaintext[8] = {0xc4, 0x50, 0xc7, 0x72, 0x7a, 0x9b, 0x8a, 0x7d};
    unsigned char Ciphertext[8] = {0x00, };
    const unsigned char masterKey[16] = {0xbd, 0x91, 0x73, 0x1e, 0xb6, 0xbc, 0x27, 0x13, 0xa1, 0xf9, 0xf6, 0xff, 0xc7, 0x50, 0x44, 0xe7};
                                    //        k7          k6          k5          k4          k3          k2           k1        k0     
    gift(Plaintext, Ciphertext, masterKey); 
    return 0;
}

void SubCells(unsigned char state[16]) {
    for (char i = 0; i < 16; i++) {
        state[i] = GS[state[i]];
    }
}

// PermBits에 순서섞는 공식
#define P(i) (((4*(i/16)) + (16*((((3*((i%16)/4))+(i%4)))%4)) + (i%4))) 

void PermBits(unsigned char state[16]) {
   unsigned char tmp[64] = {0, };
   unsigned char tmp1[64] = {0, }; 
   
    // state를 64-bit tmp로 쪼개기
    for (char i = 0; i < 64; i++) {
        tmp[i] = state[15 - (i / 4)] >> (i % 4) & 0x01;
    }

    // permbit 시키기 (순서 섞기)
    for (char i = 0; i < 64; i++) {
        tmp1[P(i)] = tmp[i];
    }

    // 64-bit tmp를 state로 뭉치기
    for (char i = 0; i < 16; i++) {
        state[15-i] = tmp1[4*i] ^ (tmp1[4*i+1]<<1) ^ (tmp1[4*i+2]<<2) ^ (tmp1[4*i+3]<<3);
    }
}

void UpdateRoundKey(unsigned char RoundKey[16]) {
    unsigned char tmp[16];
    memcpy(tmp, RoundKey, 16);

    // RoundKey update 하기
    for (char i = 15; i > 3; i--) {
        tmp[i] = RoundKey[i - 4];
    }
    tmp[3] = (RoundKey[15]<<4)^(RoundKey[14]>>4);
    tmp[2] = (RoundKey[14]<<4)^(RoundKey[15]>>4);
    tmp[1] = (RoundKey[13]>>2)^(RoundKey[12]<<6);
    tmp[0] = (RoundKey[12]>>2)^(RoundKey[13]<<6);

    memcpy(RoundKey, tmp, 16);
}

void AddRoundKey(unsigned char state[16], unsigned char RoundKey[16], unsigned char RoundConstant) {
    unsigned char tmp[64] = {0x0, };

    // state[16] 을 64 - bit tmp 로 쪼개기
    for (char i = 0; i < 64; i++) {
        tmp[i] = state[15-(i/4)]>>(i%4)&0x01;
    }

    // U, V의 후방 8 bit xor
    for (char i = 0; i < 8; i++) {
        tmp[4*i + 1] ^= ((RoundKey[13] >> i) & (0x01));
        tmp[4*i] ^= ((RoundKey[15] >> i) & (0x01));
    }

    // U, V의 전방 8 bit xor
    for (char i = 8; i < 16; i++) {
        tmp[4*i + 1] ^= ((RoundKey[12] >> (i-8)) & (0x01));
        tmp[4*i] ^= ((RoundKey[14] >> (i-8)) & (0x01));
    }

    tmp[63] ^= 0x01;
    for (char i = 0; i < 6; i++) {
        tmp[4 * i + 3] ^= (RoundConstant >> i) & 0x01;
    }

    // 64-bit tmp를 state[16] 으로 돌려놓기
    for (char i = 0; i < 16; i++) {
        state[15-i] = tmp[4*i] ^ (tmp[4*i+1]<<1) ^ (tmp[4*i+2]<<2) ^ (tmp[4*i+3]<<3);
    }
}

// 역함수들
void InvSubCells(unsigned char state[16]) {
    for (char i = 0; i < 16; i++) {
        state[i] = InvGS[state[i]];
    }
}

void InvPermBits(unsigned char state[16]) {
   unsigned char tmp[64] = {0, };
   unsigned char tmp1[64] = {0, }; 
   
    // state를 64 - bit tmp 로 쪼개기
    for (char i = 0; i < 64; i++) {
        tmp[i] = state[15 - (i / 4)] >> (i % 4) & 0x01;
    }

    // permbit 시키기 (순서 섞기)
    for (char i = 63; i >= 0; i--) {
        tmp1[i] = tmp[P(i)];
    }

    // 64-bit tmp 를 state로 뭉치기
    for (char i = 0; i < 16; i++) {
        state[15-i] = tmp1[4*i] ^ (tmp1[4*i+1]<<1) ^ (tmp1[4*i+2]<<2) ^ (tmp1[4*i+3]<<3);
    }
}
void InvUpdateRoundKey(unsigned char RoundKey[16]) {
    unsigned char tmp[16];
    memcpy(tmp, RoundKey, 16);

    // RoundKey Invupdate 하기
    for (char i = 0; i < 12; i++) {
        tmp[i] = RoundKey[i + 4];
    }
    tmp[15] = (RoundKey[3]>>4) ^ (RoundKey[2]<<4);
    tmp[14] = (RoundKey[2]>>4) ^ (RoundKey[3]<<4);
    tmp[13] = (RoundKey[1]<<2) ^ (RoundKey[0]>>6);
    tmp[12] = (RoundKey[0]<<2) ^ (RoundKey[1]>>6);

    memcpy(RoundKey, tmp, 16);
}


void InvAddRoundKey(unsigned char state[16], unsigned char RoundKey[16], unsigned char RoundConstant) {
    unsigned char tmp[64] = {0x0, };

    // state를 64 - bit tmp 로 쪼개기
    for (char i = 0; i < 64; i++) {
        tmp[i] = state[15 - (i / 4)] >> (i % 4) & 0x01;
    }

    // U, V의 후방 8 bit xor
    for (char i = 0; i < 8; i++) {
        tmp[4 * i + 1] ^= ((RoundKey[13] >> i) & (0x01));
        tmp[4 * i] ^= ((RoundKey[15] >> i) & (0x01));
    }
    // printf("\n");


    // U, V의 전방 8 bit xor
    for (char i = 8; i < 16; i++) {
        tmp[4*i + 1] ^= ((RoundKey[12] >> (i-8)) & (0x01));
        tmp[4*i] ^= ((RoundKey[14] >> (i-8)) & (0x01));
    }

    tmp[63] ^= 0x01;
    for (char i = 0; i < 6; i++) {
        tmp[4 * i + 3] ^= (RoundConstant >> i) & 0x01;
    }

    // 64-bit tmp를 state[16] 으로 돌려놓기
    for (char i = 0; i < 16; i++) {
        state[15-i] = tmp[4*i] ^ (tmp[4*i+1]<<1) ^ (tmp[4*i+2]<<2) ^ (tmp[4*i+3]<<3);
    }
}


void gift(const unsigned char Plaintext[8], unsigned char Ciphertext[8], const unsigned char masterKey[16]) {
    unsigned char tmp[16] = {0x0, };
    unsigned char RK[16] = {0x0, };
    unsigned char PT[16] = {0x00, }; // 복호화 과정을 마치고 Plaintext 를 확인시켜 줄 것임
    memcpy(RK, masterKey, 16);

    // Plaintext[8] -> tmp[16] 으로 쪼개기 /////////
    for (char i = 0; i < 16; i++) {
        if (i%2 == 0) {
            tmp[i] = Plaintext[i>>1]>>4;
        }
        else {
            tmp[i] = Plaintext[i>>1]&0x0f;
        }
    }
    printf("\nPlaintext = ");
    for (char i = 0; i < 8; i++) {
        PT[i] = (tmp[i<<1] << 4) ^ (tmp[(i<<1) + 1]);
        printf("%x ",PT[i]);
    } printf("\n"); 
    //////////////////////////////////////////////
    // round 별로 암호화 과정 거치기
    for(char i = 0; i <= 27; i++) {
        SubCells(tmp);
        // printf("%d: after SubCells: ", i);
        // ShowState(tmp);

        PermBits(tmp);
        // printf("%d: after PermBits: ", i);
        // ShowState(tmp);

        AddRoundKey(tmp, RK, RC(i));
        // printf("%d: after AddRoundKeys: ", i);
        // ShowState(tmp);
        
        UpdateRoundKey(RK);
        // printf("%d: updated Key: ", i);
        // for (char j = 0; j < 16; j++) {
            // printf("%x ", RK[j]);
        // } printf("\n\n");
    }
    // 암호화 완료/////////////////////////////
    // tmp[16] -> Ciphertext[8] 으로 뭉치기
    printf("Ciphertext = ");
    for (char i = 0; i < 8; i++) {
        Ciphertext[i] = (tmp[i<<1] << 4) ^ (tmp[(i<<1) + 1]);
        printf("%x ",Ciphertext[i]);
    } printf("\n");
    
    ////////////////////////////////////////
    // round 별로 복호화 과정 거치기//////////////

    for (char i = 27 ; i >= 0 ; i-- ){
        InvUpdateRoundKey(RK);
        InvAddRoundKey(tmp, RK, RC(i));
        // printf("%d: after inverse AddRoundKeys: ", i);
        // ShowState(tmp);

        InvPermBits(tmp);
        // printf("%d: after inverse Permbits: ", i);
        // ShowState(tmp);

        InvSubCells(tmp);
        // printf("%d: after inverse SubCells: ", i);
        // ShowState(tmp);
    }
    // 복호화 완료/////////////////////////////////////
    // tmp[16] -> Plaintext[8] 으로 뭉치기
    printf("\nDecrypted text = ");
    for (char i = 0; i < 8; i++) {
        PT[i] = (tmp[i<<1] << 4) ^ (tmp[(i<<1) + 1]);
        printf("%x ",PT[i]);
    } printf("\n");  
}

void ShowState(unsigned char state[16]) {
    for (char i = 0; i < 16; i++) {
        printf("%x", state[i]);
        if (i%2 == 1) {
            printf(" ");
        }
    } printf("\n");
}
