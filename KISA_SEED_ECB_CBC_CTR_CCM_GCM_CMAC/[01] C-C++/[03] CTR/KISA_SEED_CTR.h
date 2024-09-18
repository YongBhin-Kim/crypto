/**
@file KISA_SEED_CTR.h
@brief SEED CTR ��ȣ �˰���
@author Copyright (c) 2013 by KISA
@remarks http://seed.kisa.or.kr/
*/

#ifndef SEED_CTR_H
#define SEED_CTR_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#if __alpha__	||	__alpha	||	__i386__	||	i386	||	_M_I86	||	_M_IX86	||	\
	__OS2__		||	sun386	||	__TURBOC__	||	vax		||	vms		||	VMS		||	__VMS || __linux__
#define LITTLE_ENDIAN
#else
#define BIG_ENDIAN
#endif

typedef unsigned int        DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;

#ifndef _KISA_ENC_DEC_
#define _KISA_ENC_DEC_
typedef enum _SEED_ENC_DEC
{
	KISA_DECRYPT,
	KISA_ENCRYPT,
}KISA_ENC_DEC;
#endif

#ifndef _KISA_SEED_KEY_
#define _KISA_SEED_KEY_
typedef struct kisa_seed_key_st 
{
	DWORD key_data[32];
} KISA_SEED_KEY;
#endif

#ifndef _KISA_SEED_INFO_
#define _KISA_SEED_INFO_
typedef struct kisa_seed_info_st 
{	
	KISA_ENC_DEC	encrypt;				
	DWORD			ctr[4];				
	KISA_SEED_KEY	seed_key;				
	DWORD			cbc_buffer[4];			
	int				buffer_length;			
	DWORD			cbc_last_block[4];		
	int				last_block_flag;		
} KISA_SEED_INFO;
#endif

#ifndef _NONCE_TYPE_
#define _NONCE_TYPE_
typedef enum _NONCE_TYPE
{
	NONCE_NONE,
	NONCE_OR,
	NONCE_AND,
	NONCE_XOR,
}NONCE_TYPE;
#endif

/**
@brief SEED_CTR_init�� ����ϴ� �ʱⰪ(BYTE *pbszCounter)�� �����Ѵ�.
@param type : OR, AND, XOR ������ Ÿ�� ����
@param pbszIV : IV(�ʱ� ����) �迭 ������
@param pbszCounter : ī���� �ʱ� ��
@param pbszNonce : IV �� Counter�� type�� ���� ��Ʈ ����� ��� ��
*/
void InitNonce_SEED( IN NONCE_TYPE type, IN BYTE *pbszIV, IN BYTE *pbszCounter, OUT BYTE *pbszNonce );

/**
@brief BYTE �迭�� int �迭�� ��ȯ�Ѵ�.
@param in :��ȯ�� BYTE ������
@param inLen : ��ȯ�� BYTE �迭 ����
@return ���ڷ� ���� BYTE �迭�� int�� ��ȯ�� �����͸� ��ȯ�Ѵ�. (���������� malloc������ free�� �� ����� �Ѵ�)
@remarks ���������� ������ ����� �Լ��� SEED CTR, CBC, HIGHT CTR, CBC�� ������ include �� 
���� �Լ��� ��� �浹 ������ �ڿ� ������ �� �ֵ��� ���带 ���δ�.
*/
DWORD* chartoint32_for_SEED_CTR( IN BYTE *in, IN int inLen );

/**
@brief int �迭�� BYTE �迭�� ��ȯ�Ѵ�.
@param in :��ȯ�� int ������
@param inLen : ��ȯ�� int �迭 ����
@return ���ڷ� ���� int �迭�� char�� ��ȯ�� �����͸� ��ȯ�Ѵ�. (���������� malloc������ free�� �� ����� �Ѵ�)
@remarks ���������� ������ ����� �Լ��� SEED CTR, CBC, HIGHT CTR, CBC�� ������ include �� 
���� �Լ��� ��� �浹 ������ �ڿ� ������ �� �ֵ��� ���带 ���δ�.
*/
BYTE* int32tochar_for_SEED_CTR( IN DWORD *in, IN int inLen );

/**
@brief SEED CTR �˰��� �ʱ�ȭ �Լ�
@param pInfo : CBC ���ο��� ���Ǵ� ����ü�ν� ������ �����ϸ� �ȵȴ�.(�޸� �Ҵ�Ǿ� �־�� �Ѵ�.)
@param enc : ��ȣȭ �� ��ȣȭ ��� ����
@param pbszUserKey : ����ڰ� �����ϴ� �Է� Ű(16 BYTE)
@param pbszIV : ����ڰ� �����ϴ� �ʱ�ȭ ����(16 BYTE)
*/
int SEED_CTR_init( OUT KISA_SEED_INFO *pInfo, IN KISA_ENC_DEC enc, IN BYTE *pszUserKey, IN BYTE *pbszCounter );

/**
@brief SEED CTR ���� �� ��ȣȭ/��ȣȭ �Լ�
@param pInfo : SEED_CTR_init ���� ������ KISA_HIGHT_INFO ����ü
@param in : ��/��ȣ�� ( ���� chartoint32_for_SEED_CTR�� ����Ͽ� int�� ��ȯ�� ������)
@param inLen : ��/��ȣ�� ����(BYTE ����)
@param out : ��/��ȣ�� ����
@param outLen : ����� ��/��ȣ���� ����(BYTE ������ �Ѱ��ش�)
@return 0: inLen�� ���� 0���� ���� ���, KISA_SEED_INFO ����ü�� in, out�� �� �����Ͱ� �Ҵ�Ǿ��� ���
        1: ����
*/
int SEED_CTR_Process( OUT KISA_SEED_INFO *pInfo, IN DWORD *in, IN int inLen, OUT DWORD *out, OUT int *outLen );

/**
@brief ó���ϰ��� �ϴ� �����Ͱ� ���� ��쿡 ���
@param pbszUserKey : ����ڰ� �����ϴ� �Է� Ű(16 BYTE)
@param pszbIV : ����ڰ� �����ϴ� �ʱ�ȭ ����(16 BYTE)
@param pbszPlainText : ����� �Է� ��
@param nPlainTextLen : �� ����(BYTE ������ �򹮱���)
@param pbszCipherText : ��ȣ�� ��� ����
@return ��ȣȭ�� ����� ����(char ����)
@remarks �е� ���������� 16����Ʈ ������ ó�������� pbszCipherText�� �򹮺��� 16����Ʈ Ŀ�� �Ѵ�.
(���� 16����Ʈ �� �� �е� ����Ÿ�� 16����Ʈ�� ����.)
*/
int SEED_CTR_Encrypt( IN BYTE *pbszUserKey, IN BYTE *pbszIV, IN BYTE *pbszPlainText, IN int nPlainTextLen, OUT BYTE *pbszCipherText );

/**
@brief ó���ϰ��� �ϴ� �����Ͱ� ���� ��쿡 ���
@param pbszUserKey : ����ڰ� �����ϴ� �Է� Ű(16 BYTE)
@param pszbIV : ����ڰ� �����ϴ� �ʱ�ȭ ����(16 BYTE)
@param pbszCipherText : ��ȣ��
@param nCipherTextLen : ��ȣ�� ����(BYTE ������ �򹮱���)
@param pbszPlainText : �� ��� ����
@return ��ȣȭ�� ����� ����(char ����)
*/
int SEED_CTR_Decrypt( IN BYTE *pbszUserKey, IN BYTE *pbszIV, IN BYTE *pbszCipherText, IN int nCipherTextLen, OUT BYTE *pbszPlainText );

#ifdef  __cplusplus
}
#endif

#endif
