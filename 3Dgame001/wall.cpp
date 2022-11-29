//=============================================================================
//
// 壁処理 [wall.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "wall.h"
#include "meshwall.h"
#include "input.h"

//マクロ定義
#define MOVE		(0.0f)
#define MAX_TOP		(4)

//グローバル変数宣言
const char *c_apFilenameWall[] =					//ファイル読み込み
{
	"data\\TEXTURE\\Wall2.png",
};
LPDIRECT3DTEXTURE9 g_apTextureWall[(sizeof c_apFilenameWall) / sizeof(*c_apFilenameWall)] = {};	//テクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//頂点バッファへのポインタ
WALL g_aWall[MAX_WALL];									//壁の情報

//==================================================================================
//壁の初期化処理
//==================================================================================
void InitWall(void)
{

	////デバイスの取得
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\TEXTURE\\wall000.jpg",
	//	&g_pTextureWall);

	////各要素初期化
	//for (int nCount = 0; nCount < MAX_WALL; nCount++)
	//{
	//	g_aWall[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	g_aWall[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	g_aWall[nCount].bUse = false;
	//}

	////頂点バッファの生成
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_WALL,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_3D,
	//	D3DPOOL_MANAGED,
	//	&g_pVtxBuffWall,
	//	NULL);

	//VERTEX_3D *pVtx;			//頂点情報へのポインタ

	////頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCount = 0; nCount < MAX_WALL; nCount++)
	//{
	//	//頂点座標の設定
	//	pVtx[0].pos = D3DXVECTOR3(-1000.0f, 1000.0f, 0.0f);
	//	pVtx[1].pos = D3DXVECTOR3(1000.0f, 1000.0f, 0.0f);
	//	pVtx[2].pos = D3DXVECTOR3(-1000.0f, 0.0f, 0.0f);
	//	pVtx[3].pos = D3DXVECTOR3(1000.0f, 0.0f, 0.0f);

	//	//法線ベクトルの設定
	//	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);


	//	//頂点カラーの設定
	//	pVtx[0].col =
	//	pVtx[1].col =
	//	pVtx[2].col =
	//	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//	//テクスチャ座標の設定
	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	pVtx += MAX_TOP;
	//}

	////頂点バッファをアンロックする
	//g_pVtxBuffWall->Unlock();

	//SetWall(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

}

//==================================================================================
//壁の終了処理
//==================================================================================
void UninitWall(void)
{
	////テクスチャの破棄
	//if (g_pTextureWall != NULL)
	//{
	//	g_pTextureWall->Release();
	//	g_pTextureWall = NULL;
	//}

	////頂点バッファの破棄
	//if (g_pVtxBuffWall != NULL)
	//{
	//	g_pVtxBuffWall->Release();
	//	g_pVtxBuffWall = NULL;
	//}
}

//==================================================================================
//壁の更新処理
//==================================================================================
void UpdateWall(void)
{
	//if (GetKeyboardPress(DIK_1) == true)
	//{//←キーが押された,左移動

	//	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	//	{
	//		if (g_aWall[nCntWall].bUse == true)
	//		{//使用していたら
	//			g_aWall[nCntWall].rot.y += 0.05f;
	//		}
	//	}
	//}
}

//==================================================================================
//壁の描画処理
//==================================================================================
void DrawWall(void)
{

	////デバイスの取得
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言

	//for (int nCount = 0; nCount < MAX_WALL; nCount++)
	//{
	//	if (g_aWall[nCount].bUse == true)
	//	{//使用していたら

	//		//ワールドマトリックスの初期化
	//		D3DXMatrixIdentity(&g_aWall[nCount].mtxWorld);

	//		//向きを反映する
	//		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCount].rot.y, g_aWall[nCount].rot.x, g_aWall[nCount].rot.z);
	//		D3DXMatrixMultiply(&g_aWall[nCount].mtxWorld, &g_aWall[nCount].mtxWorld, &mtxRot);

	//		//位置を反映する
	//		D3DXMatrixTranslation(&mtxTrans, g_aWall[nCount].pos.x, g_aWall[nCount].pos.y, g_aWall[nCount].pos.z);
	//		D3DXMatrixMultiply(&g_aWall[nCount].mtxWorld, &g_aWall[nCount].mtxWorld, &mtxTrans);

	//		//ワールドマトリックスの設定
	//		pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCount].mtxWorld);

	//		//頂点バッファをデータストリームに設定
	//		pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

	//		//頂点フォーマットの設定
	//		pDevice->SetFVF(FVF_VERTEX_3D);

	//		//テクスチャの設定
	//		pDevice->SetTexture(0, g_pTextureWall);

	//		//壁の描画
	//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);

	//	}

	//}

}

//==================================================================================
//壁の設定処理
//==================================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{

	//VERTEX_3D *pVtx;			//頂点情報へのポインタ

	////頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCount = 0; nCount < MAX_WALL; nCount++)
	//{
	//	if (g_aWall[nCount].bUse == false)
	//	{//使用していなかったら

	//		g_aWall[nCount].pos = pos;	//位置
	//		g_aWall[nCount].rot = rot;	//向き

	//		g_aWall[nCount].bUse = true;	//使用している状況にする

	//		break;
	//	}

	//	pVtx += MAX_TOP;
	//}

	////頂点バッファをアンロックする
	//g_pVtxBuffWall->Unlock();

}

//==================================================================================
//壁の情報取得
//==================================================================================
//WALL *GetWall(void)
//{
	//return &g_aWall[0];
//}