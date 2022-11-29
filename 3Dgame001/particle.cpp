//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "particle.h"
#include "input.h"
#include "effect.h"
#include "camera.h"

//マクロ定義
#define MAX_PARTICLE	(12800)	//パーティクルの最大数
#define SMOKE_LIFE		(40)	//煙の寿命
#define FIRE_LIFE		(20)	//火の寿命
#define HEAT_LIFE		(20)	//温度上昇の寿命
#define COOL_LIFE		(30)	//温度減少の寿命
#define OFFSETTING_LIFE	(60)	//打ち消しの寿命

//プロトタイプ宣言
void SmokeParticle(int nCntParticle);
void SmokeParticleRed(int nCntParticle);
void SmokeParticleYel(int nCntParticle);
void FireParticle(int nCntParticle, int nAngle);
void FireParticleYellow(int nCntParticle, int nAngle);
void HeatParticle(int nCntParticle);
void CoalBulletParticle(int nCntParticle);
void CoolParticle(int nCntParticle);
void OffSettingParticle(int nCntParticle);
void LandParticle(int nCntParticle);
void WalkParticle(int nCntParticle);
void DeadParticle(int nCntParticle);

//グローバル変数宣言
Particle g_aParticle[MAX_PARTICLE];			//パーティクルの情報
D3DXCOLOR g_Smokecol[MAX_PARTICLE];			//煙の色
int g_nParticleNum;

//==================================================================================
//パーティクルの初期化処理
//==================================================================================
void InitParticle(void)
{
	int nCntParticle;

	//パーティクルの情報の初期化
	for (nCntParticle =0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].bUse = false;	//使用していない状態にする
	}
	g_nParticleNum = 0;
}

//==================================================================================
//パーティクルの終了処理
//==================================================================================
void UninitParticle(void)
{
	
}

//==================================================================================
//パーティクルの更新処理
//==================================================================================
void UpdateParticle(void)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{

		if (g_aParticle[nCntParticle].bUse == true)
		{//パーティクルが使用されていたら

			//寿命の更新
			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
				g_nParticleNum--;
			}

		}

	}
	
}

//==================================================================================
//エフェクトの描画処理を進める
//==================================================================================
void MoveParticle(float fMove)
{

	for (int nCntEffect = 0; nCntEffect < MAX_PARTICLE; nCntEffect++)
	{

		if (g_aParticle[nCntEffect].bUse == true)
		{//ブロックが使用されていたら

			g_aParticle[nCntEffect].pos.x -= fMove;
		}

	}

}

//==================================================================================
//パーティクルの描画処理
//==================================================================================
void DrawParticle(void)
{
	
}

//==================================================================================
//パーティクルの設定処理
//==================================================================================
void SetParticle(D3DXVECTOR3 pos, int nType)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//パーティクルが使用されていなかったら

			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].nType = nType;
			g_aParticle[nCntParticle].bUse = true;


			switch (nType)
			{
			case PARTICLE_TYPE_EXPLOSION:
				g_aParticle[nCntParticle].nLife = SMOKE_LIFE;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				SmokeParticle(nCntParticle);
				SmokeParticleRed(nCntParticle);
				SmokeParticleYel(nCntParticle);
				break;

			case PARTICLE_TYPE_SMOKE:

				g_aParticle[nCntParticle].nLife = 30;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				SmokeParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_SMOKE_RED:

				g_aParticle[nCntParticle].nLife = SMOKE_LIFE;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				SmokeParticleRed(nCntParticle);
				break;


			case PARTICLE_TYPE_SMOKE_YEL:

				g_aParticle[nCntParticle].nLife = SMOKE_LIFE;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				SmokeParticleYel(nCntParticle);
				break;
					

			case PARTICLE_TYPE_FIRE:

				g_aParticle[nCntParticle].nLife = FIRE_LIFE;
				FireParticle(nCntParticle, 1);
				FireParticleYellow(nCntParticle, 1);
				break;
					
			case PARTICLE_TYPE_FIRE_YEL:

				g_aParticle[nCntParticle].nLife = FIRE_LIFE;
				FireParticleYellow(nCntParticle, 1);
				break;


			case PARTICLE_TYPE_HEAT:

				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aParticle[nCntParticle].nLife = HEAT_LIFE;
				HeatParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_COALBULLET:
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aParticle[nCntParticle].nLife = HEAT_LIFE;
				CoalBulletParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_COOL:
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aParticle[nCntParticle].nLife = COOL_LIFE;
				CoolParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_OFFSETTING:

				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				OffSettingParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_LAND:
				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				LandParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_WALK:
				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				WalkParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_DEAD:
				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				DeadParticle(nCntParticle);
				break;

			}

			//パーティクルの数加算
			g_nParticleNum++;

			g_aParticle[nCntParticle].nMaxLife = g_aParticle[nCntParticle].nLife;
			break;
		}

		
	}
}

//==================================================================================
//煙のパーティクル
//==================================================================================
void SmokeParticle(int nCntParticle)
{

	float fAngle, fMove;

	//カメラの情報取得
	Camera aCamera = GetCamera();

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 1.0f;		//移動量
		fAngle = (float)(rand() % 101 - 50) / 100.0f;	//角度

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, g_Smokecol[nCntParticle].a);
		g_Smokecol[nCntParticle].a += 0.05f;

		g_aParticle[nCntParticle].fRadius = 100.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if(rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);

	}
}



//==================================================================================
//煙のパーティクル(赤)
//==================================================================================
void SmokeParticleRed(int nCntParticle)
{
	float fAngle, fMove;

	//カメラの情報取得
	Camera aCamera = GetCamera();

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 2.0f;		//移動量
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//角度

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.4f, 0.3f, g_Smokecol[nCntParticle].a);
		g_Smokecol[nCntParticle].a -= 0.02f;

		g_aParticle[nCntParticle].fRadius = 30.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//煙のパーティクル(黄)
//==================================================================================
void SmokeParticleYel(int nCntParticle)
{
	float fAngle, fMove;

	//カメラの情報取得
	Camera aCamera = GetCamera();

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 2.0f;		//移動量
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//角度

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.8f, 0.5f, 0.0f, g_Smokecol[nCntParticle].a);
		g_Smokecol[nCntParticle].a -= 0.02f;

		g_aParticle[nCntParticle].fRadius = 30.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//火のパーティクル
//==================================================================================
void FireParticle(int nCntParticle, int nAngle)
{
	float fAngle, fMove;

	fMove = (float)(rand() % 10) / 1 + 20.5f;		//移動量
	fAngle = (float)(rand() % 41 - 20) / 100.0f;	//角度


	for (int nCntUse = 0; nCntUse < 3; nCntUse++)
	{

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 41 - 20) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = sinf((float)(rand() % 41 - 20) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].fRadius = 55.0f;
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.95f, 0.15f, 0.0f, 1.0f);

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//火のパーティクル(黄)
//==================================================================================
void FireParticleYellow(int nCntParticle, int nAngle)
{
	float fAngle, fMove;

	fMove = (float)(rand() % 10) / 1 + 20.5f;		//移動量
	fAngle = (float)(rand() % 41 - 20) / 100.0f;	//角度

	for (int nCntUse = 0; nCntUse < 3; nCntUse++)
	{

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 41 - 20) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = sinf((float)(rand() % 41 - 20) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].fRadius = 30.0f;
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//温度上昇のパーティクル
//==================================================================================
void HeatParticle(int nCntParticle)
{
	float fAngle, fMove;

	fMove = (float)(rand() % 15) / 10 + 8.5f;		//移動量
	fAngle = (float)(rand() % 41 - 20) / 100.0f;	//角度

	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf(D3DX_PI * fAngle) * -((float)(rand() % 15) / 10 + 6.5f);
		g_aParticle[nCntParticle].move.y = cosf(D3DX_PI * fAngle) * -((float)(rand() % 15) / 10 + 2.5f);
		g_aParticle[nCntParticle].move.z = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.15f, 0.15f, 0.15f, g_Smokecol[nCntParticle].a);

		g_Smokecol[nCntParticle].a = ((float)g_aParticle[nCntParticle].nLife / (float)g_aParticle[nCntParticle].nMaxLife);

		g_aParticle[nCntParticle].fRadius = 30.0f;

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//温度上昇のパーティクル
//==================================================================================
void CoalBulletParticle(int nCntParticle)
{
	float fAngle, fMove;

	fMove = (float)(rand() % 15) / 10 + 8.5f;		//移動量
	fAngle = (float)(rand() % 11 - 5) / 100.0f + -1.5f;	//角度

	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf(D3DX_PI * fAngle) * -((float)(rand() % 5) / 10 + 0.5f);
		g_aParticle[nCntParticle].move.y = cosf(D3DX_PI * fAngle) * -((float)(rand() % 15) / 10 + 10.5f);
		g_aParticle[nCntParticle].move.z = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.15f, 0.15f, 0.15f, g_Smokecol[nCntParticle].a);

		g_Smokecol[nCntParticle].a = ((float)g_aParticle[nCntParticle].nLife / (float)g_aParticle[nCntParticle].nMaxLife);

		g_aParticle[nCntParticle].fRadius = 40.0f;

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//温度減少のパーティクル
//==================================================================================
void CoolParticle(int nCntParticle)
{
	float fAngle, fMove;

	fMove = (float)(rand() % 15) / 1 + 0.5f;		//移動量
	fAngle = (float)(rand() % 629 - 314) / 100.0f;	//角度

	for (int nCntUse = 0; nCntUse < 3; nCntUse++)
	{

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf(D3DX_PI * fAngle) * ((float)(rand() % 5) / 10 + 2.5f);
		g_aParticle[nCntParticle].move.y = cosf(D3DX_PI * fAngle) * ((float)(rand() % 5) / 10 + 2.5f);
		g_aParticle[nCntParticle].move.z = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.3f, 0.3f, 1.0f, g_Smokecol[nCntParticle].a);
		g_Smokecol[nCntParticle].a -= 0.025f;

		g_aParticle[nCntParticle].fRadius = 50.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_SMOKE);
	}
}

//==================================================================================
//打ち消しのパーティクル
//==================================================================================
void OffSettingParticle(int nCntParticle)
{

	float fAngle, fMove;

	//カメラの情報取得
	Camera aCamera = GetCamera();

	for (int nCntUse = 0; nCntUse < 20; nCntUse++)
	{

		fMove = (float)(rand() % 10) / 1 + 3.0f;		//移動量
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//角度

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_aParticle[nCntParticle].fRadius = 40.0f;

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_NORMAL);
	}
}

//==================================================================================
// 着地のパーティクル
//==================================================================================
void LandParticle(int nCntParticle)
{
	float fMove;

	for (int nCntUse = 0; nCntUse < 20; nCntUse++)
	{
		fMove = (float)(rand() % 61 - 30) / 10 + 1.0f;		//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

		g_aParticle[nCntParticle].fRadius = 10.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
// 移動のパーティクル
//==================================================================================
void WalkParticle(int nCntParticle)
{
	float fMove;

	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 1.0f;		//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

		g_aParticle[nCntParticle].fRadius = 5.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
// 死亡のパーティクル
//==================================================================================
void DeadParticle(int nCntParticle)
{
	float fMove;

	for (int nCntUse = 0; nCntUse < 50; nCntUse++)
	{
		fMove = (float)(rand() % 30) / 10 + 1.0f;		//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.1f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);

		g_aParticle[nCntParticle].fRadius = 120.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_NORMAL);
	}

}

//==================================================================================
//パーティクル数取得
//==================================================================================
int GetParticleNum(void)
{
	return g_nParticleNum;
}
