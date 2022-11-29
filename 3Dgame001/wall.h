//=============================================================================
//
// �Ǐ��� [wall.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _WALL_H_		//���̃}�N����`������Ă��Ȃ�������
#define _WALL_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�|���S���\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p���Ă��邩
}WALL;

//�v���g�^�C�v�錾
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
WALL *GetWall(void);

#endif