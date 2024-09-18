<%@ page language="java" contentType="text/html; charset=EUC-KR"
    pageEncoding="EUC-KR"%>
<%@ include file="KISA_SHA256.jsp" %>
<%!
public byte[] getBytes(String data) {
	String[] str = data.split(",");
	byte[] result = new byte[str.length];
	for(int i=0; i<result.length; i++) {
		result[i] = getHex(str[i]);
	}
	return result;
}

public String getString(byte[] data) {
	String result = "";
	for(int i=0; i<data.length; i++) {
		result = result + toHex(data[i]);
	}
	return result;
}

public byte getHex(String str) {
	str = str.trim();
	if(str.length() == 0)
		str = "00";
	else if(str.length() == 1)
		str = "0" + str;
	
	str = str.toUpperCase();
	return (byte)(getHexNibble(str.charAt(0)) * 16 + getHexNibble(str.charAt(1)));
}

public byte getHexNibble(char c) {
	if(c >= '0' && c<='9')
		return (byte)(c - '0');
	if(c >= 'A' && c<='F')
		return (byte)(c - 'A' + 10);
	return 0;
}

public String toHex(int b) {
	char c[] = new char[2];
	c[0] = toHexNibble((b>>4) & 0x0f);
	c[1] = toHexNibble(b & 0x0f);
	return new String(c);
}

public char toHexNibble(int b) {
	if(b >= 0 && b <= 9)
		return (char)(b + '0');
	if(b >= 0x0a && b <= 0x0f)
		return (char)(b + 'A' - 10);
	return '0';
}

%>

<%
byte defaultPlainText[] = {
		(byte)0x00, (byte)0x01, (byte)0x02, (byte)0x03, (byte)0x04, (byte)0x05, (byte)0x06, (byte)0x07, (byte)0x08, (byte)0x09, (byte)0x0a, (byte)0x0b, (byte)0x0c, (byte)0x0d, (byte)0x0e, (byte)0x0f
};
byte[] defaultCipherText = Sha256EncryptB(defaultPlainText);

String method = request.getParameter("method");
String encrypt_plainText = "";
String encrypt_cipherText = "";
String encrypt_cipherText_base64 = "";

if(method != null && method.equals("e")) {
	String plainTextStr = request.getParameter("encrypt_plainText");
	byte[] plainText = plainTextStr.getBytes("UTF-8");
	String cipherTextStr = getString(Sha256EncryptB(plainText));
 
	encrypt_plainText = plainTextStr;
	encrypt_cipherText = cipherTextStr;
	encrypt_cipherText_base64 = Sha256Encrypt(plainText);
	
}



%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<title>SHA256 Example</title>
<script type="text/javascript">
function encrypt()
{
	var frm = document.myform;
	document.getElementById("method").value = "e";
	frm.submit();
}


</script>
</head>
<body>

<center>
<h1>[SHA256] 테스트 페이지</h1>
<form name="myform" method="post" action="index.jsp">
<input type="hidden" name="method" id="method" />
<table border="0">
<tr>
<td style="text-align:center;">
<table border="0">
<tr><td></td><td>&lt;암호화 예제&gt;</td></tr>
<tr>
<td>평문(String) : </td> 
<td><textarea name="encrypt_plainText" style="width:400px;height:100px;"><%=encrypt_plainText %></textarea></td>
</tr>
<tr>
<td></td>
<td><button onclick="encrypt();">▼암호화</button></td>
</tr>
<tr>
<td>암호문(HEX) : </td> 
<td><textarea name="encrypt_cipherText" style="width:400px;height:100px;"><%=encrypt_cipherText %></textarea></td>
</tr>
</table>
</td>
</tr>
</table>
<table border="0">
<tr>
</tr>
</table>
<div style="border: 1px solid #aaaaff; background-color:#ddddff;">
</div>
</form>

</center>
</body>
</html>