//=============================================================================
//
// ライト処理 [light.cpp]
// Author : 相馬靜雅
//
//=============================================================================
//インクルード
#include "main.h"
#include "light.h"
#include "input.h"

//グローバル変数宣言
D3DLIGHT9 g_light[MAX_LIGHT];	//ライトの情報
D3DXVECTOR3 g_vecDir[MAX_LIGHT];//設定用方向ベクトル

//==================================================================================
//ライトの初期化処理
//==================================================================================
void InitLight(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR3 vecDir;	//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{

		//ライトの情報をクリアする
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntLight)
		{
		case 0:
			//ライトの拡散光を設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//ライトの方向を設定
			g_vecDir[nCntLight] = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			break;

		case 1:
			//ライトの拡散光を設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			//ライトの方向を設定
			g_vecDir[nCntLight] = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			break;

		case 2:
			//ライトの拡散光を設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			//ライトの方向を設定
			g_vecDir[nCntLight] = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			break;
		}

		D3DXVec3Normalize(&g_vecDir[nCntLight], &g_vecDir[nCntLight]);	//ベクトルを正規化する(1.0にする)
		g_light[nCntLight].Direction = g_vecDir[nCntLight];

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);

	}


}

//==================================================================================
//ライトの終了処理
//==================================================================================
void UninitLight(void)
{

}

//==================================================================================
//ライトの更新処理
//==================================================================================
void UpdateLight(void)
{

}

//==================================================================================
//ライトの情報取得
//==================================================================================
D3DLIGHT9 *GetLight(void)
{
	return &g_light[0];
}

//==================================================================================
// ライトの拡散光
//==================================================================================
D3DXVECTOR3 *GetLightDir(void)
{
	return &g_vecDir[0];
}