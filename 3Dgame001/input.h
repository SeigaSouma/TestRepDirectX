//=============================================================================
//
// ���͏��� [input.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _INPUT_H_	//���̃}�N����`������Ă��Ȃ�������
#define _INPUT_H_	//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "Xinput.h"
#pragma comment(lib, "xinput.lib")	//�R���g���[���[�ɕK�v

typedef enum
{
	BUTTON_UP = 0,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_START,
	BUTTON_BACK,
	BUTTON_LSTICKPUSH,
	BUTTON_RSTICKPUSH,
	BUTTON_LB,
	BUTTON_RB,
	BUTTON_LT,
	BUTTON_RT,
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_LX,
	BUTTON_LY,
	BUTTON_18,
	BUTTON_19,
	BUTTON_MAX
}BUTTON;

typedef enum
{
	STICK_0 = 0,
	STICK_MAX
}STICK;

typedef enum
{
	MOUSE_BUTTON_LEFT = 0,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_WHEEL,
	MOUSE_BUTTON_SIDE_1,
	MOUSE_BUTTON_SIDE_2
}MOUSE_BUTTON;


typedef enum
{
	VIBRATION_STATE_NONE = 0,	//�������ĂȂ����
	VIBRATION_STATE_DMG,		//�_���[�W���
	VIBRATION_STATE_ITEM,		//�A�C�e��

}VIBRATION_STATE;


HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);


//�L�[�{�[�h===============================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);

//�v���g�^�C�v�錾
int GetnCntPad(void);
int GetnCntKey(void);
//�L�[�{�[�h================================

//�R���g���[���[============================
HRESULT InitGamepad(void);
void UninitGamepad(void);
void UpdateGamepad(void);
bool GetGamepadPress(BUTTON nKey, int nCntPlayer);
bool GetGamepadPressLTrigger(BUTTON nKey, int nCntPlayer);
bool GetGamepadPressRTrigger(BUTTON nKey, int nCntPlayer);
bool GetGamepadTrigger(BUTTON nKey, int nCntPlayer);
bool GetGamepadRepeat(BUTTON nKey, int nCntPlayer);
bool GetGamepadRelease(BUTTON nKey, int nCntPlayer);

SHORT XGetStickPressL(BUTTON nKey, int nCntPlayer);
SHORT YGetStickPressL(BUTTON nKey, int nCntPlayer);
SHORT XGetStickPressR(BUTTON nKey, int nCntPlayer);
SHORT YGetStickPressR(BUTTON nKey, int nCntPlayer);

BYTE GetLeftTriggerPress(int nPlayer);
BYTE GetRightTriggerPress(int nPlayer);

void SetVibration(VIBRATION_STATE VibState, int nCntPlayer);
void SetEnableStickSelect(bool bStick);
bool GetStickSelect(void);
void SetEnableVibration(void);
bool GetEnableVibration(void);
//�R���g���[���[============================

//�}�E�X====================================
HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(MOUSE_BUTTON nKey);

D3DXVECTOR3 GetMouseMove(void);

#endif