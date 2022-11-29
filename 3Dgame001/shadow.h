//=============================================================================
//
// 影処理 [shadow.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _SHADOW_H_		//このマクロ定義がされていなかったら
#define _SHADOW_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_SHADOW	(128)	//影の最大数

//ポリゴン構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用しているか
}SHADOW;

//プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(float fWidth, float fHeight);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void SetEnableShadow(int nIdxShadow);

#endif