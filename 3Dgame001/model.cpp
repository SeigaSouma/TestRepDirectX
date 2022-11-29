//=============================================================================
//
// モデル処理 [model.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "enemy.h"

//マクロ定義
#define MOVE		(1.0f)
#define MAX_TX		(20)

//プロトタイプ宣言
void ControllModel(void);
void CheckVtxModel(void);
void CollisionEnemy(void);

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureModel[MAX_TX] = {};	//テクスチャへのポインタ
LPD3DXMESH g_pMeshModel = NULL;			//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL;	//マテリアルへのポインタ
DWORD g_dwNumMatModel = 0;				//マテリアルの数
Model g_aModel;							//モデルの情報

//==================================================================================
//モデルの初期化処理
//==================================================================================
void InitModel(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//各要素初期化
	g_aModel.pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
	g_aModel.posOld = g_aModel.pos;
	g_aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aModel.nIdxShadow = -1;

	g_aModel.nNumVtx = 0;	//頂点数
	g_aModel.vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	g_aModel.vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\cube.x",
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_pBuffMatModel,
						NULL,
						&g_dwNumMatModel,
						&g_pMeshModel);

	D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	//頂点数分繰り返し
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{

		if (pMat[nCntMat].pTextureFilename != NULL)
		{//ファイルが存在する

			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureModel[nCntMat]);

		}

	}

	//モデルの全頂点チェック
	CheckVtxModel();

	g_aModel.nIdxShadow = SetShadow(g_aModel.vtxMax.x, g_aModel.vtxMax.z);	//影を設定
}

//==================================================================================
//モデルの終了処理
//==================================================================================
void UninitModel(void)
{

	for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
	{

		//テクスチャの破棄
		if (g_pTextureModel[nCntTex] != NULL)
		{
			g_pTextureModel[nCntTex]->Release();
			g_pTextureModel[nCntTex] = NULL;
		}
	}

	//メッシュの破棄
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//==================================================================================
//モデルの更新処理
//==================================================================================
void UpdateModel(void)
{
	//前回の位置を取得
	g_aModel.posOld = g_aModel.pos;

	//モデルの操作
	ControllModel();

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{//SPACEが押された,弾発射

		SetBullet(D3DXVECTOR3(
			g_aModel.pos.x - sinf(g_aModel.rot.y) * (POS_BULLET * 2.0f),
			g_aModel.pos.y + POS_BULLET,
			g_aModel.pos.z - cosf(g_aModel.rot.y) * (POS_BULLET * 2.0f)),	//敵の座標
			D3DXVECTOR3(
				sinf(D3DX_PI + g_aModel.rot.y), 
				0.0f,
				cosf(D3DX_PI + g_aModel.rot.y)),	//移動方向
			BULLETTYPE_PLAYER);	//弾の種類
	}

	//移動制限
	if (g_aModel.pos.x + g_aModel.vtxMin.x <= -1000.0f && g_aModel.posOld.x + g_aModel.vtxMin.x >= -1000.0f)
	{//今回が-1000以下, 前回が-1000以上

		g_aModel.pos.x = -1000.0f - g_aModel.vtxMin.x;
	}
	if (g_aModel.pos.x + g_aModel.vtxMax.x >= 1000.0f && g_aModel.posOld.x + g_aModel.vtxMax.x <= 1000.0f)
	{//今回が1000以上, 前回が1000以下

		g_aModel.pos.x = 1000.0f - g_aModel.vtxMax.x;
	}
	if (g_aModel.pos.z + g_aModel.vtxMax.z >= 1000.0f && g_aModel.posOld.z + g_aModel.vtxMax.z <= 1000.0f)
	{//今回が1000以上, 前回が1000以下

		g_aModel.pos.z = 1000.0f - g_aModel.vtxMax.z;
	}
	if (g_aModel.pos.z + g_aModel.vtxMin.z <= -1000.0f && g_aModel.posOld.z + g_aModel.vtxMin.z >= -1000.0f)
	{//今回が-1000以下, 前回が-1000以上

		g_aModel.pos.z = -1000.0f - g_aModel.vtxMin.z;
	}

	//敵との当たり判定
	CollisionEnemy();

	//影の位置更新
	SetPositionShadow(g_aModel.nIdxShadow, g_aModel.pos);

}

//==================================================================================
//モデルの操作
//==================================================================================
void ControllModel(void)
{
	//カメラの情報取得
	Camera aCamera = GetCamera();

	float fRotMove = g_aModel.rot.y;	//現在の角度
	float fRotDest = g_aModel.rot.y;	//目標の角度
	float fRotDiff = 0.0f;				//補正

	if (GetKeyboardPress(DIK_LEFT) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
	{//←キーが押された,左移動

		if (GetKeyboardPress(DIK_UP) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
		{//← + ↑,左上移動

			g_aModel.move.x += sinf(-D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(-D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
			fRotDest = D3DX_PI * MOVE_LRUP + aCamera.rot.y;
		}
		else if (GetKeyboardPress(DIK_DOWN) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
		{//← + ↓,左下移動

			g_aModel.move.x += sinf(-D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(-D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
			fRotDest = D3DX_PI * MOVE_LRDW + aCamera.rot.y;
		}
		else
		{//左移動

			g_aModel.move.x += sinf(-D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(-D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
			fRotDest = D3DX_PI * MOVE_LR + aCamera.rot.y;
		}

	}
	else if (GetKeyboardPress(DIK_RIGHT) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
	{//→キーが押された,右移動

		if (GetKeyboardPress(DIK_UP) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
		{//→ + ↑,右上移動

			g_aModel.move.x += sinf(D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
			fRotDest = -D3DX_PI * MOVE_LRUP + aCamera.rot.y;
		}
		else if (GetKeyboardPress(DIK_DOWN) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
		{//← + ↓,右下移動

			g_aModel.move.x += sinf(D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
			fRotDest = -D3DX_PI * MOVE_LRDW + aCamera.rot.y;
		}
		else
		{//右移動

			g_aModel.move.x += sinf(D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
			fRotDest = -D3DX_PI * MOVE_LR + aCamera.rot.y;
		}

	}
	else if (GetKeyboardPress(DIK_UP) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
	{//↑キーが押された,上移動

		g_aModel.move.x += sinf(aCamera.rot.y) * MOVE;
		g_aModel.move.z += cosf(aCamera.rot.y) * MOVE;
		fRotDest = D3DX_PI + aCamera.rot.y;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
	{//↓キーが押された,下移動

		g_aModel.move.x += sinf(D3DX_PI + aCamera.rot.y) * MOVE;
		g_aModel.move.z += cosf(D3DX_PI + aCamera.rot.y) * MOVE;
		fRotDest = aCamera.rot.y;
	}

	if (GetKeyboardPress(DIK_I) == true)
	{//Iキーが押された,上昇

		g_aModel.pos.y += MOVE;
	}
	else if (GetKeyboardPress(DIK_K) == true)
	{//Kキーが押された,下降

		g_aModel.pos.y -= MOVE;
	}

	//現在と目標の差分を求める
	fRotDiff = fRotDest - fRotMove;

	//1周分越えたら補正する
	if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2.0f;
	}
	else if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2.0f;
	}

	//角度の補正をする
	g_aModel.rot.y += fRotDiff * 0.1f;

	//1周分越えたら補正する
	if (g_aModel.rot.y < -D3DX_PI)
	{
		g_aModel.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_aModel.rot.y > D3DX_PI)
	{
		g_aModel.rot.y -= D3DX_PI * 2.0f;
	}

	//位置更新
	g_aModel.pos += g_aModel.move;

	//慣性つける
	g_aModel.move.x += (0.0f - g_aModel.move.x) * 0.1f;
	g_aModel.move.z += (0.0f - g_aModel.move.z) * 0.1f;

}

//==================================================================================
//敵との当たり判定
//==================================================================================
void CollisionEnemy(void)
{
	//敵の情報取得
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//敵が使用されていたら

			//横の判定
			if (g_aModel.pos.z + g_aModel.vtxMax.z > pEnemy->pos.z + pEnemy->vtxMin.z &&	//敵の最小値に入ってる
				g_aModel.pos.z + g_aModel.vtxMin.z < pEnemy->pos.z + pEnemy->vtxMax.z)		//敵の最大値に入ってる
			{//敵のZ幅の中にいるとき

				if (g_aModel.pos.x + g_aModel.vtxMin.x <= pEnemy->pos.x + pEnemy->vtxMax.x &&	//現在の最小値が敵の最大値よりも小さい
					g_aModel.posOld.x + g_aModel.vtxMin.x >= pEnemy->pos.x + pEnemy->vtxMax.x)	//前回の最小値が敵の最大値よりも大きい
				{//敵の位置に補正

					g_aModel.pos.x = (pEnemy->pos.x + pEnemy->vtxMax.x) - g_aModel.vtxMin.x;
				}
				if (g_aModel.pos.x + g_aModel.vtxMax.x >= pEnemy->pos.x + pEnemy->vtxMin.x &&	//現在の最大値が敵の最小値よりも大きい
					g_aModel.posOld.x + g_aModel.vtxMax.x <= pEnemy->pos.x + pEnemy->vtxMin.x)	//前回の最大値が敵の最小値よりも小さい
				{//敵の位置に補正

					g_aModel.pos.x = (pEnemy->pos.x + pEnemy->vtxMin.x) - g_aModel.vtxMax.x;
				}
			}


			//縦の判定
			if (g_aModel.pos.x + g_aModel.vtxMax.x > pEnemy->pos.x + pEnemy->vtxMin.x &&	//敵の最小値に入ってる
				g_aModel.pos.x + g_aModel.vtxMin.x < pEnemy->pos.x + pEnemy->vtxMax.x)		//敵の最大値に入ってる
			{//敵のX幅の中にいるとき

				if (g_aModel.pos.z + g_aModel.vtxMin.z <= pEnemy->pos.z + pEnemy->vtxMax.z &&	//現在の最小値が敵の最大値よりも小さい
					g_aModel.posOld.z + g_aModel.vtxMin.z >= pEnemy->pos.z + pEnemy->vtxMax.z)	//前回の最小値が敵の最大値よりも大きい
				{//敵の位置に補正

					g_aModel.pos.z = (pEnemy->pos.z + pEnemy->vtxMax.z) - g_aModel.vtxMin.z;
				}
				if (g_aModel.pos.z + g_aModel.vtxMax.z >= pEnemy->pos.z + pEnemy->vtxMin.z &&	//現在の最大値が敵の最小値よりも大きい
					g_aModel.posOld.z + g_aModel.vtxMax.z <= pEnemy->pos.z + pEnemy->vtxMin.z)	//前回の最大値が敵の最小値よりも小さい
				{//敵の位置に補正

					g_aModel.pos.z = (pEnemy->pos.z + pEnemy->vtxMin.z) - g_aModel.vtxMax.z;
				}

			}

		}

	}

}

//==================================================================================
//モデルの描画処理
//==================================================================================
void DrawModel(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aModel.mtxWorld);

	//向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel.rot.y, g_aModel.rot.x, g_aModel.rot.z);
	D3DXMatrixMultiply(&g_aModel.mtxWorld, &g_aModel.mtxWorld, &mtxRot);

	//位置を反映する
	D3DXMatrixTranslation(&mtxTrans, g_aModel.pos.x, g_aModel.pos.y, g_aModel.pos.z);
	D3DXMatrixMultiply(&g_aModel.mtxWorld, &g_aModel.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aModel.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	//頂点数分繰り返し
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{

		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureModel[nCntMat]);

		//モデル(パーツ)の描画
		g_pMeshModel->DrawSubset(nCntMat);

	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

}

//==================================================================================
//モデルの全頂点チェック
//==================================================================================
void CheckVtxModel(void)
{
	//モデルの頂点数を取得
	g_aModel.nNumVtx = g_pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	g_aModel.dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//頂点バッファをロック
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aModel.pVtxBuff);

	//全ての頂点をチェックする
	for (int nCntVtx = 0; nCntVtx < g_aModel.nNumVtx; nCntVtx++)
	{
		//今回のデータ
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aModel.pVtxBuff;	//頂点座標の代入

		//X
		if (g_aModel.vtxMin.x > vtx.x)
		{//今の最小値よりも今回の値がちいさかったら

			g_aModel.vtxMin.x = vtx.x;	//入れ替え
		}
		if (g_aModel.vtxMax.x < vtx.x)
		{//今の最大値よりも今回の値が大きかったら

			g_aModel.vtxMax.x = vtx.x;	//入れ替え
		}

		//Y
		if (g_aModel.vtxMin.y > vtx.y)
		{//今の最小値よりも今回の値がちいさかったら

			g_aModel.vtxMin.y = vtx.y;	//入れ替え
		}
		if (g_aModel.vtxMax.y < vtx.y)
		{//今の最大値よりも今回の値が大きかったら

			g_aModel.vtxMax.y = vtx.y;	//入れ替え
		}

		//Z
		if (g_aModel.vtxMin.z > vtx.z)
		{//今の最小値よりも今回の値がちいさかったら

			g_aModel.vtxMin.z = vtx.z;	//入れ替え
		}
		if (g_aModel.vtxMax.z < vtx.z)
		{//今の最大値よりも今回の値が大きかったら

			g_aModel.vtxMax.z = vtx.z;	//入れ替え
		}

		g_aModel.pVtxBuff += g_aModel.dwSizeFVF;	//サイズ分ポインタ移動
	}

	//頂点バッファをアンロック
	g_pMeshModel->UnlockVertexBuffer();
}

//==================================================================================
//モデルの情報取得
//==================================================================================
Model GetModel(void)
{
	return g_aModel;
}
