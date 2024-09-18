<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="EUC-KR"%>
<%@ include file="KISA_SEED_ECB.jsp" %>
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
		if(i<data.length-1)
			result = result + ",";
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
byte bszUser_key[] = new byte[] {
		(byte)0x2b, (byte)0x7e, (byte)0x15, (byte)0x16, (byte)0x28, (byte)0xae, (byte)0xd2, (byte)0xa6,
        (byte)0xab, (byte)0xf7, (byte)0x15, (byte)0x88, (byte)0x09, (byte)0xcf, (byte)0x4f, (byte)0x3c
};


byte defaultPlainText[] = {
		(byte)0x00, (byte)0x01, (byte)0x02, (byte)0x03, (byte)0x04, (byte)0x05, (byte)0x06, (byte)0x07, (byte)0x08, (byte)0x09, (byte)0x0a, (byte)0x0b, (byte)0x0c, (byte)0x0d, (byte)0x0e, (byte)0x0f
};
byte[] defaultCipherText = SEED_ECB_Encrypt(bszUser_key, defaultPlainText, 0, defaultPlainText.length);

// method 1 start
String method = request.getParameter("method");
String encrypt_key = "";
String encrypt_plainText = "";
String encrypt_cipherText = "";
String decrypt_key = "";
String decrypt_plainText = "";
String decrypt_cipherText = "";
if(method != null && method.equals("e")) {
	byte[] key;
	String KeyStr = request.getParameter("encrypt_key");
	if (KeyStr == null)
	{
		key = bszUser_key;
	}
	else {
		key = getBytes(KeyStr);
	}
	String plainTextStr = request.getParameter("encrypt_plainText");
	byte[] plainText = getBytes(plainTextStr);
	String cipherTextStr = getString(SEED_ECB_Encrypt(key, plainText, 0, plainText.length));
	
	encrypt_plainText = plainTextStr;
	encrypt_key = KeyStr;
	encrypt_cipherText = cipherTextStr;
}
else if(method != null && method.equals("d")) {
	byte[] key;
	String KeyStr = request.getParameter("decrypt_key");
	if (KeyStr == null)
	{
		key = bszUser_key;
	}
	else {
		key = getBytes(KeyStr);
	}
	String cipherTextStr = request.getParameter("decrypt_cipherText");
	byte[] cipherText = getBytes(cipherTextStr);
	String plainTextStr = getString(SEED_ECB_Decrypt(key, cipherText, 0, cipherText.length));
	
	decrypt_plainText = plainTextStr;
	decrypt_key = KeyStr;
	decrypt_cipherText = cipherTextStr;
	
}
// method 1 end

/*
//method 2 start
String method = request.getParameter("method");
String encrypt_key = "";
String encrypt_plainText = "";
String encrypt_cipherText = "";
String decrypt_key = "";
String decrypt_plainText = "";
String decrypt_cipherText = "";
if(method != null && method.equals("e")) {
	byte[] key;
	String KeyStr = request.getParameter("encrypt_key");
	if (KeyStr == null)
	{
		key = bszUser_key;
	}
	else {
		key = getBytes(KeyStr);
	}
	String plainTextStr = request.getParameter("encrypt_plainText");
	byte[] plainText = getBytes(plainTextStr);
	
	KISA_SEED_INFO info = new KISA_SEED_INFO();
	int message_length = plainText.length; 
	
	SEED_ECB_init( info, KISA_ENC_DEC.KISA_ENCRYPT, key);
	
	int process_blockLeng = 32;
	int[] outbuf = new int[process_blockLeng];
	int i;
	int[] data;
	byte[] cdata;
	int nRetOutLeng[] = new int[] { 0 };
	int nPaddingLeng[] = new int[] { 0 };
	byte[] pbszPlainText = new byte[process_blockLeng];
	int nPlainTextPadding = (BLOCK_SIZE_SEED - (message_length)%BLOCK_SIZE_SEED);
	byte[] pbszCipherText = new byte[message_length + nPlainTextPadding];
	
	for (i = 0; i < message_length - process_blockLeng; )
	{
		arraycopy_system(plainText, i, pbszPlainText, 0, process_blockLeng);
		data = chartoint32_for_SEED_ECB(pbszPlainText, process_blockLeng);
		SEED_ECB_Process( info, data, process_blockLeng, outbuf, nRetOutLeng );
		cdata = int32tochar_for_SEED_ECB(outbuf, nRetOutLeng[0]);
		arraycopy_system(cdata, 0, pbszCipherText, i, nRetOutLeng[0]);
		i += nRetOutLeng[0];
	}
	int remainleng = message_length % process_blockLeng;
	if (remainleng == 0)
	{
		remainleng = process_blockLeng;
	}
	arraycopy_system(plainText, i, pbszPlainText, 0, remainleng);
	data = chartoint32_for_SEED_ECB(pbszPlainText, remainleng);
	SEED_ECB_Process( info, data, remainleng, outbuf, nRetOutLeng );
	cdata = int32tochar_for_SEED_ECB(outbuf, nRetOutLeng[0]);
	arraycopy_system(cdata, 0, pbszCipherText, i, nRetOutLeng[0]);
	i += nRetOutLeng[0];
	
	SEED_ECB_Close( info, outbuf, 0, nPaddingLeng );
	cdata = int32tochar_for_SEED_ECB(outbuf, nPaddingLeng[0]);
	arraycopy_system(cdata, 0, pbszCipherText, i, nPaddingLeng[0]);
	
	data = null;
	cdata = null;
	outbuf = null;
	
	encrypt_plainText = plainTextStr;
	encrypt_key = KeyStr;
	encrypt_cipherText = getString(pbszCipherText);
}
else if(method != null && method.equals("d")) {
	byte[] key;
	String KeyStr = request.getParameter("decrypt_key");
	if (KeyStr == null)
	{
		key = bszUser_key;
	}
	else {
		key = getBytes(KeyStr);
	}
	String cipherTextStr = request.getParameter("decrypt_cipherText");
	byte[] cipherText = getBytes(cipherTextStr);
	
	KISA_SEED_INFO info = new KISA_SEED_INFO();

	int i;
	int[] data;
	byte[] cdata;
	int nRetOutLeng[] = new int[] { 0 };
	int nPaddingLeng[] = new int[] { 0 };

	int EncryptedMessage_length = cipherText.length; 
	if (EncryptedMessage_length % BLOCK_SIZE_SEED > 0)
	{
		decrypt_key = KeyStr;
		decrypt_cipherText = cipherTextStr;
		byte[] result = {};
		decrypt_plainText = getString(result);
	}
	else
	{
		SEED_ECB_init( info, KISA_ENC_DEC.KISA_DECRYPT, key);
		
		int process_blockLeng = 32;
		
		int[] outbuf = new int[process_blockLeng];
		
		byte[] pbszCipherText = new byte[process_blockLeng];
		byte[] pbszPlainText = new byte[EncryptedMessage_length];

		for (i = 0; i < EncryptedMessage_length - process_blockLeng; )
		{
			arraycopy_system(cipherText, i, pbszCipherText, 0, process_blockLeng);
			data = chartoint32_for_SEED_ECB(pbszCipherText, process_blockLeng);
			SEED_ECB_Process( info, data, process_blockLeng, outbuf, nRetOutLeng );
			cdata = int32tochar_for_SEED_ECB(outbuf, nRetOutLeng[0]);
			arraycopy_system(cdata, 0, pbszPlainText, i, nRetOutLeng[0]);
			i += nRetOutLeng[0];
		}
		
		int remainleng = EncryptedMessage_length % process_blockLeng;
		if (remainleng == 0)
		{
			remainleng = process_blockLeng;
		}
		arraycopy_system(cipherText, i, pbszCipherText, 0, remainleng);
		data = chartoint32_for_SEED_ECB(pbszCipherText, remainleng);
		SEED_ECB_Process( info, data, remainleng, outbuf, nRetOutLeng );
		if (SEED_ECB_Close( info, outbuf, nRetOutLeng[0], nPaddingLeng ) > 0 )
		{
			cdata = int32tochar_for_SEED_ECB( outbuf, remainleng - nPaddingLeng[0] );
			arraycopy_system(cdata, 0, pbszPlainText, i, remainleng - nPaddingLeng[0]);
			int message_length = i + remainleng - nPaddingLeng[0];
			
			byte[] result = new byte[message_length];
			arraycopy_system(pbszPlainText, 0, result, 0, message_length);
			decrypt_plainText = getString(result);
			data = null;
			cdata = null;
			outbuf = null;
		}
		else
		{
			byte[] result = {};
			decrypt_plainText = getString(result);
		}
		decrypt_key = KeyStr;
		decrypt_cipherText = cipherTextStr;
	}
}
//method 2 end
*/
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<title>SEED ECB Example</title>
<script type="text/javascript">
function encrypt()
{
	var frm = document.myform;
	document.getElementById("method").value = "e";
	frm.submit();
}

function decrypt()
{
	var frm = document.myform;
	document.getElementById("method").value = "d";
	frm.submit();
}

</script>
</head>
<body>

<center>
<h1>���� ��ȣ [SEED-ECB] �׽�Ʈ ������</h1>
<form name="myform" method="post" action="index.jsp">
<input type="hidden" name="method" id="method" />
<table border="0">
<tr>
<td style="text-align:center;">
<table border="0">
<tr><td></td><td>&lt;��ȣȭ ����&gt;</td></tr>
<tr>
<td>Ű(KEY) : </td> 
<td><textarea name="encrypt_key" style="width:400px;height:100px;"><%=encrypt_key %></textarea></td>
</tr>
<tr>
<td>�� : </td> 
<td><textarea name="encrypt_plainText" style="width:400px;height:100px;"><%=encrypt_plainText %></textarea></td>
</tr>
<tr>
<td></td>
<td><button onclick="encrypt();">���ȣȭ</button></td>
</tr>
<tr>
<td>��ȣ�� : </td> 
<td><textarea name="encrypt_cipherText" style="width:400px;height:100px;"><%=encrypt_cipherText %></textarea></td>
</tr>
</table>
</td>
<td style="width:1px;background-color:#aaaaff;">
</td>
<td style="text-align:center;">
<table border="0">
<tr><td></td><td>&lt;��ȣȭ ����&gt;</td></tr>
<tr>
<td>Ű(KEY) : </td> 
<td><textarea name="decrypt_key" style="width:400px;height:100px;"><%=decrypt_key %></textarea></td>
</tr>
<tr>
<td>��ȣ�� : </td> 
<td><textarea name="decrypt_cipherText" style="width:400px;height:100px;"><%=decrypt_cipherText %></textarea></td>
</tr>
<tr>
<td></td>
<td><button onclick="decrypt();">�庹ȣȭ</button></td>
</tr>
<tr>
<td>�� : </td> 
<td><textarea name="decrypt_plainText" style="width:400px;height:100px;"><%=decrypt_plainText %></textarea></td>
</tr>
</table>
</td>
</tr>
</table>
<div style="margin-top:20px;margin-bottom:20px;"><font color="#ff0000">�� �� �� ��ȣ���� Hex ���� 0x�� �����ϰ� �޸��� �����Ͽ� ���� ���� �Է��մϴ�.(ex : 00,01,0A,0B)</font></div>
<div style="border: 1px solid #aaaaff; background-color:#ddddff;">
<table border="0">
<tr>
<td>&lt;Ű(KEY)&gt; : </td>
<td><% out.print(getString(bszUser_key)); %></td>
</tr>
<tr>
<td>&lt;��&gt; : </td>
<td><% out.print(getString(defaultPlainText)); %></td>
</tr>
<tr>
<td>&lt;��ȣ��&gt; : </td>
<td><% out.print(getString(defaultCipherText)); %></td>
</tr>
</table>
</div>
</form>

</center>
</body>
</html>