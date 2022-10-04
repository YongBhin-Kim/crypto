# Crypto
현대 암호와 사용되는 수학

**[Environment]**
- Visual Studio Code 2

**[Language]**
- C
- C++

**[Run]**
- Makefile 만들 예정
- AES8 
- - 경로  `AES/AES8`
- - 명령어 `g++ -o AES8.exec AES8.cpp ../Math/GF_Matrix_Math.cpp ../Math/GF_Math.c -o AES8`

- AES32
- - 경로  `AES/AES32`
- - 명령어 `g++ -o AES32.exec AES32.cpp  -o AES32`

<h3/>대칭키 암호</h3>

**[AES]** <br>
대칭키 암호인 AES는 10라운드로 구성되며 `key gen` -> `1Round` -> `2Round` -> ... -> `10Round` 순으로 진행된다.<br>
1라운드 시작 전 `AddRoundKey` 연산을 1회 진행한다. <br>
각 라운드에서 `SBox` -> `ShiftRow` -> `Mixcolumns` -> `AddRoundKey` 순으로 진행된다. <br>
마지막 라운드 (10Round)에서는 `Mixcolumns` 연산을 제외한다.
<br>

**AES 8비트** <br>

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
  ```row_1 = row_1 <<< 0
  row_2 = row_2 <<< 1
  row_3 = row_3 <<< 2
  row_4 = row_4 <<< 3
  ``` 
<br>

- **(MixColumns)**
- AES의 MixColumns 연산은 Quotient Ring : GF(2^8)^4 = GF(2^8)[x]/<x^4 + 1>; 계수 = 0 or 1 or 2 or ... or 255; 위에서의 연산으로 이루어진다. <br>
Quotient Ring의 원소인 fixed_a(x) = (a0 a1 a2 a3) (== (3 1 1 2)) 의 계수 ai는 GF(2^8)의 원소이다. 즉 0~255 사이의 숫자이며, 각각 일대일 대응되는 다항식을 가진다. <br> 선형 변환인 MixColumns는 행렬로 표현 가능하며 quotient ring GF(2^8)^4 위에서의 연산을 이용하여 AES Mincolumns 연산을 구현한다. 행렬 연산의 원소들은 각각 GF(2^8)의 원소이므로 원소들의 연산도 유한체 위에서의 연산으로 진행한다.
- `fixed_a(x) = 0x03 x^3 + 0x01 x^2 + 0x01 x + 0x02`
- `output_column_vector = fixed_Matrix_MC * input_column_vector`
<br>

- **(AddRoundKey)**
- AES의 AddRoundKey 연산은 input과 key를 xor연산하여 출력한다.
- `output = input ^ key`
<br>

**AES 32비트** <br>
- AES의 1~9라운드의 라운드 함수는 SubBytes --> ShiftRows --> MixColumns --> AddRoundKey 순으로 진행된다. <br>
AES 8비트의 4*4행렬의 열들을 각각 Little Endian 방식으로 이어붙인다고 생각하면 1개의 행과 4개의 32비트 크기의 열이 존재하는 배열로 생각할 수 있다.<br>
SubBytes 연산에 사용되는 SBox는 모든 256개의 원소에 대해 미리 계산하여 테이블 형태로 메모리에 저장해둘 수 있다. <br>
MixColumns 연산은 Quotient Ring의 고정된 다항식이며 고정된 행렬로 변환하여 생각할 수 있다고 앞서 언급하였다. <br>
행렬과 벡터의 곱이므로 행렬의 각 열을 하나의 32bit unsigned int(Little Endian)로 생각한다면 4개의 벡터와 벡터의 내적으로 변환할 수 있고, 그 결과값인 scalar(물론 여기서는 GF(2^8)의 원소 4개의 이어짐)의 합 4번으로 MixColumns연산을 수행할 수 있다. 여기서 말하는 합은 GF(2^8)에서의 합이므로 xor연산으로 처리한다. <br>
MixColumns 연산 이전에 수행되는 ShiftRows 연산은 1행, 2행, 3행을 각각 1,2,3 회 left rotate연산을 수행하는데, 행렬의 1행, 2행, 3행을 각각 1,2,3 회 right rotate한 위치에서 원소를 가져와서 MixColumns의 0열, 1열, 2열, 3열에 삽입한다. <br>
<br>
AES의 10라운드는 MixColumns 연산이 제외되므로 고정된 행렬을 단위행렬로 계산하여 테이블을 이용한다. <br>
이 방식으로 사전 계산된 테이블을 이용하여 AES 32비트를 구현한다. <br>

- use `pre-computated Table 0~3` : 1-9 Round 0-4 Column 에 해당하는 Table
- use `pre-computated Table 4 : 10 Round 0~4열에 해당하는 Table
<br>


