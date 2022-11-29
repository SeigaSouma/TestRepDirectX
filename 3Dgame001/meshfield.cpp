//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "meshfield.h"
#include "input.h"

//マクロ定義
#define POS_MESHFIELD	(1000.0f)
#define POS_MESHFIELD_X	(250.0f)
#define POS_MESHFIELD_Z	(250.0f)
#define MOVE		(0.0f)
#define WIDTH		(16)
#define HEIGHT		(16)

//グローバル変数宣言
const char *c_apFilenameMeshField[] =					//ファイル読み込み
{
	"data\\TEXTURE\\field002.png",
};
//LPDIRECT3DTEXTURE9 g_apTextureMeshField[(sizeof c_apFilenameMeshField) / sizeof(*c_apFilenameMeshField)] = {};	//テクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;	//インデックスバッファへのポインタ
MESHFIELD g_aMeshField;									//メッシュフィールドの情報

//==================================================================================
//メッシュフィールドの初期化処理
//==================================================================================
void InitMeshField(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\field002.jpg",
		&g_pTextureMeshField);

	//各要素初期化
	g_aMeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.nNumIndex = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//インデックス数

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((WIDTH + 1) * (HEIGHT +1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
	{//縦の頂点数分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//横の頂点数分繰り返す

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(
				(POS_MESHFIELD_X * nCntWidth) - ((POS_MESHFIELD_X * WIDTH) * 0.5f),
				0.0f,
				-((POS_MESHFIELD_Z * nCntHeight) - ((POS_MESHFIELD_Z * HEIGHT) * 0.5f)));

			//法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2
			(
				((float)nCntWidth / (1.0f / (float)(WIDTH + 1))) * (1.0f / (float)(WIDTH + 1)),
				((float)nCntHeight / (1.0f / (float)(HEIGHT + 1))) * (1.0f / (float)(HEIGHT + 1))
			);

			pVtx += 1;
		}

	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshField->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshField.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスバッファの要素番号
	int nCntIdx = 0;

	//頂点番号データの設定
	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//高さの分割数分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//横の頂点数分繰り返す

			pIdx[nCntIdx + 0] = nCntWidth + (WIDTH + 1) * (nCntHeight + 1);
			pIdx[nCntIdx + 1] = nCntWidth + ((WIDTH + 1) * nCntHeight);
			nCntIdx += 2;	//2つずつ見てるから
		}

		if (nCntHeight + 1 < HEIGHT)
		{//最後のちょんは打たない

			//空打ち2つ分
			pIdx[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (WIDTH + 1) * (nCntHeight + 2);
			nCntIdx += 2;	//2つずつ見てるから

		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshField->Unlock();

}

//==================================================================================
//メッシュフィールドの終了処理
//==================================================================================
void UninitMeshField(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}

}

//==================================================================================
//メッシュフィールドの更新処理
//==================================================================================
void UpdateMeshField(void)
{

}

//==================================================================================
//メッシュフィールドの描画処理
//==================================================================================
void DrawMeshField(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aMeshField.mtxWorld);

	//向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField.rot.y, g_aMeshField.rot.x, g_aMeshField.rot.z);
	D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxRot);

	//位置を反映する
	D3DXMatrixTranslation(&mtxTrans, g_aMeshField.pos.x, g_aMeshField.pos.y, g_aMeshField.pos.z);
	D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		((WIDTH + 1) * (HEIGHT + 1)),
		0,
		g_aMeshField.nNumIndex - 2);

}