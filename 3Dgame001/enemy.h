//=============================================================================
//
// 敵処理 [enemy.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _ENEMY_H_		//このマクロ定義がされていなかったら
#define _ENEMY_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define MAX_ENEMY	(128)	//モデルの最大数
#define MAX_MAT		(128)	//マテリアルサイズ

#include "main.h"

//モデルの種類
typedef enum
{
	MODELTYPE_CAR = 0,	//車
	MODELTYPE_SHIELD,	//盾
	MODELTYPE_WAGON,	//ワゴン
	MODELTYPE_HEAD,		//頭
	MODELTYPE_MAX
}MODELTYPE;

typedef enum
{
	ENEMYSTATE_NONE = 0,	//何もしていない状態
	ENEMYSTATE_DMG,			//ダメージ状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

//敵の構造体
typedef struct
{
	D3DXVECTOR3 Startpos;	//位置
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nIdxShadow;			//影のインデックス番号
	bool bUse;				//使用しているか
	bool bJump;				//ジャンプしているか
	int state;				//状態
	int nLife;				//体力
	int nType;				//読み込むモデルの種類
	int nCntState;			//状態カウンター
	int nATKInterval;		//攻撃の間隔
	int nCntATK;			//攻撃の間隔カウント
	LPDIRECT3DTEXTURE9 pTexture[30];	//テクスチャへのポインタ
	LPD3DXMESH pMesh;		//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;	//マテリアルへのポインタ
	D3DXMATERIAL pMatData[MAX_MAT];	//マテリアルのデータ
	DWORD dwNumMat;			//マテリアルの数

	int nNumVtx;			//頂点数
	DWORD dwSizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;			//頂点バッファのポインタ
	D3DXVECTOR3 vtxMin;		//モデルの最小値
	D3DXVECTOR3	vtxMax;		//モデルの最大値
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife);
void HitEnemy(int nCntEnemy, D3DXVECTOR3 move, int nDMG);

#endif