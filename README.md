# Crypto
현대 암호와 사용되는 수학

**[Environment]**
- Visual Studio Code 2

**[Language]**
- C
- C++

<h3/>대칭키 암호</h3>

**[AES]** <br>
AES의 SBox 연산은 유한체인 Rijndael Field (GF(2^8)) 에서 다루어지므로 기존의 연산과 다른 Quotient Field : GF(2)[x]/<m(x)>; 계수 = 0 or 1; 위에서의 연산으로 이루어진다. <br>
따라서 GF(2^8) 위에서의 연산의 구현이 필요하다. <br>
AES의 GF(2^8) 위에서의 기약다항식 m(x)는 x^8 + x^4 + x^3 + x + 1 으로 사용한다. <br>
- GF_Math.c : 유한체 GF(2^8)위에서의 연산 구현
- xtime, 합연산, 차연산, 곱연산, 나눗셈 연산, 역원 연산
- - GF_print(poly)
- - GF_add(poly 1, poly 2)
- - GF_sub(poly 1, poly 2)
- - GF_xtime(poly)
- - GF_mul(poly 1, poly 2)
- - GF_div(poly 1, poly 2)
- - gcd(poly 1, poly 2)
- - xgcd(poly 1, poly 2)
- - xgcd_poly(poly 1, poly 2) // Extended Euclidean Algorithm over GF(2^8), 구현중
- - GF_inv(poly)
- Sbox.cpp : 유한체 GF(2^8)위에서의 연산을 이용하여 AES Sbox를 구한다.
- - Get_AES_Sbox(Sbox)
- - Get_AES_ISbox(Inverse of Sbox)

AES의 MixColumns 연산은 Quotient Ring : GF(2^8)[x]/<x^4 + 1>; 계수 = 0 or 1 or 2 or ... or 255; 위에서의 연산으로 이루어진다. <br>
원소인 (a0 a1 a2 a3) 의 계수 ai는 GF(2^8)의 원소이다. 즉 0~255 사이의 숫자이며, 일대일 대응되는 다항식을 가진다. <br>
