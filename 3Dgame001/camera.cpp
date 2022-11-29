//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 相馬靜雅
//
//=============================================================================
//インクルード
#include "main.h"
#include "camera.h"
#include "input.h"
#include "model.h"
#include "player.h"

//マクロ
#define MOVE			(5.0f)		//移動量
#define START_LENGTH	(560.0f)	//元の距離
#define MAX_LENGTH		(2000.0f)	//最大距離
#define MIN_LENGTH		(10.0f)		//最少距離
#define ROT_MOVE		(0.03f)		//回転移動量
#define MIN_ROT			(-D3DX_PI * 0.499f)	//カメラ固定用
#define MAX_ROT			(D3DX_PI * 0.499f)	//カメラ固定用
#define BACKFOLLOW_TIME	(60)		//背面補正までの時間
#define DIS_Y			(50.0f)

//プロトタイプ宣言
void MoveCameraInput(void);
void MoveCameraVR(void);
void MoveCameraV(void);
void MoveCameraR(void);
void MoveCameraDistance(void);
void SetCameraV(void);
void SetCameraR(void);
void SetCameraRot(void);
void MoveCameraFollow(void);

//グローバル変数宣言
Camera g_camera;	//カメラの情報

//==================================================================================
//カメラの初期化処理
//==================================================================================
void InitCamera(void)
{

	g_camera.posV = D3DXVECTOR3(0.0f, 200.0f, -400.0f);	//視点
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//注視点
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//上方向ベクトル
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, -0.3f);		//向き
	g_camera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	g_camera.fDistance = START_LENGTH;					//距離
	g_camera.bFollow = false;							//追従
	g_camera.bBackFollow = false;						//背面追従
	g_camera.nCntBackFollow = 0;						//追従カウント

	g_camera.posVDest = g_camera.posV;	//目標の視点
	g_camera.posRDest = g_camera.posR;	//目標の注視点

	g_camera.rotVDest = g_camera.rot;	//目標の向き
	g_camera.rotVDiff = 0.0f;			//目標の視点の差分

}

//==================================================================================
//カメラの終了処理
//==================================================================================
void UninitCamera(void)
{

}

//==================================================================================
//カメラの更新処理
//==================================================================================
void UpdateCamera(void)
{

	//視点・注視点移動
	MoveCameraFollow();
	MoveCameraInput();
	MoveCameraVR();
	MoveCameraR();
	MoveCameraV();
	MoveCameraDistance();

	if (GetKeyboardTrigger(DIK_F6) == true)
	{//F6が押された,背面追従切り替え

		g_camera.bBackFollow = g_camera.bBackFollow ? false : true;
	}
	if (GetKeyboardTrigger(DIK_F7) == true)
	{//F7が押された,追従切り替え

		g_camera.bFollow = g_camera.bFollow ? false : true;
	}

}

//==================================================================================
//入力機器のカメラ移動
//==================================================================================
void MoveCameraInput(void)
{

	if (GetMousePress(MOUSE_BUTTON_LEFT) == true &&
		GetMousePress(MOUSE_BUTTON_RIGHT) == true)
	{//左右同時押し

		g_camera.move.x += (GetMouseMove().x * sinf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE) -
							(GetMouseMove().y * cosf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE);

		g_camera.move.z += (GetMouseMove().x * cosf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE) +
							(GetMouseMove().y * sinf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE);

		//移動量補正
		MoveCameraVR();

		//回転の補正
		SetCameraRot();

		//注視点設定
		SetCameraR();
	}
	else if (GetMousePress(MOUSE_BUTTON_LEFT) == true)
	{//左クリックしてるとき,視点回転

		g_camera.rot.y += GetMouseMove().x * ROT_MOVE;
		g_camera.rot.z += GetMouseMove().y * ROT_MOVE;

		//回転の補正
		SetCameraRot();

		//視点の代入処理
		SetCameraV();

	}
	else if (GetMousePress(MOUSE_BUTTON_RIGHT) == true)
	{//右クリックしてるとき,注視点回転

		g_camera.rot.y += GetMouseMove().x * ROT_MOVE;
		g_camera.rot.z += GetMouseMove().y * ROT_MOVE;

		//回転の補正
		SetCameraRot();

		//注視点の位置更新
		SetCameraR();
	}

	//マウスホイールで距離調整
	g_camera.fDistance += GetMouseMove().z * (MOVE * 0.1f);
	MoveCameraDistance();

	//視点の代入処理
	SetCameraV();

}

//==================================================================================
//カメラの視点移動
//==================================================================================
void MoveCameraV(void)
{

	//視点移動
	if (GetKeyboardPress(DIK_Y) == true)
	{//Yキーが押された,視点上移動

		g_camera.rot.z -= ROT_MOVE;
	}
	else if (GetKeyboardPress(DIK_N) == true)
	{//Nキーが押された,視点下移動

		g_camera.rot.z += ROT_MOVE;
	}

	if (GetKeyboardPress(DIK_Z) == true)
	{//Zキーが押された

		g_camera.rot.y += ROT_MOVE;

	}
	if (GetKeyboardPress(DIK_C) == true)
	{//Cキーが押された

		g_camera.rot.y -= ROT_MOVE;

	}

	//回転の補正
	SetCameraRot();

	//視点の代入処理
	SetCameraV();

}

//==================================================================================
//カメラの注視点移動
//==================================================================================
void MoveCameraR(void)
{
	//旋回
	if (GetKeyboardPress(DIK_Q) == true)
	{//Qキーが押された,左旋回

		
		g_camera.rot.y -= ROT_MOVE;

	}
	else if (GetKeyboardPress(DIK_E) == true)
	{//Eキーが押された,左旋回

		g_camera.rot.y += ROT_MOVE;
	}

	//上昇下降
	if (GetKeyboardPress(DIK_T) == true)
	{//Tキーが押された

		g_camera.rot.z += ROT_MOVE;

	}
	else if (GetKeyboardPress(DIK_B) == true)
	{//Bキーが押された

		g_camera.rot.z -= ROT_MOVE;

	}

	//回転の補正
	SetCameraRot();

	//注視点の位置更新
	SetCameraR();
}

//==================================================================================
//カメラの視点・注視点移動
//==================================================================================
void MoveCameraVR(void)
{

	if (g_camera.bFollow == false)
	{//追従がOFFの時だけ

		if (GetKeyboardPress(DIK_A) == true || XGetStickPressR(BUTTON_LX, 0) < 0)
		{//Aキーが押された,視点左移動

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
			{//A+W,左上移動

				g_camera.move.x += sinf(-D3DX_PI * MOVE_LRDW + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(-D3DX_PI * MOVE_LRDW + g_camera.rot.y) * MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
			{//A+S,左下移動

				g_camera.move.x += sinf(-D3DX_PI * MOVE_LRUP + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(-D3DX_PI * MOVE_LRUP + g_camera.rot.y) * MOVE;
			}
			else
			{//A,左移動

				g_camera.move.x += sinf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE;
			}

		}
		else if (GetKeyboardPress(DIK_D) == true || XGetStickPressR(BUTTON_LX, 0) > 0)
		{//Dキーが押された,視点右移動

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
			{//D+W,右上移動

				g_camera.move.x += sinf(D3DX_PI * MOVE_LRDW + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(D3DX_PI * MOVE_LRDW + g_camera.rot.y) * MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
			{//D+S,右下移動

				g_camera.move.x += sinf(D3DX_PI * MOVE_LRUP + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(D3DX_PI * MOVE_LRUP + g_camera.rot.y) * MOVE;
			}
			else
			{//D,右移動

				g_camera.move.x += sinf(D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE;
			}

		}
		else if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
		{//Wが押された、奥移動

			g_camera.move.x += sinf(g_camera.rot.y) * MOVE;
			g_camera.move.z += cosf(g_camera.rot.y) * MOVE;
		}
		else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
		{//Sが押された、手前移動

			g_camera.move.x += sinf(D3DX_PI + g_camera.rot.y) * MOVE;
			g_camera.move.z += cosf(D3DX_PI + g_camera.rot.y) * MOVE;
		}

	}

	//移動量分を加算
	g_camera.posV.x += g_camera.move.x;
	g_camera.posV.z += g_camera.move.z;

	//移動量をリセット
	g_camera.move.x = 0.0f;
	g_camera.move.z = 0.0f;

	//注視点の位置更新
	SetCameraR();
}

//==================================================================================
// 視点・注視点間距離
//==================================================================================
void MoveCameraDistance(void)
{
	if (GetKeyboardPress(DIK_U) == true)
	{//Uキーが押された,近づく

		g_camera.fDistance -= MOVE;
	}
	else if (GetKeyboardPress(DIK_M) == true)
	{//Mキーが押された,近づく

		g_camera.fDistance += MOVE;
	}

	//限界値補正
	if (g_camera.fDistance >= MAX_LENGTH)
	{
		g_camera.fDistance = MAX_LENGTH;
	}
	else if (g_camera.fDistance <= MIN_LENGTH)
	{
		g_camera.fDistance = MIN_LENGTH;
	}

}

//==================================================================================
//カメラの視点代入処理
//==================================================================================
void SetCameraV(void)
{

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	if (g_camera.bFollow == false)
	{//追従しないとき

		//視点の代入処理
		g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.z) * sinf(g_camera.rot.y) * -g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.z) * cosf(g_camera.rot.y) * -g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y + sinf(g_camera.rot.z) * -g_camera.fDistance;
	}
	else
	{//追従ON

		float fYcamera = 0.0f;

		if (pPlayer->pos.y >= 90.0f)
		{
			fYcamera = pPlayer->pos.y - 90.0f;
		}

		//視点の代入処理
		g_camera.posVDest.x = (pPlayer->pos.x + sinf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_camera.rot.z) * sinf(g_camera.rot.y) * -g_camera.fDistance;
		g_camera.posVDest.z = (pPlayer->pos.z + cosf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_camera.rot.z) * cosf(g_camera.rot.y) * -g_camera.fDistance;
		g_camera.posVDest.y = fYcamera + DIS_Y + sinf(g_camera.rot.z) * -g_camera.fDistance;

		//補正する
		g_camera.posV += (g_camera.posVDest - g_camera.posV) * 0.12f;
	}

}

//==================================================================================
//カメラの注視点代入処理
//==================================================================================
void SetCameraR(void)
{

	if (g_camera.bFollow == false)
	{//追従しないとき

		//注視点の代入処理
		g_camera.posR.x = g_camera.posV.x + cosf(g_camera.rot.z) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.z) * cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.y = g_camera.posV.y + sinf(g_camera.rot.z) * g_camera.fDistance;
	}
	else
	{//追従ON

		//注視点の代入処理
		g_camera.posRDest.x = g_camera.posV.x + cosf(g_camera.rot.z) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posRDest.z = g_camera.posV.z + cosf(g_camera.rot.z) * cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posRDest.y = g_camera.posV.y + sinf(g_camera.rot.z) * g_camera.fDistance;

		//補正する
		g_camera.posR += (g_camera.posRDest - g_camera.posR) * 0.08f;

	}

}

//==================================================================================
//カメラの回転補正
//==================================================================================
void SetCameraRot(void)
{
	//Y軸の補正
	if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y = -D3DX_PI;
	}
	else if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y = D3DX_PI;
	}

	//Z軸の固定
	if (g_camera.rot.z > MAX_ROT)
	{
		g_camera.rot.z = MAX_ROT;
	}
	else if (g_camera.rot.z < MIN_ROT)
	{
		g_camera.rot.z = MIN_ROT;
	}

}

//==================================================================================
//カメラの背面自動追従
//==================================================================================
void MoveCameraFollow(void)
{

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();
	g_camera.rotVDest.y = (D3DX_PI + pPlayer->rot.y);

	if (g_camera.bBackFollow == true)
	{//追従するとき

		if (pPlayer->move.x > 1.0f || pPlayer->move.x < -1.0f || pPlayer->move.z > 1.0f || pPlayer->move.z < -1.0f)
		{//移動してるとき

			g_camera.nCntBackFollow = 0;	//カウントリセット
		}
		else
		{
			g_camera.nCntBackFollow++;	//カウント加算

		}

		if (g_camera.nCntBackFollow > BACKFOLLOW_TIME)
		{//背面補正のカウントが規定値超えたら

			//補正する
			g_camera.rotVDiff = (g_camera.rotVDest.y - g_camera.rot.y);

			if (g_camera.rotVDiff > D3DX_PI)
			{
				g_camera.rotVDiff += (-D3DX_PI * 2.0f);
			}
			else if (g_camera.rotVDiff < -D3DX_PI)
			{
				g_camera.rotVDiff += (D3DX_PI * 2.0f);
			}

			//差分で補正する
			g_camera.rot.y += g_camera.rotVDiff * 0.025f;

			if (g_camera.rot.y > D3DX_PI)
			{
				g_camera.rot.y += (-D3DX_PI * 2.0f);
			}
			else if (g_camera.rot.y < -D3DX_PI)
			{
				g_camera.rot.y += (D3DX_PI * 2.0f);
			}

			//回転の補正
			SetCameraRot();

			//注視点の位置更新
			SetCameraR();

			////視点の代入処理
			SetCameraV();

			//視点の代入処理
			g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.z) * sinf(g_camera.rot.y) * -g_camera.fDistance;
			g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.z) * cosf(g_camera.rot.y) * -g_camera.fDistance;
			g_camera.posV.y = g_camera.posR.y + sinf(g_camera.rot.z) * -g_camera.fDistance;

		}

	}

}


//==================================================================================
//カメラの設定処理
//==================================================================================
void SetCamera(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								D3DXToRadian(45.0f),	//視野角
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//アスペクト比
								10.0f,		//奥行きの制限
								10000.0f);	//奥行きの制限

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
						&g_camera.posV,	//視点
						&g_camera.posR,	//注視点
						&g_camera.vecU);//上方向ベクトル

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

}

//==================================================================================
//カメラの情報取得
//==================================================================================
Camera GetCamera(void)
{
	return g_camera;
}