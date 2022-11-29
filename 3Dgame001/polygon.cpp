//=============================================================================
//
// ポリゴン処理 [polygon.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "polygon.h"
#include "input.h"

//マクロ定義
#define POS_POLYGON	(1000.0f)
#define MOVE		(0.0f)
#define MAX_TOP		(4)

//グローバル変数宣言
const char *c_apFilenamePolygon[] =					//ファイル読み込み
{
	"data\\TEXTURE\\Polygon2.png",
};
//LPDIRECT3DTEXTURE9 g_apTexturePolygon[(sizeof c_apFilenamePolygon) / sizeof(*c_apFilenamePolygon)] = {};	//テクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	//頂点バッファへのポインタ
POLYGON g_aPolygon;									//ポリゴンの情報

//==================================================================================
//ポリゴンの初期化処理
//==================================================================================
void InitPolygon(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\field002.jpg",
		&g_pTexturePolygon);

	//各要素初期化
	g_aPolygon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPolygon.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-POS_POLYGON, 0.0f, POS_POLYGON);
	pVtx[1].pos = D3DXVECTOR3(POS_POLYGON, 0.0f, POS_POLYGON);
	pVtx[2].pos = D3DXVECTOR3(-POS_POLYGON, 0.0f, -POS_POLYGON);
	pVtx[3].pos = D3DXVECTOR3(POS_POLYGON, 0.0f, -POS_POLYGON);

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

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//==================================================================================
//ポリゴンの終了処理
//==================================================================================
void UninitPolygon(void)
{
	//テクスチャの破棄
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//==================================================================================
//ポリゴンの更新処理
//==================================================================================
void UpdatePolygon(void)
{

}

//==================================================================================
//ポリゴンの描画処理
//==================================================================================
void DrawPolygon(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aPolygon.mtxWorld);

	//向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPolygon.rot.y, g_aPolygon.rot.x, g_aPolygon.rot.z);
	D3DXMatrixMultiply(&g_aPolygon.mtxWorld, &g_aPolygon.mtxWorld, &mtxRot);

	//位置を反映する
	D3DXMatrixTranslation(&mtxTrans, g_aPolygon.pos.x, g_aPolygon.pos.y, g_aPolygon.pos.z);
	D3DXMatrixMultiply(&g_aPolygon.mtxWorld, &g_aPolygon.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aPolygon.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygon);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}