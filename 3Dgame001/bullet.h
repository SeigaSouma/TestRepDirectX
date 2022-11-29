//=============================================================================
//
// バレット処理 [bullet.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _BULLET_H_		//このマクロ定義がされていなかったら
#define _BULLET_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//列挙型定義
#define MAX_BULLET	(128)	//弾の最大数
#define POS_BULLET	(20.0f)	//弾の大きさ

//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,	//プレイヤーの弾
	BULLETTYPE_ENEMY,		//敵の弾
	BULLETTYPE_MAX
}BULLETTYPE;

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nIdxShadow;			//影のインデックス番号
	int nLife;				//寿命
	bool bUse;				//使用しているかどうか
	int nType;				//弾の種類
}Bullet;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType);
Bullet *GetBullet(void);

#endif