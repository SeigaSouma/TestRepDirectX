//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "model.h"
#include "bullet.h"
#include "particle.h"
#include "enemy.h"

//マクロ定義
#define MOVE		(1.0f)
#define MAX_TX		(30)
#define DMG_TIME	(10)	//ダメージ状態の時間

//プロトタイプ宣言
void ControllPlayer(void);
void ReadSetPlayer(void);
void ReadSetMotionPlayer(void);
void UpdateMotionPlayer(void);
void LimitPosPlayer(void);
void CollisionEnemyPlayer(void);
void UpdateStatePlayer(void);

//グローバル変数宣言
const char *c_apModelnamePlayer[] =					//モデルデータ読み込み
{
	"data\\MODEL\\00_body.x",		//体
	"data\\MODEL\\01_head.x",		//頭
	"data\\MODEL\\02_ninoudeR.x",	//右二の腕
	"data\\MODEL\\03_udeR.x",		//右腕
	"data\\MODEL\\04_ThehandR.x",	//右手
	"data\\MODEL\\05_ninoudeL.x",	//右もも
	"data\\MODEL\\06_udeL.x",		//右ふくらはぎ
	"data\\MODEL\\07_ThehandL.x",	//右足首
	"data\\MODEL\\08_futomomoR.x",	//右足
	"data\\MODEL\\09_fukuraR.x",	//左二の腕
	"data\\MODEL\\10_asikubiR.x",	//左腕
	"data\\MODEL\\11_footR.x",		//左手
	"data\\MODEL\\12_futomomoL.x",	//左もも
	"data\\MODEL\\13_fukuraL.x",	//左ふくらはぎ
	"data\\MODEL\\14_asikubiL.x",	//左足首
	"data\\MODEL\\15_footL.x",		//左足
	"data\\MODEL\\handgun_01.x",		//ハンドガン
};

Player g_aPlayer;				//プレイヤーの情報
SetPartsPlayer g_aSetPartsPlayer[MAX_PLAYER];		//外部ファイル用
MotionData g_aMotionData[MAX_MOTION];				//モーション用の情報

//構造体はネストして、モーションの構造体、キーの構造体、パーツの構造体って分けていく

//==================================================================================
//プレイヤーの初期化処理
//==================================================================================
void InitPlayer(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{//モーションの数繰り返す

		g_aMotionData[nCntMotion].bUse = false;	//使用しているか
		g_aMotionData[nCntMotion].nCntFrame = 0;//フレームのカウント
		g_aMotionData[nCntMotion].nLoop = MOTION_LOOP_ON;	//ループ判定
		g_aMotionData[nCntMotion].nNumKey = 0;				//キーの数
		g_aMotionData[nCntMotion].nPatternKey = 0;			//今、何個目のキーか

		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{//キーの数繰り返す

			g_aMotionData[nCntMotion].aKey[nCntKey].nFrame = 0;	//再生フレーム

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{//パーツの数繰り返す

				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目標の位置
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置の差分
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目標の向き
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向きの差分
			}
			
		}

	}

	//各要素初期化
	g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.posOld = g_aPlayer.pos;
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.nIdxShadow = -1;
	g_aPlayer.nLife = 10;
	g_aPlayer.nState = PLAYERSTATE_NONE;	//状態
	g_aPlayer.nCntState = 0;				//状態カウント
	g_aPlayer.nCntWalk = 0;
	g_aPlayer.nNowMotionNum = PLAYERMOTION_DEF;
	g_aPlayer.bUse = true;	//使用している状況にする
	g_aPlayer.bJump = false;
	g_aPlayer.bATK = false;
	g_aPlayer.bUseGun = false;
	g_aPlayer.nIdxGun = -1;
	g_aPlayer.bMove = false;
	g_aPlayer.nIdxShadow = SetShadow(20.0f, 20.0f);	//影を設定
	g_aPlayer.nPartsNum = 0;
	g_aPlayer.fRadius = 50.0f;	//半径

	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		g_aSetPartsPlayer[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSetPartsPlayer[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSetPartsPlayer[nCount].nType = -1;	//NULL
		g_aSetPartsPlayer[nCount].nParent = -1;	//NULL
		g_aSetPartsPlayer[nCount].nWeapon = 0;	//武器かどうか
		g_aSetPartsPlayer[nCount].bUse = false;	//使用しているか
		g_aSetPartsPlayer[nCount].posOrigin = g_aSetPartsPlayer[nCount].pos;
	}


	//外部ファイル読み込み
	ReadSetPlayer();
	ReadSetMotionPlayer();
}

//==================================================================================
//プレイヤーの終了処理
//==================================================================================
void UninitPlayer(void)
{


	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//テクスチャの破棄
			if (g_aSetPartsPlayer[nCount].pTexture[nCntTex] != NULL)
			{
				g_aSetPartsPlayer[nCount].pTexture[nCntTex]->Release();
				g_aSetPartsPlayer[nCount].pTexture[nCntTex] = NULL;
			}
		}

		//メッシュの破棄
		if (g_aSetPartsPlayer[nCount].pMesh != NULL)
		{
			g_aSetPartsPlayer[nCount].pMesh->Release();
			g_aSetPartsPlayer[nCount].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aSetPartsPlayer[nCount].pBuffMat != NULL)
		{
			g_aSetPartsPlayer[nCount].pBuffMat->Release();
			g_aSetPartsPlayer[nCount].pBuffMat = NULL;
		}
	}

}

//==================================================================================
//プレイヤーの更新処理
//==================================================================================
void UpdatePlayer(void)
{

	if (g_aPlayer.bUse == true)
	{//使用していたら

		g_aPlayer.posOld = g_aPlayer.pos;

		//プレイヤーの操作
		ControllPlayer();

		//座標制限
		LimitPosPlayer();
			
		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//移動してるとき

			g_aPlayer.nCntWalk++;	//カウント加算

			if (g_aPlayer.nCntWalk > 5)
			{
				//移動パーティクル
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);

				g_aPlayer.nCntWalk = 0;
			}
			

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_WALK)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_WALK);
			}
			
		}
		else if (g_aPlayer.bJump == true)
		{//ジャンプしている

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_JUMP)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_JUMP);
			}
		}
		else
		{//ニュートラルモーション

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_DEF && g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_LAND && g_aPlayer.nNowMotionNum != PLAYERMOTION_SHOT)
			{

				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DEF);

			}
		}

		//モーションの更新
		UpdateMotionPlayer();

		//影の位置更新
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);

		//敵との当たり判定
		CollisionEnemyPlayer();

		//状態更新
		UpdateStatePlayer();

	}

}

//==================================================================================
//モデルの操作
//==================================================================================
void ControllPlayer(void)
{
	//カメラの情報取得
	Camera aCamera = GetCamera();

	float fRotMove = g_aPlayer.rot.y;	//現在の角度
	float fRotDest = g_aPlayer.rot.y;	//目標の角度
	float fRotDiff = 0.0f;				//補正

	if (aCamera.bFollow == true)
	{//追従がONの時だけ

		if (GetKeyboardPress(DIK_A) == true || XGetStickPressR(BUTTON_LX, 0) < 0)
		{//←キーが押された,左移動

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
			{//A+W,左上移動

				g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
				fRotDest = D3DX_PI * MOVE_LRUP + aCamera.rot.y;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
			{//A+S,左下移動

				g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
				fRotDest = D3DX_PI * MOVE_LRDW + aCamera.rot.y;
			}
			else
			{//A,左移動

				g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
				fRotDest = D3DX_PI * MOVE_LR + aCamera.rot.y;
			}

			g_aPlayer.bMove = true;
		}
		else if (GetKeyboardPress(DIK_D) == true || XGetStickPressR(BUTTON_LX, 0) > 0)
		{//Dキーが押された,右移動

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
			{//D+W,右上移動

				g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
				fRotDest = -D3DX_PI * MOVE_LRUP + aCamera.rot.y;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
			{//D+S,右下移動

				g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
				fRotDest = -D3DX_PI * MOVE_LRDW + aCamera.rot.y;
			}
			else
			{//D,右移動

				g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
				fRotDest = -D3DX_PI * MOVE_LR + aCamera.rot.y;
			}

			g_aPlayer.bMove = true;
		}
		else if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
		{//Wが押された、奥移動

			g_aPlayer.move.x += sinf(aCamera.rot.y) * MOVE;
			g_aPlayer.move.z += cosf(aCamera.rot.y) * MOVE;
			fRotDest = D3DX_PI + aCamera.rot.y;
			g_aPlayer.bMove = true;
		}
		else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
		{//Sが押された、手前移動

			g_aPlayer.move.x += sinf(D3DX_PI + aCamera.rot.y) * MOVE;
			g_aPlayer.move.z += cosf(D3DX_PI + aCamera.rot.y) * MOVE;
			fRotDest = aCamera.rot.y;
			g_aPlayer.bMove = true;
		}
		else
		{//なんのキーも押されていない

			if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f ||
				g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
			{
				g_aPlayer.bMove = false;
			}
		}

		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{//J回転

			g_aPlayer.rot.y += D3DX_PI * 0.05f;
		}
		if (GetKeyboardPress(DIK_RSHIFT) == true)
		{//J回転

			g_aPlayer.rot.y -= D3DX_PI * 0.05f;
		}

		if (GetKeyboardPress(DIK_I) == true)
		{//Iキーが押された,上昇

			g_aPlayer.pos.y += MOVE;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//Kキーが押された,下降

			g_aPlayer.pos.y -= MOVE;
		}

		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//SPACEが押された,ジャンプ

			//ハンドガン使用
			g_aSetPartsPlayer[g_aPlayer.nIdxGun].bUse = true;

			if (g_aPlayer.bJump == false)
			{//ジャンプしてないときだけ

				g_aPlayer.move.y += JUMP;
				g_aPlayer.bJump = true;
			}

		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//ENTERが押された,,弾発射

			//攻撃状態
			g_aPlayer.bATK = true;

			if (g_aSetPartsPlayer[g_aPlayer.nIdxGun].bUse == false)
			{//ハンドガンなし

				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_ACTION);
			}
			else if (g_aSetPartsPlayer[g_aPlayer.nIdxGun].bUse == true)
			{//ハンドガンあり

				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_SHOT);
			}

			SetBullet(D3DXVECTOR3(
				g_aPlayer.pos.x - sinf(g_aPlayer.rot.y) * (POS_BULLET * 2.0f),
				g_aPlayer.pos.y + POS_BULLET,
				g_aPlayer.pos.z - cosf(g_aPlayer.rot.y) * (POS_BULLET * 2.0f)),	//敵の座標
				D3DXVECTOR3(
					sinf(D3DX_PI + g_aPlayer.rot.y),
					0.0f,
					cosf(D3DX_PI + g_aPlayer.rot.y)),	//移動方向
				BULLETTYPE_PLAYER);	//弾の種類

		}

	}

	//現在と目標の差分を求める
	fRotDiff = fRotDest - fRotMove;

	//角度の正規化
	fRotDiff = RotNormalize(fRotDiff);

	//角度の補正をする
	g_aPlayer.rot.y += fRotDiff * 0.1f;

	//角度の正規化
	g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);

	//重力処理
	g_aPlayer.move.y -= 1.0f;

	//位置更新
	g_aPlayer.pos += g_aPlayer.move;

	//慣性つける
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.1f;
	g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * 0.1f;

}

//==================================================================================
//プレイヤーの座標制限
//==================================================================================
void LimitPosPlayer(void)
{
	if (g_aPlayer.pos.y < 0.0f)
	{//地面より下に行ったら

		g_aPlayer.pos.y = 0.0f;

		//重力処理
		g_aPlayer.move.y = 0.0f;

		if (g_aPlayer.bJump == true)
		{//ジャンプ中だったら

			//モーションの設定
			SetMotisonPlayer(PLAYERMOTION_LAND);

			//着地パーティクル
			SetParticle(g_aPlayer.pos, PARTICLE_TYPE_LAND);
		}

		g_aPlayer.bJump = false;

	}

	//移動制限
	if (g_aPlayer.pos.x + g_aPlayer.fRadius <= -1000.0f && g_aPlayer.posOld.x + g_aPlayer.fRadius >= -1000.0f)
	{//今回が-1000以下, 前回が-1000以上

		g_aPlayer.pos.x = -1000.0f - g_aPlayer.fRadius;
	}
	if (g_aPlayer.pos.x + g_aPlayer.fRadius >= 1000.0f && g_aPlayer.posOld.x + g_aPlayer.fRadius <= 1000.0f)
	{//今回が1000以上, 前回が1000以下

		g_aPlayer.pos.x = 1000.0f - g_aPlayer.fRadius;
	}
	if (g_aPlayer.pos.z + g_aPlayer.fRadius >= 1000.0f && g_aPlayer.posOld.z + g_aPlayer.fRadius <= 1000.0f)
	{//今回が1000以上, 前回が1000以下

		g_aPlayer.pos.z = 1000.0f - g_aPlayer.fRadius;
	}
	if (g_aPlayer.pos.z + g_aPlayer.fRadius <= -1000.0f && g_aPlayer.posOld.z + g_aPlayer.fRadius >= -1000.0f)
	{//今回が-1000以下, 前回が-1000以上

		g_aPlayer.pos.z = -1000.0f - g_aPlayer.fRadius;
	}
	
}

//==================================================================================
//弾のヒット処理
//==================================================================================
void HitPlayer(D3DXVECTOR3 move, int nDMG)
{

	g_aPlayer.move = move;	//移動量加算
	//g_aPlayer.nLife -= nDMG;	//体力減らす

	if (g_aPlayer.nLife <= 0)
	{//体力がなくなった

		g_aPlayer.nLife = 0;
		g_aPlayer.bUse = false;
		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//死亡のパーティクルセット
		SetEnableShadow(g_aPlayer.nIdxShadow);	//影消す
		SetParticle(g_aPlayer.pos, PARTICLE_TYPE_DEAD);
	}
	else
	{//まだ体力が残ってる

		g_aPlayer.nState = ENEMYSTATE_DMG;	//ダメージ状態へ
		g_aPlayer.nCntState = DMG_TIME;	//ダメージ状態を保つ時間を与える

		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
		{//パーツ分繰り返す

			D3DXMATERIAL *pMat;	//マテリアルデータへのポインタ

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aSetPartsPlayer[nCntParts].pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aSetPartsPlayer[nCntParts].dwNumMat; nCntMat++)
			{//マテリアルを赤くする

				pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
		}
	}

}

//==================================================================================
//プレイヤーの状態更新
//==================================================================================
void UpdateStatePlayer(void)
{

	switch (g_aPlayer.nState)
	{
	case ENEMYSTATE_NONE:
		break;

	case ENEMYSTATE_DMG:

		//ダメージカウンター減算
		g_aPlayer.nCntState--;

		if (g_aPlayer.nCntState <= 0)
		{//ダメージカウンターが0以下

			//状態をもとに戻す
			g_aPlayer.nState = ENEMYSTATE_NONE;

			for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
			{//パーツ分繰り返す

				D3DXMATERIAL *pMat;	//マテリアルデータへのポインタ

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aSetPartsPlayer[nCntParts].pBuffMat->GetBufferPointer();

				//頂点数分繰り返し
				for (int nCntMat = 0; nCntMat < (int)g_aSetPartsPlayer[nCntParts].dwNumMat; nCntMat++)
				{
					//元の色に戻す
					pMat[nCntMat].MatD3D.Diffuse = g_aSetPartsPlayer[nCntParts].pMatData[nCntMat].MatD3D.Diffuse;
					pMat[nCntMat].MatD3D.Ambient = g_aSetPartsPlayer[nCntParts].pMatData[nCntMat].MatD3D.Ambient;
				}
			}
		}

		break;
	}
}

//==================================================================================
//敵との当たり判定
//==================================================================================
void CollisionEnemyPlayer(void)
{
	//敵の情報取得
	Enemy *pEnemy = GetEnemy();

	float fLength = (g_aPlayer.pos.x - pEnemy->pos.x) * (g_aPlayer.pos.x - pEnemy->pos.x) +
					(g_aPlayer.pos.z - pEnemy->pos.z) * (g_aPlayer.pos.y - pEnemy->pos.z);



	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//敵が使用されていたら

			//横の判定
			if (g_aPlayer.pos.z + g_aPlayer.fRadius > pEnemy->pos.z + pEnemy->vtxMin.z &&	//敵の最小値に入ってる
				g_aPlayer.pos.z - g_aPlayer.fRadius < pEnemy->pos.z + pEnemy->vtxMax.z)		//敵の最大値に入ってる
			{//敵のZ幅の中にいるとき

				if (g_aPlayer.pos.x - g_aPlayer.fRadius <= pEnemy->pos.x + pEnemy->vtxMax.x &&	//現在の最小値が敵の最大値よりも小さい
					g_aPlayer.posOld.x - g_aPlayer.fRadius >= pEnemy->pos.x + pEnemy->vtxMax.x)	//前回の最小値が敵の最大値よりも大きい
				{//右から

					g_aPlayer.pos.x = (pEnemy->pos.x + pEnemy->vtxMax.x) + g_aPlayer.fRadius;
				}
				if (g_aPlayer.pos.x + g_aPlayer.fRadius >= pEnemy->pos.x + pEnemy->vtxMin.x &&	//現在の最大値が敵の最小値よりも大きい
					g_aPlayer.posOld.x + g_aPlayer.fRadius <= pEnemy->pos.x + pEnemy->vtxMin.x)	//前回の最大値が敵の最小値よりも小さい
				{//左から

					g_aPlayer.pos.x = (pEnemy->pos.x + pEnemy->vtxMin.x) - g_aPlayer.fRadius;
				}

			}


			//縦の判定
			if (g_aPlayer.pos.x + g_aPlayer.fRadius > pEnemy->pos.x + pEnemy->vtxMin.x &&	//敵の最小値に入ってる
				g_aPlayer.pos.x - g_aPlayer.fRadius < pEnemy->pos.x + pEnemy->vtxMax.x)		//敵の最大値に入ってる
			{//敵のX幅の中にいるとき

				if (g_aPlayer.pos.z - g_aPlayer.fRadius <= pEnemy->pos.z + pEnemy->vtxMax.z &&	//現在の最小値が敵の最大値よりも小さい
					g_aPlayer.posOld.z - g_aPlayer.fRadius >= pEnemy->pos.z + pEnemy->vtxMax.z)	//前回の最小値が敵の最大値よりも大きい
				{//敵の位置に補正

					g_aPlayer.pos.z = (pEnemy->pos.z + pEnemy->vtxMax.z) + g_aPlayer.fRadius;
				}
				if (g_aPlayer.pos.z + g_aPlayer.fRadius >= pEnemy->pos.z + pEnemy->vtxMin.z &&	//現在の最大値が敵の最小値よりも大きい
					g_aPlayer.posOld.z + g_aPlayer.fRadius <= pEnemy->pos.z + pEnemy->vtxMin.z)	//前回の最大値が敵の最小値よりも小さい
				{//敵の位置に補正

					g_aPlayer.pos.z = (pEnemy->pos.z + pEnemy->vtxMin.z) - g_aPlayer.fRadius;
				}

			}

		}

	}


}

//==================================================================================
//プレイヤーのモーション処理
//==================================================================================
void SetMotisonPlayer(int nMotionType)
{

	if (g_aPlayer.nNowMotionNum != nMotionType)
	{

		for (int nCntKey = 0; nCntKey < g_aMotionData[nMotionType].nNumKey; nCntKey++)
		{//キー数分繰り返す

			for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
			{//パーツ分繰り返す

				g_aSetPartsPlayer[nCntParts].rot = g_aMotionData[nMotionType].aKey[nCntKey].aParts[nCntParts].rot;
				g_aSetPartsPlayer[nCntParts].pos = g_aSetPartsPlayer[nCntParts].posOrigin;

			}

			g_aMotionData[nMotionType].nCntFrame = 0;	//フレームカウントリセット
		}

		//今のモーションの更新
		g_aPlayer.nNowMotionNum = nMotionType;

		g_aMotionData[nMotionType].nPatternKey = 0;	//現在のキー番号リセット
	}


	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//パーツ分繰り返す

		//元の向きと現在の向きで目標の向きを求める
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDest = g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rot;
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].posDest = g_aSetPartsPlayer[nCntParts].posOrigin + g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].pos;

		//現在と目標の差分を求める
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff = g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDest - g_aSetPartsPlayer[nCntParts].rot;
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].posDiff = g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].posDest - g_aSetPartsPlayer[nCntParts].pos;

		//角度の正規化
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.x = RotNormalize(g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.x);
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.y = RotNormalize(g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.y);
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.z = RotNormalize(g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.z);

	}

}

//==================================================================================
//プレイヤーのモーション更新処理
//==================================================================================
void UpdateMotionPlayer(void)
{
	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//パーツ分繰り返す

		//差分を求めて規定フレームで終わるように補正
		g_aSetPartsPlayer[nCntParts].rot +=
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey].aParts[nCntParts].rotDiff /	//目的と現在の差分
			(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey].nFrame;					//再生フレームで割る

		//位置
		g_aSetPartsPlayer[nCntParts].pos +=
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey].aParts[nCntParts].posDiff /	//目的と現在の差分
			(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey].nFrame;					//再生フレームで割る

		//角度の正規化
		g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.x = RotNormalize(g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.x);
		g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.y = RotNormalize(g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.y);
		g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.z = RotNormalize(g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.z);

	}

	//フレームのカウントを加算
	g_aMotionData[g_aPlayer.nNowMotionNum].nCntFrame++;

	if (g_aMotionData[g_aPlayer.nNowMotionNum].nCntFrame >= g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey].nFrame)
	{//フレームのカウントがフレーム数に達したら

		//フレームのカウントをゼロに戻す
		g_aMotionData[g_aPlayer.nNowMotionNum].nCntFrame = 0;

		//パターンNO.更新
		g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey = (g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey + 1) % g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey;

		if (g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey == 0)
		{//キーがゼロの時

			g_aPlayer.bATK = false;

			if (g_aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{

				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
			else
			{
				//モーションの設定
				SetMotisonPlayer(g_aPlayer.nNowMotionNum);
			}

		}
		else
		{
			//モーションの設定
			SetMotisonPlayer(g_aPlayer.nNowMotionNum);
		}

	}

}

//==================================================================================
//プレイヤーの描画処理
//==================================================================================
void DrawPlayer(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATRIX mtxnParent;			//親の計算用マトリックス

	if (g_aPlayer.bUse == true)
	{//使用していたら

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

		//向きを反映する
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

		//位置を反映する
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.mtxWorld);

		for (int nCount = 0; nCount < g_aPlayer.nPartsNum; nCount++)
		{

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSetPartsPlayer[nCount].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSetPartsPlayer[nCount].rot.y, g_aSetPartsPlayer[nCount].rot.x, g_aSetPartsPlayer[nCount].rot.z);
			D3DXMatrixMultiply(&g_aSetPartsPlayer[nCount].mtxWorld, &g_aSetPartsPlayer[nCount].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aSetPartsPlayer[nCount].pos.x, g_aSetPartsPlayer[nCount].pos.y, g_aSetPartsPlayer[nCount].pos.z);
			D3DXMatrixMultiply(&g_aSetPartsPlayer[nCount].mtxWorld, &g_aSetPartsPlayer[nCount].mtxWorld, &mtxTrans);


			if (g_aSetPartsPlayer[nCount].nParent == -1)
			{//NULLの場合,親

				//プレイヤーの向きを反映する
				mtxnParent = g_aPlayer.mtxWorld;
			}
			else
			{//パーツ

				//親の向きを反映する
				mtxnParent = g_aSetPartsPlayer[g_aSetPartsPlayer[nCount].nParent].mtxWorld;
			}

			//自分のやつに親のワールドマトリックスを掛け算
			D3DXMatrixMultiply(&g_aSetPartsPlayer[nCount].mtxWorld, &g_aSetPartsPlayer[nCount].mtxWorld, &mtxnParent);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSetPartsPlayer[nCount].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aSetPartsPlayer[nCount].pBuffMat->GetBufferPointer();

			if (g_aSetPartsPlayer[nCount].bUse == true)
			{//パーツが使用されていたら

				//頂点数分繰り返し
				for (int nCntMat = 0; nCntMat < (int)g_aSetPartsPlayer[nCount].dwNumMat; nCntMat++)
				{

					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//テクスチャの設定
					pDevice->SetTexture(0, g_aSetPartsPlayer[nCount].pTexture[nCntMat]);

					//プレイヤー(パーツ)の描画
					g_aSetPartsPlayer[nCount].pMesh->DrawSubset(nCntMat);

				}

			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}

}

//==================================================================================
//プレイヤーの情報取得
//==================================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer;
}

//==================================================================================
//プレイヤーの設定処理
//==================================================================================
void SetPlayer(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//パーツ分繰り返す

		//Xファイルの読み込み
		D3DXLoadMeshFromX(c_apModelnamePlayer[nCntParts],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aSetPartsPlayer[nCntParts].pBuffMat,
			NULL,
			&g_aSetPartsPlayer[nCntParts].dwNumMat,
			&g_aSetPartsPlayer[nCntParts].pMesh);

		D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aSetPartsPlayer[nCntParts].pBuffMat->GetBufferPointer();

		//頂点数分繰り返し
		for (int nCntMat = 0; nCntMat < (int)g_aSetPartsPlayer[nCntParts].dwNumMat; nCntMat++)
		{
			//マテリアルのデータ取得
			g_aSetPartsPlayer[nCntParts].pMatData[nCntMat] = pMat[nCntMat];

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//ファイルが存在する

				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aSetPartsPlayer[nCntParts].pTexture[nCntMat]);

			}
		}

		if (g_aSetPartsPlayer[nCntParts].nWeapon != 0)
		{//武器の仕様判定が0以外なら

			//パーツは使用しない
			g_aSetPartsPlayer[nCntParts].bUse = false;

			//要素番号保存
			g_aPlayer.nIdxGun = nCntParts;
		}
		else
		{//武器の仕様判定が0

			//パーツを使用状態
			g_aSetPartsPlayer[nCntParts].bUse = true;
		}

	}
}

//==================================================================================
//プレイヤーの読み込み処理
//==================================================================================
void ReadSetPlayer(void)
{
	FILE *pFile = NULL;	//ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen("data\\TEXT\\player_set.txt", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		char aComment[MAX_COMMENT];	//コメント、確認等

		int nCntSetParts = 0;

		while (1)
		{//END_SCRIPTが来るまで繰り返す

			//文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			if (strcmp(aComment, "NUM_MODEL") == 0)
			{//モデルの数読み込み

				//文字列の読み込み
				fscanf(pFile, "%s", &aComment[0]);	//=の分
				fscanf(pFile, "%d", &g_aPlayer.nPartsNum);	//パーツ数

				/*for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCount++)
				{*/

			}


			//各パーツの設定
			if (strcmp(aComment, "CHARACTERSET") == 0)
			{//キャラクター情報の読み込みを開始

				while (strcmp(aComment, "END_CHARACTERSET") != 0)
				{//END_CHARACTERSETが来るまで繰り返し

					fscanf(pFile, "%s", &aComment[0]);	//確認する

					if (strcmp(aComment, "PARTSSET") == 0)
					{//PARTSSETでパーツ情報の読み込み開始

						while (strcmp(aComment, "END_PARTSSET") != 0)
						{//END_PARTSSETが来るまで繰り返す

							fscanf(pFile, "%s", &aComment[0]);	//確認する

							if (strcmp(aComment, "INDEX") == 0)
							{//INDEXが来たらモデル番号読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%d", &g_aSetPartsPlayer[nCntSetParts].nType);	//モデル種類の列挙

							}

							if (strcmp(aComment, "PARENT") == 0)
							{//PARENTが来たら親の番号読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%d", &g_aSetPartsPlayer[nCntSetParts].nParent);	//親の番号

							}

							if (strcmp(aComment, "POS") == 0)
							{//POSが来たら位置読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].pos.x);	//X座標
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].pos.y);	//Y座標
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].pos.z);	//Z座標

								//元の位置割り当て
								g_aSetPartsPlayer[nCntSetParts].posOrigin = g_aSetPartsPlayer[nCntSetParts].pos;

							}

							if (strcmp(aComment, "ROT") == 0)
							{//ROTが来たら向き読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].rot.x);	//Xの角度
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].rot.y);	//Yの角度
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].rot.z);	//Zの角度

							}

							if (strcmp(aComment, "WEAPON") == 0)
							{//WEAPONが来たら武器かどうか読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%d", &g_aSetPartsPlayer[nCntSetParts].nWeapon);	//武器かどうか

							}

						}//END_PARTSSETのかっこ

						nCntSetParts++;	//パーツのカウントを加算
					}

				}//END_CHARACTERSETのかっこ

			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//終了文字でループを抜ける

				break;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	//プレイヤーの配置
	SetPlayer();

}

//==================================================================================
//プレイヤーの読み込み処理
//==================================================================================
void ReadSetMotionPlayer(void)
{

	FILE *pFile = NULL;	//ファイルポインタを宣言

	char aComment[MAX_COMMENT];	//コメント用
	int nCntMotion = 0, nCntKey = 0, nCntParts = 0;

	//ファイルを開く
	pFile = fopen("data\\TEXT\\motion_set_player.txt", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		while (1)
		{//END_SCRIPTが来るまで繰り返す

			//文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			//==================================================
			//各モーションの設定
			//==================================================
			if (strcmp(aComment, "MOTIONSET") == 0)
			{//モーション情報の読み込みを開始

				while (strcmp(aComment, "END_MOTIONSET") != 0)
				{//END_MOTIONSETが来るまで繰り返し

					fscanf(pFile, "%s", &aComment[0]);	//確認する

					if (strcmp(aComment, "LOOP") == 0)
					{//LOOPが来たらループON/OFF読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aMotionData[nCntMotion].nLoop);	//ループ0か1

					}

					if (strcmp(aComment, "NUM_KEY") == 0)
					{//NUM_KEYが来たらキー数読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aMotionData[nCntMotion].nNumKey);	//キー数

					}

					//==================================================
					//各キーの設定
					//==================================================
					if (strcmp(aComment, "KEYSET") == 0)
					{//KEYSETでキー情報の読み込み開始

						while (strcmp(aComment, "END_KEYSET") != 0)
						{//END_KEYSETが来るまで繰り返し

							fscanf(pFile, "%s", &aComment[0]);	//確認する

							if (strcmp(aComment, "FRAME") == 0)
							{//FRAMEが来たら再生フレーム数読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%d", &g_aMotionData[nCntMotion].aKey[nCntKey].nFrame);	//再生フレーム

							}

							//==================================================
							//パーツの設定
							//==================================================
							if (strcmp(aComment, "PARTS") == 0)
							{//PARTSでパーツ情報の読み込み開始

								while (strcmp(aComment, "END_PARTS") != 0)
								{//END_PARTSが来るまで繰り返し

									fscanf(pFile, "%s", &aComment[0]);	//確認する

									if (strcmp(aComment, "POS") == 0)
									{//POSが来たら位置読み込み

										fscanf(pFile, "%s", &aComment[0]);		//=の分
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	//X座標
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	//Y座標
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	//Z座標
									}

									if (strcmp(aComment, "ROT") == 0)
									{//ROTが来たら角度読み込み

										fscanf(pFile, "%s", &aComment[0]);		//=の分
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	//X角度
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	//Y角度
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	//Z角度
									}

								}//END_PARTSのかっこ

								nCntParts++;	//パーツのカウント加算
							}


						}//END_KEYSETのかっこ

						nCntParts = 0;	//パーツのカウントリセット
						nCntKey++;	//キーのカウント加算
					}

				}//END_MOTIONSETのかっこ

				nCntKey = 0;	//キーのカウントリセット
				nCntMotion++;	//モーションのカウント加算

			}


			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//終了文字でループを抜ける

				break;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}
