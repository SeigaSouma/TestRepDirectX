//=============================================================================
//
// �G���� [player.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _PLAYER_H_		//���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define MAX_PLAYER	(128)	//���f���̍ő吔
#define MAX_MOTION	(16)	//���[�V�����̍ő吔
#define MAX_KEY		(16)	//�L�[�̍ő吔
#define MAX_PARTS	(32)	//�L�[�̍ő吔
#define JUMP		(20.0f)	//�W�����v�ړ���
#define GRAVITY		(1.0f)	//�d��
#define MAX_MAT		(128)	//�}�e���A���T�C�Y

#include "main.h"

//���f���̎��
typedef enum
{
	PLAYERPARTS_BODY = 0,	//��
	PLAYERPARTS_HEAD,		//��
	PLAYERPARTS_R_NINO,		//�E��̘r
	PLAYERPARTS_R_UDE,		//�E�r
	PLAYERPARTS_R_HAND,		//�E��
	PLAYERPARTS_R_MOMO,		//�E����
	PLAYERPARTS_R_LEG,		//�E�ӂ���͂�
	PLAYERPARTS_R_ASIKUBI,	//�E����
	PLAYERPARTS_R_FOOT,		//�E��
	PLAYERPARTS_L_NINO,		//����̘r
	PLAYERPARTS_L_UDE,		//���r
	PLAYERPARTS_L_HAND,		//����
	PLAYERPARTS_L_MOMO,		//������
	PLAYERPARTS_L_LEG,		//���ӂ���͂�
	PLAYERPARTS_L_ASIKUBI,	//������
	PLAYERPARTS_L_FOOT,		//����
	PLAYERPARTS_MAX
}PLAYERPARTS;

typedef enum
{
	MOTION_LOOP_OFF = 0,	//���[�v����
	MOTION_LOOP_ON,		//���[�v����
	MOTION_LOOP_MAX
}MOTION_LOOP;

typedef enum
{
	PLAYERMOTION_DEF = 0,	//�j���[�g�������[�V����
	PLAYERMOTION_WALK,		//�ړ����[�V����
	PLAYERMOTION_ACTION,	//�A�N�V�������[�V����
	PLAYERMOTION_JUMP,		//�W�����v���[�V����
	PLAYERMOTION_LAND,		//���n���[�V����
	PLAYERMOTION_SHOT,		//�ˌ����[�V����
	PLAYERMOTION_MAX
}PLAYERMOTION;

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_NONE = 0,	//�������Ă��Ȃ����
	PLAYERSTATE_DMG,		//�_���[�W���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�G�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//���݂̈ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nIdxShadow;			//�e�̃C���f�b�N�X�ԍ�
	int nLife;				//�̗�
	int nState;				//���
	int nCntState;			//��ԃJ�E���g
	bool bUse;				//�g�p���Ă��邩
	int nNowMotionNum;		//���݂̃��[�V�����ԍ�
	bool bMove;				//�ړ����Ă��邩
	bool bJump;				//�W�����v���Ă��邩
	bool bUseGun;			//�e���g���Ă��邩
	bool bATK;				//�U�����Ă邩
	int nIdxGun;			//�e�̃C���f�b�N�X�ԍ�
	int nCntWalk;			//�ړ��̃J�E���g
	int nPartsNum;			//�p�[�c��
	float fRadius;			//���a
}Player;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	D3DXVECTOR3 posOrigin;	//���̈ʒu
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nType;			//�ǂݍ��ރ��f���̎��
	int nParent;		//�e�̔ԍ�
	int nWeapon;		//���킩�ǂ���
	bool bUse;			//�g�p���Ă��邩

	LPD3DXMESH pMesh = {};			//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = {};		//�}�e���A���ւ̃|�C���^
	D3DXMATERIAL pMatData[MAX_MAT];	//�}�e���A���̃f�[�^
	DWORD dwNumMat = { 0 };			//�}�e���A���̐�
	LPDIRECT3DTEXTURE9 pTexture[30] = {};	//�e�N�X�`���ւ̃|�C���^
}SetPartsPlayer;

//���[�V�����̍\����
typedef struct
{
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 rotDest;	//�ڕW�̌���
	D3DXVECTOR3 rotDiff;	//�����̍���
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posDest;	//�ڕW�̈ʒu
	D3DXVECTOR3 posDiff;	//�ʒu�̍���
}Parts;

typedef struct
{
	Parts aParts[MAX_PARTS];
	int nFrame;			//�Đ��t���[��
}Key;

typedef struct
{
	Key aKey[MAX_KEY];
	int nCntFrame;		//�t���[���̃J�E���g
	int nPatternKey;	//���ڂ̃L�[��
	int nNumKey;		//�L�[�̐�
	int nLoop;			//���[�v����
	bool bUse;			//�g�p���Ă��邩
}MotionData;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void SetPlayer(void);
void SetMotisonPlayer(int nMotionType);
void HitPlayer(D3DXVECTOR3 move, int nDMG);

#endif