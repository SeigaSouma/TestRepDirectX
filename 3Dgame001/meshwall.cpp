//=============================================================================
//
// メッシュウォール処理 [meshwall.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "meshwall.h"
#include "input.h"

//グローバル変数宣言
const char *c_apFilenameMeshWall[] =					//ファイル読み込み
{
	"data\\TEXTURE\\field002.png",
};
//LPDIRECT3DTEXTURE9 g_apTextureMeshWall[(sizeof c_apFilenameMeshWall) / sizeof(*c_apFilenameMeshWall)] = {};	//テクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshWall = NULL;	//インデックスバッファへのポインタ
MESHWALL g_aMeshWall[MAX_WALL];						//メッシュウォールの情報
int g_nNumIndexaMeshWall;			//インデックス数
int g_nNumVertexaMeshWall;			//頂点数

//==================================================================================
//メッシュウォールの初期化処理
//==================================================================================
void InitMeshWall(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall000.jpg",
		&g_pTextureMeshWall);

	//各要素初期化
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aMeshWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].nNumVertex = 0;
		g_aMeshWall[nCntWall].nNumIndex = 0;
		g_aMeshWall[nCntWall].nWidth = 0;
		g_aMeshWall[nCntWall].nHeight = 0;
		g_aMeshWall[nCntWall].bUse = false;
	}
	g_nNumIndexaMeshWall = 0;
	g_nNumVertexaMeshWall = 0;

	//壁の設置
	SetMeshWall(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 8, 2);
	SetMeshWall(D3DXVECTOR3(1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 8, 2);
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 8, 2);
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 8, 2);

	//インデックス・頂点数を求める
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//使用していたら

			g_nNumIndexaMeshWall += g_aMeshWall[nCntWall].nNumIndex;	//インデックス数
			g_nNumVertexaMeshWall += g_aMeshWall[nCntWall].nNumVertex;	//頂点数
		}
	}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexaMeshWall,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//使用していたら

			//頂点情報の設定
			for (int nCntHeight = 0; nCntHeight < g_aMeshWall[nCntWall].nHeight + 1; nCntHeight++)
			{//縦の頂点数分繰り返す

				for (int nCntWidth = 0; nCntWidth < g_aMeshWall[nCntWall].nWidth + 1; nCntWidth++)
				{//横の頂点数分繰り返す

					//頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3
					(
						(POS_MESHWALL * nCntWidth) - ((POS_MESHWALL * g_aMeshWall[nCntWall].nWidth) * 0.5f),
						(POS_MESHWALL * g_aMeshWall[nCntWall].nHeight) - ((POS_MESHWALL * nCntHeight)),
						0.0f
					);

					//法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					//頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2
					(
						((float)nCntWidth / (1.0f / (float)(g_aMeshWall[nCntWall].nWidth + 1))) * (1.0f / (float)(g_aMeshWall[nCntWall].nWidth + 1)),
						((float)nCntHeight / (1.0f / (float)(g_aMeshWall[nCntWall].nHeight + 1))) * (1.0f / (float)(g_aMeshWall[nCntWall].nHeight + 1))
					);

					pVtx += 1;
				}
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshWall->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexaMeshWall,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshWall,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスバッファの要素番号
	int nCntIdx = 0;
	int nVtxPoint = 0;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//使用していたら

			//頂点番号データの設定
			for (int nCntHeight = 0; nCntHeight < g_aMeshWall[nCntWall].nHeight; nCntHeight++)
			{//高さの分割数分繰り返す

				for (int nCntWidth = 0; nCntWidth < g_aMeshWall[nCntWall].nWidth + 1; nCntWidth++)
				{//横の頂点数分繰り返す

					pIdx[nCntIdx + 0] = (nCntWidth + (g_aMeshWall[nCntWall].nWidth + 1) * (nCntHeight + 1)) + nVtxPoint;
					pIdx[nCntIdx + 1] = (nCntWidth + ((g_aMeshWall[nCntWall].nWidth + 1) * nCntHeight)) + nVtxPoint;
					nCntIdx += 2;	//2つずつ見てるから
				}

				if (nCntHeight + 1 < g_aMeshWall[nCntWall].nHeight)
				{//最後のちょんは打たない

					//空打ち2つ分
					pIdx[nCntIdx + 0] = (((g_aMeshWall[nCntWall].nWidth + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
					pIdx[nCntIdx + 1] = ((g_aMeshWall[nCntWall].nWidth + 1) * (nCntHeight + 2)) + nVtxPoint;
					nCntIdx += 2;	//2つずつ見てるから
				}
			}

			nVtxPoint += g_aMeshWall[nCntWall].nNumVertex;	//今回のインデックス数を加算
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshWall->Unlock();

}

//==================================================================================
//メッシュウォールの終了処理
//==================================================================================
void UninitMeshWall(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
	}

}

//==================================================================================
//メッシュウォールの更新処理
//==================================================================================
void UpdateMeshWall(void)
{
	if (GetKeyboardPress(DIK_1) == true)
	{//←キーが押された,左移動

		for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
		{
			if (g_aMeshWall[nCntWall].bUse == true)
			{//使用していたら
				g_aMeshWall[nCntWall].rot.y += 0.05f;
			}
		}
	}
}

//==================================================================================
//メッシュウォールの描画処理
//==================================================================================
void DrawMeshWall(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	int nIdxPoint = 0;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//使用していたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshWall[nCntWall].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[nCntWall].rot.y, g_aMeshWall[nCntWall].rot.x, g_aMeshWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntWall].mtxWorld, &g_aMeshWall[nCntWall].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[nCntWall].pos.x, g_aMeshWall[nCntWall].pos.y, g_aMeshWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntWall].mtxWorld, &g_aMeshWall[nCntWall].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[nCntWall].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshWall);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshWall);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aMeshWall[nCntWall].nNumVertex,
				nIdxPoint,
				g_aMeshWall[nCntWall].nNumIndex - 2);
		}

		nIdxPoint += g_aMeshWall[nCntWall].nNumIndex;	//今回のインデックス数を加算
	}

}

//==================================================================================
//メッシュウォールの設定処理
//==================================================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Width, int Height)
{

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == false)
		{//使用していなかったら

			g_aMeshWall[nCntWall].pos = pos;	//位置
			g_aMeshWall[nCntWall].rot = rot;	//向き
			g_aMeshWall[nCntWall].bUse = true;	//使用している状況にする
			g_aMeshWall[nCntWall].nNumVertex = (Width + 1) * (Height + 1);	//頂点数
			g_aMeshWall[nCntWall].nNumIndex = (Height * ((Width + 1) * 2)) + (2 * (Height - 1));	//インデックス数
			g_aMeshWall[nCntWall].nWidth = Width;
			g_aMeshWall[nCntWall].nHeight = Height;
			break;
		}
	}

}

//==================================================================================
//メッシュウォールの情報取得
//==================================================================================
MESHWALL *GetMeshWall(void)
{
	return &g_aMeshWall[0];
}

//==================================================================================
//メッシュウォールの頂点情報取得
//==================================================================================
LPDIRECT3DVERTEXBUFFER9 GetBuffMeshWall(void)
{
	return g_pVtxBuffMeshWall;
}