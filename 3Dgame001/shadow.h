//=============================================================================
//
// �e���� [shadow.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _SHADOW_H_		//���̃}�N����`������Ă��Ȃ�������
#define _SHADOW_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_SHADOW	(128)	//�e�̍ő吔

//�|���S���\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p���Ă��邩
}SHADOW;

//�v���g�^�C�v�錾
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(float fWidth, float fHeight);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void SetEnableShadow(int nIdxShadow);

#endif