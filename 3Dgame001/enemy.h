//=============================================================================
//
// �G���� [enemy.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _ENEMY_H_		//���̃}�N����`������Ă��Ȃ�������
#define _ENEMY_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define MAX_ENEMY	(128)	//���f���̍ő吔
#define MAX_MAT		(128)	//�}�e���A���T�C�Y

#include "main.h"

//���f���̎��
typedef enum
{
	MODELTYPE_CAR = 0,	//��
	MODELTYPE_SHIELD,	//��
	MODELTYPE_WAGON,	//���S��
	MODELTYPE_HEAD,		//��
	MODELTYPE_MAX
}MODELTYPE;

typedef enum
{
	ENEMYSTATE_NONE = 0,	//�������Ă��Ȃ����
	ENEMYSTATE_DMG,			//�_���[�W���
	ENEMYSTATE_MAX
}ENEMYSTATE;

//�G�̍\����
typedef struct
{
	D3DXVECTOR3 Startpos;	//�ʒu
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nIdxShadow;			//�e�̃C���f�b�N�X�ԍ�
	bool bUse;				//�g�p���Ă��邩
	bool bJump;				//�W�����v���Ă��邩
	int state;				//���
	int nLife;				//�̗�
	int nType;				//�ǂݍ��ރ��f���̎��
	int nCntState;			//��ԃJ�E���^�[
	int nATKInterval;		//�U���̊Ԋu
	int nCntATK;			//�U���̊Ԋu�J�E���g
	LPDIRECT3DTEXTURE9 pTexture[30];	//�e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh;		//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;	//�}�e���A���ւ̃|�C���^
	D3DXMATERIAL pMatData[MAX_MAT];	//�}�e���A���̃f�[�^
	DWORD dwNumMat;			//�}�e���A���̐�

	int nNumVtx;			//���_��
	DWORD dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			//���_�o�b�t�@�̃|�C���^
	D3DXVECTOR3 vtxMin;		//���f���̍ŏ��l
	D3DXVECTOR3	vtxMax;		//���f���̍ő�l
}Enemy;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife);
void HitEnemy(int nCntEnemy, D3DXVECTOR3 move, int nDMG);

#endif