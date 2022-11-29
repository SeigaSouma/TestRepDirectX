//=============================================================================
//
// ���C������ [main.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _MAIN_H_	//���̃}�N����`������Ă��Ȃ�������
#define _MAIN_H_	//��d�C���N���[�h�h�~�̃}�N�����`����


#include <Windows.h>
#include <stdio.h>
#include "time.h"
#include "d3dx9.h"						//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION	(0x0800)	//build���̌x���p�}�N��
#include "dinput.h"						//���͏����ɕK�v
#include "xaudio2.h"					//�T�E���h�����ɕK�v
#include <dsound.h>

//���C�u�����̃����N
#pragma comment(lib, "d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")		//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib, "dxguid.lib")		//DirectX�R���|�l�[�g(���i)�g�p�ɕK�v
#pragma comment(lib, "winmm.lib")		//�V�X�e�������擾�ɕK�v
#pragma comment(lib, "dinput8.lib")		//���͏����ɕK�v
#pragma comment(lib, "dsound.lib")

//�}�N����`
#define SCREEN_WIDTH		(1280)		//�E�C���h�E�̕�
#define SCREEN_HEIGHT		(720)		//�E�C���h�E�̍���
#define MOVE_UP				(1.0f)		//��ړ�
#define MOVE_LRUP			(0.75f)		//�΂ߏ�ړ�
#define MOVE_LR				(0.5f)		//�����ړ�
#define MOVE_LRDW			(0.25f)		//�΂ߏ�ړ�
#define MOVE_DOWN			(0.0f)		//���ړ�
#define MAX_COMMENT	(256)	//�R�����g�̍ő吔

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//���_�t�H�[�}�b�g
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//���_�t�H�[�}�b�g,�@��

//��ʑJ��(���[�h)�̎��
typedef enum
{
	MODE_TITLE = 0,		//�^�C�g�����
	MODE_SELECT,		//��Փx�Z���N�g���
	MODE_GAME,			//�Q�[�����
	MODE_RESULT,		//���U���g���
	MODE_TUTORIAL,		//�`���[�g���A��
	MODE_RANKING,		//�����L���O���
	MODE_MAX
}MODE;

//���_���̍\���̂�錾
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

//���_���[3D]�̍\���̂�錾
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_3D;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
LPD3DXFONT GetFont(void);
void SetMode(MODE mode);
MODE GetMode(void);
float RotNormalize(float fRot);
float CircleRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);

#endif