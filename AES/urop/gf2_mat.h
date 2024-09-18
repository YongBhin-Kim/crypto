#ifndef GF2_MAT_H
#define GF2_MAT_H

#include <iostream>
#include <NTL/mat_GF2.h>

typedef unsigned char byte;

byte GF_add(byte gf1, byte gf2);
byte GF_xtime(byte gf);
byte GF_mul(byte f, byte g);
byte GF_inv(byte f);
void GF_print(byte gf);

byte AES_Affine(byte w);

void Get_AES_Sbox(byte sbox[256]);
void Get_AES_Inv_Sbox(byte isbox[256]);

NTL::mat_GF2 gen_gf2_rand_matrix(int dimension);
NTL::mat_GF2 gen_gf2_rand_invertible_matrix(int dimension);
// NTL::vec_GF2 scalar_to_vec_GF2(const uint8_t src);
// NTL::vec_GF2 scalar_to_vec_GF2(const uint32_t src);
// template <typename T>
// T vec_GF2_to_scalar(const NTL::vec_GF2 &src);
// uint8_t mat_GF2_times_vec_GF2(const NTL::mat_GF2 &x, const uint8_t y);
// uint32_t mat_GF2_times_vec_GF2(const NTL::mat_GF2 &x, const uint32_t y);

template<typename T>
inline NTL::vec_GF2 from_scalar(T in);

template<>
inline NTL::vec_GF2 from_scalar(uint8_t in) {
  NTL::vec_GF2 result;
  result.SetLength(8);
  for (int i = 0; i < 8; i++) {
    result[7 - i] = ((in >> i) & 1);
  }
  return result;
}

template<>
inline NTL::vec_GF2 from_scalar(uint32_t in) {
  NTL::vec_GF2 result;
  result.SetLength(32);
  for (int i = 0; i < 32; i++) {
    result[31 - i] = ((in >> i) & 1);
  }
  return result;
}

template<typename T>
inline T to_scalar(const NTL::vec_GF2& in);

template<>
inline uint8_t to_scalar(const NTL::vec_GF2& in) {
  uint8_t result = 0;
  for (int i = 0; i < 2; i++) {
    long i0 = NTL::rep(in[i*4+0]), i1 = NTL::rep(in[i*4+1]),
         i2 = NTL::rep(in[i*4+2]), i3 = NTL::rep(in[i*4+3]);
    result = (result << 4) | (i0 << 3) | (i1 << 2) | (i2 << 1) | (i3 << 0);
  }
  return result;
}

template<>
inline uint32_t to_scalar(const NTL::vec_GF2& in) {
  uint32_t result = 0;
  for (int i = 0; i < 8; i++) {
    long i0 = NTL::rep(in[i*4+0]), i1 = NTL::rep(in[i*4+1]),
         i2 = NTL::rep(in[i*4+2]), i3 = NTL::rep(in[i*4+3]);
    result = (result << 4) | (i0 << 3) | (i1 << 2) | (i2 << 1) | (i3 << 0);
  }
  return result;
}

template<typename T>
inline T mul(const NTL::mat_GF2& mat, T x) {
  return to_scalar<T>(mat * from_scalar<T>(x));
}


#endif /* GF2_MAT_H */