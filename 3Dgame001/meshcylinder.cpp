//=============================================================================
//
// メッシュシリンダー処理 [meshcylinder.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "meshcylinder.h"
#include "input.h"

//マクロ定義
#define POS_MESHCYLINDER	(5000.0f)
#define POS_MESHCYLINDER_Y	(1000.0f)
#define MOVE		(0.0f)
#define WIDTH		(16)
#define HEIGHT		(1)

//グローバル変数宣言
const char *c_apFilenameMeshCylinder[] =					//ファイル読み込み
{
	"data\\TEXTURE\\field002.png",
};
//LPDIRECT3DTEXTURE9 g_apTextureMeshCylinder[(sizeof c_apFilenameMeshCylinder) / sizeof(*c_apFilenameMeshCylinder)] = {};	//テクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;	//インデックスバッファへのポインタ
MESHCYLINDER g_aMeshCylinder;									//メッシュシリンダーの情報

//==================================================================================
//メッシュシリンダーの初期化処理
//==================================================================================
void InitMeshCylinder(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\mountain000.png",
		&g_pTextureMeshCylinder);

	//各要素初期化
	g_aMeshCylinder.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.nNumIndex = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//インデックス数
	g_aMeshCylinder.nNumVertex = (HEIGHT + 1) * (WIDTH + 1);	//頂点数

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshCylinder.nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[128];		//計算用の座標
	float fRot = (D3DX_PI * 2) / (float)(WIDTH);	//1つごとの角度を求める

	D3DXVECTOR3 posTest = D3DXVECTOR3(1000.0f, 500.0f, 2000.0f);
	D3DXVECTOR3 NormalizeNor[128];

	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//縦の頂点数分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//横の頂点数分繰り返す

			pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * POS_MESHCYLINDER,
				(POS_MESHCYLINDER_Y * HEIGHT) - ((POS_MESHCYLINDER_Y * nCntHeight)),
				cosf(nCntWidth % WIDTH * fRot) * POS_MESHCYLINDER
			);

			pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * POS_MESHCYLINDER,
				(POS_MESHCYLINDER_Y * HEIGHT) - ((POS_MESHCYLINDER_Y * (nCntHeight + 1))),
				cosf(nCntWidth % WIDTH * fRot) * POS_MESHCYLINDER
			);

			//各頂点から原点を引く
			NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aMeshCylinder.pos;
			NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aMeshCylinder.pos;

			//出た向きの値を正規化する
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))],				&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))],	&NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);

		}
	}

	//頂点情報の設定
	for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
	{//縦の頂点数分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//横の頂点数分繰り返す

			//頂点座標の設定
			pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

			//法線ベクトルの設定
			pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))];

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2
			(
				nCntWidth * (1.0f / (float)(WIDTH / 4)),
				nCntHeight * (1.0f / (float)(HEIGHT))
			);

			pVtx += 1;
		}

	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshCylinder->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshCylinder.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

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

			pIdx[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (WIDTH + 1) * (nCntHeight + 2);

			nCntIdx += 2;	//2つずつ見てるから
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshCylinder->Unlock();




	//for (int nCntWidth = 0; nCntWidth < g_aMeshWall[nCntWall].nWidth + 1; nCntWidth++)
	//{//横の頂点数分繰り返す

	//	pIdx[nCntIdx + 0] = nCntWidth + (WIDTH + 1) * (nCntHeight + 1);
	//	pIdx[nCntIdx + 1] = nCntWidth + ((WIDTH + 1) * nCntHeight);
	//	nCntIdx += 2;	//2つずつ見てるから
	//}

	//if (nCntHeight + 1 < g_aMeshWall[nCntWall].nHeight)
	//{//最後のちょんは打たない

	// //空打ち2つ分
	//	pIdx[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 1)) - 1;
	//	pIdx[nCntIdx + 1] = (WIDTH + 1) * (nCntHeight + 2);
	//	nCntIdx += 2;	//2つずつ見てるから
	//}






}

//==================================================================================
//メッシュシリンダーの終了処理
//==================================================================================
void UninitMeshCylinder(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}

}

//==================================================================================
//メッシュシリンダーの更新処理
//==================================================================================
void UpdateMeshCylinder(void)
{

}

//==================================================================================
//メッシュシリンダーの描画処理
//==================================================================================
void DrawMeshCylinder(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aMeshCylinder.mtxWorld);

	//向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshCylinder.rot.y, g_aMeshCylinder.rot.x, g_aMeshCylinder.rot.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.mtxWorld, &g_aMeshCylinder.mtxWorld, &mtxRot);

	//位置を反映する
	D3DXMatrixTranslation(&mtxTrans, g_aMeshCylinder.pos.x, g_aMeshCylinder.pos.y, g_aMeshCylinder.pos.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.mtxWorld, &g_aMeshCylinder.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_aMeshCylinder.nNumVertex,
		0,
		g_aMeshCylinder.nNumIndex - 2);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}