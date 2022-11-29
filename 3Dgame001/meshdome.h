//=============================================================================
//
// メッシュドーム処理 [meshdome.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MESHDOME_H_		//このマクロ定義がされていなかったら
#define _MESHDOME_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//メッシュドーム構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nNumIndex;			//インデックス数
	int nNumIndexLid;		//蓋のインデックス数
	int nNumVertex;			//頂点数
}MESHDOME;

//プロトタイプ宣言
void InitMeshDome(void);
void UninitMeshDome(void);
void UpdateMeshDome(void);
void DrawMeshDome(void);

#endif