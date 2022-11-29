//=============================================================================
//
// メッシュシリンダー処理 [meshcylinder.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MESHCYLINDER_H_		//このマクロ定義がされていなかったら
#define _MESHCYLINDER_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//メッシュフィールド構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nNumIndex;			//インデックス数
	int nNumVertex;			//頂点数
}MESHCYLINDER;

//プロトタイプ宣言
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

#endif