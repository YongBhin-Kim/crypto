# Crypto
현대 암호와 사용되는 수학

**[Environment]**
- Visual Studio Code 2

**[Language]**
- C
- C++

**[Run]**
- Makefile 만들 예정
- `g++ -o MixCol.exec Matrix_ver2.cpp SBox.cpp GF_Math.c -o MixCol`

<h3/>대칭키 암호</h3>

**[AES]** <br>
대칭키 암호인 AES는 10라운드로 구성되며 `key gen` -> `1Round` -> `2Round` -> ... -> `10Round` 순으로 진행된다.<br>
각 라운드에서 `AddRoundKey` -> `SBox` -> `ShiftRow` -> `Mixcolumns` 순으로 진행된다. <br>
마지막 라운드 (10Round)에서는 `Mixcolumns` 연산을 제외하고 진행한다.
<br>

AES의 SBox 연산은 유한체인 Rijndael Field (GF(2^8)) 에서 다루어지므로 기존의 연산과 다른 Quotient Field : GF(2^8) = GF(2)[x]/<m(x)>; 계수 = 0 or 1; 위에서의 연산으로 이루어진다. 따라서 GF(2^8) 위에서의 연산의 구현이 필요하다. <br> AES의 GF(2^8) 위에서의 기약다항식 m(x)는 x^8 + x^4 + x^3 + x + 1 으로 사용한다. <br>

- **(유한체 연산)**
- - 유한체 합연산 == 유한체 차연산 (유한체의 덧셈의 역원은 자기 자신이므로)
- - 유한체 x배 == 유한체 2배
- - 유한체 곱연산
- - 유한체 나눗셈
- - Euclidean Algorithm
- - Extended Euclidean Algorithm
- - 유한체 Extended Euclidean Algorithm, 구현중
- - 유한체의 곱셈의 역원 - Little Fermat Theorem
<br>

- **(Create SBox Table)**
- 유한체 GF(2^8)위에서의 연산을 이용하여 AES Sbox를 구한다.
- Affine 변환 : `output = fixed_Matrix_A * input(^-1) + fixed_vector_b`
<br>

- **(SBox)**
- AES의 SBox 연산은 비선형 변환으로 input을 SBox에 집어넣어 출력값을 가진다.
- `output = SBox[input]`
<br>

- **(ShiftRow)**
- AES의 ShiftRow 연산은 1행을 0번, 2행을 1번, 3행을 2번, 4행을 3번 left rotation 연산한다.
- row_1 = row_1 <<< 0
- row_2 = row_2 <<< 1
- row_3 = row_3 <<< 2
- row_4 = row_4 <<< 3

- **(MixColumns)**
- AES의 MixColumns 연산은 Quotient Ring : GF(2^8)^4 = GF(2^8)[x]/<x^4 + 1>; 계수 = 0 or 1 or 2 or ... or 255; 위에서의 연산으로 이루어진다. <br>
Quotient Ring의 원소인 fixed_a(x) = (a0 a1 a2 a3) (== (3 1 1 2)) 의 계수 ai는 GF(2^8)의 원소이다. 즉 0~255 사이의 숫자이며, 각각 일대일 대응되는 다항식을 가진다. <br> 선형 변환인 MixColumns는 행렬로 표현 가능하며 quotient ring GF(2^8)^4 위에서의 연산을 이용하여 AES Mincolumns 연산을 구현한다. 행렬 연산의 원소들은 각각 GF(2^8)의 원소이므로 원소들의 연산도 유한체 위에서의 연산으로 진행한다.
- `fixed_a(x) = 0x03 x^3 + 0x01 x^2 + 0x01 x + 0x02`
- `output_column_vector = fixed_Matrix_MC * input_column_vector`
<br>

- **(AddRoundKey)**
- AES의 AddRoundKey 연산은 input과 key를 xor연산하여 출력한다.
- `output = input ^ key`
