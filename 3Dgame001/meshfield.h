//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MESHFIELD_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MESHFIELD_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//���b�V���t�B�[���h�\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nNumIndex;			//���_��
}MESHFIELD;

//�v���g�^�C�v�錾
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif