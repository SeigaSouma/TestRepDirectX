//=============================================================================
//
// バレット処理 [bullet.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "meshwall.h"
#include "wall.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "enemy.h"
#include "player.h"

//マクロ定義
#define MAX_TOP		(4)		//頂点数
#define MOVE		(8.0f)	//弾の移動量
#define DEF_LIFE	(120)	//元の寿命
#define DMG			(1)		//攻撃力

//プロトタイプ宣言
void CollisionWall(int nCntBullet);
void CollisionEnemy(int nCntBullet);
void CollisionPlayer(int nCntBullet);

//グローバル変数宣言
const char *c_apFilenameBullet[] =		//ファイル読み込み
{
	"data\\TEXTURE\\bullet000.png",
};
LPDIRECT3DTEXTURE9 g_pTextureBullet[(sizeof c_apFilenameBullet) / sizeof(*c_apFilenameBullet)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];						//弾の情報

//==================================================================================
//弾の初期化処理
//==================================================================================
void InitBullet(void)
{
	
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameBullet) / sizeof(*c_apFilenameBullet); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBullet[nCntTex],
			&g_pTextureBullet[nCntTex]);

	}

	//弾の情報の初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
		g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;	//前回の位置
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
		g_aBullet[nCntBullet].nIdxShadow = -1;						//影のインデックス番号
		g_aBullet[nCntBullet].nType = BULLETTYPE_PLAYER;			//弾の種類
		g_aBullet[nCntBullet].bUse = false;							//使用していない状態にする
	}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_BULLET,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffBullet,
								NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-POS_BULLET, +POS_BULLET, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+POS_BULLET, +POS_BULLET, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-POS_BULLET, -POS_BULLET, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+POS_BULLET, -POS_BULLET, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//==================================================================================
//弾の終了処理
//==================================================================================
void UninitBullet(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameBullet) / sizeof(*c_apFilenameBullet); nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureBullet[nCount] != NULL)
		{
			g_pTextureBullet[nCount]->Release();
			g_pTextureBullet[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//==================================================================================
//弾の更新処理
//==================================================================================
void UpdateBullet(void)
{

	//カメラの情報取得
	Camera aCamera = GetCamera();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{

		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されていたら

			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			//影の位置更新
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos);

			SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f), POS_BULLET * 1.5f, 10,
				-g_aBullet[nCntBullet].move * 0.01f, MOVEEFFECT_SUB, EFFECTTYPE_NORMAL);

			//パーティクルの設定処理
			SetParticle(g_aBullet[nCntBullet].pos, PARTICLE_TYPE_FIRE);

			//壁との当たり判定
			CollisionWall(nCntBullet);

			//弾の位置の更新
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;

			if (g_aBullet[nCntBullet].nType != BULLETTYPE_ENEMY)
			{//弾の種類が敵じゃなかったら

				//敵と弾の当たり判定
				CollisionEnemy(nCntBullet);
			}
			else if (g_aBullet[nCntBullet].nType != BULLETTYPE_PLAYER)
			{//弾の種類がプレイヤーじゃなかったら

				//プレイヤーと弾の当たり判定
				CollisionPlayer(nCntBullet);
			}

			//寿命のカウントダウン
			g_aBullet[nCntBullet].nLife--;

			if (g_aBullet[nCntBullet].nLife <= 0)
			{//寿命が尽きた場合

				g_aBullet[nCntBullet].bUse = false;	//使用していない状態にする
				SetEnableShadow(g_aBullet[nCntBullet].nIdxShadow);	//影消す
				SetExplosion(g_aBullet[nCntBullet].pos);			//爆発呼び出し

			}
			
		}

	}
}

//==================================================================================
//弾の描画処理
//==================================================================================
void DrawBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス宣言
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	
	 //アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCount = 0; nCount < MAX_BULLET; nCount++)
	{

		if (g_aBullet[nCount].bUse == true)
		{//使用されていたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCount].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBullet[nCount].mtxWorld, NULL, &mtxView);	//逆配列を求める
			g_aBullet[nCount].mtxWorld._41 = 0.0f;
			g_aBullet[nCount].mtxWorld._42 = 0.0f;
			g_aBullet[nCount].mtxWorld._43 = 0.0f;

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCount].pos.x, g_aBullet[nCount].pos.y, g_aBullet[nCount].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCount].mtxWorld, &g_aBullet[nCount].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCount].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet[0]);

			//ビルボードの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);

		}

	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==================================================================================
//弾の設定処理
//==================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType)
{

	//モデルの情報を取得
	Model aModel = GetModel();

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{

		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていなかったら

			g_aBullet[nCntBullet].pos = pos;	//位置
			g_aBullet[nCntBullet].nIdxShadow = SetShadow(POS_BULLET, POS_BULLET);	//影を設定
			g_aBullet[nCntBullet].move = move * MOVE;	//移動量
			g_aBullet[nCntBullet].nLife = DEF_LIFE;		//寿命
			g_aBullet[nCntBullet].nType = nType;	//弾の種類
			g_aBullet[nCntBullet].bUse = true;		//使用している状態に変更

			break;
		}

		pVtx += MAX_TOP;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();

}

//==================================================================================
// 弾と壁の当たり判定
//==================================================================================
void CollisionWall(int nCntBullet)
{

	//壁の情報取得
	MESHWALL *pMeshWall = GetMeshWall();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pMeshWall++)
	{//壁の数分繰り返す

		if (pMeshWall->bUse == true)
		{//壁が使用されているとき

			//壁の幅
			float fLen = (pMeshWall->nWidth * POS_MESHWALL) * 0.5f;

			D3DXVECTOR3 vecLine;
			vecLine.x = (pMeshWall->pos.x + cosf(pMeshWall->rot.y) * fLen) - (pMeshWall->pos.x + cosf(pMeshWall->rot.y) * -fLen);
			vecLine.z = (pMeshWall->pos.z + sinf(pMeshWall->rot.y) * -fLen) - (pMeshWall->pos.z + sinf(pMeshWall->rot.y) * fLen);

			D3DXVECTOR3 vecToPos;
			vecToPos.x = g_aBullet[nCntBullet].pos.x - (pMeshWall->pos.x + cosf(pMeshWall->rot.y) * -fLen);
			vecToPos.z = g_aBullet[nCntBullet].pos.z - (pMeshWall->pos.z + sinf(pMeshWall->rot.y) * fLen);

			if((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0)
			{//壁に当たったら

				g_aBullet[nCntBullet].bUse = false;	//使用していない状態にする
				SetEnableShadow(g_aBullet[nCntBullet].nIdxShadow);	//影消す
				SetExplosion(g_aBullet[nCntBullet].pos);			//爆発呼び出し
			}

		}

	}

}

//==================================================================================
//敵と弾の当たり判定
//==================================================================================
void CollisionEnemy(int nCntBullet)
{
	//敵の情報取得
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{//敵の数分繰り返す

		if (pEnemy->bUse == true)
		{//敵が使用されていたら

			if (
				g_aBullet[nCntBullet].pos.x - POS_BULLET < pEnemy->pos.x + pEnemy->vtxMax.x &&
				g_aBullet[nCntBullet].pos.x + POS_BULLET > pEnemy->pos.x + pEnemy->vtxMin.x &&
				g_aBullet[nCntBullet].pos.z - POS_BULLET < pEnemy->pos.z + pEnemy->vtxMax.z &&
				g_aBullet[nCntBullet].pos.z + POS_BULLET > pEnemy->pos.z + pEnemy->vtxMin.z &&
				g_aBullet[nCntBullet].pos.y - POS_BULLET < pEnemy->pos.y + pEnemy->vtxMax.y &&
				g_aBullet[nCntBullet].pos.y + POS_BULLET > pEnemy->pos.y + pEnemy->vtxMin.y
				)
			{//弾が敵に当たったら

				g_aBullet[nCntBullet].bUse = false;	//使用していない状態にする
				SetEnableShadow(g_aBullet[nCntBullet].nIdxShadow);	//影消す
				SetExplosion(g_aBullet[nCntBullet].pos);			//爆発呼び出し

				//敵のヒット処理
				HitEnemy(nCntEnemy, D3DXVECTOR3(g_aBullet[nCntBullet].move.x, 10.0f, g_aBullet[nCntBullet].move.z), DMG);


			}

		}
	}

}

//==================================================================================
//プレイヤーと弾の当たり判定
//==================================================================================
void CollisionPlayer(int nCntBullet)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	if (pPlayer->bUse == true)
	{//プレイヤーが使用されていたら

		if (
			g_aBullet[nCntBullet].pos.x - POS_BULLET < pPlayer->pos.x + pPlayer->fRadius &&
			g_aBullet[nCntBullet].pos.x + POS_BULLET > pPlayer->pos.x - pPlayer->fRadius &&
			g_aBullet[nCntBullet].pos.z - POS_BULLET < pPlayer->pos.z + pPlayer->fRadius &&
			g_aBullet[nCntBullet].pos.z + POS_BULLET > pPlayer->pos.z - pPlayer->fRadius &&
			g_aBullet[nCntBullet].pos.y - POS_BULLET < pPlayer->pos.y + pPlayer->fRadius &&
			g_aBullet[nCntBullet].pos.y + POS_BULLET > pPlayer->pos.y - pPlayer->fRadius
			)
		{//弾がプレイヤーに当たったら

			g_aBullet[nCntBullet].bUse = false;	//使用していない状態にする
			SetEnableShadow(g_aBullet[nCntBullet].nIdxShadow);	//影消す
			SetExplosion(g_aBullet[nCntBullet].pos);			//爆発呼び出し

			//プレイヤーのヒット処理
			HitPlayer(D3DXVECTOR3(g_aBullet[nCntBullet].move.x, 10.0f, g_aBullet[nCntBullet].move.z), DMG);
		}
	}

}

//==================================================================================
//弾の情報取得
//==================================================================================
Bullet *GetBullet(void)
{
	return &g_aBullet[0];
}
