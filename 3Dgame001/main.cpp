//==================================================================================
//
//3D制作[main.cpp]
//Author:相馬靜雅
//
//==================================================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "shadow.h"
#include "enemy.h"
#include "wall.h"
#include "billboard.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "meshdome.h"

//マクロ定義
#define CLASS_NAME			"WindowsClass"		//ウインドウクラスの名前
#define WINDOW_NAME			"無題"	//ウインドウの名前(キャプション)

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);
void DrawFPS(void);
void DrawControll(void);
void DrawPosCamera(void);
void DrawPosPlayer(void);
void DrawPosLight(void);
void DrawBillboardTest(void);

//グローバル変数宣言
LPDIRECT3D9 g_pD3D = NULL;					//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		//Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;					//フォントへのポインタ
int g_nCountFPS = 0;						//FPSカウンタ
MODE g_mode = MODE_TITLE;					//現在のモード

//==================================================================================
//メイン関数
//==================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmbLine, int nCmbShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSのメモリサイズ
		CS_CLASSDC,							//ウインドウのスタイル
		WindowProc,							//ウインドウプロシージャ
		0,									//0にする(通常は使用しない)
		0,									//0にする(通常は使用しない)
		hInstance,							//インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),	//タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),		//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		NULL,								//メニューバー
		CLASS_NAME,							//ウインドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)		//ファイルのアイコン
	};

	HWND hWnd;								//ウインドウハンドル(識別子)
	MSG msg;								//メッセージを格納する変数

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	//画面サイズの構造体

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウの生成
	hWnd = CreateWindowEx(
		0,									//拡張ウインドウスタイル
		CLASS_NAME,							//ウインドウクラスの名前
		WINDOW_NAME,						//ウインドウの名前
		WS_OVERLAPPEDWINDOW,				//ウインドウスタイル
		CW_USEDEFAULT,						//ウインドウの左上X座標
		CW_USEDEFAULT,						//ウインドウの左上Y座標
		(rect.right - rect.left),			//ウインドウの幅
		(rect.bottom - rect.top),			//ウインドウの高さ
		NULL,								//親ウインドウのハンドル
		NULL,								//メニューハンドルまたは子ウインドウID
		hInstance,							//インスタンスハンドル
		NULL								//ウインドウ作成データ
	);

	DWORD dwCurrentTime;					//現在時刻
	DWORD dwExecLastTime;					//最後に処理した時刻
	DWORD dwFrameCount;						//フレームカウント
	DWORD dwFPSLastTime;					//最後にFPSを計測した時刻

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合

		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;					//初期化する
	dwExecLastTime = timeGetTime();		//現在時刻を取得(保存)

	//ウインドウの表示
	ShowWindow(hWnd, nCmbShow);				//ウインドウの表示状態を設定
	UpdateWindow(hWnd);						//クライアント領域を更新

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//メッセージループ
	while(1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windowの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);				//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);				//ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理

			dwCurrentTime = timeGetTime();	//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;		//FPSを測定した時刻を保存
				dwFrameCount = 0;					//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;		//処理開始の時刻[現在時刻]を保存

				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;						//フレームカウントを加算
			}
		}
	}

	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
		
	return (int)msg.wParam;
}

//==================================================================================
//ウインドウプロシージャ
//==================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;	//返り値を格納

	switch (uMsg)
	{
	case WM_DESTROY:				//ウインドウ破棄のメッセージ
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:				//ESCキー入力

			////メッセージボックスを表示
			//nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			//if (nID == IDYES)
			//{
				//ウインドウを破棄する(VK_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			//}
			break;
		}
		break;

	case WM_CLOSE:				//バツ閉じ
		//メッセージボックスを表示
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			//ウインドウを破棄する(VK_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;			//0を返して終了を防ぐ
		}
		break;

	case WM_LBUTTONDOWN:		//マウス左クリック
		//対象ウインドウにフォーカスを合わせる
		SetFocus(hWnd);
		break;

	case WM_TIMER:				//タイマー発行のメッセージ
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//既定の処理を返す
}
//==================================================================================
//初期化処理
//==================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションモード

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{//失敗したとき
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D -> GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{//失敗したとき
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));		//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;		//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;		//バックバッファの形式
	d3dpp.BackBufferCount = 1;					//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;		//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//デバイスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;					//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D ->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//埋めるモード

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステート
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバッグ表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//入力機器の初期化処理
	if (FAILED (InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//エフェクトの初期化処理
	InitEffect();

	//パーティクルの初期化処理
	InitParticle();

	//影の初期化処理
	InitShadow();

	//ポリゴンの初期化処理
	InitPolygon();

	//壁の初期化処理
	InitWall();

	//カメラの初期化処理
	InitCamera();

	//ライトの初期化処理
	InitLight();

	//モデルの初期化処理
	InitModel();

	//敵の初期化処理
	InitEnemy();

	//プレイヤーの初期化処理
	InitPlayer();

	//ビルボードの初期化処理
	InitBillboard();

	//弾の初期化処理
	InitBullet();

	//爆発の初期化処理
	InitExplosion();

	//デバッグ表示の初期化処理
	InitDebugProc();

	//メッシュフィールドの初期化処理
	InitMeshField();

	//メッシュウォールの初期化処理
	InitMeshWall();

	//メッシュシリンダーの初期化処理
	InitMeshCylinder();

	//メッシュウォールの初期化処理
	InitMeshDome();

	//乱数の種を設定
	srand((unsigned int)time(0));

	return S_OK;
}
//==================================================================================
//終了処理
//==================================================================================
void Uninit(void)
{

	//エフェクトの終了処理
	UninitEffect();

	//パーティクルの終了処理
	UninitParticle();

	//入力機器の終了処理
	UninitInput();

	//ポリゴンの終了処理
	UninitPolygon();

	//壁の終了処理
	UninitWall();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//モデルの終了処理
	UninitModel();

	//影の終了処理
	UninitShadow();

	//敵の終了処理
	UninitEnemy();

	//プレイヤーの終了処理
	UninitPlayer();

	//ビルボードの終了処理
	UninitBillboard();

	//弾の終了処理
	UninitBullet();

	//爆発の終了処理
	UninitExplosion();

	//デバッグ表示の終了処理
	UninitDebugProc();

	//メッシュフィールドの終了処理
	UninitMeshField();

	//メッシュウォールの終了処理
	UninitMeshWall();

	//メッシュシリンダーの終了処理
	UninitMeshCylinder();

	//メッシュウォールの終了処理
	UninitMeshDome();


	//デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}
//==================================================================================
//更新処理
//==================================================================================
void Update(void)
{


	if (GetKeyboardPress(DIK_F2) == true)
	{//F9が押された,Zテスト切り替え

		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//埋めるモード
	}
	else
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//埋めるモード
	}

	//エフェクトの更新処理
	UpdateEffect();

	//パーティクルの更新処理
	UpdateParticle();

	//入力機器の更新処理
	UpdateInput();

	//ポリゴンの更新処理
	UpdatePolygon();

	//壁の更新処理
	UpdateWall();

	//ビルボードの更新処理
	UpdateBillboard();

	//カメラの更新処理
	UpdateCamera();

	//ライトの更新処理
	UpdateLight();

	//モデルの更新処理
	UpdateModel();

	//影の更新処理
	UpdateShadow();

	//敵の更新処理
	UpdateEnemy();

	//プレイヤーの更新処理
	UpdatePlayer();

	//弾の更新処理
	UpdateBullet();

	//爆発の更新処理
	UpdateExplosion();

	//デバッグ表示の更新処理
	UpdateDebugProc();

	//メッシュフィールドの更新処理
	UpdateMeshField();

	//メッシュウォールの更新処理
	UpdateMeshWall();

	//メッシュシリンダーの更新処理
	UpdateMeshCylinder();

	//メッシュウォールの更新処理
	UpdateMeshDome();

	
}
//==================================================================================
//描画処理
//==================================================================================
void Draw(void)
{
	//画面クリア(バックバッファとZバッファのクリア)
	g_pD3DDevice->Clear
	(	0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0
	);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice -> BeginScene()))
	{//描画が成功したとき

		//カメラの設定処理
		SetCamera();

		//ポリゴンの描画処理
		//DrawPolygon();

		//メッシュフィールドの描画処理
		DrawMeshField();

		//メッシュシリンダーの描画処理
		DrawMeshCylinder();

		//メッシュウォールの描画処理
		DrawMeshWall();

		//メッシュウォールの描画処理
		DrawMeshDome();


		//壁の描画処理
		DrawWall();

		//モデルの描画処理
		DrawModel();

		//影の描画処理
		DrawShadow();

		//敵の描画処理
		DrawEnemy();

		//プレイヤーの描画処理
		DrawPlayer();

		//ビルボードの描画処理
		DrawBillboard();

		//弾の描画処理
		DrawBullet();

		//爆発の描画処理
		DrawExplosion();

		//エフェクトの描画処理
		DrawEffect();

		//パーティクルの描画処理
		DrawParticle();

		//デバッグ表記
		DrawDebug();

		//デバッグ表示の描画処理
		DrawDebugProc();

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==================================================================================
//モードの設定
//==================================================================================
void SetMode(MODE mode)
{
	//現在の画面(モード)の終了処理
	//switch (g_mode)
	{
	

	}

	//新しい画面(モード)の初期化処理
	//switch (mode)
	{
	

	}

	g_mode = mode;		//モードの設定
}

//==================================================================================
//モードの取得
//==================================================================================
MODE GetMode(void)
{
	return g_mode;
}

//==================================================================================
//角度の正規化処理
//==================================================================================
float RotNormalize(float fRot)
{
	//1周分越えたら補正する
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2.0f;
	}
	else if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2.0f;
	}

	return fRot;
}

//==================================================================================
// オブジェクト間の距離算出
//==================================================================================
float CircleRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float fLength = (pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z);

	return fLength;
}

//==================================================================================
//デバイスの取得
//==================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==================================================================================
//フォントの取得
//==================================================================================
LPD3DXFONT GetFont(void)
{
	return g_pFont;
}

//==================================================================================
//デバッグ表記
//==================================================================================
void DrawDebug(void)
{

//#ifdef _DEBUG

	//FPS表示
	DrawFPS();

	//操作説明
	DrawControll();

	//カメラの位置情報
	DrawPosCamera();

	//モデルの情報
	DrawPosPlayer();

	//ライトの情報
	DrawPosLight();

	DrawBillboardTest();

//#endif
}

//==================================================================================
//FPSの表示
//==================================================================================
void DrawFPS(void)
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char aStr[256];

	//文字列に代入
	wsprintf(&aStr[0],	"FPS：%d\n", g_nCountFPS);

	//テキストの描画処理
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//==================================================================================
//操作方法の表示
//==================================================================================
void DrawControll(void)
{
	RECT rect = { 0, 150, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//文字列に代入
	wsprintf(&aStr[0], "------------- カメラ移動 -------------\n"
		"＜視点移動＞　　　　 [Y/N/Z/C]\n"
		"＜注視点移動＞　　　 [T/B/Q/E]\n"
		"＜視点 & 注視点移動＞[W/A/S/D]\n\n"
		"視点間距離移動　 　　　　 ：[U/M]\n"
		"※カメラの位置情報リセット：[SPACE]\n\n");

	//テキストの描画処理
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

}

//==================================================================================
// カメラの位置情報表示
//==================================================================================
void DrawPosCamera(void)
{
	RECT rect = { 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//カメラの情報取得
	Camera aCamera = GetCamera();

	//文字列に代入
	sprintf(&aStr[0],
		"------------- カメラの位置情報 -------------\n"
		"＜視点＞　　　　　　　(%.2f, %.2f, %.2f)\n"
		"＜注視点＞　　　　　　(%.2f, %.2f, %.2f)\n"
		"＜カメラの向き＞　　　(%.2f, %.2f, %.2f))\n"
		"＜視点と注視点の距離＞(%.2f)", aCamera.posV.x, aCamera.posV.y, aCamera.posV.z, aCamera.posR.x, aCamera.posR.y, aCamera.posR.z, aCamera.rot.x, aCamera.rot.y, aCamera.rot.z, aCamera.fDistance);

	//テキストの描画処理
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//==================================================================================
// プレイヤーの位置情報表示
//==================================================================================
void DrawPosPlayer(void)
{
	RECT rect = { 0, 300, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[512];

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	Model aModel = GetModel();

	//文字列に代入
	sprintf(&aStr[0], "------------- モデル情報 -------------\n"
		"＜プレイヤー座標＞(%.2f, %.2f, %.2f)\n"
		"＜プレイヤー向き＞(%.2f, %.2f, %.2f)\n"
		"＜プレイヤー移動＞[W/A/S/D]　　　※追従状態で\n"
		"プレイヤー周辺にモデル配置：[F2]\n"
		"モデル[X：%.2f, %.2f  Y：%.2f, %.2f  Z：%.2f, %.2f}]", pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z, pPlayer->rot.x, pPlayer->rot.y, pPlayer->rot.z,
		aModel.vtxMin.x, aModel.vtxMax.x, aModel.vtxMin.y, aModel.vtxMax.y, aModel.vtxMin.z, aModel.vtxMax.z);

	//テキストの描画処理
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//==================================================================================
// ライトの情報
//==================================================================================
void DrawPosLight(void)
{
	RECT rect = { 0, 420, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//ライトの情報
	D3DLIGHT9 *pLight = GetLight();
	D3DXVECTOR3 *pLightDir = GetLightDir();

	//文字列に代入
	wsprintf(&aStr[0], "------------- ライト情報 -------------\n");

	//テキストの描画処理
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//情報描画
	char aStrLight[256];

	for (int nCount = 0; nCount < MAX_LIGHT; nCount++, pLightDir++, pLight++)
	{
		rect = { 0, 440 + 20 * nCount, SCREEN_WIDTH, SCREEN_HEIGHT };

		//文字列に代入
		sprintf(&aStrLight[0], "＜ライト%d＞方向：(%.2f, %.2f, %.2f)　拡散光：(%.2f, %.2f, %.2f)\n", nCount,
			pLightDir->x, pLightDir->y, pLightDir->z,
			pLight->Diffuse.r, pLight->Diffuse.g, pLight->Diffuse.b);

		//テキストの描画処理
		g_pFont->DrawText(NULL, &aStrLight[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	}

}

//==================================================================================
// 各種設定
//==================================================================================
void DrawBillboardTest(void)
{
	RECT rect = { 0, 520, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	bool bZTest = GetZTest();
	bool bAlphaTest = GetAlphaTest();
	Camera aCamera = GetCamera();
	bool bEffectUse = GetEffectUse();

	char aBackFollow[256], aFollow[256], aZStr[256], aAlphaStr[256], aEffectUse[256];

	//背面追従判定
	if (aCamera.bBackFollow == true){ strcpy(&aBackFollow[0], "ON"); }
	else{ strcpy(&aBackFollow[0], "OFF"); }

	//追従判定
	if (aCamera.bFollow == true){ strcpy(&aFollow[0], "ON"); }
	else{ strcpy(&aFollow[0], "OFF"); }

	//Zテスト
	if (bZTest == false) { strcpy(&aZStr[0], "ON"); }
	else { strcpy(&aZStr[0], "OFF"); }

	//アルファテスト
	if (bAlphaTest == true) { strcpy(&aAlphaStr[0], "ON"); }
	else { strcpy(&aAlphaStr[0], "OFF"); }

	//エフェクト
	if (bEffectUse == true) { strcpy(&aEffectUse[0], "ON"); }
	else { strcpy(&aEffectUse[0], "OFF"); }

	//文字列に代入
	wsprintf(&aStr[0], "------------- 各種設定 -------------\n"
		"エフェクト　　：[F5]【 %s 】\n"
		"背面追従　　　：[F6]【 %s 】\n"
		"追従カメラ	　 ：[F7]【 %s 】\n"
		"Zテスト 　　　：[F8]【 %s 】\n"
		"アルファテスト：[F9]【 %s 】\n", aEffectUse, aBackFollow, aFollow, aZStr, aAlphaStr);

	//テキストの描画処理
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
