//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _PARTICLE_H_		//このマクロ定義がされていなかったら
#define _PARTICLE_H_		//二重インクルード防止のマクロを定義する

//列挙型定義
typedef enum
{
	PARTICLE_TYPE_SMOKE = 0,	//煙のパーティクル
	PARTICLE_TYPE_SMOKE_RED,	//煙のパーティクル赤
	PARTICLE_TYPE_SMOKE_YEL,	//煙のパーティクル黄
	PARTICLE_TYPE_EXPLOSION,	//爆発のパーティクル
	PARTICLE_TYPE_FIRE,			//加熱の煙パーティクル
	PARTICLE_TYPE_FIRE_YEL,		//加熱の煙パーティクル
	PARTICLE_TYPE_HEAT,			//温度上昇のパーティクル
	PARTICLE_TYPE_COALBULLET,	//温度上昇のパーティクル
	PARTICLE_TYPE_COOL,			//温度減少のパーティクル
	PARTICLE_TYPE_OFFSETTING,	//打ち消しのパーティクル
	PARTICLE_TYPE_LAND,			//着地のパーティクル
	PARTICLE_TYPE_WALK,			//移動のパーティクル
	PARTICLE_TYPE_DEAD,			//死亡時のパーティクル
}PARTICLE_TYPE;

//パーティクル構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//色
	float fRadius;		//半径
	int nLife;			//寿命
	int nMaxLife;		//最大寿命
	int nType;			//種類
	bool bUse;			//使用しているかどうか
}Particle;

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, int nType);
int GetParticleNum(void);

#endif