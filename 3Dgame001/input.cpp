//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "input.h"

//マクロ定義
#define NUM_KEY_MAX	(256)	//キーの最大数
#define SHOT_FPS	(15)	//弾の間隔
#define MAX_PLAYER	(4)		//最大プレイヤー
#define DMG_TIME	(30)	//バイブの時間
#define WIN_TIME	(15)	//バイブの時間
#define MOUSE_SENS (0.5f) //マウス感度の補正

//マウス
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevMouse = NULL; //入力デバイスへのポインタ
	DIMOUSESTATE2 MouseState; //全入力情報の保管
	DIMOUSESTATE2 MouseStateTrigger; //トリガー情報
	DIMOUSESTATE2 MouseStateRerease; //リリース情報
	DIMOUSESTATE2 MouseStateRepeat; //リピート情報
	BYTE aOldState[8]; //前回の入力情報を保存
}Mouse;

//グローバル変数宣言
LPDIRECTINPUT8 g_pInput = NULL;				//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	//入力デバイス(キーボード)へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];				//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];		//キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];		//キーボードのリリース情報
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];		//キーボードのリピート情報
int g_nCntPadrepeat = 0;
int g_nCntKeyRepeat = 0;

//ゲームパッド用グローバル
XINPUT_STATE g_aGamepadState[MAX_PLAYER];				//ゲームパッドのプレス情報
XINPUT_STATE g_aGamepadStateTrigger[MAX_PLAYER];		//ゲームパッドのトリガー情報
XINPUT_STATE g_aGamepadStateRepeat[MAX_PLAYER];			//ゲームパッドのリピート情報
XINPUT_STATE g_aGamepadStateRelease[MAX_PLAYER];		//ゲームパッドのリリース情報
XINPUT_VIBRATION g_aGamepadStateVib[MAX_PLAYER];		//ゲームパッドのバイブレーション
int g_nCntVibration[MAX_PLAYER];						//振動の時間
int g_nMaxCntVibration[MAX_PLAYER];						//振動の時間
VIBRATION_STATE g_VibrationState[MAX_PLAYER];
bool g_bLeftStickSelect;								//左トリガーの選択判定
int g_nLeftStickCount;									//左トリガーの選択カウント
bool g_bVibrationUse;									//バイブを使用するかどうか
Mouse g_Mouse;											//マウスの情報

//==================================================================================
//入力機器の初期化処理
//==================================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードの初期化処理
	InitKeyboard(hInstance, hWnd);

	//ゲームパッドの初期化処理
	InitGamepad();

	//マウスの初期化処理
	InitMouse(hWnd);

	return S_OK;
}

//==================================================================================
//入力機器の終了処理
//==================================================================================
void UninitInput(void)
{
	//キーボードの終了処理
	UninitKeyboard();

	//ゲームパッドの終了処理
	UninitGamepad();

	//マウスの終了処理
	UninitMouse();

}

//==================================================================================
//入力機器の更新処理
//==================================================================================
void UpdateInput(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//ゲームパッドの更新処理
	UpdateGamepad();

	//マウスの更新処理
	UpdateMouse();

}


//==================================================================================
//キーボードの初期化処理
//==================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	g_nCntKeyRepeat = 0;
	g_nCntPadrepeat = 0;


	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//==================================================================================
//キーボードの終了処理
//==================================================================================
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();	//キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//頂点バッファの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}

}

//==================================================================================
//キーボードの更新処理
//==================================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{

			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//トリガー情報を保存
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];	//リリース情報を保存

			if (GetKeyboardPress(DIK_SPACE) == true)
			{
				g_nCntKeyRepeat++;
			}
			
			if ((g_nCntKeyRepeat % SHOT_FPS) == 0)
			{
				g_nCntKeyRepeat = (g_nCntKeyRepeat + 1) % SHOT_FPS;
				g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey] ;	//リリース情報を保存
			}
			else
			{
				g_aKeyStateRepeat[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];	//リピートにリリースの情報保存
			}
			g_aKeyState[nCntKey] = aKeyState[nCntKey];		//キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	//キーボードへのアクセス権を獲得
	}
}

//==================================================================================
//キーボードのプレス情報を取得
//==================================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//==================================================================================
//キーボードのトリガー情報を取得
//==================================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==================================================================================
//キーボードのリリース情報を取得
//==================================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//==================================================================================
//キーボードのリピート情報を取得
//==================================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//==================================================================================
//ゲームパッドの初期化処理
//==================================================================================
HRESULT InitGamepad(void)
{

	//XInputを有効化
	XInputEnable(true);

	//左スティックの情報を初期化
	g_bLeftStickSelect = false;
	g_nLeftStickCount = 0;

	//バイブの情報を初期化
	g_bVibrationUse = true;

	//メモリクリア
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_nCntVibration[nCntPlayer] = 0;
		g_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;
		g_nMaxCntVibration[nCntPlayer] = 0;

		memset(&g_aGamepadState[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&g_aGamepadStateTrigger[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&g_aGamepadStateRepeat[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&g_aGamepadStateRelease[nCntPlayer], 0, sizeof(XINPUT_STATE));
		memset(&g_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
	}

	return S_OK;
}

//==================================================================================
//ゲームパッドの終了処理
//==================================================================================
void UninitGamepad(void)
{
	//XInputを無効化
	XInputEnable(false);
}

//==================================================================================
//ゲームパッドの更新処理
//==================================================================================
void UpdateGamepad(void)
{
	XINPUT_STATE  aGamepadState[MAX_PLAYER];
	int nCntPlayer;

	//入力デバイスからデータを取得
	for (nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (XInputGetState(nCntPlayer, &aGamepadState[nCntPlayer]) == ERROR_SUCCESS)
		{
			//キーボードのトリガー情報を保存
			g_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons =
				(~g_aGamepadState[nCntPlayer].Gamepad.wButtons) & aGamepadState[nCntPlayer].Gamepad.wButtons;

			//リリース情報を保存
			g_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons = 
				(g_aGamepadState[nCntPlayer].Gamepad.wButtons ^ aGamepadState[nCntPlayer].Gamepad.wButtons) & g_aGamepadState[nCntPlayer].Gamepad.wButtons;


			//キーボードのプレス情報を保存
			g_aGamepadState[nCntPlayer] = aGamepadState[nCntPlayer];
			
			if ((g_nCntPadrepeat % SHOT_FPS) == 0)
			{
				//リピート情報を保存
				g_nCntPadrepeat = (g_nCntPadrepeat + 1) % SHOT_FPS;
				g_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons = aGamepadState[nCntPlayer].Gamepad.wButtons;
			}
			else
			{
				//リピートにリリースの情報保存
				g_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons =
					(g_aGamepadState[nCntPlayer].Gamepad.wButtons ^ aGamepadState[nCntPlayer].Gamepad.wButtons) & g_aGamepadState[nCntPlayer].Gamepad.wButtons;
			}
		}

		//タイマーを減算
		g_nCntVibration[nCntPlayer]--;

		if (g_nCntVibration[nCntPlayer] < 0)
		{//カウントがゼロ以下

			//何もしてない状態に設定
			g_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;

			//メモリ初期化
			memset(&g_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		}
		
		if (g_VibrationState[nCntPlayer] != VIBRATION_STATE_NONE)
		{//ダメージの時

			if (g_VibrationState[nCntPlayer] == VIBRATION_STATE_ITEM)
			{//アイテムの時は増えていく

				g_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed += (USHRT_MAX / g_nMaxCntVibration[nCntPlayer]);
				g_aGamepadStateVib[nCntPlayer].wRightMotorSpeed += (USHRT_MAX / g_nMaxCntVibration[nCntPlayer]);
			}
			else
			{
				g_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed -= (USHRT_MAX / g_nMaxCntVibration[nCntPlayer]);
				g_aGamepadStateVib[nCntPlayer].wRightMotorSpeed -= (USHRT_MAX / g_nMaxCntVibration[nCntPlayer]);
			}

			if (g_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed <= 0)
			{//スピードが0以下

				g_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = 0;
			}
			else if (g_aGamepadStateVib[nCntPlayer].wRightMotorSpeed <= 0)
			{//スピードが0以下

				g_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = 0;
			}
		}


		//コントローラーにバイブの情報をXINPUTに送る
		XInputSetState(nCntPlayer, &g_aGamepadStateVib[nCntPlayer]);
	}

	
	if (YGetStickPressL(BUTTON_LY, 0) == 0 && XGetStickPressL(BUTTON_LX, 0) == 0)
	{//スティックがもとに戻っているとき

		//カウントを初期化
		g_nLeftStickCount = 0;

		//判定をOFF
		g_bLeftStickSelect = false;
	}

}

//==================================================================================
//バイブの設定処理
//==================================================================================
void SetVibration(VIBRATION_STATE VibState, int nCntPlayer)
{
	if (g_bVibrationUse == true)
	{

		//状態を代入
		g_VibrationState[nCntPlayer] = VibState;

		switch (VibState)
		{
		case VIBRATION_STATE_DMG:

			g_nCntVibration[nCntPlayer] = 15;
			g_nMaxCntVibration[nCntPlayer] = g_nCntVibration[nCntPlayer];
			g_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = USHRT_MAX * (short)0.8f;
			g_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = USHRT_MAX * (short)0.8f;
			break;

		case VIBRATION_STATE_ITEM:
			g_nCntVibration[nCntPlayer] = 100;
			g_nMaxCntVibration[nCntPlayer] = g_nCntVibration[nCntPlayer];
			g_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = (USHRT_MAX * (short)0.00001f);
			g_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = (USHRT_MAX * (short)0.00001f);
			break;
		}

		//コントローラーにバイブの情報をXINPUTに送る
		XInputSetState(nCntPlayer, &g_aGamepadStateVib[nCntPlayer]);

	}
}

//==================================================================================
//ゲームパッドのバイブ設定処理
//==================================================================================
void SetEnableVibration(void)
{
	//切り替え
	g_bVibrationUse ^= true;
}

//==================================================================================
//ゲームパッドのバイブ設定処理
//==================================================================================
bool GetEnableVibration(void)
{
	return g_bVibrationUse;
}

//==================================================================================
//ゲームパッドのプレス処理
//==================================================================================
bool GetGamepadPress(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadState[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==================================================================================
//ゲームパッドのトリガー処理
//==================================================================================
bool GetGamepadTrigger(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==================================================================================
//ゲームパッドのリピート処理
//==================================================================================
bool GetGamepadRelease(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==================================================================================
//ゲームパッドのリピート処理
//==================================================================================
bool GetGamepadRepeat(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}


//==================================================================================
//ゲームパッドのプレス処理,LT
//==================================================================================
bool GetGamepadPressLTrigger(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadState[nCntPlayer].Gamepad.bLeftTrigger & (0x01 << nKey)) ? true : false;
}

//==================================================================================
//ゲームパッドのプレス処理,RT
//==================================================================================
bool GetGamepadPressRTrigger(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadState[nCntPlayer].Gamepad.bRightTrigger & (0x01 << nKey)) ? true : false;
}

//==================================================================================
//LスティックのXプレス処理
//==================================================================================
SHORT XGetStickPressL(BUTTON nKey, int nCntPlayer)
{
	return g_aGamepadState[nCntPlayer].Gamepad.sThumbLX;
}

//==================================================================================
//LスティックのYプレス処理
//==================================================================================
SHORT YGetStickPressL(BUTTON nKey, int nCntPlayer)
{
	return g_aGamepadState[nCntPlayer].Gamepad.sThumbLY;
}

//==================================================================================
//RスティックのXプレス処理
//==================================================================================
SHORT XGetStickPressR(BUTTON nKey, int nCntPlayer)
{
	return g_aGamepadState[nCntPlayer].Gamepad.sThumbRX;
}

//==================================================================================
//RスティックのXプレス処理
//==================================================================================
SHORT YGetStickPressR(BUTTON nKey, int nCntPlayer)
{
	return g_aGamepadState[nCntPlayer].Gamepad.sThumbRY;
}

//==================================================================================
//ゲームパッドのLトリガー
//==================================================================================
BYTE GetLeftTriggerPress(int nPlayer)
{
	return g_aGamepadState[nPlayer].Gamepad.bLeftTrigger;
}

//==================================================================================
//ゲームパッドのRトリガー
//==================================================================================
BYTE GetRightTriggerPress(int nPlayer)
{
	return g_aGamepadState[nPlayer].Gamepad.bRightTrigger;
}

//==================================================================================
// 左スティックの判定を取得
//==================================================================================
bool GetStickSelect(void)
{
	return g_bLeftStickSelect;
}

//==================================================================================
// 左スティックの判定
//==================================================================================
void SetEnableStickSelect(bool bStick)
{
	g_bLeftStickSelect = bStick;
}

//==================================================================================
//パッドのリピートFPS
//==================================================================================
int GetnCntPad(void)
{
	return g_nCntKeyRepeat;
}

//==================================================================================
//キーボードのリピートFPS
//==================================================================================
int GetnCntKey(void)
{
	return g_nCntPadrepeat;
}


//==========================================
//  マウスの初期化
//==========================================
HRESULT InitMouse(HWND hWnd)
{
	//入力デバイスの設定
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_Mouse.pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_Mouse.pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_Mouse.pDevMouse->SetCooperativeLevel
	(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; //相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(g_Mouse.pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// デバイスの設定に失敗
		return E_FAIL;
	}

	//マウスへのアクセス権を取得
	g_Mouse.pDevMouse->Acquire();

	//マウスカーソルの消去
	//ShowCursor(false);

	return S_OK;
}

//==========================================
//  マウスの終了処理
//==========================================
void UninitMouse(void)
{
	//入力デバイス(マウス)の破棄
	if (g_Mouse.pDevMouse != NULL)
	{
		g_Mouse.pDevMouse->Unacquire();
		g_Mouse.pDevMouse->Release();
		g_Mouse.pDevMouse = NULL;
	}
}

//==========================================
//  マウスの更新処理
//==========================================
void UpdateMouse(void)
{
	DIMOUSESTATE2 mouse; //マウスの入力情報

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_Mouse.pDevMouse->GetDeviceState(sizeof(mouse), &mouse)))
	{
		//入力情報の保存
		g_Mouse.MouseState = mouse;
	}
	else
	{
		g_Mouse.pDevMouse->Acquire(); //マウスへのアクセス権
	}
}

//==========================================
//  マウスのプレス情報
//==========================================
bool GetMousePress(MOUSE_BUTTON nKey)
{
	return (g_Mouse.MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================
//  マウスの移動量
//==========================================
D3DXVECTOR3 GetMouseMove(void)
{
	return D3DXVECTOR3(((float)g_Mouse.MouseState.lX) * MOUSE_SENS, (-(float)g_Mouse.MouseState.lY) * MOUSE_SENS, (-(float)g_Mouse.MouseState.lZ) * MOUSE_SENS);
}

