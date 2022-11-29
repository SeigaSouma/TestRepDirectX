//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "effect.h"
#include "input.h"
#include "particle.h"
#include "camera.h"

//マクロ定義
#define MAX_TOP			(4)		//頂点数
#define MAX_EFFECT		(12800)	//エフェクトの最大数
#define EFFECTSIZE1		(0.85f)
#define EFFECTSIZE2		(0.95f)
#define EFFECTSIZE3		(0.9f)


//プロトタイプ宣言
void UpdateSubEffect(int nCntEffect);
void UpdateAddEffect(int nCntEffect);

//グローバル変数宣言
const char *c_apFilenameEffect[] =		//ファイル読み込み
{
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect002.jpg",
	"data\\TEXTURE\\effect002.tga",
	"data\\TEXTURE\\effect000.png",
};

LPDIRECT3DTEXTURE9 g_pTextureEffect[(sizeof c_apFilenameEffect) / sizeof(*c_apFilenameEffect)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];						//エフェクトの情報
int g_EffectNum;									//エフェクトの数
bool g_bEffectUse;

//==================================================================================
//エフェクトの初期化処理
//==================================================================================
void InitEffect(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameEffect) / sizeof(*c_apFilenameEffect); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameEffect[nCntTex],
			&g_pTextureEffect[nCntTex]);

	}


	//エフェクトの情報の初期化
	for (int nCntEffect =0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].bUse = false;	//使用していない状態にする
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].nLife = EFFECT_LIFE;
		g_aEffect[nCntEffect].bAddAlpha = false;
	}
	g_EffectNum = 0;
	g_bEffectUse = true;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_EFFECT,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffEffect,
								NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//==================================================================================
//エフェクトの終了処理
//==================================================================================
void UninitEffect(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameEffect) / sizeof(*c_apFilenameEffect); nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureEffect[nCount] != NULL)
		{
			g_pTextureEffect[nCount]->Release();
			g_pTextureEffect[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//==================================================================================
//エフェクトの更新処理
//==================================================================================
void UpdateEffect(void)
{

	//カメラの情報取得
	Camera aCamera = GetCamera();

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_F5) == true)
	{//F5が押された,エフェクト使用切り替え

		g_bEffectUse = g_bEffectUse ? false : true;
	}

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されていたら

			//頂点座標の更新(小さくする)
			if (g_aEffect[nCntEffect].nType == EFFECTTYPE_SMOKEBLACK)
			{//黒煙だった場合

				if (((float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nMaxLife) <= 0.5f)
				{//最大体力の半分以下

					g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move * 0.001f;
				}
				else
				{//半分より上

					g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move * (((float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nMaxLife));
				}
			}
			else 
			{//黒煙以外

				g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
			}
			

			//小さくしていく処理の場合
			if (g_aEffect[nCntEffect].moveType == MOVEEFFECT_SUB)
			{//エフェクトを小さくしていく

				//縮小処理
				UpdateSubEffect(nCntEffect);
			}
			else
			{//エフェクトを大きくしていく

				//拡大処理
				UpdateAddEffect(nCntEffect);
			}


			//寿命の更新
			g_aEffect[nCntEffect].nLife--;

			//不透明度の更新
			g_aEffect[nCntEffect].col.a = (float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nMaxLife;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{//寿命が尽きたら

				g_aEffect[nCntEffect].bUse = false;
				g_aEffect[nCntEffect].bAddAlpha = false;
				g_EffectNum--;
			}

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//頂点座標の更新(小さくする)
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

		}

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//==================================================================================
//エフェクトの縮小処理
//==================================================================================
void UpdateSubEffect(int nCntEffect)
{

	int nEffectType = rand() % 3;

	if (nEffectType == 0)
	{
		g_aEffect[nCntEffect].fRadius *= EFFECTSIZE1;
	}
	else if (nEffectType == 1)
	{
		g_aEffect[nCntEffect].fRadius *= EFFECTSIZE2;
	}
	else if (nEffectType == 2)
	{
		g_aEffect[nCntEffect].fRadius *= EFFECTSIZE3;
	}

}

//==================================================================================
//エフェクトの拡大処理
//==================================================================================
void UpdateAddEffect(int nCntEffect)
{

	int nEffectType = rand() % 3;

	if (nEffectType == 0)
	{
		g_aEffect[nCntEffect].fRadius += 1.8f;
	}
	else if (nEffectType == 1)
	{
		g_aEffect[nCntEffect].fRadius += 0.8f;
	}
	else if (nEffectType == 2)
	{
		g_aEffect[nCntEffect].fRadius += 0.0f;
	}

}

//==================================================================================
//エフェクトの描画処理
//==================================================================================
void DrawEffect(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス宣言
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//ポリゴンの描画
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true && g_bEffectUse == true)
		{//エフェクトが使用されている

			//αブレンディングを加算合成に設定
			if (g_aEffect[nCntEffect].bAddAlpha == false)
			{
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxView);	//逆配列を求める
			g_aEffect[nCntEffect].mtxWorld._41 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._42 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._43 = 0.0f;

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].nType]);

			//ビルボードの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * MAX_TOP, 2);

			//αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		}

	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==================================================================================
//エフェクトの設定処理
//==================================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType)
{

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//エフェクトが使用されていなかったら

			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nMaxLife = nLife;
			g_aEffect[nCntEffect].moveType = moveType;
			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].bUse = true;		//使用している状態に変更
			g_EffectNum++;

			if (g_aEffect[nCntEffect].nType == EFFECTTYPE_NORMAL)
			{//通常の時

				g_aEffect[nCntEffect].bAddAlpha = false;
			}
			else if (g_aEffect[nCntEffect].nType == EFFECTTYPE_SMOKE)
			{//煙のエフェクト

				g_aEffect[nCntEffect].bAddAlpha = false;
			}
			else if (g_aEffect[nCntEffect].nType == EFFECTTYPE_SMOKEBLACK)
			{//黒煙のエフェクト

				g_aEffect[nCntEffect].bAddAlpha = true;
			}
			else if (g_aEffect[nCntEffect].nType == EFFECTTYPE_BLACK)
			{//黒のエフェクト

				g_aEffect[nCntEffect].bAddAlpha = true;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			break;
		}

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//==================================================================================
//エフェクトの数取得
//==================================================================================
int GetEffectNum(void)
{
	return g_EffectNum;
}

//==================================================================================
//エフェクトを使用するか
//==================================================================================
bool GetEffectUse(void)
{
	return g_bEffectUse;
}