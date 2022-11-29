//=============================================================================
//
// ビルボード処理 [billboard.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _BILLBOARD_H_		//このマクロ定義がされていなかったら
#define _BILLBOARD_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_BILLBOARD	(128)	//ビルボードの最大数

//ポリゴン構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用しているか
}Billboard;

//プロトタイプ宣言
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos);
bool GetAlphaTest(void);
bool GetZTest(void);

#endif