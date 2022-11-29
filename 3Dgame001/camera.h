//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _CAMERA_H_		//このマクロ定義がされていなかったら
#define _CAMERA_H_		//二重インクルード防止のマクロを定義する

//カメラの構造体
typedef struct
{
	D3DXVECTOR3 posV;		//視点(カメラの位置)
	D3DXVECTOR3 posR;		//注視点(見たい場所)
	D3DXVECTOR3 posVDest;	//目標の視点
	D3DXVECTOR3 posRDest;	//目標の注視点
	D3DXVECTOR3 vecU;		//上方向ベクトル
	D3DXMATRIX	mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX	mtxView;	//ビューマトリックス
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotVDest;	//目標の視点の向き
	float rotVDiff;			//目標の視点の差分
	float fDistance;		//距離
	int nCntBackFollow;		//背面補正するまでの時間
	bool bBackFollow;		//背面追従するかどうか
	bool bFollow;			//追従するかどうか
}Camera;

//プロトタイプ宣言
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera GetCamera(void);
void SetBackFollowCamera(void);

#endif