//=============================================================================
//
// バレット処理 [explosion.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "explosion.h"
#include "input.h"
#include "particle.h"

//マクロ定義
#define MAX_TOP		(4)			//頂点数
#define MOVE		(10.0f)		//爆発の移動量
#define MOVE_U		(0.125f)	//U座標移動量
#define MOVE_V		(1.0f)		//V座標移動量
#define PAUSE_TIME	(8)			//アニメーション速度
#define MAX_CUT		(8)			//U分割数
#define MOVE_SIZE	(0.3f)		//サイズ移動量

//グローバル変数宣言
const char *c_apFilenameExplosion[] =		//ファイル読み込み
{
	"data\\TEXTURE\\explosion000.png",
};
LPDIRECT3DTEXTURE9 g_pTextureExplosion[(sizeof c_apFilenameExplosion) / sizeof(*c_apFilenameExplosion)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	//頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];						//爆発の情報

//==================================================================================
//爆発の初期化処理
//==================================================================================
void InitExplosion(void)
{
	
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameExplosion) / sizeof(*c_apFilenameExplosion); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameExplosion[nCntTex],
			&g_pTextureExplosion[nCntTex]);

	}

	//爆発の情報の初期化
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].nLife = PAUSE_TIME * MAX_CUT;
		g_aExplosion[nCntExplosion].nLife = g_aExplosion[nCntExplosion].nLife;
		g_aExplosion[nCntExplosion].nCntAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;	//使用していない状態にする

	}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_EXPLOSION,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffExplosion,
								NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-POS_EXPLOSION, +POS_EXPLOSION, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+POS_EXPLOSION, +POS_EXPLOSION, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-POS_EXPLOSION, -POS_EXPLOSION, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+POS_EXPLOSION, -POS_EXPLOSION, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = g_aExplosion[nCntExplosion].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(MOVE_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(MOVE_U, 1.0f);

		pVtx += MAX_TOP;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//==================================================================================
//爆発の終了処理
//==================================================================================
void UninitExplosion(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameExplosion) / sizeof(*c_apFilenameExplosion); nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureExplosion[nCount] != NULL)
		{
			g_pTextureExplosion[nCount]->Release();
			g_pTextureExplosion[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//==================================================================================
//爆発の更新処理
//==================================================================================
void UpdateExplosion(void)
{

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されていたら

			g_aExplosion[nCntExplosion].nCntAnim++;	//カウンターを加算

			if ((g_aExplosion[nCntExplosion].nCntAnim % PAUSE_TIME) == 0)
			{//規定カウント進んだら

				//パターンNo.を更新
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % MAX_CUT;
				g_aExplosion[nCntExplosion].nCntAnim = 0;
			}

			if (g_aExplosion[nCntExplosion].nPatternAnim >= MAX_CUT - 1)
			{//総パターン数を超えたら

				g_aExplosion[nCntExplosion].bUse = false;
			}

			//寿命を減らす
			g_aExplosion[nCntExplosion].nLife--;

			//不透明度を寿命で減らす
			g_aExplosion[nCntExplosion].col.a = (float)g_aExplosion[nCntExplosion].nLife / (float)g_aExplosion[nCntExplosion].nMaxLife;

			//頂点座標の更新
			pVtx[0].pos.x -= MOVE_SIZE;
			pVtx[0].pos.y += MOVE_SIZE;

			pVtx[1].pos.x += MOVE_SIZE;
			pVtx[1].pos.y += MOVE_SIZE;

			pVtx[2].pos.x -= MOVE_SIZE;
			pVtx[2].pos.y -= MOVE_SIZE;

			pVtx[3].pos.x += MOVE_SIZE;
			pVtx[3].pos.y -= MOVE_SIZE;

			//頂点カラーの設定
			pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = g_aExplosion[nCntExplosion].col;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % MAX_CUT) * MOVE_U, (g_aExplosion[nCntExplosion].nPatternAnim / MAX_CUT) * MOVE_V);
			pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % MAX_CUT + 1) * MOVE_U, (g_aExplosion[nCntExplosion].nPatternAnim / MAX_CUT) * MOVE_V);
			pVtx[2].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % MAX_CUT) * MOVE_U, (g_aExplosion[nCntExplosion].nPatternAnim / MAX_CUT) * MOVE_V + MOVE_V);
			pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % MAX_CUT + 1) * MOVE_U, (g_aExplosion[nCntExplosion].nPatternAnim / MAX_CUT) * MOVE_V + MOVE_V);
			
		}

		pVtx += MAX_TOP;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();

}

//==================================================================================
//爆発の描画処理
//==================================================================================
void DrawExplosion(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス宣言
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する
	
	 //アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCount = 0; nCount < MAX_EXPLOSION; nCount++)
	{

		if (g_aExplosion[nCount].bUse == true)
		{//使用されていたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aExplosion[nCount].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aExplosion[nCount].mtxWorld, NULL, &mtxView);	//逆配列を求める
			g_aExplosion[nCount].mtxWorld._41 = 0.0f;
			g_aExplosion[nCount].mtxWorld._42 = 0.0f;
			g_aExplosion[nCount].mtxWorld._43 = 0.0f;

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCount].pos.x, g_aExplosion[nCount].pos.y, g_aExplosion[nCount].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCount].mtxWorld, &g_aExplosion[nCount].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCount].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExplosion[0]);

			//ビルボードの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);

		}

	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==================================================================================
//爆発の設定処理
//==================================================================================
void SetExplosion(D3DXVECTOR3 pos)
{


	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{

		if (g_aExplosion[nCntExplosion].bUse == false)
		{//爆発が使用されていなかったら

			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].nLife = PAUSE_TIME * MAX_CUT;
			g_aExplosion[nCntExplosion].nMaxLife = g_aExplosion[nCntExplosion].nLife;
			g_aExplosion[nCntExplosion].nCntAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;		//使用している状態に変更
			
			//パーティクルの設定処理
			SetParticle(g_aExplosion[nCntExplosion].pos, PARTICLE_TYPE_SMOKE);
			SetParticle(g_aExplosion[nCntExplosion].pos, PARTICLE_TYPE_SMOKE_RED);
			SetParticle(g_aExplosion[nCntExplosion].pos, PARTICLE_TYPE_SMOKE_YEL);

			
			SetParticle(g_aExplosion[nCntExplosion].pos, PARTICLE_TYPE_OFFSETTING);


			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-POS_EXPLOSION, +POS_EXPLOSION, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+POS_EXPLOSION, +POS_EXPLOSION, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-POS_EXPLOSION, -POS_EXPLOSION, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+POS_EXPLOSION, -POS_EXPLOSION, 0.0f);
			break;
		}

		pVtx += MAX_TOP;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();

}

//==================================================================================
//爆発の情報取得
//==================================================================================
Explosion *GetExplosion(void)
{
	return &g_aExplosion[0];
}
