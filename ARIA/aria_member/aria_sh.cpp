#include <stdio.h>

#define ENC 0
#define DEC 1
using Byte = unsigned char;

const Byte S1[256] = {
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
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};
const Byte S2[256] = {
    0xe2, 0x4e, 0x54, 0xfc, 0x94, 0xc2, 0x4a, 0xcc, 0x62, 0x0d, 0x6a, 0x46, 0x3c, 0x4d, 0x8b, 0xd1,
    0x5e, 0xfa, 0x64, 0xcb, 0xb4, 0x97, 0xbe, 0x2b, 0xbc, 0x77, 0x2e, 0x03, 0xd3, 0x19, 0x59, 0xc1,
    0x1d, 0x06, 0x41, 0x6b, 0x55, 0xf0, 0x99, 0x69, 0xea, 0x9c, 0x18, 0xae, 0x63, 0xdf, 0xe7, 0xbb,
    0x00, 0x73, 0x66, 0xfb, 0x96, 0x4c, 0x85, 0xe4, 0x3a, 0x09, 0x45, 0xaa, 0x0f, 0xee, 0x10, 0xeb,
    0x2d, 0x7f, 0xf4, 0x29, 0xac, 0xcf, 0xad, 0x91, 0x8d, 0x78, 0xc8, 0x95, 0xf9, 0x2f, 0xce, 0xcd,
    0x08, 0x7a, 0x88, 0x38, 0x5c, 0x83, 0x2a, 0x28, 0x47, 0xdb, 0xb8, 0xc7, 0x93, 0xa4, 0x12, 0x53,
    0xff, 0x87, 0x0e, 0x31, 0x36, 0x21, 0x58, 0x48, 0x01, 0x8e, 0x37, 0x74, 0x32, 0xca, 0xe9, 0xb1,
    0xb7, 0xab, 0x0c, 0xd7, 0xc4, 0x56, 0x42, 0x26, 0x07, 0x98, 0x60, 0xd9, 0xb6, 0xb9, 0x11, 0x40,
    0xec, 0x20, 0x8c, 0xbd, 0xa0, 0xc9, 0x84, 0x04, 0x49, 0x23, 0xf1, 0x4f, 0x50, 0x1f, 0x13, 0xdc,
    0xd8, 0xc0, 0x9e, 0x57, 0xe3, 0xc3, 0x7b, 0x65, 0x3b, 0x02, 0x8f, 0x3e, 0xe8, 0x25, 0x92, 0xe5,
    0x15, 0xdd, 0xfd, 0x17, 0xa9, 0xbf, 0xd4, 0x9a, 0x7e, 0xc5, 0x39, 0x67, 0xfe, 0x76, 0x9d, 0x43,
    0xa7, 0xe1, 0xd0, 0xf5, 0x68, 0xf2, 0x1b, 0x34, 0x70, 0x05, 0xa3, 0x8a, 0xd5, 0x79, 0x86, 0xa8,
    0x30, 0xc6, 0x51, 0x4b, 0x1e, 0xa6, 0x27, 0xf6, 0x35, 0xd2, 0x6e, 0x24, 0x16, 0x82, 0x5f, 0xda,
    0xe6, 0x75, 0xa2, 0xef, 0x2c, 0xb2, 0x1c, 0x9f, 0x5d, 0x6f, 0x80, 0x0a, 0x72, 0x44, 0x9b, 0x6c,
    0x90, 0x0b, 0x5b, 0x33, 0x7d, 0x5a, 0x52, 0xf3, 0x61, 0xa1, 0xf7, 0xb0, 0xd6, 0x3f, 0x7c, 0x6d,
    0xed, 0x14, 0xe0, 0xa5, 0x3d, 0x22, 0xb3, 0xf8, 0x89, 0xde, 0x71, 0x1a, 0xaf, 0xba, 0xb5, 0x81};
const Byte iS1[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};
const Byte iS2[256] = {
    0x30, 0x68, 0x99, 0x1b, 0x87, 0xb9, 0x21, 0x78, 0x50, 0x39, 0xdb, 0xe1, 0x72, 0x09, 0x62, 0x3c,
    0x3e, 0x7e, 0x5e, 0x8e, 0xf1, 0xa0, 0xcc, 0xa3, 0x2a, 0x1d, 0xfb, 0xb6, 0xd6, 0x20, 0xc4, 0x8d,
    0x81, 0x65, 0xf5, 0x89, 0xcb, 0x9d, 0x77, 0xc6, 0x57, 0x43, 0x56, 0x17, 0xd4, 0x40, 0x1a, 0x4d,
    0xc0, 0x63, 0x6c, 0xe3, 0xb7, 0xc8, 0x64, 0x6a, 0x53, 0xaa, 0x38, 0x98, 0x0c, 0xf4, 0x9b, 0xed,
    0x7f, 0x22, 0x76, 0xaf, 0xdd, 0x3a, 0x0b, 0x58, 0x67, 0x88, 0x06, 0xc3, 0x35, 0x0d, 0x01, 0x8b,
    0x8c, 0xc2, 0xe6, 0x5f, 0x02, 0x24, 0x75, 0x93, 0x66, 0x1e, 0xe5, 0xe2, 0x54, 0xd8, 0x10, 0xce,
    0x7a, 0xe8, 0x08, 0x2c, 0x12, 0x97, 0x32, 0xab, 0xb4, 0x27, 0x0a, 0x23, 0xdf, 0xef, 0xca, 0xd9,
    0xb8, 0xfa, 0xdc, 0x31, 0x6b, 0xd1, 0xad, 0x19, 0x49, 0xbd, 0x51, 0x96, 0xee, 0xe4, 0xa8, 0x41,
    0xda, 0xff, 0xcd, 0x55, 0x86, 0x36, 0xbe, 0x61, 0x52, 0xf8, 0xbb, 0x0e, 0x82, 0x48, 0x69, 0x9a,
    0xe0, 0x47, 0x9e, 0x5c, 0x04, 0x4b, 0x34, 0x15, 0x79, 0x26, 0xa7, 0xde, 0x29, 0xae, 0x92, 0xd7,
    0x84, 0xe9, 0xd2, 0xba, 0x5d, 0xf3, 0xc5, 0xb0, 0xbf, 0xa4, 0x3b, 0x71, 0x44, 0x46, 0x2b, 0xfc,
    0xeb, 0x6f, 0xd5, 0xf6, 0x14, 0xfe, 0x7c, 0x70, 0x5a, 0x7d, 0xfd, 0x2f, 0x18, 0x83, 0x16, 0xa5,
    0x91, 0x1f, 0x05, 0x95, 0x74, 0xa9, 0xc1, 0x5b, 0x4a, 0x85, 0x6d, 0x13, 0x07, 0x4f, 0x4e, 0x45,
    0xb2, 0x0f, 0xc9, 0x1c, 0xa6, 0xbc, 0xec, 0x73, 0x90, 0x7b, 0xcf, 0x59, 0x8f, 0xa1, 0xf9, 0x2d,
    0xf2, 0xb1, 0x00, 0x94, 0x37, 0x9f, 0xd0, 0x2e, 0x9c, 0x6e, 0x28, 0x3f, 0x80, 0xf0, 0x3d, 0xd3,
    0x25, 0x8a, 0xb5, 0xe7, 0x42, 0xb3, 0xc7, 0xea, 0xf7, 0x4c, 0x11, 0x33, 0x03, 0xa2, 0xac, 0x60};

const Byte C1[16] = {0x51, 0x7c, 0xc1, 0xb7, 0x27, 0x22, 0x0a, 0x94, 0xfe, 0x13, 0xab, 0xe8, 0xfa, 0x9a, 0x6e, 0xe0};
const Byte C2[16] = {0x6d, 0xb1, 0x4a, 0xcc, 0x9e, 0x21, 0xc8, 0x20, 0xff, 0x28, 0xb1, 0xd5, 0xef, 0x5d, 0xe2, 0xb0};
const Byte C3[16] = {0xdb, 0x92, 0x37, 0x1d, 0x21, 0x26, 0xe9, 0x70, 0x03, 0x24, 0x97, 0x75, 0x04, 0xe8, 0xc9, 0x0e};


class ARIA
{
    public:

    int R;                      // Round: 12(128bits), 14(192bits), 16(256bits)
    const Byte *K[3];           // Initialization constants
    Byte **EncKey = nullptr;    // Encrypt round keys
    Byte **DecKey = nullptr;    // Decrypt round keys

    ARIA(int bits = 128) {      // Constructor
        if (bits == 128) {
            R = 12; K[0] = C1; K[1] = C2; K[2] = C3;            
        } else if (bits == 192) {
            R = 14; K[0] = C2; K[1] = C3; K[2] = C1;
        } else {
            R = 16; K[0] = C3; K[1] = C1; K[2] = C2;
        }

        EncKey = new Byte* [R+1];
        for (int i = 0; i < R+1; i++) { EncKey[i] = new Byte[16](); }
        DecKey = new Byte* [R+1];
        for (int i = 0; i < R+1; i++) { DecKey[i] = new Byte[16](); }
    }
    ~ARIA() {           // Destructor
        for (int i = 0; i < R+1; i++) { delete[] EncKey[i]; delete[] DecKey[i]; }
        delete[] EncKey; delete[] DecKey;
    }

    void Fo(const Byte *In, const Byte *Key, Byte *Out);
    void Fe(const Byte *In, const Byte *Key, Byte *Out);
    void diffLayer(const Byte *In, Byte *Out);
    void setupEncKey(const Byte *In1, const Byte *In2, int Shift, const int Round);
    void setupRKey(const Byte *Key, const int KeySize);
};

void ARIA::Fo(const Byte *In, const Byte *Key, Byte* Out)
{
    int i;
    Byte temp[16];

    for (i = 0; i < 16; i++) { temp[i] = In[i] ^ Key[i]; }
    for (i = 0; i <= 12; i += 4)
    {
        temp[i] = S1[temp[i]];
        temp[i+1] = S2[temp[i+1]];
        temp[i+2] = iS1[temp[i+2]];
        temp[i+3] = iS2[temp[i+3]];
    }

    diffLayer(temp, Out);
}

void ARIA::Fe(const Byte *In, const Byte *Key, Byte* Out)
{
    int i;
    Byte temp[16];
    
    for (i = 0 ; i < 16; i++) { temp[i] = In[i] ^ Key[i]; }
    for (i = 0; i <= 12; i += 4)
    {
        temp[i] = iS1[temp[i]];
        temp[i+1] = iS2[temp[i+1]];
        temp[i+2] = S1[temp[i+2]];
        temp[i+3] = S2[temp[i+3]];
    }

    diffLayer(temp, Out);
}

void ARIA::diffLayer(const Byte *In, Byte* Out)
{
    Byte temp;

    temp = In[3] ^ In[4] ^ In[9] ^ In[14];
    Out[0] = In[6] ^ In[8] ^ In[13] ^ temp;
    Out[5] = In[1] ^ In[10] ^ In[15] ^ temp;
    Out[11] = In[2] ^ In[7] ^ In[12] ^ temp;
    Out[14] = In[0] ^ In[5] ^ In[11] ^ temp;

    temp = In[2] ^ In[5] ^ In[8] ^ In[15];
    Out[1] = In[7] ^ In[9] ^ In[12] ^ temp;
    Out[4] = In[0] ^ In[11] ^ In[14] ^ temp;
    Out[10] = In[3] ^ In[6] ^ In[13] ^ temp;
    Out[15] = In[1] ^ In[4] ^ In[10] ^ temp;
    
    temp = In[1] ^ In[6] ^ In[11] ^ In[12];
    Out[2] = In[4] ^ In[10] ^ In[15] ^ temp;
    Out[7] = In[3] ^ In[8] ^ In[13] ^ temp;
    Out[9] = In[0] ^ In[5] ^ In[14] ^ temp;
    Out[12] = In[2] ^ In[7] ^ In[9] ^ temp;
    
    temp = In[0] ^ In[7] ^ In[10] ^ In[13];
    Out[3] = In[5] ^ In[11] ^ In[14] ^ temp;
    Out[6] = In[2] ^ In[9] ^ In[12] ^ temp;
    Out[8] = In[1] ^ In[4] ^ In[15] ^ temp;
    Out[13] = In[3] ^ In[6] ^ In[8] ^ temp;
}

void ARIA::setupEncKey(const Byte *In1, const Byte *In2, int Shift, const int Round)
{
    int j = Shift / 8;
    Shift %= 8;

    for (int i = 0; i < 16; i++)
    {
        EncKey[Round][(i + j) % 16] ^= (In2[i] >> Shift);
        EncKey[Round][(i + j + 1) % 16] ^= (In2[i] << (8 - Shift));
    }

    for (int i = 0; i < 16; i++) { EncKey[Round][i] ^= In1[i]; }
}

void ARIA::setupRKey(const Byte *Key, const int KeySize)
{
    int i;
    Byte KL[16]{0}, KR[16]{0}, W0[16], W1[16], W2[16], W3[16];
    
    for (i = 0; i < 16; i++) { KL[i] = W0[i] = Key[i]; }
    for (; i < KeySize; i++) { KR[i-16] = Key[i]; }

    Fo(W0, K[0], W1);
    for (i = 0; i < 16; i++) { W1[i] ^= KR[i]; }

    Fe(W1, K[1], W2);
    for (i = 0; i < 16; i++) { W2[i] ^= W0[i]; }

    Fo(W2, K[2], W3);
    for (i = 0; i < 16; i++) { W3[i] ^= W1[i]; }

    setupEncKey(W0, W1, 19, 0); setupEncKey(W1, W2, 19, 1); setupEncKey(W2, W3, 19, 2); setupEncKey(W3, W0, 19, 3);
    setupEncKey(W0, W1, 31, 4); setupEncKey(W1, W2, 31, 5); setupEncKey(W2, W3, 31, 6); setupEncKey(W3, W0, 31, 7);

    setupEncKey(W0, W1, 128-61, 8); setupEncKey(W1, W2, 128-61, 9); setupEncKey(W2, W3, 128-61, 10);
    setupEncKey(W3, W0, 128-61, 11); setupEncKey(W0, W1, 128-31, 12);

    if (R > 12) { setupEncKey(W1, W2, 128-31, 13); setupEncKey(W2, W3, 128-31, 14); }
    if (R > 14) { setupEncKey(W3, W0, 128-31, 15); setupEncKey(W0, W1, 128-19, 16); }

    for (int i = 0; i < 16; i++) { DecKey[0][i] = EncKey[R][i]; DecKey[R][i] = EncKey[0][i]; }
    for (int i = 1; i < R; i++) { diffLayer(EncKey[R-i], DecKey[i]); }
}

void runAria(ARIA *aria, Byte *In, Byte *Out, int OPTION)
{
    if (aria == nullptr) return;
    if (aria->EncKey == nullptr || aria->DecKey == nullptr) return;

    Byte state[16] = {0, };
    Byte **OP;
    int i, j, Round = aria->R;

    if (OPTION == ENC) { OP = aria->EncKey; }
    else { OP = aria->DecKey; }

    for (i = 0; i < 16; i++) { state[i] = In[i]; }

    for (i = 0; i < Round-1; i++) { i % 2 == 0 ? aria->Fo(state, OP[i], Out) : aria->Fe(Out, OP[i], state); }

    // Ff
    for (j = 0; j < 16; j++) { Out[j] = Out[j] ^ OP[i][j]; }
    for (j = 0; j <= 12; j += 4)
    {
        Out[j] = iS1[Out[j]];
        Out[j+1] = iS2[Out[j+1]];
        Out[j+2] = S1[Out[j+2]];
        Out[j+3] = S2[Out[j+3]];
    }
    for (j = 0; j < 16; j++) { Out[j] = Out[j] ^ OP[Round][j]; }
}

int main(void)
{
    const Byte Key_128[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                              0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    // const Byte Key_192[24] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    //                           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    //                           0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    // const Byte Key_256[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    //                           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    //                           0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    //                           0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
    Byte Plaintext[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    Byte Ciphertext[16] = {0x00, };
    Byte Decrypted[16] = {0x00, };

    ARIA* aria_128 = new ARIA(128);
    aria_128->setupRKey(Key_128, 16);
    runAria(aria_128, Plaintext, Ciphertext, ENC);

    printf("Plaintext = \n");
    for (int i = 0; i < 16; i++) { printf("%02x", Plaintext[i]); }
    printf("\n");

    printf("Masterkey = \n");
    for (int i = 0; i < 16; i++) { printf("%02x", Key_128[i]); }
    printf("\n");

    printf("Ciphertext = \n");
    for (int i = 0; i < 16; i++) { printf("%02x", Ciphertext[i]); }
    printf("\n");

    runAria(aria_128, Ciphertext, Decrypted, DEC);

    printf("Decrypted = \n");
    for (int i = 0; i < 16; i++) { printf("%02x", Decrypted[i]); }
    printf("\n");

    delete aria_128;
    return 0;
}