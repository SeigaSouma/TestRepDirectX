//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _EXPLOSION_H_		//このマクロ定義がされていなかったら
#define _EXPLOSION_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//列挙型定義
#define MAX_EXPLOSION	(128)	//爆発の最大数
#define POS_EXPLOSION	(20.0f)	//爆発の大きさ

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR col;			//色
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用しているかどうか
	int nPatternAnim;		//アニメーションパターンNo.
	int nCntAnim;			//アニメーションカウンター
	int nLife;				//寿命
	int nMaxLife;			//最大寿命
}Explosion;

//プロトタイプ宣言
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos);
Explosion *GetExplosion(void);

#endif