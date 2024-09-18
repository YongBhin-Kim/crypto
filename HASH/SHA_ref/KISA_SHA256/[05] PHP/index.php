<?
require_once ('KISA_SHA256.php');

function encrypt($str) {
	$planBytes = array_slice(unpack('c*',$str), 0); // 평문을 바이트 배열로 변환
	$ret = null;
	$bszChiperText = null;
	KISA_SEED_SHA256::SHA256_Encrypt($planBytes, count($planBytes), $bszChiperText);
	$r = count($bszChiperText);

	foreach($bszChiperText as $encryptedString) {
		$ret .= bin2hex(chr($encryptedString)); // 암호화된 16진수 스트링 추가 저장
	}
	return $ret;
}

$enc = $_POST['ENC'];
$check = $_POST['check'];
//암호화 시작
if($enc != NULL || $check == 1)
{
	$g_bszPlainText = $enc;
	$enc2 = encrypt($g_bszPlainText);
}

?>
<head>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 
<title>[SHA256] 테스트 페이지</title>
</head>
<body>
<center>
<h1>[SHA256] 테스트 페이지</h1>
<table border="0">
<tr>
<td style="text-align:center;">
<form method="post" action="./index.php">
<input type = "hidden" name = "check" value = "1">
<table border="0">
<tr><td></td><td>&lt;암호화 예제&gt;</td></tr>
<tr>
<td>평문 : </td> 
<td><textarea name="ENC" style="width:400px;height:100px;"><?=$enc?></textarea></td>
</tr>
<tr>
<td></td>
<td><input type="submit" name="gogo" value="▼ 암호화"> </td>
</tr>
<tr>
<td>암호문 : </td> 
<td><textarea name="ENC2" style="width:400px;height:100px;"><?=$enc2?></textarea></td>
</tr>
</table>
</form>	
</td>
</tr>
</table>
<div style="margin-top:20px;margin-bottom:20px;"><font color="#ff0000"></font></div>
<div style="border: 1px solid #aaaaff; background-color:#ddddff;">
</div>
</form>

</center>
</body>
</html>