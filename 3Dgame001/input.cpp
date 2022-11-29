//=============================================================================
//
// ���͏��� [input.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input.h"

//�}�N����`
#define NUM_KEY_MAX	(256)	//�L�[�̍ő吔
#define SHOT_FPS	(15)	//�e�̊Ԋu
#define MAX_PLAYER	(4)		//�ő�v���C���[
#define DMG_TIME	(30)	//�o�C�u�̎���
#define WIN_TIME	(15)	//�o�C�u�̎���
#define MOUSE_SENS (0.5f) //�}�E�X���x�̕␳

//�}�E�X
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevMouse = NULL; //���̓f�o�C�X�ւ̃|�C���^
	DIMOUSESTATE2 MouseState; //�S���͏��̕ۊ�
	DIMOUSESTATE2 MouseStateTrigger; //�g���K�[���
	DIMOUSESTATE2 MouseStateRerease; //�����[�X���
	DIMOUSESTATE2 MouseStateRepeat; //���s�[�g���
	BYTE aOldState[8]; //�O��̓��͏���ۑ�
}Mouse;

//�O���[�o���ϐ��錾
LPDIRECTINPUT8 g_pInput = NULL;				//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
BYTE g_aKeyState[NUM_KEY_MAX];				//�L�[�{�[�h�̃v���X���
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];		//�L�[�{�[�h�̃g���K�[���
BYTE g_aKeyStateRelease[NUM_KEY_MAX];		//�L�[�{�[�h�̃����[�X���
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];		//�L�[�{�[�h�̃��s�[�g���
int g_nCntPadrepeat = 0;
int g_nCntKeyRepeat = 0;

//�Q�[���p�b�h�p�O���[�o��
XINPUT_STATE g_aGamepadState[MAX_PLAYER];				//�Q�[���p�b�h�̃v���X���
XINPUT_STATE g_aGamepadStateTrigger[MAX_PLAYER];		//�Q�[���p�b�h�̃g���K�[���
XINPUT_STATE g_aGamepadStateRepeat[MAX_PLAYER];			//�Q�[���p�b�h�̃��s�[�g���
XINPUT_STATE g_aGamepadStateRelease[MAX_PLAYER];		//�Q�[���p�b�h�̃����[�X���
XINPUT_VIBRATION g_aGamepadStateVib[MAX_PLAYER];		//�Q�[���p�b�h�̃o�C�u���[�V����
int g_nCntVibration[MAX_PLAYER];						//�U���̎���
int g_nMaxCntVibration[MAX_PLAYER];						//�U���̎���
VIBRATION_STATE g_VibrationState[MAX_PLAYER];
bool g_bLeftStickSelect;								//���g���K�[�̑I�𔻒�
int g_nLeftStickCount;									//���g���K�[�̑I���J�E���g
bool g_bVibrationUse;									//�o�C�u���g�p���邩�ǂ���
Mouse g_Mouse;											//�}�E�X�̏��

//==================================================================================
//���͋@��̏���������
//==================================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�̏���������
	InitKeyboard(hInstance, hWnd);

	//�Q�[���p�b�h�̏���������
	InitGamepad();

	//�}�E�X�̏���������
	InitMouse(hWnd);

	return S_OK;
}

//==================================================================================
//���͋@��̏I������
//==================================================================================
void UninitInput(void)
{
	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//�Q�[���p�b�h�̏I������
	UninitGamepad();

	//�}�E�X�̏I������
	UninitMouse();

}

//==================================================================================
//���͋@��̍X�V����
//==================================================================================
void UpdateInput(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	//�Q�[���p�b�h�̍X�V����
	UpdateGamepad();

	//�}�E�X�̍X�V����
	UpdateMouse();

}


//==================================================================================
//�L�[�{�[�h�̏���������
//==================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	g_nCntKeyRepeat = 0;
	g_nCntPadrepeat = 0;


	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//==================================================================================
//�L�[�{�[�h�̏I������
//==================================================================================
void UninitKeyboard(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�������
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}

}

//==================================================================================
//�L�[�{�[�h�̍X�V����
//==================================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//�L�[�{�[�h�̓��͏��
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{

			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//�g���K�[����ۑ�
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];	//�����[�X����ۑ�

			if (GetKeyboardPress(DIK_SPACE) == true)
			{
				g_nCntKeyRepeat++;
			}
			
			if ((g_nCntKeyRepeat % SHOT_FPS) == 0)
			{
				g_nCntKeyRepeat = (g_nCntKeyRepeat + 1) % SHOT_FPS;
				g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey] ;	//�����[�X����ۑ�
			}
			else
			{
				g_aKeyStateRepeat[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];	//���s�[�g�Ƀ����[�X�̏��ۑ�
			}
			g_aKeyState[nCntKey] = aKeyState[nCntKey];		//�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//==================================================================================
//�L�[�{�[�h�̃v���X�����擾
//==================================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//==================================================================================
//�L�[�{�[�h�̃g���K�[�����擾
//==================================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==================================================================================
//�L�[�{�[�h�̃����[�X�����擾
//==================================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//==================================================================================
//�L�[�{�[�h�̃��s�[�g�����擾
//==================================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//==================================================================================
//�Q�[���p�b�h�̏���������
//==================================================================================
HRESULT InitGamepad(void)
{

	//XInput��L����
	XInputEnable(true);

	//���X�e�B�b�N�̏���������
	g_bLeftStickSelect = false;
	g_nLeftStickCount = 0;

	//�o�C�u�̏���������
	g_bVibrationUse = true;

	//�������N���A
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
//�Q�[���p�b�h�̏I������
//==================================================================================
void UninitGamepad(void)
{
	//XInput�𖳌���
	XInputEnable(false);
}

//==================================================================================
//�Q�[���p�b�h�̍X�V����
//==================================================================================
void UpdateGamepad(void)
{
	XINPUT_STATE  aGamepadState[MAX_PLAYER];
	int nCntPlayer;

	//���̓f�o�C�X����f�[�^���擾
	for (nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (XInputGetState(nCntPlayer, &aGamepadState[nCntPlayer]) == ERROR_SUCCESS)
		{
			//�L�[�{�[�h�̃g���K�[����ۑ�
			g_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons =
				(~g_aGamepadState[nCntPlayer].Gamepad.wButtons) & aGamepadState[nCntPlayer].Gamepad.wButtons;

			//�����[�X����ۑ�
			g_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons = 
				(g_aGamepadState[nCntPlayer].Gamepad.wButtons ^ aGamepadState[nCntPlayer].Gamepad.wButtons) & g_aGamepadState[nCntPlayer].Gamepad.wButtons;


			//�L�[�{�[�h�̃v���X����ۑ�
			g_aGamepadState[nCntPlayer] = aGamepadState[nCntPlayer];
			
			if ((g_nCntPadrepeat % SHOT_FPS) == 0)
			{
				//���s�[�g����ۑ�
				g_nCntPadrepeat = (g_nCntPadrepeat + 1) % SHOT_FPS;
				g_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons = aGamepadState[nCntPlayer].Gamepad.wButtons;
			}
			else
			{
				//���s�[�g�Ƀ����[�X�̏��ۑ�
				g_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons =
					(g_aGamepadState[nCntPlayer].Gamepad.wButtons ^ aGamepadState[nCntPlayer].Gamepad.wButtons) & g_aGamepadState[nCntPlayer].Gamepad.wButtons;
			}
		}

		//�^�C�}�[�����Z
		g_nCntVibration[nCntPlayer]--;

		if (g_nCntVibration[nCntPlayer] < 0)
		{//�J�E���g���[���ȉ�

			//�������ĂȂ���Ԃɐݒ�
			g_VibrationState[nCntPlayer] = VIBRATION_STATE_NONE;

			//������������
			memset(&g_aGamepadStateVib[nCntPlayer], 0, sizeof(XINPUT_VIBRATION));
		}
		
		if (g_VibrationState[nCntPlayer] != VIBRATION_STATE_NONE)
		{//�_���[�W�̎�

			if (g_VibrationState[nCntPlayer] == VIBRATION_STATE_ITEM)
			{//�A�C�e���̎��͑����Ă���

				g_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed += (USHRT_MAX / g_nMaxCntVibration[nCntPlayer]);
				g_aGamepadStateVib[nCntPlayer].wRightMotorSpeed += (USHRT_MAX / g_nMaxCntVibration[nCntPlayer]);
			}
			else
			{
				g_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed -= (USHRT_MAX / g_nMaxCntVibration[nCntPlayer]);
				g_aGamepadStateVib[nCntPlayer].wRightMotorSpeed -= (USHRT_MAX / g_nMaxCntVibration[nCntPlayer]);
			}

			if (g_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed <= 0)
			{//�X�s�[�h��0�ȉ�

				g_aGamepadStateVib[nCntPlayer].wLeftMotorSpeed = 0;
			}
			else if (g_aGamepadStateVib[nCntPlayer].wRightMotorSpeed <= 0)
			{//�X�s�[�h��0�ȉ�

				g_aGamepadStateVib[nCntPlayer].wRightMotorSpeed = 0;
			}
		}


		//�R���g���[���[�Ƀo�C�u�̏���XINPUT�ɑ���
		XInputSetState(nCntPlayer, &g_aGamepadStateVib[nCntPlayer]);
	}

	
	if (YGetStickPressL(BUTTON_LY, 0) == 0 && XGetStickPressL(BUTTON_LX, 0) == 0)
	{//�X�e�B�b�N�����Ƃɖ߂��Ă���Ƃ�

		//�J�E���g��������
		g_nLeftStickCount = 0;

		//�����OFF
		g_bLeftStickSelect = false;
	}

}

//==================================================================================
//�o�C�u�̐ݒ菈��
//==================================================================================
void SetVibration(VIBRATION_STATE VibState, int nCntPlayer)
{
	if (g_bVibrationUse == true)
	{

		//��Ԃ���
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

		//�R���g���[���[�Ƀo�C�u�̏���XINPUT�ɑ���
		XInputSetState(nCntPlayer, &g_aGamepadStateVib[nCntPlayer]);

	}
}

//==================================================================================
//�Q�[���p�b�h�̃o�C�u�ݒ菈��
//==================================================================================
void SetEnableVibration(void)
{
	//�؂�ւ�
	g_bVibrationUse ^= true;
}

//==================================================================================
//�Q�[���p�b�h�̃o�C�u�ݒ菈��
//==================================================================================
bool GetEnableVibration(void)
{
	return g_bVibrationUse;
}

//==================================================================================
//�Q�[���p�b�h�̃v���X����
//==================================================================================
bool GetGamepadPress(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadState[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==================================================================================
//�Q�[���p�b�h�̃g���K�[����
//==================================================================================
bool GetGamepadTrigger(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadStateTrigger[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==================================================================================
//�Q�[���p�b�h�̃��s�[�g����
//==================================================================================
bool GetGamepadRelease(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadStateRelease[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//==================================================================================
//�Q�[���p�b�h�̃��s�[�g����
//==================================================================================
bool GetGamepadRepeat(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadStateRepeat[nCntPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}


//==================================================================================
//�Q�[���p�b�h�̃v���X����,LT
//==================================================================================
bool GetGamepadPressLTrigger(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadState[nCntPlayer].Gamepad.bLeftTrigger & (0x01 << nKey)) ? true : false;
}

//==================================================================================
//�Q�[���p�b�h�̃v���X����,RT
//==================================================================================
bool GetGamepadPressRTrigger(BUTTON nKey, int nCntPlayer)
{
	return (g_aGamepadState[nCntPlayer].Gamepad.bRightTrigger & (0x01 << nKey)) ? true : false;
}

//==================================================================================
//L�X�e�B�b�N��X�v���X����
//==================================================================================
SHORT XGetStickPressL(BUTTON nKey, int nCntPlayer)
{
	return g_aGamepadState[nCntPlayer].Gamepad.sThumbLX;
}

//==================================================================================
//L�X�e�B�b�N��Y�v���X����
//==================================================================================
SHORT YGetStickPressL(BUTTON nKey, int nCntPlayer)
{
	return g_aGamepadState[nCntPlayer].Gamepad.sThumbLY;
}

//==================================================================================
//R�X�e�B�b�N��X�v���X����
//==================================================================================
SHORT XGetStickPressR(BUTTON nKey, int nCntPlayer)
{
	return g_aGamepadState[nCntPlayer].Gamepad.sThumbRX;
}

//==================================================================================
//R�X�e�B�b�N��X�v���X����
//==================================================================================
SHORT YGetStickPressR(BUTTON nKey, int nCntPlayer)
{
	return g_aGamepadState[nCntPlayer].Gamepad.sThumbRY;
}

//==================================================================================
//�Q�[���p�b�h��L�g���K�[
//==================================================================================
BYTE GetLeftTriggerPress(int nPlayer)
{
	return g_aGamepadState[nPlayer].Gamepad.bLeftTrigger;
}

//==================================================================================
//�Q�[���p�b�h��R�g���K�[
//==================================================================================
BYTE GetRightTriggerPress(int nPlayer)
{
	return g_aGamepadState[nPlayer].Gamepad.bRightTrigger;
}

//==================================================================================
// ���X�e�B�b�N�̔�����擾
//==================================================================================
bool GetStickSelect(void)
{
	return g_bLeftStickSelect;
}

//==================================================================================
// ���X�e�B�b�N�̔���
//==================================================================================
void SetEnableStickSelect(bool bStick)
{
	g_bLeftStickSelect = bStick;
}

//==================================================================================
//�p�b�h�̃��s�[�gFPS
//==================================================================================
int GetnCntPad(void)
{
	return g_nCntKeyRepeat;
}

//==================================================================================
//�L�[�{�[�h�̃��s�[�gFPS
//==================================================================================
int GetnCntKey(void)
{
	return g_nCntPadrepeat;
}


//==========================================
//  �}�E�X�̏�����
//==========================================
HRESULT InitMouse(HWND hWnd)
{
	//���̓f�o�C�X�̐ݒ�
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_Mouse.pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_Mouse.pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_Mouse.pDevMouse->SetCooperativeLevel
	(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; //���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	if (FAILED(g_Mouse.pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// �f�o�C�X�̐ݒ�Ɏ��s
		return E_FAIL;
	}

	//�}�E�X�ւ̃A�N�Z�X�����擾
	g_Mouse.pDevMouse->Acquire();

	//�}�E�X�J�[�\���̏���
	//ShowCursor(false);

	return S_OK;
}

//==========================================
//  �}�E�X�̏I������
//==========================================
void UninitMouse(void)
{
	//���̓f�o�C�X(�}�E�X)�̔j��
	if (g_Mouse.pDevMouse != NULL)
	{
		g_Mouse.pDevMouse->Unacquire();
		g_Mouse.pDevMouse->Release();
		g_Mouse.pDevMouse = NULL;
	}
}

//==========================================
//  �}�E�X�̍X�V����
//==========================================
void UpdateMouse(void)
{
	DIMOUSESTATE2 mouse; //�}�E�X�̓��͏��

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_Mouse.pDevMouse->GetDeviceState(sizeof(mouse), &mouse)))
	{
		//���͏��̕ۑ�
		g_Mouse.MouseState = mouse;
	}
	else
	{
		g_Mouse.pDevMouse->Acquire(); //�}�E�X�ւ̃A�N�Z�X��
	}
}

//==========================================
//  �}�E�X�̃v���X���
//==========================================
bool GetMousePress(MOUSE_BUTTON nKey)
{
	return (g_Mouse.MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================
//  �}�E�X�̈ړ���
//==========================================
D3DXVECTOR3 GetMouseMove(void)
{
	return D3DXVECTOR3(((float)g_Mouse.MouseState.lX) * MOUSE_SENS, (-(float)g_Mouse.MouseState.lY) * MOUSE_SENS, (-(float)g_Mouse.MouseState.lZ) * MOUSE_SENS);
}

