//=============================================================================
//
// ライト処理 [light.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _LIGHT_H_		//このマクロ定義がされていなかったら
#define _LIGHT_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define MAX_LIGHT	(3)	//ライトの最大数

//プロトタイプ宣言
void InitLight(void);
void UninitLight(void);
void UpdateLight(void);
D3DLIGHT9 *GetLight(void);
D3DXVECTOR3 *GetLightDir(void);

#endif