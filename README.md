# Crypto
현대 암호와 사용되는 수학



**[Environment]**
- MacOS Visual Studio Code 2

**[Language]**
- C
- C++

**[Implementation]**
- C style implementation
- C++ implemtation using Class

**[Run]**
- AES8 
- - 경로  `AES/AES8`
- - 명령어 `make` -> `./AES8`

- AES32
- - 경로  `AES/AES32`
- - 명령어 `make` -> `./AES32`

- BCM(Block Cipher Mode)
- - 경로 `AES/Mode/~`
- - 명령어 `make` -> `./~`

- ARIA


**[Coming soon]**

- Gray box 
- - FOCPA(First Order Correlation Power Attack) <br>
- - Masked AES resistant to FOCPA
- ARIA 

<h3/>대칭키 암호</h3>

**[AES]** <br>
대칭키 암호인 AES는 10라운드로 구성되며 `key gen` -> `1Round` -> `2Round` -> ... -> `10Round` 순으로 진행된다.<br>
1라운드 시작 전 `AddRoundKey` 연산을 1회 진행한다. <br>
각 라운드에서 `SBox` -> `ShiftRow` -> `Mixcolumns` -> `AddRoundKey` 순으로 진행된다. <br>
마지막 라운드 (10Round)에서는 AES의 대칭성을 고려하여 `Mixcolumns` 연산을 제외한다.<br>
<br>

**AES 8비트** <br>

AES의 SBox 연산은 유한체인 Rijndael Field (GF(2^8)) 에서 다루어지므로 기존의 연산과 다른 Quotient Field : GF(2^8) = GF(2)[x]/<m(x)>; 계수 = 0 or 1; 위에서의 연산으로 이루어진다. 따라서 GF(2^8) 위에서의 연산의 구현이 필요하다. <br> AES의 GF(2^8) 위에서의 기약다항식 m(x)는 x^8 + x^4 + x^3 + x + 1 으로 사용한다. <br>

- **(유한체 연산)**
- - 유한체 합연산 == 유한체 차연산 (계수가 GF(2)의 원소인 유한체 원소의 덧셈의 역원은 자기 자신이므로)
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
- - 위의 다항식은 gcd(mixcol_poly, primitive_poly) = 1 이어서 Extended Euclidean Algorithm에 의해 inverse mixcol_poly가 존재하며 구할 수 있고, 따라서 AES의 복호화가 가능하도록 선정한 다항식이다.
- - 위의 다항식은 branch bumber = 5가 되도록 선정하였다.
- - 위의 다항식은 연산의 편의성을 고려하여 선정한 다항식이다.
- - 위의 다항식은 Invmixcol_poly = mixcol_poly * primitive_poly 식이 성립한다.
- - 위의 다항식은 I = mixcol_poly ** 4 식이 성립한다.
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
- use `pre-computated Table 4` : 10 Round 0~4열에 해당하는 Table
<br>

- **(Key Scheduling)**
- AES의 key generation에 해당하는 부분 <br>
1 Round 이전, 10 Round Whitening Key로 이용하기 위해 AES-128 기준 44 word (32비트 : 11 * 4 word, 8비트 : 11 * 16 byte) 로 구성한다.
<br>


**AES 복호화** <br>

AES의 복호화를 위해 AES의 대칭성에 대하여 알아야 한다. <br>
AES의 10라운드에는 MixColumns이 없으므로 복호화는 `InvAddroundKey --> InvShiftRows --> InvSubBytes --> InvMixColumns --> InvAddroundKey -->` 순으로 진행된다. <br>
<br>

- **(Addroundkey)**
- AddRoundKey xor 연산이므로 그 역또한 xor 연산이다. 
- 따라서 복호화는  `AddroundKey --> InvShiftRows --> InvSubBytes --> InvMixColumns --> AddroundKey` 으로 표현 가능하다. <br>
<br>

- **(InvSubBytes)**
- SubBytes의 SBox의 역인 InvSBox로 대체하여 계산한다.
<br>

- **(InvShiftRow)**
- AES의 `InvShiftRows` 연산은 `ShiftRows`의 역연산으로 1행을 0번, 2행을 1번, 3행을 2번, 4행을 3번 right rotation 연산하며 이는 선형성의 특징이 있다.<br>
  ```row_1 = row_1 >>> 0
  row_2 = row_2 >>> 1
  row_3 = row_3 >>> 2
  row_4 = row_4 >>> 3
  ``` 
<br>

- **(InvMixColumns)**
- `InvMixColumns` 연산은 `MixColumns`의 역연산으로, MixColumns의 `fixed_a(x) = 0x03 x^3 + 0x01 x^2 + 0x01 x + 0x02` GF(2^8)^4에서의 역원을 구하면 `0x0b x^3 + 0x0d x^2 + 0x09 x + 0x0e` 이므로 InvMixColumns의 `fixed_a(x) = 0x0b x^3 + 0x0d x^2 + 0x09 x + 0x0e`로 사용하면 MixColumns의 역연산이 된다. 따라서, MixColumns가 선형 연산이기 때문에 InvMixColumns 연산 또한 선형 연산이다. <br>
<br>

`InvShiftRows` 연산과 `InvSubBytes` 연산의 순서는 바뀌어도 무관하기 때문에 `InvShiftRows --> InvSubBytes == InvSubBytes --> InvShiftRows` 로 조정 가능하다. <br>
`MixColumns` 는 선형 연산이라 했으므로 
`AddRoundKey --> MixColumns` <br>
 == `ARK(MC(state), rk)` <br>
 == `MC(state) ^ rk` <br>
 == `MC(state ^ InvMC(rk)) ` <br>
 == `MC(state ^ rk_prime) ` <br>
 == `MixColumns --> AddRoundKey_prime` <br>
으로 표현 가능하며, 따라서 AddRoundKey와 MixColumns 연산의 순서 또한 자유롭게 조정 가능하다. <br>
따라서 복호화 순서는 다음과 같이 변경 가능하다. <br>
```
AddRoundKey --> 
InvSubBytes --> InvShiftRows --> InvMixColumns --> AddRoundKey_prime --> ... --> AddRoundKey_prime --> 
InvSubBytes --> InvShiftRows --> AddRoundKey
```
정리하면 AES 복호화는 다음과 같은 순서로 진행된다. <br>
10 Round  : `AddRoundKey` <br>
9~1 Round : `InvSubBytes --> InvShiftRows --> InvMixColumns --> AddRoundKey_prime` <br>
0 Round   : `InvSubBytes --> InvShiftRows --> AddRoundKey` <br>
<br>

**AES key Schedule property** <br>

- AES의 임의의 라운드 키를 이용하여 모든 라운드 키를 찾을 수 있다. <br>
<br>

**Mode Of Operation, BCM(Block Cipher Mode)** <br>

- Implementation and test vectors for various modes of AES will be provided. <br>
- - Mode Of Operation Test : ECB, CBC, OFB, CFB, CTR, GCM <br>

- **(AES-ECB)**
- CAVP - KAT Test
- CAVP - MCT Test
- **(AES-CBC)**
- CAVP - MMT Test
- - Versus ECB
- CAVP - MCT Test

- **(AES-GCM)**
<br>

**AES Gray box** <br>

AES는 현재까지 존재하는 다양한 대칭키 암호 공격에 안전하다. 
하지만 AES 작동 시에 다양한 연산이 진행되며 이 때 발생하는 소비전력 등을 분석하는 관점에서는 순수한 AES가 안전하지 않을 수 있다.
<br>

- **(AES FOCPA(First Order Correlation Power Attack))**
- **(Masked AES resistant to FOCPA)** 
