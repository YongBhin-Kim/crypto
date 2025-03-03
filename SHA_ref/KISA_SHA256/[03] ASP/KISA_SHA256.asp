<%
On Error Resume Next

Private m_lOnBits(30)
Private m_l2Power(30)
Private K

Private Const BITS_TO_A_BYTE = 8
Private Const BYTES_TO_A_WORD = 4
Private Const BITS_TO_A_WORD = 32

K = Array( _
	&H428A2F98, &H71374491, &HB5C0FBCF, &HE9B5DBA5, _
	&H3956C25B, &H59F111F1, &H923F82A4, &HAB1C5ED5, _
	&HD807AA98, &H12835B01, &H243185BE, &H550C7DC3, _
	&H72BE5D74, &H80DEB1FE, &H9BDC06A7, &HC19BF174, _
	&HE49B69C1, &HEFBE4786, &HFC19DC6, &H240CA1CC, _
	&H2DE92C6F, &H4A7484AA, &H5CB0A9DC, &H76F988DA, _
	&H983E5152, &HA831C66D, &HB00327C8, &HBF597FC7, _
	&HC6E00BF3, &HD5A79147, &H6CA6351, &H14292967, _
	&H27B70A85, &H2E1B2138, &H4D2C6DFC, &H53380D13, _
	&H650A7354, &H766A0ABB, &H81C2C92E, &H92722C85, _
	&HA2BFE8A1, &HA81A664B, &HC24B8B70, &HC76C51A3, _
	&HD192E819, &HD6990624, &HF40E3585, &H106AA070, _
	&H19A4C116, &H1E376C08, &H2748774C, &H34B0BCB5, _
	&H391C0CB3, &H4ED8AA4A, &H5B9CCA4F, &H682E6FF3, _
	&H748F82EE, &H78A5636F, &H84C87814, &H8CC70208, _
	&H90BEFFFA, &HA4506CEB, &HBEF9A3F7, &HC67178F2 _
)

m_lOnBits(0) = CLng(1)
m_lOnBits(1) = CLng(3)
m_lOnBits(2) = CLng(7)
m_lOnBits(3) = CLng(15)
m_lOnBits(4) = CLng(31)
m_lOnBits(5) = CLng(63)
m_lOnBits(6) = CLng(127)
m_lOnBits(7) = CLng(255)
m_lOnBits(8) = CLng(511)
m_lOnBits(9) = CLng(1023)
m_lOnBits(10) = CLng(2047)
m_lOnBits(11) = CLng(4095)
m_lOnBits(12) = CLng(8191)
m_lOnBits(13) = CLng(16383)
m_lOnBits(14) = CLng(32767)
m_lOnBits(15) = CLng(65535)
m_lOnBits(16) = CLng(131071)
m_lOnBits(17) = CLng(262143)
m_lOnBits(18) = CLng(524287)
m_lOnBits(19) = CLng(1048575)
m_lOnBits(20) = CLng(2097151)
m_lOnBits(21) = CLng(4194303)
m_lOnBits(22) = CLng(8388607)
m_lOnBits(23) = CLng(16777215)
m_lOnBits(24) = CLng(33554431)
m_lOnBits(25) = CLng(67108863)
m_lOnBits(26) = CLng(134217727)
m_lOnBits(27) = CLng(268435455)
m_lOnBits(28) = CLng(536870911)
m_lOnBits(29) = CLng(1073741823)
m_lOnBits(30) = CLng(2147483647)

m_l2Power(0) = CLng(1)
m_l2Power(1) = CLng(2)
m_l2Power(2) = CLng(4)
m_l2Power(3) = CLng(8)
m_l2Power(4) = CLng(16)
m_l2Power(5) = CLng(32)
m_l2Power(6) = CLng(64)
m_l2Power(7) = CLng(128)
m_l2Power(8) = CLng(256)
m_l2Power(9) = CLng(512)
m_l2Power(10) = CLng(1024)
m_l2Power(11) = CLng(2048)
m_l2Power(12) = CLng(4096)
m_l2Power(13) = CLng(8192)
m_l2Power(14) = CLng(16384)
m_l2Power(15) = CLng(32768)
m_l2Power(16) = CLng(65536)
m_l2Power(17) = CLng(131072)
m_l2Power(18) = CLng(262144)
m_l2Power(19) = CLng(524288)
m_l2Power(20) = CLng(1048576)
m_l2Power(21) = CLng(2097152)
m_l2Power(22) = CLng(4194304)
m_l2Power(23) = CLng(8388608)
m_l2Power(24) = CLng(16777216)
m_l2Power(25) = CLng(33554432)
m_l2Power(26) = CLng(67108864)
m_l2Power(27) = CLng(134217728)
m_l2Power(28) = CLng(268435456)
m_l2Power(29) = CLng(536870912)
m_l2Power(30) = CLng(1073741824)

Private Function LShift(lValue, iShiftBits)
    If iShiftBits = 0 Then
        LShift = lValue
        Exit Function
    ElseIf iShiftBits = 31 Then
        If lValue And 1 Then
            LShift = &H80000000
        Else
            LShift = 0
        End If
        Exit Function
    ElseIf iShiftBits < 0 Or iShiftBits > 31 Then
        Err.Raise 6
    End If
    
    If (lValue And m_l2Power(31 - iShiftBits)) Then
        LShift = ((lValue And m_lOnBits(31 - (iShiftBits + 1))) * m_l2Power(iShiftBits)) Or &H80000000
    Else
        LShift = ((lValue And m_lOnBits(31 - iShiftBits)) * m_l2Power(iShiftBits))
    End If
End Function

Private Function RShift(lValue, iShiftBits)
    If iShiftBits = 0 Then
        RShift = lValue
        Exit Function
    ElseIf iShiftBits = 31 Then
        If lValue And &H80000000 Then
            RShift = 1
        Else
            RShift = 0
        End If
        Exit Function
    ElseIf iShiftBits < 0 Or iShiftBits > 31 Then
        Err.Raise 6
    End If
    
    RShift = (lValue And &H7FFFFFFE) \ m_l2Power(iShiftBits)
    
    If (lValue And &H80000000) Then
        RShift = (RShift Or (&H40000000 \ m_l2Power(iShiftBits - 1)))
    End If
End Function

Private Function AddUnsigned(lX, lY)
    Dim lX4
    Dim lY4
    Dim lX8
    Dim lY8
    Dim lResult
 
    lX8 = lX And &H80000000
    lY8 = lY And &H80000000
    lX4 = lX And &H40000000
    lY4 = lY And &H40000000
 
    lResult = (lX And &H3FFFFFFF) + (lY And &H3FFFFFFF)
 
    If lX4 And lY4 Then
        lResult = lResult Xor &H80000000 Xor lX8 Xor lY8
    ElseIf lX4 Or lY4 Then
        If lResult And &H40000000 Then
            lResult = lResult Xor &HC0000000 Xor lX8 Xor lY8
        Else
            lResult = lResult Xor &H40000000 Xor lX8 Xor lY8
        End If
    Else
        lResult = lResult Xor lX8 Xor lY8
    End If
 
    AddUnsigned = lResult
End Function

Private Function Ch(x, y, z)
    Ch = ((x And y) Xor ((Not x) And z))
End Function

Private Function Maj(x, y, z)
    Maj = ((x And y) Xor (x And z) Xor (y And z))
End Function

Private Function S(x, n)
    S = (RShift(x, (n And m_lOnBits(4))) Or LShift(x, (32 - (n And m_lOnBits(4)))))
End Function

Private Function R(x, n)
    R = RShift(x, CInt(n And m_lOnBits(4)))
End Function

Private Function Sigma0(x)
    Sigma0 = (S(x, 2) Xor S(x, 13) Xor S(x, 22))
End Function

Private Function Sigma1(x)
    Sigma1 = (S(x, 6) Xor S(x, 11) Xor S(x, 25))
End Function

Private Function Gamma0(x)
    Gamma0 = (S(x, 7) Xor S(x, 18) Xor R(x, 3))
End Function

Private Function Gamma1(x)
    Gamma1 = (S(x, 17) Xor S(x, 19) Xor R(x, 10))
End Function

'@brief 연쇄변수와 길이변수를 초기화하는 함수
'@param sMessage : 사용자 입력 평문
Private Function SHA256_Process(sMessage)
    Dim lMessageLength
    Dim lNumberOfWords
    Dim lWordArray()
    Dim lBytePosition
    Dim lByteCount
    Dim lWordCount
    Dim lByte
    
    Const MODULUS_BITS = 512
    Const CONGRUENT_BITS = 448
    
    lMessageLength = Len(sMessage)
    
    lNumberOfWords = (((lMessageLength + ((MODULUS_BITS - CONGRUENT_BITS) \ BITS_TO_A_BYTE)) \ (MODULUS_BITS \ BITS_TO_A_BYTE)) + 1) * (MODULUS_BITS \ BITS_TO_A_WORD)
    ReDim lWordArray(lNumberOfWords - 1)
    
    lBytePosition = 0
    lByteCount = 0
    Do Until lByteCount >= lMessageLength
        lWordCount = lByteCount \ BYTES_TO_A_WORD
        
        lBytePosition = (3 - (lByteCount Mod BYTES_TO_A_WORD)) * BITS_TO_A_BYTE
        
        lByte = AscB(Mid(sMessage, lByteCount + 1, 1))

        lWordArray(lWordCount) = lWordArray(lWordCount) Or LShift(lByte, lBytePosition)
        lByteCount = lByteCount + 1
    Loop

    lWordCount = lByteCount \ BYTES_TO_A_WORD
    lBytePosition = (3 - (lByteCount Mod BYTES_TO_A_WORD)) * BITS_TO_A_BYTE

    lWordArray(lWordCount) = lWordArray(lWordCount) Or LShift(&H80, lBytePosition)

    lWordArray(lNumberOfWords - 1) = LShift(lMessageLength, 3)
    lWordArray(lNumberOfWords - 2) = RShift(lMessageLength, 29)
    
    SHA256_Process = lWordArray
End Function

'@brief 연쇄변수와 길이변수를 초기화하는 함수
Public Function SHA256_Init()
	Dim HASH(7)
    HASH(0) = &H6A09E667
    HASH(1) = &HBB67AE85
    HASH(2) = &H3C6EF372
    HASH(3) = &HA54FF53A
    HASH(4) = &H510E527F
    HASH(5) = &H9B05688C
    HASH(6) = &H1F83D9AB
    HASH(7) = &H5BE0CD19

	SHA256_Init = HASH

End Function

'@brief 메시지 덧붙이기와 길이 덧붙이기를 수행한 후 마지막 메시지 블록을 가지고 압축함수를 호출하는 함수
'@param HASH : SHA256_Init 호출하여 초기화된 구조체(내부적으로 사용된다.)
'@param M : SHA256_Process 로 생성된 암호문
Public Function SHA256_Close(HASH, M)

    Dim W(63)
    Dim a
    Dim b
    Dim c
    Dim d
    Dim e
    Dim f
    Dim g
    Dim h
    Dim i
    Dim j
    Dim T1
    Dim T2

    For i = 0 To UBound(M) Step 16
        a = HASH(0)
        b = HASH(1)
        c = HASH(2)
        d = HASH(3)
        e = HASH(4)
        f = HASH(5)
        g = HASH(6)
        h = HASH(7)
        
        For j = 0 To 63
            If j < 16 Then
                W(j) = M(j + i)
            Else
                W(j) = AddUnsigned(AddUnsigned(AddUnsigned(Gamma1(W(j - 2)), W(j - 7)), Gamma0(W(j - 15))), W(j - 16))
            End If
                
            T1 = AddUnsigned(AddUnsigned(AddUnsigned(AddUnsigned(h, Sigma1(e)), Ch(e, f, g)), K(j)), W(j))
            T2 = AddUnsigned(Sigma0(a), Maj(a, b, c))
            
            h = g
            g = f
            f = e
            e = AddUnsigned(d, T1)
            d = c
            c = b
            b = a
            a = AddUnsigned(T1, T2)
        Next
        
        HASH(0) = AddUnsigned(a, HASH(0))
        HASH(1) = AddUnsigned(b, HASH(1))
        HASH(2) = AddUnsigned(c, HASH(2))
        HASH(3) = AddUnsigned(d, HASH(3))
        HASH(4) = AddUnsigned(e, HASH(4))
        HASH(5) = AddUnsigned(f, HASH(5))
        HASH(6) = AddUnsigned(g, HASH(6))
        HASH(7) = AddUnsigned(h, HASH(7))
    Next

	SHA256_Close = HASH

End Function

'@brief 사용자 입력 평문을 한번에 처리
'@param sMessage : 사용자 입력 평문
'@remarks 내부적으로 SHA256_Init, SHA256_Process, SHA256_Close를 호출한다.
'@return 해쉬값을 반환한다.
Public Function SHA256_Encrypt(sMessage)

    Dim M
    Dim W(63)
    Dim a
    Dim b
    Dim c
    Dim d
    Dim e
    Dim f
    Dim g
    Dim h
    Dim i
    Dim j
    Dim T1
    Dim T2
	Dim HASH

	HASH = SHA256_Init()

    M = SHA256_Process(sMessage)

	HASH = SHA256_Close(HASH, M)

	SHA256_Encrypt = LCase(Right("00000000" & Hex(HASH(0)), 8) & Right("00000000" & Hex(HASH(1)), 8) & Right("00000000" & Hex(HASH(2)), 8) & Right("00000000" & Hex(HASH(3)), 8) & Right("00000000" & Hex(HASH(4)), 8) & Right("00000000" & Hex(HASH(5)), 8) & Right("00000000" & Hex(HASH(6)), 8) & Right("00000000" & Hex(HASH(7)), 8))
End Function
%>
