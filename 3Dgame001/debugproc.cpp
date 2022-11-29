//==================================================================================
//
//�f�o�b�O�\��[debugproc.cpp]
//Author:���n�Ή�
//
//==================================================================================
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "debugproc.h"
#include "main.h"

//�}�N����`
#define MAX_STR	(1024)	//������̍ő吔

//�O���[�o���ϐ��錾
LPD3DXFONT g_pFontProc = NULL;	//�t�H���g�ւ̃|�C���^
char g_aStrDebug[MAX_STR];		//������(�f�o�b�O���)���i�[����o�b�t�@
char g_aStrBuffDebug[MAX_STR];	//�ێ��p�̃o�b�t�@

//==================================================================================
//�f�o�b�O�\���̏���������
//==================================================================================
void InitDebugProc(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFontProc);

	//���o�b�t�@�̃N���A
	memset(&g_aStrDebug[0], 0, sizeof g_aStrDebug);

	PrintDebugProc("�łȂ�\n");
	PrintDebugProc("�ł�\n");
	PrintDebugProc("�����%d��\n", 3);
	PrintDebugProc("%d\n",4);

}

//==================================================================================
//�f�o�b�O�\���̏I������
//==================================================================================
void UninitDebugProc(void)
{
	//�f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFontProc != NULL)
	{
		g_pFontProc->Release();
		g_pFontProc = NULL;
	}
}

//==================================================================================
//�f�o�b�O�\���̍X�V����
//==================================================================================
void UpdateDebugProc(void)
{
	//ON.OFF
}

//==================================================================================
//�f�o�b�O�\���̕`�揈��
//==================================================================================
void DrawDebugProc(void)
{

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//�e�L�X�g�̕`�揈��
	g_pFontProc->DrawText(NULL, &g_aStrDebug[0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

}

//==================================================================================
//�f�o�b�O�\���̐ݒ菈��
//==================================================================================
void PrintDebugProc(const char *fmt, ...)
{

	va_list pList;

	strcpy(&g_aStrBuffDebug[0], fmt);

	va_start(pList, fmt);

	char aBuff[64] = "";

	//�I�[����������܂ŌJ��Ԃ�
	for (char *pCnt = &g_aStrBuffDebug[0]; *pCnt != '\0' ; pCnt++)
	{

		if (*pCnt == '%')
		{

			*pCnt = ' ';
			*pCnt++;

			switch (*pCnt)
			{
			case 'd':
				*pCnt = ' ';
				sprintf(&aBuff[0], "%d", va_arg(pList, int));
				//vsprintf(&aBuff[0], pCnt, pList);

				strcat(&g_aStrBuffDebug[0], &aBuff[0]);
				break;

			case 'f':
				//*pCnt = va_arg(pList, float);	//float�^�Ŏ��o��
				break;

			case 'c':
				*pCnt = va_arg(pList, char);	//char�^�Ŏ��o��
				break;

			case 's':
				*pCnt = va_arg(pList, const char);	//char*�Ŏ��o��
				break;

			default:
				break;
			}
		}

	}

	va_end(pList);

	strcat(&g_aStrDebug[0], &g_aStrBuffDebug[0]);

}
