//=============================================================================
//
// ポリゴン処理 [polygon.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _POLYGON_H_		//このマクロ定義がされていなかったら
#define _POLYGON_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//ポリゴン構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
}POLYGON;

//プロトタイプ宣言
void InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);

#endif