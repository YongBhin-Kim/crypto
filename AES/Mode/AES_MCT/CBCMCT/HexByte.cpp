/*************************************************
*   HexByte - Treating Hex value and byte value
**************************************************/

#include "HexByte.h"

/*************************************************
* Name:         isHex
* 
* Description:  If input valuee is hexa value then return true
*			    
* Arguments:    char ch (1 byte)
**************************************************/
bool isHex(char ch) {
	return ((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'F')) || ((ch >= 'a') && (ch <= 'f'));
}

/*************************************************
* Name:         Hex2Digit
* 
* Description:  input  : ch  '0' '1' ... '9'  or 'a' ... 'f' or 'A' ... 'F'
*				output : 숫자  0   1  ...  15
*			    
* Arguments:    char ch		문자열
**************************************************/
byte Hex2Digit(char ch) {
	if (!isHex(ch)) {
		cout << ch << " is not a hex value." << endl;
		return -1;
	}
	if ((ch >= '0') && (ch <= '9')) {
		return  ch - '0'; // 예: '7' - '4' = 3
	}
	else if ((ch >= 'A') && (ch <= 'F')) {
		return ch - 'A' + 10;
	}
	else if ((ch >= 'a') && (ch <= 'f')) {
		return ch - 'a' + 10;
	}
	cout << "Unknown error." << endl;
	return -1;
}

/*************************************************
* Name:         Hex2Byte
* 
* Description:  ['8','d']		-->		8*16 + 13
*			    [문자열,문자열]		-->		숫자
* 
* Arguments:    const char h[2]   문자열 배열
**************************************************/
byte Hex2Byte(const char h[2]) {
	byte upper, lower; // (ex) upper = x  lower = y --> ret = xy
	upper = h[0];
	lower = h[1];
	if ((!isHex(upper)) || (!isHex(lower))) {
		cout << "Hex Error" << endl;
		return -1;
	}
	return Hex2Digit(upper) * 16 + Hex2Digit(lower);
}

/*************************************************
* Name:         Hex2Array
* 
* Description:  문자열 -> 바이트 배열
*			    
* Arguments:    const char hex_str[]  입력 (문자열)
				int hex_len			  문자열의 길이(32) // 바이트배열의 크기(16) <- hex_len / 2
				byte barr[]			  출력 (바이트 배열)
**************************************************/
void Hex2Array(const char hex_str[], int hex_len, byte barr[]) {
	char h[2];
	byte b_value;
	for (int i = 0; i < hex_len / 2; i++) {
		h[0] = hex_str[2 * i];
		h[1] = hex_str[2 * i + 1];
		b_value = Hex2Byte(h);
		barr[i] = b_value;
	}
}

/*************************************************
* Name:         print_b_array
* 
* Description:  				
*			    
* Arguments:    byte b_arr[]		바이트 배열
* 				int len				바이트 배열 크기 (128비트 --> 16칸)
*				const char* pStr	추가로 출력 문자열 (기본값 : nullptr)
**************************************************/
void print_b_array(byte b_arr[], int len, const char* pStr) {
	if (pStr != nullptr) {
		printf("%s = ", pStr);
	}
	for (int i = 0; i < len; i++) {
		printf("%02x ", b_arr[i]);
	}
	printf("\n");
}

/*************************************************
* Name:         copy_b_array
* 
* Description:  바이트 배열 복사 (ex, using as previous_ct)
**************************************************/
void copy_b_array(byte src[], int len, byte dest[]) {
	for (int i = 0; i < len; i++) {
		dest[i] = src[i];
	}
}

/*************************************************
* Name:         xor_b_array
* 
* Description:  바이트 배열에 다른 배열의 값을 XOR 하기
*			    주의 : X <- X xor Y
* 				data[] <- data[] xor xor_arr[]
**************************************************/
void xor_b_array(byte data[], int len, byte xor_arr[]) {
	for (int i = 0; i < len; i++) {
		data[i] ^= xor_arr[i];
	}
}

/*************************************************
* Name:         hex_test
* 
* Description:  test function
**************************************************/
void hex_test() {
	cout << "isHex('a') = " << isHex('a') << endl;
	cout << "Hex2Digit('a') = " << (int)Hex2Digit('a') << endl;

	char H[2] = { 'f', 'f' };
	cout << "Hex2Byte(H) = " << (int)Hex2Byte(H) << endl;
	const char* HexString = "8d2e60365f17c7df1040d7501b4a7b5a";
	byte HexArray[16];
	Hex2Array(HexString, 32, HexArray);
	
	cout << HexString << " = ";
	print_b_array(HexArray, 16);

	cout << "Print array..." << endl;
	print_b_array(HexArray, 16, "Byte Array = ");
}