# Crypto
현대 암호와 사용되는 수학

**[Environment]**
- Visual Studio Code 2

**[Language]**
- C
- C++

<h3/>대칭키 암호</h3>

**[AES]** <br>
AES의 SBox 연산은 유한체인 Rijndael Field (GF(2^8)) 에서 다루어지므로 기존의 연산과 다른 Quotient Field 위에서의 연산으로 이루어진다. <br>
따라서 GF(2^8) 위에서의 연산의 구현이 필요하다. <br>
AES의 GF(2^8) 위에서의 기약다항식 m(x)는 x^8 + x^4 + x^3 + x + 1 으로 사용한다. <br>
- GF_Math.c : 유한체 GF(2^8)위에서의 연산
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

