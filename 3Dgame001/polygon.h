//=============================================================================
//
// �|���S������ [polygon.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _POLYGON_H_		//���̃}�N����`������Ă��Ȃ�������
#define _POLYGON_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�|���S���\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
}POLYGON;

//�v���g�^�C�v�錾
void InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);

#endif