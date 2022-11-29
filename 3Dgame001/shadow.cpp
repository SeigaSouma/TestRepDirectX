//=============================================================================
//
// 影処理 [shadow.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "shadow.h"
#include "input.h"

//マクロ定義
#define POS_SHADOW	(50.0f)
#define MOVE		(0.0f)
#define MAX_TOP		(4)

//グローバル変数宣言
const char *c_apFilenameShadow[] =					//ファイル読み込み
{
	"data\\TEXTURE\\Shadow2.png",
};
//LPDIRECT3DTEXTURE9 g_apTextureShadow[(sizeof c_apFilenameShadow) / sizeof(*c_apFilenameShadow)] = {};	//テクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
SHADOW g_aShadow[MAX_SHADOW];						//影の情報

//==================================================================================
//影の初期化処理
//==================================================================================
void InitShadow(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShadow);

	//各要素初期化
	for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
	{
		g_aShadow[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCount].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-POS_SHADOW, 0, POS_SHADOW);
		pVtx[1].pos = D3DXVECTOR3(POS_SHADOW, 0, POS_SHADOW);
		pVtx[2].pos = D3DXVECTOR3(-POS_SHADOW, 0, -POS_SHADOW);
		pVtx[3].pos = D3DXVECTOR3(POS_SHADOW, 0, -POS_SHADOW);

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

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//==================================================================================
//影の終了処理
//==================================================================================
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//==================================================================================
//影の更新処理
//==================================================================================
void UpdateShadow(void)
{

}

//==================================================================================
//影の描画処理
//==================================================================================
void DrawShadow(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
	{

		if (g_aShadow[nCount].bUse == true)
		{//使用していたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCount].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCount].rot.y, g_aShadow[nCount].rot.x, g_aShadow[nCount].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCount].mtxWorld, &g_aShadow[nCount].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCount].pos.x, g_aShadow[nCount].pos.y, g_aShadow[nCount].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCount].mtxWorld, &g_aShadow[nCount].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCount].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//影の描画
 			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);

		}

	}

	//減算合成をもとに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//==================================================================================
//影の設定処理
//==================================================================================
int SetShadow(float fWidth, float fHeight)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	int nCntShadow = -1;

	for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
	{
		if (g_aShadow[nCount].bUse == false)
		{//使用していなかったら

			nCntShadow = nCount;			//インデックス番号取得
			g_aShadow[nCount].bUse = true;	//使用している状況にする

			fWidth *= 1.5f;
			fHeight *= 1.5f;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-fWidth, 0, fWidth);
			pVtx[1].pos = D3DXVECTOR3(fWidth, 0, fWidth);
			pVtx[2].pos = D3DXVECTOR3(-fWidth, 0, -fWidth);
			pVtx[3].pos = D3DXVECTOR3(fWidth, 0, -fWidth);
			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//==================================================================================
//影の有効設定処理
//==================================================================================
void SetEnableShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}

//==================================================================================
//影の位置設定処理
//==================================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{

	g_aShadow[nIdxShadow].pos = D3DXVECTOR3(pos.x, 0.0f, pos.z);
}