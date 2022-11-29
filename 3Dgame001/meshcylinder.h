//=============================================================================
//
// ���b�V���V�����_�[���� [meshcylinder.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MESHCYLINDER_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MESHCYLINDER_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//���b�V���t�B�[���h�\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nNumIndex;			//�C���f�b�N�X��
	int nNumVertex;			//���_��
}MESHCYLINDER;

//�v���g�^�C�v�錾
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

#endif