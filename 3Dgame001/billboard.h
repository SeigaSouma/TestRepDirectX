//=============================================================================
//
// �r���{�[�h���� [billboard.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _BILLBOARD_H_		//���̃}�N����`������Ă��Ȃ�������
#define _BILLBOARD_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_BILLBOARD	(128)	//�r���{�[�h�̍ő吔

//�|���S���\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p���Ă��邩
}Billboard;

//�v���g�^�C�v�錾
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos);
bool GetAlphaTest(void);
bool GetZTest(void);

#endif