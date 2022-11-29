//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MESHFIELD_H_		//このマクロ定義がされていなかったら
#define _MESHFIELD_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//メッシュフィールド構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nNumIndex;			//頂点数
}MESHFIELD;

//プロトタイプ宣言
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif