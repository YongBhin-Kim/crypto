<!--#include file="KISA_SHA256.asp"-->
<%response.Charset = "utf-8"%>
<%
Dim enc2

enc = request.form("ENC")
check = request.form("check")

If check = 1 then
	enc2 = test(enc)
End If

function test(str)
	test = SHA256_Encrypt(str)
end function

%>
<head>
<html>
<head>
<title>[SHA256] 테스트 페이지</title>
</head>
<body>
<center>
<h1>[SHA256] 테스트 페이지</h1>
<form name="myform" method="post" aciton="./index.jsp">
<input type="hidden" name="method" id="method" />
<table border="0">
<tr>
<td style="text-align:center;">
<form method="post" action="./index.asp">
<input type = "hidden" name = "check" value = "1">
<table border="0">
<tr><td></td><td>&lt;암호화 예제&gt;</td></tr>
<tr>
<td>평문(String) : </td> 
<td><textarea name="ENC" style="width:400px;height:100px;"><%=enc%></textarea></td>
</tr>
<tr>
<td></td>
<td><input type="submit" name="gogo" value="▼ 암호화"> </td>
</tr>
<tr>
<td>암호문(HEX) : </td> 
<td><textarea name="ENC2" style="width:400px;height:100px;"><%=enc2 %></textarea></td>
</tr>
</table>
</form>	
</td>
</tr>
</table>
<div style="border: 1px solid #aaaaff; background-color:#ddddff;">
</div>
</form>

</center>
</body>
</html>