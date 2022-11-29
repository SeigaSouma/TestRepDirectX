//=============================================================================
//
// ���b�V���h�[������ [meshdome.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MESHDOME_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MESHDOME_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//���b�V���h�[���\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nNumIndex;			//�C���f�b�N�X��
	int nNumIndexLid;		//�W�̃C���f�b�N�X��
	int nNumVertex;			//���_��
}MESHDOME;

//�v���g�^�C�v�錾
void InitMeshDome(void);
void UninitMeshDome(void);
void UpdateMeshDome(void);
void DrawMeshDome(void);

#endif