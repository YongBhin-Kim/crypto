/**
@file KISA_SEED_CBC.h
@brief SEED CBC ��ȣ �˰���
@author Copyright (c) 2013 by KISA
@remarks http://seed.kisa.or.kr/
*/

#ifndef SEED_CBC_H
#define SEED_CBC_H

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
	DWORD			ivec[4];				
	KISA_SEED_KEY	seed_key;				
	DWORD			cbc_buffer[4];			
	int				buffer_length;			
	DWORD			cbc_last_block[4];		
	int				last_block_flag;		
} KISA_SEED_INFO;
#endif

/**
@brief BYTE �迭�� int �迭�� ��ȯ�Ѵ�.
@param in :��ȯ�� BYTE ������
@param nLen : ��ȯ�� BYTE �迭 ����
@return ���ڷ� ���� BYTE �迭�� int�� ��ȯ�� �����͸� ��ȯ�Ѵ�. (���������� malloc������ free�� �� ����� �Ѵ�)
@remarks ���������� ������ ����� �Լ��� SEED CTR, CBC, HIGHT CTR, CBC�� ������ include �� 
���� �Լ��� ��� �浹 ������ �ڿ� ������ �� �ֵ��� ���带 ���δ�.
*/
DWORD* chartoint32_for_SEED_CBC( IN BYTE *in, IN int nLen );

/**
@brief int �迭�� BYTE �迭�� ��ȯ�Ѵ�.
@param in :��ȯ�� int ������
@param nLen : ��ȯ�� int �迭 ����
@return ���ڷ� ���� int �迭�� char�� ��ȯ�� �����͸� ��ȯ�Ѵ�. (���������� malloc������ free�� �� ����� �Ѵ�)
@remarks ���������� ������ ����� �Լ��� SEED CTR, CBC, HIGHT CTR, CBC�� ������ include �� 
���� �Լ��� ��� �浹 ������ �ڿ� ������ �� �ֵ��� ���带 ���δ�.
*/
BYTE* int32tochar_for_SEED_CBC( IN DWORD *in, IN int nLen );

/**
@brief SEED CBC �˰��� �ʱ�ȭ �Լ�
@param pInfo : CBC ���ο��� ���Ǵ� ����ü�ν� ������ �����ϸ� �ȵȴ�.(�޸� �Ҵ�Ǿ� �־�� �Ѵ�.)
@param enc : ��ȣȭ �� ��ȣȭ ��� ����
@param pbszUserKey : ����ڰ� �����ϴ� �Է� Ű(16 BYTE)
@param pbszIV : ����ڰ� �����ϴ� �ʱ�ȭ ����(16 BYTE)
@return 0: pInfo �Ǵ� pbszUserKey �Ǵ� pbszIV �����Ͱ� NULL�� ���, 
        1: ����
*/
int SEED_CBC_init( OUT KISA_SEED_INFO *pInfo, IN KISA_ENC_DEC enc, IN BYTE *pbszUserKey, IN BYTE *pbszIV );

/**
@brief SEED CBC ���� �� ��ȣȭ/��ȣȭ �Լ�
@param pInfo : SEED_CBC_init ���� ������ KISA_HIGHT_INFO ����ü
@param in : ��/��ȣ�� ( ���� chartoint32_for_SEED_CBC�� ����Ͽ� int�� ��ȯ�� ������)
@param inLen : ��/��ȣ�� ����(BYTE ����)
@param out : ��/��ȣ�� ����
@param outLen : ����� ��/��ȣ���� ����(BYTE ������ �Ѱ��ش�)
@return 0: inLen�� ���� 0���� ���� ���, KISA_SEED_INFO ����ü�� in, out�� �� �����Ͱ� �Ҵ�Ǿ��� ���
        1: ����
*/
int SEED_CBC_Process( OUT KISA_SEED_INFO *pInfo, IN DWORD *in, IN int inLen, OUT DWORD *out, OUT int *outLen );

/**
@brief SEED CBC ���� ���� �� �е� ó��(PKCS7)
@param pInfo : SEED_CBC_Process �� ��ģ KISA_HIGHT_INFO ����ü
@param out : ��/��ȣ�� ��� ����
@param outLen : ��� ���ۿ� ����� ������ ����(BYTE ������ �򹮱���)
@return 
- 0 :  inLen�� ���� 0���� ���� ���,
       KISA_SEED_INFO ����ü�� out�� �� �����Ͱ� �Ҵ�Ǿ��� ���
- 1 :  ����
@remarks �е� ���������� 16����Ʈ ������ ó�������� ��ȣȭ �� ��� ���۴� 
�򹮺��� 16����Ʈ Ŀ�� �Ѵ�.(���� 16����Ʈ �� �� �е� ����Ÿ�� 16����Ʈ�� ����.)
*/
int SEED_CBC_Close( OUT KISA_SEED_INFO *pInfo, IN DWORD *out, IN int *outLen );

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
int SEED_CBC_Encrypt( IN BYTE *pbszUserKey, IN BYTE *pbszIV, IN BYTE *pbszPlainText, IN int nPlainTextLen, OUT BYTE *pbszCipherText );

/**
@brief ó���ϰ��� �ϴ� �����Ͱ� ���� ��쿡 ���
@param pbszUserKey : ����ڰ� �����ϴ� �Է� Ű(16 BYTE)
@param pszbIV : ����ڰ� �����ϴ� �ʱ�ȭ ����(16 BYTE)
@param pbszCipherText : ��ȣ��
@param nCipherTextLen : ��ȣ�� ����(BYTE ������ �򹮱���)
@return ��ȣȭ�� ����� ����(char ����)
@param pbszPlainText : �� ��� ����
*/
int SEED_CBC_Decrypt( IN BYTE *pbszUserKey, IN BYTE *pbszIV, IN BYTE *pbszCipherText, IN int nCipherTextLen, OUT BYTE *pbszPlainText );

#ifdef  __cplusplus
}
#endif

#endif
