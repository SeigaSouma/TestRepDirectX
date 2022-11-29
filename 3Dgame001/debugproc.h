//=============================================================================
//
// デバッグ表示処理 [debugproc.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _DEBUGPROC_H_	//このマクロ定義がされていなかったら
#define _DEBUGPROC_H_	//二重インクルード防止のマクロを定義する

#include "debugproc.h"

//マクロ定義

//プロトタイプ宣言
void InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);
void PrintDebugProc(const char *fmt, ...);

#endif