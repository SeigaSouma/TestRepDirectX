//=============================================================================
//
// 敵処理 [player.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _PLAYER_H_		//このマクロ定義がされていなかったら
#define _PLAYER_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define MAX_PLAYER	(128)	//モデルの最大数
#define MAX_MOTION	(16)	//モーションの最大数
#define MAX_KEY		(16)	//キーの最大数
#define MAX_PARTS	(32)	//キーの最大数
#define JUMP		(20.0f)	//ジャンプ移動量
#define GRAVITY		(1.0f)	//重力
#define MAX_MAT		(128)	//マテリアルサイズ

#include "main.h"

//モデルの種類
typedef enum
{
	PLAYERPARTS_BODY = 0,	//体
	PLAYERPARTS_HEAD,		//頭
	PLAYERPARTS_R_NINO,		//右二の腕
	PLAYERPARTS_R_UDE,		//右腕
	PLAYERPARTS_R_HAND,		//右手
	PLAYERPARTS_R_MOMO,		//右もも
	PLAYERPARTS_R_LEG,		//右ふくらはぎ
	PLAYERPARTS_R_ASIKUBI,	//右足首
	PLAYERPARTS_R_FOOT,		//右足
	PLAYERPARTS_L_NINO,		//左二の腕
	PLAYERPARTS_L_UDE,		//左腕
	PLAYERPARTS_L_HAND,		//左手
	PLAYERPARTS_L_MOMO,		//左もも
	PLAYERPARTS_L_LEG,		//左ふくらはぎ
	PLAYERPARTS_L_ASIKUBI,	//左足首
	PLAYERPARTS_L_FOOT,		//左足
	PLAYERPARTS_MAX
}PLAYERPARTS;

typedef enum
{
	MOTION_LOOP_OFF = 0,	//ループ無し
	MOTION_LOOP_ON,		//ループする
	MOTION_LOOP_MAX
}MOTION_LOOP;

typedef enum
{
	PLAYERMOTION_DEF = 0,	//ニュートラルモーション
	PLAYERMOTION_WALK,		//移動モーション
	PLAYERMOTION_ACTION,	//アクションモーション
	PLAYERMOTION_JUMP,		//ジャンプモーション
	PLAYERMOTION_LAND,		//着地モーション
	PLAYERMOTION_SHOT,		//射撃モーション
	PLAYERMOTION_MAX
}PLAYERMOTION;

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_NONE = 0,	//何もしていない状態
	PLAYERSTATE_DMG,		//ダメージ状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//敵の構造体
typedef struct
{
	D3DXVECTOR3 pos;		//現在の位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nIdxShadow;			//影のインデックス番号
	int nLife;				//体力
	int nState;				//状態
	int nCntState;			//状態カウント
	bool bUse;				//使用しているか
	int nNowMotionNum;		//現在のモーション番号
	bool bMove;				//移動しているか
	bool bJump;				//ジャンプしているか
	bool bUseGun;			//銃を使っているか
	bool bATK;				//攻撃してるか
	int nIdxGun;			//銃のインデックス番号
	int nCntWalk;			//移動のカウント
	int nPartsNum;			//パーツ数
	float fRadius;			//半径
}Player;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	D3DXVECTOR3 posOrigin;	//元の位置
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nType;			//読み込むモデルの種類
	int nParent;		//親の番号
	int nWeapon;		//武器かどうか
	bool bUse;			//使用しているか

	LPD3DXMESH pMesh = {};			//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat = {};		//マテリアルへのポインタ
	D3DXMATERIAL pMatData[MAX_MAT];	//マテリアルのデータ
	DWORD dwNumMat = { 0 };			//マテリアルの数
	LPDIRECT3DTEXTURE9 pTexture[30] = {};	//テクスチャへのポインタ
}SetPartsPlayer;

//モーションの構造体
typedef struct
{
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotDest;	//目標の向き
	D3DXVECTOR3 rotDiff;	//向きの差分
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posDest;	//目標の位置
	D3DXVECTOR3 posDiff;	//位置の差分
}Parts;

typedef struct
{
	Parts aParts[MAX_PARTS];
	int nFrame;			//再生フレーム
}Key;

typedef struct
{
	Key aKey[MAX_KEY];
	int nCntFrame;		//フレームのカウント
	int nPatternKey;	//何個目のキーか
	int nNumKey;		//キーの数
	int nLoop;			//ループ判定
	bool bUse;			//使用しているか
}MotionData;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void SetPlayer(void);
void SetMotisonPlayer(int nMotionType);
void HitPlayer(D3DXVECTOR3 move, int nDMG);

#endif