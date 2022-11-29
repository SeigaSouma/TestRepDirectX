//=============================================================================
//
// ビルボード処理 [billboard.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "billboard.h"
#include "input.h"

//マクロ定義
#define POS_BILLBOARD	(50.0f)
#define MOVE		(0.0f)
#define MAX_TOP		(4)

//グローバル変数宣言
const char *c_apFilenameBillboard[] =					//ファイル読み込み
{
	"data\\TEXTURE\\char_02.png",
};
LPDIRECT3DTEXTURE9 g_pTextureBillboard[(sizeof c_apFilenameBillboard) / sizeof(*c_apFilenameBillboard)] = {};	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	//頂点バッファへのポインタ
Billboard g_aBillboard[MAX_BILLBOARD];				//ビルボードの情報
bool g_bZFunc;			//Zテスト
bool g_bAlphaFunc;		//アルファテスト

//==================================================================================
//ビルボードの初期化処理
//==================================================================================
void InitBillboard(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameBillboard) / sizeof(*c_apFilenameBillboard); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBillboard[nCntTex],
			&g_pTextureBillboard[nCntTex]);

	}

	//各要素初期化
	for (int nCount = 0; nCount < MAX_BILLBOARD; nCount++)
	{
		g_aBillboard[nCount].pos = D3DXVECTOR3(0.0f, POS_BILLBOARD, 0.0f);
		g_aBillboard[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCount].bUse = false;
	}
	g_bZFunc = true;
	g_bAlphaFunc = true;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BILLBOARD; nCount++)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-POS_BILLBOARD, POS_BILLBOARD, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(POS_BILLBOARD, POS_BILLBOARD, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-POS_BILLBOARD, -POS_BILLBOARD, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(POS_BILLBOARD, -POS_BILLBOARD, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffBillboard->Unlock();

	//ビルボードの設定処理
	SetBillboard(D3DXVECTOR3(0.0f, POS_BILLBOARD, 0.0f));
	SetBillboard(D3DXVECTOR3(30.0f, POS_BILLBOARD, POS_BILLBOARD));
	SetBillboard(D3DXVECTOR3(970.0f, POS_BILLBOARD, POS_BILLBOARD));
}

//==================================================================================
//ビルボードの終了処理
//==================================================================================
void UninitBillboard(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameBillboard) / sizeof(*c_apFilenameBillboard); nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureBillboard[nCntTex] != NULL)
		{
			g_pTextureBillboard[nCntTex]->Release();
			g_pTextureBillboard[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//==================================================================================
//ビルボードの更新処理
//==================================================================================
void UpdateBillboard(void)
{
	if (GetKeyboardTrigger(DIK_F8) == true)
	{//F8が押された,Zテスト切り替え

		g_bZFunc = g_bZFunc ? false : true;
	}
	if (GetKeyboardTrigger(DIK_F9) == true)
	{//F9が押された,Zテスト切り替え

		g_bAlphaFunc = g_bAlphaFunc ? false : true;
	}

}

//==================================================================================
//ビルボードの描画処理
//==================================================================================
void DrawBillboard(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス宣言
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	if (g_bZFunc == false)
	{//判定OFFのとき

		//Zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	}

	if (g_bAlphaFunc == true)
	{//判定ONのとき

		//アルファテストを有効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	}

	for (int nCount = 0; nCount < MAX_BILLBOARD; nCount++)
	{

		if (g_aBillboard[nCount].bUse == true)
		{//使用されていたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBillboard[nCount].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBillboard[nCount].mtxWorld, NULL, &mtxView);	//逆配列を求める

#if 0

	//X軸に沿ってる
			g_aBillboard[nCount].mtxWorld._12 = 0.0f;
			g_aBillboard[nCount].mtxWorld._13 = 0.0f;
			g_aBillboard[nCount].mtxWorld._14 = 0.0f;

#else

	//上で消える
			g_aBillboard[nCount].mtxWorld._21 = 0.0f;
			g_aBillboard[nCount].mtxWorld._23 = 0.0f;
			g_aBillboard[nCount].mtxWorld._24 = 0.0f;

			//ライト
			/*g_aBillboard[nCount].mtxWorld._31 = 0.0f;
			g_aBillboard[nCount].mtxWorld._32 = 0.0f;
			g_aBillboard[nCount].mtxWorld._34 = 0.0f;*/

#endif

			g_aBillboard[nCount].mtxWorld._41 = 0.0f;
			g_aBillboard[nCount].mtxWorld._42 = 0.0f;
			g_aBillboard[nCount].mtxWorld._43 = 0.0f;

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCount].pos.x, g_aBillboard[nCount].pos.y, g_aBillboard[nCount].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCount].mtxWorld, &g_aBillboard[nCount].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCount].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBillboard[0]);

			//ビルボードの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);

		}

	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

}

//==================================================================================
//ビルボードの設定処理
//==================================================================================
void SetBillboard(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BILLBOARD; nCount++)
	{

		if (g_aBillboard[nCount].bUse == false)
		{//使用されていなかったら

			g_aBillboard[nCount].pos = pos;
			g_aBillboard[nCount].bUse = true;
			break;
		}

	}

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//==================================================================================
//Zテストの判定
//==================================================================================
bool GetZTest(void)
{
	return g_bZFunc;
}

//==================================================================================
//アルファテストの判定
//==================================================================================
bool GetAlphaTest(void)
{
	return g_bAlphaFunc;
}