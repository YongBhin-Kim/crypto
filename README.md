# Crypto
현대 암호와 사용되는 수학


<h3/>대칭키 암호</h3>

**[AES]** <br>
AES의 SBox는 유한체인 Rijndael Field (GF(2^8)) 에서 다루어지므로 기존의 연산과 다른 Quotient Field 위에서의 연산으로 이루어진다. <br>
따라서 GF(2^8) 위에서의 연산의 구현이 필요하다. <br>
AES의 GF(2^8) 위에서의 기약다항식 m(x)는 x^8 + x^4 + x^3 + x + 1 으로 사용한다. <br>
- GF_Math.c : 유한체 GF(2^8)에서의 연산
- - xtime, 합연산, 차연산, 곱연산, 나눗셈 연산, 역원 연산
