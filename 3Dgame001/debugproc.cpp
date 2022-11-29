//==================================================================================
//
//デバッグ表示[debugproc.cpp]
//Author:相馬靜雅
//
//==================================================================================
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "debugproc.h"
#include "main.h"

//マクロ定義
#define MAX_STR	(1024)	//文字列の最大数

//グローバル変数宣言
LPD3DXFONT g_pFontProc = NULL;	//フォントへのポインタ
char g_aStrDebug[MAX_STR];		//文字列(デバッグ情報)を格納するバッファ
char g_aStrBuffDebug[MAX_STR];	//保持用のバッファ

//==================================================================================
//デバッグ表示の初期化処理
//==================================================================================
void InitDebugProc(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//デバッグ表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFontProc);

	//情報バッファのクリア
	memset(&g_aStrDebug[0], 0, sizeof g_aStrDebug);

	PrintDebugProc("でない\n");
	PrintDebugProc("でた\n");
	PrintDebugProc("これは%dつ目\n", 3);
	PrintDebugProc("%d\n",4);

}

//==================================================================================
//デバッグ表示の終了処理
//==================================================================================
void UninitDebugProc(void)
{
	//デバッグ表示用フォントの破棄
	if (g_pFontProc != NULL)
	{
		g_pFontProc->Release();
		g_pFontProc = NULL;
	}
}

//==================================================================================
//デバッグ表示の更新処理
//==================================================================================
void UpdateDebugProc(void)
{
	//ON.OFF
}

//==================================================================================
//デバッグ表示の描画処理
//==================================================================================
void DrawDebugProc(void)
{

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//テキストの描画処理
	g_pFontProc->DrawText(NULL, &g_aStrDebug[0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

}

//==================================================================================
//デバッグ表示の設定処理
//==================================================================================
void PrintDebugProc(const char *fmt, ...)
{

	va_list pList;

	strcpy(&g_aStrBuffDebug[0], fmt);

	va_start(pList, fmt);

	char aBuff[64] = "";

	//終端文字が来るまで繰り返す
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
				//*pCnt = va_arg(pList, float);	//float型で取り出す
				break;

			case 'c':
				*pCnt = va_arg(pList, char);	//char型で取り出す
				break;

			case 's':
				*pCnt = va_arg(pList, const char);	//char*で取り出す
				break;

			default:
				break;
			}
		}

	}

	va_end(pList);

	strcat(&g_aStrDebug[0], &g_aStrBuffDebug[0]);

}
