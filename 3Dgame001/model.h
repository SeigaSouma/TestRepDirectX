//=============================================================================
//
// モデル処理 [model.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MODEL_H_		//このマクロ定義がされていなかったら
#define _MODEL_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define MAX_MODEL	(128)	//モデルの最大数

typedef struct
{
	D3DXVECTOR3 pos;		//現在の位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 move;		//移動量
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nIdxShadow;			//影のインデックス番号

	int nNumVtx;			//頂点数
	DWORD dwSizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;			//頂点バッファのポインタ
	D3DXVECTOR3 vtxMin;		//モデルの最小値
	D3DXVECTOR3	vtxMax;		//モデルの最大値
}Model;

//プロトタイプ宣言
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model GetModel(void);

#endif