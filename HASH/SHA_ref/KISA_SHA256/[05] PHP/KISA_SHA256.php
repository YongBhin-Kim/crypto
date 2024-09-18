<?php
/*
@file KISA_SHA256.php
@brief SHA256 암호 알고리즘
@author Copyright (c) 2013 by KISA
@remarks http://seed.kisa.or.kr/
*/

if(!defined('_KISA_COMMON_LIB')) {
define('_KISA_COMMON_LIB', 1);

function isLittleEndian() {
	$testint = 0x00FF;
	$p = pack('S', $testint);
	return $testint===current(unpack('v', $p));
}
$_kisa_common_is_little_endian = isLittleEndian();
if(!defined('_KISA_COMMON_IS_LITTLE_ENDIAN')) {
	define('_KISA_COMMON_IS_LITTLE_ENDIAN', $_kisa_common_is_little_endian);
}

class Common {
	static function arraycopy(&$dst, &$src, $length) {
		for($i=0; $i<$length; $i++) {
			$dst[$i] = $src[$i];
		}
	}

	static function arraycopy_offset(&$dst, $dst_offset, &$src, $src_offset, $length) {
		for($i=0; $i<$length; $i++) {
			$dst[$dst_offset+$i] = $src[$src_offset+$i];
		}
	}

	static function arraycopy_system(&$src, $src_offset, &$dst, $dst_offset, $length) {
		for($i=0; $i<$length; $i++) {
			$dst[$dst_offset+$i] = $src[$src_offset+$i];
		}
	}

	static function arrayinit(&$dst, $value, $length) {
		for($i=0; $i<$length; $i++) {
			$dst[$i] = $value;
		}
	}

	static function arrayinit_offset(&$dst, $dst_offset, $value, $length) {
		for($i=0; $i<$length; $i++) {
			$dst[$dst_offset+$i] = $value;
		}
	}

	static function memcpy_byte2int(&$dst, &$src, $length) {
		$iLen = (int)($length / 4);
		for($i=0; $i<$iLen; $i++) {
			Common::byte_to_int($dst, $i, $src, $i*4);
		}
	}

	static function memcpy_int2int(&$dst, &$src, $src_offset, $length) {
		$iLen = (int)($length / 4);
		for($i=0; $i<$iLen; $i++) {
			$dst[$i] = $src[$src_offset+$i];
		}
	}

	static function set_byte_for_int(&$dst, $b_offset, $value) {
		if(_KISA_COMMON_IS_LITTLE_ENDIAN == true) {
			$shift_value = (3-$b_offset%4)*8;
			$mask_value =  0x0ff << $shift_value;
			$mask_value2 = ~$mask_value;
			$value2 = ($value&0x0ff) << $shift_value;
			$dst[(int)($b_offset/4)] = ($dst[(int)($b_offset/4)] & $mask_value2) | ($value2 & $mask_value);    
		}
		else {
			$shift_value = ($b_offset%4)*8;
			$mask_value =  0x0ff << $shift_value;
			$mask_value2 = ~$mask_value;
			$value2 = ($value&0x0ff) << $shift_value;
			$dst[(int)($b_offset/4)] = ($dst[(int)($b_offset/4)] & $mask_value2) | ($value2 & $mask_value);    
		}
	}

	static function get_byte_for_int(&$src, $b_offset) {
		if(_KISA_COMMON_IS_LITTLE_ENDIAN == true) {
			$shift_value = (3-$b_offset%4)*8;
			$mask_value =  0x0ff << $shift_value;
			$value = ($src[(int)($b_offset/4)] & $mask_value) >> $shift_value;
			return $value & 0x0ff;
		}
		else {
			$shift_value = ($b_offset%4)*8;
			$mask_value =  0x0ff << $shift_value;
			$value = ($src[$b_offset/4] & $mask_value) >> $shift_value;
			return $value & 0x0ff;
		}
	}

	static function byte_to_int(&$dst, $dst_offset, &$src, $src_offset) {
		if(_KISA_COMMON_IS_LITTLE_ENDIAN == true) {
			$dst[$dst_offset] = ((0x0ff&$src[$src_offset]) << 24) | ((0x0ff&$src[$src_offset+1]) << 16) | ((0x0ff&$src[$src_offset+2]) << 8) | ((0x0ff&$src[$src_offset+3]));
		}
		else {
			$dst[$dst_offset] = ((0x0ff&$src[$src_offset])) | ((0x0ff&$src[$src_offset+1]) << 8) | ((0x0ff&$src[$src_offset+2]) << 16) | ((0x0ff&$src[$src_offset+3]) << 24);
		}
	}

	static function get_byte_to_int(&$src, $src_offset) {
		if(_KISA_COMMON_IS_LITTLE_ENDIAN == true) {
			return ((0x0ff&$src[$src_offset]) << 24) | ((0x0ff&$src[$src_offset+1]) << 16) | ((0x0ff&$src[$src_offset+2]) << 8) | ((0x0ff&$src[$src_offset+3]));
		}
		else {
			return ((0x0ff&$src[$src_offset])) | ((0x0ff&$src[$src_offset+1]) << 8) | ((0x0ff&$src[$src_offset+2]) << 16) | ((0x0ff&$src[$src_offset+3]) << 24);
		}
	}

	static function int_to_byte(&$dst, $dst_offset, &$src, $src_offset) {
		Common::int_to_byte_unit($dst, $dst_offset, $src[$src_offset]);
	}

	static function int_to_byte_unit(&$dst, $dst_offset, $src) {
		if(_KISA_COMMON_IS_LITTLE_ENDIAN == true) {
			$dst[$dst_offset] = (($src>> 24) & 0x0ff);
			$dst[$dst_offset+1] = (($src >> 16) & 0x0ff);
			$dst[$dst_offset+2] = (($src >> 8) & 0x0ff);
			$dst[$dst_offset+3] = (($src) & 0x0ff);
		} else {
			$dst[$dst_offset] = (($src) & 0x0ff);
			$dst[$dst_offset+1] = (($src >> 8) & 0x0ff);
			$dst[$dst_offset+2] = (($src >> 16) & 0x0ff);
			$dst[$dst_offset+3] = (($src >> 24) & 0x0ff);
		}
		
	}

	static function URShift($x, $n) {
		if($n == 0)
			return $x;
		if($n >= 32)
			return 0;
		$v = $x >> $n;
		$v_mask = ~(0x80000000 >> ($n-1));
		return $v & $v_mask;
	}
	
	static function intToUnsigned($x) {
		if($x >= 0)
			return $x;
		return $x + pow(2, 32);
	}

}

function test() {
	$s = array( 0x12345678 );
	$b = array_pad(array(), 4, 0);
	
	Common::int_to_byte($b, 0, $s, 0);
	Common::set_byte_for_int($s, 0, 0x34);
}


}


if(!defined('_SHA256_INFO_')) {
define('_SHA256_INFO_', '1');
class SHA256_INFO {
	var $uChainVar = null;
	var $uHighLength = 0;
	var $uLowLength = 0;
	var $szBuffer = null;

	function SHA256_INFO() {
		$this->uChainVar = array_pad(array(), 32/4, 0);
		$this->szBuffer = array_pad(array(), 64, 0);
	}

}
}

class KISA_SEED_SHA256 {

	const SHA256_DIGEST_BLOCKLEN = 64;
	const SHA256_DIGEST_VALUELEN = 32;

	static $SHA256_K = array(
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
		0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
		0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
		0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
		0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
		0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
		0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
		0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	);
    static function GetB0($A){ return 0x000000ff & $A; } 

	static function ROTL_ULONG($x, $n) {
		return ($x << $n) | Common::URShift($x, 32 - $n);
	}

	static function ROTR_ULONG($x, $n) {
		return Common::URShift($x, $n) | ($x << (32 - ($n)));
	}

	static function ENDIAN_REVERSE_ULONG($dwS) {
		return ( (KISA_SEED_SHA256::ROTL_ULONG(($dwS),  8) & 0x00ff00ff) | (KISA_SEED_SHA256::ROTL_ULONG(($dwS), 24) & 0xff00ff00) );
	}

	static function BIG_D2B($D, &$B, $B_offset) {
		Common::int_to_byte_unit($B, $B_offset, $D);
	}

	static function RR($x, $n) { return KISA_SEED_SHA256::ROTR_ULONG($x, $n); }
	static function SS($x, $n) { return Common::URShift($x, $n); }

	static function Ch($x, $y, $z) { return (($x & $y) ^ ((~$x) & $z)); }
	static function Maj($x, $y, $z) { return (($x & $y) ^ ($x & $z) ^ ($y & $z)); }
	static function Sigma0($x) { return (KISA_SEED_SHA256::RR($x,  2) ^ KISA_SEED_SHA256::RR($x, 13) ^ KISA_SEED_SHA256::RR($x, 22)); }
	static function Sigma1($x) { return (KISA_SEED_SHA256::RR($x,  6) ^ KISA_SEED_SHA256::RR($x, 11) ^ KISA_SEED_SHA256::RR($x, 25)); }

	static function RHO0($x) { return (KISA_SEED_SHA256::RR($x,  7) ^ KISA_SEED_SHA256::RR($x, 18) ^ KISA_SEED_SHA256::SS($x,  3)); }
	static function RHO1($x) { return (KISA_SEED_SHA256::RR($x, 17) ^ KISA_SEED_SHA256::RR($x, 19) ^ KISA_SEED_SHA256::SS($x, 10)); }

	static $abcdefgh_a = 0;
	static $abcdefgh_b = 1;
	static $abcdefgh_c = 2;
	static $abcdefgh_d = 3;
	static $abcdefgh_e = 4;
	static $abcdefgh_f = 5;
	static $abcdefgh_g = 6;
	static $abcdefgh_h = 7;

	// 0xFFFFFFFF 를 & 연산 시키는 이유: 64bit OS에 설치된 PHP의 정수형 데이터 타입의 크기 차이로 인한 오버플로우 처리 오류 
	static function FF(&$abcdefgh, $a, $b, $c, $d, $e, $f, $g, $h, &$X, $j) {
		$T1 = null;

		$T1 = 
			Common::intToUnsigned($abcdefgh[$h]) + 
			Common::intToUnsigned(KISA_SEED_SHA256::Sigma1($abcdefgh[$e])) + 
			Common::intToUnsigned(KISA_SEED_SHA256::Ch($abcdefgh[$e], $abcdefgh[$f], $abcdefgh[$g])) + 
			Common::intToUnsigned(KISA_SEED_SHA256::$SHA256_K[$j]) + 
			Common::intToUnsigned($X[$j]);

		$T1 &= 0xFFFFFFFF;

		$abcdefgh[$d] += $T1;
		$abcdefgh[$d] &= 0xFFFFFFFF;

		$abcdefgh[$h] = (int)($T1 + Common::intToUnsigned(KISA_SEED_SHA256::Sigma0($abcdefgh[$a])) + Common::intToUnsigned(KISA_SEED_SHA256::Maj($abcdefgh[$a], $abcdefgh[$b], $abcdefgh[$c])));
		$abcdefgh[$h] &= 0xFFFFFFFF;
	}

	static function GetData(&$x, $x_offset) {
		return Common::get_byte_to_int($x, $x_offset);
	}

	// 0xFFFFFFFF 를 & 연산 시키는 이유: 64bit OS에 설치된 PHP의 정수형 데이터 타입의 크기 차이로 인한 오버플로우 처리 오류 
	static function SHA256_Transform(&$Message, &$ChainVar) {
		$abcdefgh = array_pad(array(),8,0);//int abcdefgh[] = new int[8];
		$T1 = array_pad(array(),1,0);
		$X = array_pad(array(),64,0);
		$j = 0;

		for ($j = 0; $j < 16; $j++)
			$X[$j] = KISA_SEED_SHA256::GetData($Message, $j*4);

		for ($j = 16; $j < 64; $j++)
		{
			$X[$j] = (int)(Common::intToUnsigned(KISA_SEED_SHA256::RHO1($X[$j - 2])) + Common::intToUnsigned($X[$j - 7]) + Common::intToUnsigned(KISA_SEED_SHA256::RHO0($X[$j - 15])) + Common::intToUnsigned($X[$j - 16]));
			$X[$j] &= 0xFFFFFFFF;
		}

		$abcdefgh[KISA_SEED_SHA256::$abcdefgh_a] = $ChainVar[0];
		$abcdefgh[KISA_SEED_SHA256::$abcdefgh_b] = $ChainVar[1];
		$abcdefgh[KISA_SEED_SHA256::$abcdefgh_c] = $ChainVar[2];
		$abcdefgh[KISA_SEED_SHA256::$abcdefgh_d] = $ChainVar[3];
		$abcdefgh[KISA_SEED_SHA256::$abcdefgh_e] = $ChainVar[4];
		$abcdefgh[KISA_SEED_SHA256::$abcdefgh_f] = $ChainVar[5];
		$abcdefgh[KISA_SEED_SHA256::$abcdefgh_g] = $ChainVar[6];
		$abcdefgh[KISA_SEED_SHA256::$abcdefgh_h] = $ChainVar[7];

		for ($j = 0; $j < 64; $j += 8)
		{
			KISA_SEED_SHA256::FF($abcdefgh, KISA_SEED_SHA256::$abcdefgh_a, KISA_SEED_SHA256::$abcdefgh_b, KISA_SEED_SHA256::$abcdefgh_c, KISA_SEED_SHA256::$abcdefgh_d, KISA_SEED_SHA256::$abcdefgh_e, KISA_SEED_SHA256::$abcdefgh_f, KISA_SEED_SHA256::$abcdefgh_g, KISA_SEED_SHA256::$abcdefgh_h, $X, $j + 0);
			KISA_SEED_SHA256::FF($abcdefgh, KISA_SEED_SHA256::$abcdefgh_h, KISA_SEED_SHA256::$abcdefgh_a, KISA_SEED_SHA256::$abcdefgh_b, KISA_SEED_SHA256::$abcdefgh_c, KISA_SEED_SHA256::$abcdefgh_d, KISA_SEED_SHA256::$abcdefgh_e, KISA_SEED_SHA256::$abcdefgh_f, KISA_SEED_SHA256::$abcdefgh_g, $X, $j + 1);
			KISA_SEED_SHA256::FF($abcdefgh, KISA_SEED_SHA256::$abcdefgh_g, KISA_SEED_SHA256::$abcdefgh_h, KISA_SEED_SHA256::$abcdefgh_a, KISA_SEED_SHA256::$abcdefgh_b, KISA_SEED_SHA256::$abcdefgh_c, KISA_SEED_SHA256::$abcdefgh_d, KISA_SEED_SHA256::$abcdefgh_e, KISA_SEED_SHA256::$abcdefgh_f, $X, $j + 2);
			KISA_SEED_SHA256::FF($abcdefgh, KISA_SEED_SHA256::$abcdefgh_f, KISA_SEED_SHA256::$abcdefgh_g, KISA_SEED_SHA256::$abcdefgh_h, KISA_SEED_SHA256::$abcdefgh_a, KISA_SEED_SHA256::$abcdefgh_b, KISA_SEED_SHA256::$abcdefgh_c, KISA_SEED_SHA256::$abcdefgh_d, KISA_SEED_SHA256::$abcdefgh_e, $X, $j + 3);
			KISA_SEED_SHA256::FF($abcdefgh, KISA_SEED_SHA256::$abcdefgh_e, KISA_SEED_SHA256::$abcdefgh_f, KISA_SEED_SHA256::$abcdefgh_g, KISA_SEED_SHA256::$abcdefgh_h, KISA_SEED_SHA256::$abcdefgh_a, KISA_SEED_SHA256::$abcdefgh_b, KISA_SEED_SHA256::$abcdefgh_c, KISA_SEED_SHA256::$abcdefgh_d, $X, $j + 4);
			KISA_SEED_SHA256::FF($abcdefgh, KISA_SEED_SHA256::$abcdefgh_d, KISA_SEED_SHA256::$abcdefgh_e, KISA_SEED_SHA256::$abcdefgh_f, KISA_SEED_SHA256::$abcdefgh_g, KISA_SEED_SHA256::$abcdefgh_h, KISA_SEED_SHA256::$abcdefgh_a, KISA_SEED_SHA256::$abcdefgh_b, KISA_SEED_SHA256::$abcdefgh_c, $X, $j + 5);
			KISA_SEED_SHA256::FF($abcdefgh, KISA_SEED_SHA256::$abcdefgh_c, KISA_SEED_SHA256::$abcdefgh_d, KISA_SEED_SHA256::$abcdefgh_e, KISA_SEED_SHA256::$abcdefgh_f, KISA_SEED_SHA256::$abcdefgh_g, KISA_SEED_SHA256::$abcdefgh_h, KISA_SEED_SHA256::$abcdefgh_a, KISA_SEED_SHA256::$abcdefgh_b, $X, $j + 6);
			KISA_SEED_SHA256::FF($abcdefgh, KISA_SEED_SHA256::$abcdefgh_b, KISA_SEED_SHA256::$abcdefgh_c, KISA_SEED_SHA256::$abcdefgh_d, KISA_SEED_SHA256::$abcdefgh_e, KISA_SEED_SHA256::$abcdefgh_f, KISA_SEED_SHA256::$abcdefgh_g, KISA_SEED_SHA256::$abcdefgh_h, KISA_SEED_SHA256::$abcdefgh_a, $X, $j + 7);
		}

		$ChainVar[0] += $abcdefgh[KISA_SEED_SHA256::$abcdefgh_a];
		$ChainVar[0] &= 0xFFFFFFFF;
		$ChainVar[1] += $abcdefgh[KISA_SEED_SHA256::$abcdefgh_b];
		$ChainVar[1] &= 0xFFFFFFFF;
		$ChainVar[2] += $abcdefgh[KISA_SEED_SHA256::$abcdefgh_c];
		$ChainVar[2] &= 0xFFFFFFFF;
		$ChainVar[3] += $abcdefgh[KISA_SEED_SHA256::$abcdefgh_d];
		$ChainVar[3] &= 0xFFFFFFFF;
		$ChainVar[4] += $abcdefgh[KISA_SEED_SHA256::$abcdefgh_e];
		$ChainVar[4] &= 0xFFFFFFFF;
		$ChainVar[5] += $abcdefgh[KISA_SEED_SHA256::$abcdefgh_f];
		$ChainVar[5] &= 0xFFFFFFFF;
		$ChainVar[6] += $abcdefgh[KISA_SEED_SHA256::$abcdefgh_g];
		$ChainVar[6] &= 0xFFFFFFFF;
		$ChainVar[7] += $abcdefgh[KISA_SEED_SHA256::$abcdefgh_h];
		$ChainVar[7] &= 0xFFFFFFFF;
	}

	/**
	@brief 연쇄변수와 길이변수를 초기화하는 함수
	@param Info : SHA256_Process 호출 시 사용되는 구조체
	*/
	static function SHA256_Init( &$Info ) {
		$Info->uChainVar[0] = 0x6a09e667;
		$Info->uChainVar[1] = 0xbb67ae85;
		$Info->uChainVar[2] = 0x3c6ef372;
		$Info->uChainVar[3] = 0xa54ff53a;
		$Info->uChainVar[4] = 0x510e527f;
		$Info->uChainVar[5] = 0x9b05688c;
		$Info->uChainVar[6] = 0x1f83d9ab;
		$Info->uChainVar[7] = 0x5be0cd19;
		$Info->uHighLength = $Info->uLowLength = 0;
	}

	/**
	@brief 연쇄변수와 길이변수를 초기화하는 함수
	@param Info : SHA256_Init 호출하여 초기화된 구조체(내부적으로 사용된다.)
	@param pszMessage : 사용자 입력 평문
	@param uDataLen : 사용자 입력 평문 길이
	*/
	static function SHA256_Process( &$Info, &$pszMessage, $uDataLen ) {
		$pszMessage_offset;

		if(($Info->uLowLength += ($uDataLen << 3)) < 0) {
			$Info->uHighLength++;
		}

		$Info->uHighLength += Common::URShift($uDataLen,29);

		$pszMessage_offset = 0;
		while ($uDataLen >= KISA_SEED_SHA256::SHA256_DIGEST_BLOCKLEN) {
			Common::arraycopy_offset($Info->szBuffer, 0, $pszMessage, $pszMessage_offset, KISA_SEED_SHA256::SHA256_DIGEST_BLOCKLEN);
			KISA_SEED_SHA256::SHA256_Transform($Info->szBuffer, $Info->uChainVar);
			$pszMessage_offset += KISA_SEED_SHA256::SHA256_DIGEST_BLOCKLEN;
			$uDataLen -= KISA_SEED_SHA256::SHA256_DIGEST_BLOCKLEN;
		}
		Common::arraycopy_offset($Info->szBuffer, 0, $pszMessage, $pszMessage_offset, $uDataLen);
	}

	/**
	@brief 메시지 덧붙이기와 길이 덧붙이기를 수행한 후 마지막 메시지 블록을 가지고 압축함수를 호출하는 함수
	@param Info : SHA256_Init 호출하여 초기화된 구조체(내부적으로 사용된다.)
	@param pszDigest : 암호문
	*/
	static function SHA256_Close( &$Info, &$pszDigest ) {
		$i = null;
		$Index = null;

		$Index = Common::URShift($Info->uLowLength, 3) % KISA_SEED_SHA256::SHA256_DIGEST_BLOCKLEN;
		$Info->szBuffer[$Index++] = 0x80 & 0x0ff;

		if ($Index > KISA_SEED_SHA256::SHA256_DIGEST_BLOCKLEN - 8) {
			Common::arrayinit_offset($Info->szBuffer, $Index, 0 & 0x0ff, KISA_SEED_SHA256::SHA256_DIGEST_BLOCKLEN - $Index);
			KISA_SEED_SHA256::SHA256_Transform($Info->szBuffer, $Info->uChainVar);
			Common::arrayinit($Info->szBuffer, 0 & 0x0ff, KISA_SEED_SHA256::SHA256_DIGEST_BLOCKLEN - 8);
		}
		else {
			Common::arrayinit_offset($Info->szBuffer, $Index, 0 & 0x0ff, KISA_SEED_SHA256::SHA256_DIGEST_BLOCKLEN - $Index - 8);
		}

		if(_KISA_COMMON_IS_LITTLE_ENDIAN == false) {
			$Info->uLowLength = KISA_SEED_SHA256::ENDIAN_REVERSE_ULONG($Info->uLowLength);
			$Info->uHighLength = KISA_SEED_SHA256::ENDIAN_REVERSE_ULONG($Info->uHighLength);
		}

		Common::int_to_byte_unit($Info->szBuffer, ((int)(KISA_SEED_SHA256::SHA256_DIGEST_BLOCKLEN / 4 - 2))*4, $Info->uHighLength);
		Common::int_to_byte_unit($Info->szBuffer, ((int)(KISA_SEED_SHA256::SHA256_DIGEST_BLOCKLEN / 4 - 1))*4, $Info->uLowLength);

		KISA_SEED_SHA256::SHA256_Transform($Info->szBuffer, $Info->uChainVar);

		for ($i = 0; $i < KISA_SEED_SHA256::SHA256_DIGEST_VALUELEN; $i += 4) {
			KISA_SEED_SHA256::BIG_D2B(($Info->uChainVar[$i / 4]), $pszDigest, $i);
		}
	}
	
	/**
	@brief 사용자 입력 평문을 한번에 처리
	@param pszMessage : 사용자 입력 평문
	@param uPlainTextLen : 평문 길이
	@param pszDigest : 암호문
	@remarks 내부적으로 SHA256_Init, SHA256_Process, SHA256_Close를 호출한다.
	*/
	static function SHA256_Encrypt( &$pszMessage, $uPlainTextLen, &$pszDigest ) {
		$info = new SHA256_INFO();
		KISA_SEED_SHA256::SHA256_Init( $info );
		KISA_SEED_SHA256::SHA256_Process( $info, $pszMessage, $uPlainTextLen );
		KISA_SEED_SHA256::SHA256_Close( $info, $pszDigest );
	}
}


?>
