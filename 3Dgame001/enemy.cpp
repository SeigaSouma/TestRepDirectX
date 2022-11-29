//=============================================================================
//
// 敵処理 [enemy.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "particle.h"
#include "bullet.h"

//マクロ定義
#define MOVE		(3.0f)
#define MAX_TX		(30)
#define MAX_LENGTH	(100.0f)	//元の距離
#define ROT_MOVE	(0.03f)	//回転移動量
#define DMG_TIME	(20)	//ダメージ状態の時間
#define ATK_INTERVAL	(120)

//プロトタイプ宣言
void ControllEnemy(int nCntEnemy);
void ReadSetEnemy(void);
void UpdateStateEnemy(int nCntEnemy);
void LimitPosEnemy(int nCntEnemy);
void CheckVtxEnemy(int nCntEnemy);
void RotFacePlayerEnemy(int nCntEnemy);
void ATKEnemy(int nCntEnemy);

//グローバル変数宣言
const char *c_apModelname[] =					//モデルデータ読み込み
{
	"data\\MODEL\\car_01.x",			//車
	"data\\MODEL\\KiteShield.x",	//シールド
	"data\\MODEL\\wagon_01.x",		//ワゴン
	"data\\MODEL\\01_head.x",		//頭
};

Enemy g_aEnemy[MAX_ENEMY];				//敵の情報

//==================================================================================
//敵の初期化処理
//==================================================================================
void InitEnemy(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//各要素初期化
	for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
	{
		g_aEnemy[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCount].Startpos = g_aEnemy[nCount].pos;
		g_aEnemy[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCount].nIdxShadow = -1;
		g_aEnemy[nCount].bUse = false;
		g_aEnemy[nCount].state = ENEMYSTATE_NONE;		//状態
		g_aEnemy[nCount].nLife = 9999;		//体力
		g_aEnemy[nCount].nATKInterval = 9999;		//攻撃間隔
		g_aEnemy[nCount].nCntATK = 0;				//攻撃の間隔カウント
		g_aEnemy[nCount].nCntState = 0;			//状態カウンター
		g_aEnemy[nCount].nType = -1;			//種類

		g_aEnemy[nCount].pTexture[30] = {};	//テクスチャへのポインタ
		g_aEnemy[nCount].pMesh = {};		//メッシュ(頂点情報)へのポインタ
		g_aEnemy[nCount].pBuffMat = {};		//マテリアルへのポインタ
		g_aEnemy[nCount].dwNumMat = {};			//マテリアルの数

		g_aEnemy[nCount].nNumVtx = 0;	//頂点数
		g_aEnemy[nCount].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_aEnemy[nCount].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aEnemy[nCount].pMatData[nCntMat] = {};		//マテリアルのデータ
		}

	}

	//外部ファイル読み込み
	ReadSetEnemy();

}

//==================================================================================
//敵の終了処理
//==================================================================================
void UninitEnemy(void)
{


	for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
	{

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//テクスチャの破棄
			if (g_aEnemy[nCount].pTexture[nCntTex] != NULL)
			{
				g_aEnemy[nCount].pTexture[nCntTex]->Release();
				g_aEnemy[nCount].pTexture[nCntTex] = NULL;
			}
		}

		//メッシュの破棄
		if (g_aEnemy[nCount].pMesh != NULL)
		{
			g_aEnemy[nCount].pMesh->Release();
			g_aEnemy[nCount].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aEnemy[nCount].pBuffMat != NULL)
		{
			g_aEnemy[nCount].pBuffMat->Release();
			g_aEnemy[nCount].pBuffMat = NULL;
		}
	}

}

//==================================================================================
//敵の更新処理
//==================================================================================
void UpdateEnemy(void)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	if (GetKeyboardTrigger(DIK_F2) == true)
	{//F2が押された,スポーン

		SetEnemy(pPlayer->pos, pPlayer->rot, MODELTYPE_WAGON, 3);
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{

		if (g_aEnemy[nCntEnemy].bUse == true)
		{//使用していたら

			//敵の操作
			ControllEnemy(nCntEnemy);

			//プレイヤーの方向見る
			RotFacePlayerEnemy(nCntEnemy);

			//弾発射処理
			ATKEnemy(nCntEnemy);

			//座標制限
			LimitPosEnemy(nCntEnemy);

			//状態更新
			UpdateStateEnemy(nCntEnemy);

			//影の位置更新
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);
		}

	}

}

//==================================================================================
//敵の操作
//==================================================================================
void ControllEnemy(int nCntEnemy)
{

	//カメラの情報取得
	Camera aCamera = GetCamera();

	//重力処理
	g_aEnemy[nCntEnemy].move.y -= GRAVITY;

	//位置更新
	g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;
	g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * 0.1f;
	g_aEnemy[nCntEnemy].move.z += (0.0f - g_aEnemy[nCntEnemy].move.z) * 0.1f;

}

//==================================================================================
//敵の攻撃処理
//==================================================================================
void ATKEnemy(int nCntEnemy)
{

	//攻撃間隔カウント加算
	//g_aEnemy[nCntEnemy].nCntATK++;

	if (g_aEnemy[nCntEnemy].nCntATK >= g_aEnemy[nCntEnemy].nATKInterval)
	{//カウントが規定値超えたら

		//カウントリセット
		g_aEnemy[nCntEnemy].nCntATK = 0;

		SetBullet(D3DXVECTOR3(
			g_aEnemy[nCntEnemy].pos.x - sinf(g_aEnemy[nCntEnemy].rot.y) * (POS_BULLET * 2.0f),
			g_aEnemy[nCntEnemy].pos.y + POS_BULLET,
			g_aEnemy[nCntEnemy].pos.z - cosf(g_aEnemy[nCntEnemy].rot.y) * (POS_BULLET * 2.0f)),	//敵の座標
			D3DXVECTOR3(
				sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y),
				0.0f,
				cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y)),	//移動方向
			BULLETTYPE_ENEMY);	//弾の種類

	}

}

//==================================================================================
//プレイヤーの方を見る処理
//==================================================================================
void RotFacePlayerEnemy(int nCntEnemy)
{

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	float fRotMove = g_aEnemy[nCntEnemy].rot.y;	//現在の角度
	float fRotDest = g_aEnemy[nCntEnemy].rot.y;	//目標の角度
	float fRotDiff = 0.0f;				//補正

	//目標の角度を求める
	fRotDest = atan2f((g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x), (g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z));
	fRotDiff = fRotDest - fRotMove;

	//角度の正規化
	fRotDiff = RotNormalize(fRotDiff);

	//角度の補正をする
	g_aEnemy[nCntEnemy].rot.y += fRotDiff * 0.1f;

	//角度の正規化
	g_aEnemy[nCntEnemy].rot.y = RotNormalize(g_aEnemy[nCntEnemy].rot.y);

}

//==================================================================================
//敵の座標制限
//==================================================================================
void LimitPosEnemy(int nCntEnemy)
{

	if (g_aEnemy[nCntEnemy].pos.y < 0.0f)
	{//地面より下に行ったら

		g_aEnemy[nCntEnemy].pos.y = 0.0f;

		//重力処理
		g_aEnemy[nCntEnemy].move.y = 0.0f;

		if (g_aEnemy[nCntEnemy].bJump == true)
		{//ジャンプ中だったら

			//着地パーティクル
			SetParticle(g_aEnemy[nCntEnemy].pos, PARTICLE_TYPE_LAND);
		}

		g_aEnemy[nCntEnemy].bJump = false;

	}

	//移動制限
	if (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x < -1000.0f) { g_aEnemy[nCntEnemy].pos.x = -1000.0f - g_aEnemy[nCntEnemy].vtxMin.x; }
	if (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x > 1000.0f) { g_aEnemy[nCntEnemy].pos.x = 1000.0f - g_aEnemy[nCntEnemy].vtxMax.x; }
	if (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z > 1000.0f) { g_aEnemy[nCntEnemy].pos.z = 1000.0f - g_aEnemy[nCntEnemy].vtxMax.z; }
	if (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z < -1000.0f) { g_aEnemy[nCntEnemy].pos.z = -1000.0f - g_aEnemy[nCntEnemy].vtxMin.z; }

}


//==================================================================================
//弾のヒット処理
//==================================================================================
void HitEnemy(int nCntEnemy, D3DXVECTOR3 move, int nDMG)
{

	D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

	g_aEnemy[nCntEnemy].move = move;	//移動量加算
	g_aEnemy[nCntEnemy].nLife -= nDMG;	//体力減らす

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{//体力がなくなった

		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//死亡のパーティクルセット
		SetEnableShadow(g_aEnemy[nCntEnemy].nIdxShadow);	//影消す
		SetParticle(g_aEnemy[nCntEnemy].pos, PARTICLE_TYPE_DEAD);
	}
	else
	{//まだ体力が残ってる

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DMG;	//ダメージ状態へ
		g_aEnemy[nCntEnemy].nCntState = DMG_TIME;	//ダメージ状態を保つ時間を与える

		//頂点数分繰り返し
		for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].dwNumMat; nCntMat++)
		{//マテリアルを赤くする

			pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

}

//==================================================================================
//敵の状態更新処理
//==================================================================================
void UpdateStateEnemy(int nCntEnemy)
{

	D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

	switch (g_aEnemy[nCntEnemy].state)
	{
	case ENEMYSTATE_NONE:
		break;

	case ENEMYSTATE_DMG:

		//ダメージカウンター減算
		g_aEnemy[nCntEnemy].nCntState--;

		if (g_aEnemy[nCntEnemy].nCntState <= 0)
		{//ダメージカウンターが0以下

			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NONE;

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].dwNumMat; nCntMat++)
			{
				//元の色に戻す
				pMat[nCntMat].MatD3D.Diffuse = g_aEnemy[nCntEnemy].pMatData[nCntMat].MatD3D.Diffuse;
				pMat[nCntMat].MatD3D.Ambient = g_aEnemy[nCntEnemy].pMatData[nCntMat].MatD3D.Ambient;
			}

		}
		break;
	}
}

//==================================================================================
//敵の描画処理
//==================================================================================
void DrawEnemy(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
	{

		if (g_aEnemy[nCount].bUse == true)
		{//使用していたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCount].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCount].rot.y, g_aEnemy[nCount].rot.x, g_aEnemy[nCount].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCount].mtxWorld, &g_aEnemy[nCount].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCount].pos.x, g_aEnemy[nCount].pos.y, g_aEnemy[nCount].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCount].mtxWorld, &g_aEnemy[nCount].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCount].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCount].pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCount].dwNumMat; nCntMat++)
			{

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aEnemy[nCount].pTexture[nCntMat]);

				//敵(パーツ)の描画
				g_aEnemy[nCount].pMesh->DrawSubset(nCntMat);

			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);

		}

	}

}

//==================================================================================
//敵の情報取得
//==================================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==================================================================================
//敵の設定処理
//==================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
	{
		if (g_aEnemy[nCount].bUse == false)
		{//使用していなかったら

			g_aEnemy[nCount].pos = pos;	//位置
			g_aEnemy[nCount].rot = rot;	//向き
			g_aEnemy[nCount].nLife = nLife;	//向き
			g_aEnemy[nCount].bUse = true;	//使用している状況にする
			g_aEnemy[nCount].nATKInterval = ATK_INTERVAL;		//攻撃間隔

			//Xファイルの読み込み
			D3DXLoadMeshFromX(c_apModelname[nType],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aEnemy[nCount].pBuffMat,
				NULL,
				&g_aEnemy[nCount].dwNumMat,
				&g_aEnemy[nCount].pMesh);

			D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCount].pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCount].dwNumMat; nCntMat++)
			{
				//マテリアルのデータ取得
				g_aEnemy[nCount].pMatData[nCntMat] = pMat[nCntMat];

				if (pMat[nCntMat].pTextureFilename != NULL)
				{//ファイルが存在する

					//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_aEnemy[nCount].pTexture[nCntMat]);
				}
			}

			//敵の全頂点チェック
			CheckVtxEnemy(nCount);

			g_aEnemy[nCount].nIdxShadow = SetShadow(g_aEnemy[nCount].vtxMax.x, g_aEnemy[nCount].vtxMax.z);	//影を設定

			break;
		}
	}
}

//==================================================================================
//敵の読み込み処理
//==================================================================================
void ReadSetEnemy(void)
{
	FILE *pFile = NULL;	//ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen("data\\TEXT\\model_set.txt", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		char aComment[MAX_COMMENT];	//コメント用
		int nCntEnemy = 0;

		while (1)
		{//END_SCRIPTが来るまで繰り返す

			//文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			//敵の設定
			if (strcmp(aComment, "MODELSET") == 0)
			{//モデルの読み込みを開始

				while (strcmp(aComment, "END_MODELSET"))
				{//END_MODELSETが来るまで繰り返し

					fscanf(pFile, "%s", &aComment[0]);	//確認する

					if (strcmp(aComment, "TYPE") == 0)
					{//TYPEが来たら種類読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aEnemy[nCntEnemy].nType);	//モデル種類の列挙
					}

					if (strcmp(aComment, "POS") == 0)
					{//POSが来たら位置読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.x);	//X座標
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.y);	//Y座標
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.z);	//Z座標
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROTが来たら向き読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.x);	//Xの向き
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.y);	//Yの向き
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.z);	//Zの向き
					}

					if (strcmp(aComment, "LIFE") == 0)
					{//LIFEが来たら体力読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aEnemy[nCntEnemy].nLife);	//体力
					}

				}//END_MODELSETのかっこ

				 //敵の配置
				 SetEnemy(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].nType, g_aEnemy[nCntEnemy].nLife);

				//敵の番号加算
				nCntEnemy++;
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

//==================================================================================
//敵の全頂点チェック
//==================================================================================
void CheckVtxEnemy(int nCntEnemy)
{
	//モデルの頂点数を取得
	g_aEnemy[nCntEnemy].nNumVtx = g_aEnemy[nCntEnemy].pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	g_aEnemy[nCntEnemy].dwSizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMesh->GetFVF());

	//頂点バッファをロック
	g_aEnemy[nCntEnemy].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aEnemy[nCntEnemy].pVtxBuff);

	//全ての頂点をチェックする
	for (int nCntVtx = 0; nCntVtx < g_aEnemy[nCntEnemy].nNumVtx; nCntVtx++)
	{
		//今回のデータ
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aEnemy[nCntEnemy].pVtxBuff;	//頂点座標の代入

		//X
		if (g_aEnemy[nCntEnemy].vtxMin.x > vtx.x)
		{//今の最小値よりも今回の値がちいさかったら

			g_aEnemy[nCntEnemy].vtxMin.x = vtx.x;	//入れ替え
		}
		if (g_aEnemy[nCntEnemy].vtxMax.x < vtx.x)
		{//今の最大値よりも今回の値が大きかったら

			g_aEnemy[nCntEnemy].vtxMax.x = vtx.x;	//入れ替え
		}

		//Y
		if (g_aEnemy[nCntEnemy].vtxMin.y > vtx.y)
		{//今の最小値よりも今回の値がちいさかったら

			g_aEnemy[nCntEnemy].vtxMin.y = vtx.y;	//入れ替え
		}
		if (g_aEnemy[nCntEnemy].vtxMax.y < vtx.y)
		{//今の最大値よりも今回の値が大きかったら

			g_aEnemy[nCntEnemy].vtxMax.y = vtx.y;	//入れ替え
		}

		//Z
		if (g_aEnemy[nCntEnemy].vtxMin.z > vtx.z)
		{//今の最小値よりも今回の値がちいさかったら

			g_aEnemy[nCntEnemy].vtxMin.z = vtx.z;	//入れ替え
		}
		if (g_aEnemy[nCntEnemy].vtxMax.z < vtx.z)
		{//今の最大値よりも今回の値が大きかったら

			g_aEnemy[nCntEnemy].vtxMax.z = vtx.z;	//入れ替え
		}

		g_aEnemy[nCntEnemy].pVtxBuff += g_aEnemy[nCntEnemy].dwSizeFVF;	//サイズ分ポインタ移動
	}

	//頂点バッファをアンロック
	g_aEnemy[nCntEnemy].pMesh->UnlockVertexBuffer();
}
