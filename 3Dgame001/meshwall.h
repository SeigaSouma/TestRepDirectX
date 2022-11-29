//=============================================================================
//
// メッシュウォール処理 [meshwall.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MESHWALL_H_		//このマクロ定義がされていなかったら
#define _MESHWALL_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_WALL	(128)	//壁の最大数

//マクロ定義
#define POS_MESHWALL	(250.0f)


//メッシュフィールド構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nNumIndex;			//インデックス数
	int nNumVertex;			//頂点数
	bool bUse;				//使用しているか
	int nWidth;				//横分割数
	int nHeight;			//縦分割数
}MESHWALL;

//プロトタイプ宣言
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Width, int Height);
MESHWALL *GetMeshWall(void);
LPDIRECT3DVERTEXBUFFER9 GetBuffMeshWall(void);

#endif