//=============================================================================
//
// 壁処理 [wall.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _WALL_H_		//このマクロ定義がされていなかったら
#define _WALL_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//ポリゴン構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用しているか
}WALL;

//プロトタイプ宣言
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
WALL *GetWall(void);

#endif