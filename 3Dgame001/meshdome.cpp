//=============================================================================
//
// メッシュドーム処理 [meshdome.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "meshdome.h"
#include "input.h"

//マクロ定義
#define POS_MESHDOME	(8000.0f)
#define POS_MESHDOME_Y	(POS_MESHDOME)
#define MOVE		(0.0f)
#define WIDTH		(16)
#define HEIGHT		(8)

//プロトタイプ宣言
void InitMeshDomeLid(LPDIRECT3DDEVICE9 pDevice);

//グローバル変数宣言
const char *c_apFilenameMeshDome[] =					//ファイル読み込み
{
	"data\\TEXTURE\\field002.png",
};
//LPDIRECT3DTEXTURE9 g_apTextureMeshDome[(sizeof c_apFilenameMeshDome) / sizeof(*c_apFilenameMeshDome)] = {};	//テクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshDome = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshDome = NULL;	//インデックスバッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshDomeLid = NULL;	//インデックスバッファへの蓋ポインタ
MESHDOME g_aMeshDome;									//メッシュドームの情報

//==================================================================================
//メッシュドームの初期化処理
//==================================================================================
void InitMeshDome(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sky000.jpg",
		&g_pTextureMeshDome);

	//各要素初期化
	g_aMeshDome.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshDome.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshDome.nNumIndexLid = ((WIDTH + 1) + 1);	//蓋のインデックス数
	g_aMeshDome.nNumIndex = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//蓋なしのインデックス数
	g_aMeshDome.nNumVertex = (HEIGHT + 1) * (WIDTH + 1);	//頂点数

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshDome.nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshDome,
		NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshDome->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[256];		//計算用の座標
	D3DXVECTOR3 NormalizeNor[256];
	float fRotWidth = (D3DX_PI * 2) / (float)(WIDTH);	//1つごとの角度を求める, 周囲
	float fRotHeight = (D3DX_PI * 0.5f) / (float)(HEIGHT);	//1つごとの角度を求める, 高さ
	float fRotCalW = 0.0f;
	float fRotCalH = 0.0f;

	pos[WIDTH + (HEIGHT * (WIDTH + 1))] = D3DXVECTOR3(0.0f, POS_MESHDOME_Y, 0.0f);

	for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
	{//縦の頂点数分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//横の頂点数分繰り返す

			//今回の角度を割り出す
			fRotCalH = (float)(nCntHeight % (HEIGHT +1 )) * fRotHeight;
			fRotCalW = (fRotWidth * WIDTH) - ((float)(nCntWidth % (WIDTH + 1)) * fRotWidth);

			////角度の正規化
			fRotCalW = RotNormalize(fRotCalW);
			fRotCalH = RotNormalize(fRotCalH);

			////座標割り出し
			pos[nCntWidth + (nCntHeight * (WIDTH + 1))].x = cosf(fRotCalH) * sinf(fRotCalW) * POS_MESHDOME;
			pos[nCntWidth + (nCntHeight * (WIDTH + 1))].z = cosf(fRotCalH) * cosf(fRotCalW) * POS_MESHDOME;
			pos[nCntWidth + (nCntHeight * (WIDTH + 1))].y = sinf(fRotCalH) * POS_MESHDOME_Y;

			//各頂点から原点を引く
			NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aMeshDome.pos;

			//出た向きの値を正規化する
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))], &NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))]);

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

			if (nCntHeight < HEIGHT - 1)
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.3f, 1.0f);
			}


			pVtx[0].tex = -D3DXVECTOR2
			(
				nCntWidth * (1.0f / (float)(WIDTH)),
				nCntHeight * (1.0f / (float)(HEIGHT))
			);

			pVtx += 1;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshDome->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshDome.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshDome,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスバッファの要素番号
	int nCntIdx = 0;
	int aaaa[512];

	//頂点番号データの設定
	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//高さの分割数分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//横の頂点数分繰り返す

			pIdx[nCntIdx + 0] = nCntWidth + (WIDTH + 1) * (nCntHeight + 1);
			pIdx[nCntIdx + 1] = nCntWidth + ((WIDTH + 1) * nCntHeight);

			aaaa[nCntIdx + 0] = nCntWidth + (WIDTH + 1) * (nCntHeight + 1);
			aaaa[nCntIdx + 1] = nCntWidth + ((WIDTH + 1) * nCntHeight);
			
			nCntIdx += 2;	//2つずつ見てるから
		}

		if (nCntHeight + 1 < HEIGHT)
		{//最後のちょんは打たない

			pIdx[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (WIDTH + 1) * (nCntHeight + 2);

			aaaa[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 1)) - 1;
			aaaa[nCntIdx + 1] = (WIDTH + 1) * (nCntHeight + 2);

			nCntIdx += 2;	//2つずつ見てるから
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshDome->Unlock();

	//蓋のインデックス
	InitMeshDomeLid(pDevice);
	

}

//==================================================================================
//メッシュドームの終了処理
//==================================================================================
void UninitMeshDome(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshDome != NULL)
	{
		g_pTextureMeshDome->Release();
		g_pTextureMeshDome = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshDome != NULL)
	{
		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffMeshDome != NULL)
	{
		g_pIdxBuffMeshDome->Release();
		g_pIdxBuffMeshDome = NULL;
	}

}

//==================================================================================
//メッシュドームの更新処理
//==================================================================================
void UpdateMeshDome(void)
{

}

//==================================================================================
//メッシュドームの描画処理
//==================================================================================
void DrawMeshDome(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aMeshDome.mtxWorld);

	//向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshDome.rot.y, g_aMeshDome.rot.x, g_aMeshDome.rot.z);
	D3DXMatrixMultiply(&g_aMeshDome.mtxWorld, &g_aMeshDome.mtxWorld, &mtxRot);

	//位置を反映する
	D3DXMatrixTranslation(&mtxTrans, g_aMeshDome.pos.x, g_aMeshDome.pos.y, g_aMeshDome.pos.z);
	D3DXMatrixMultiply(&g_aMeshDome.mtxWorld, &g_aMeshDome.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshDome.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshDome, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshDome);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_aMeshDome.nNumVertex,
		0,
		g_aMeshDome.nNumIndex - 2);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshDome);

	// 蓋のインデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshDomeLid);

	// ポリゴンの描画(TRIANGLEFAN)
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN,
		0,
		0,
		25,
		0,
		g_aMeshDome.nNumIndexLid - 2);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==================================================================================
//メッシュドームの蓋のインデックス初期化
//==================================================================================
void InitMeshDomeLid(LPDIRECT3DDEVICE9 pDevice)
{
	//蓋のインデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshDome.nNumIndexLid,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshDomeLid,
		NULL);

	WORD *pIdx;	//蓋のインデックス情報へのポインタ

				//蓋のインデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshDomeLid->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスバッファの要素番号
	int nCntIdx = 0;
	int aaaa[512];

	//頂点の1つ
	pIdx[0] = HEIGHT * (WIDTH + 1) + WIDTH - (WIDTH);
	aaaa[0] = HEIGHT * (WIDTH + 1) + WIDTH - (WIDTH);

	//119から136まで
	for (int nCntIdx = 0; nCntIdx < WIDTH + 1; nCntIdx++)
	{//蓋の頂点分繰り返す

		pIdx[nCntIdx + 1] = (HEIGHT * (WIDTH + 1) + nCntIdx) - (WIDTH + 1);
		aaaa[nCntIdx + 1] = (HEIGHT * (WIDTH + 1) + nCntIdx) - (WIDTH + 1);
	}


	//インデックスバッファをアンロックする
	g_pIdxBuffMeshDomeLid->Unlock();
}