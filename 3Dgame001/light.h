//=============================================================================
//
// ���C�g���� [light.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _LIGHT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _LIGHT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define MAX_LIGHT	(3)	//���C�g�̍ő吔

//�v���g�^�C�v�錾
void InitLight(void);
void UninitLight(void);
void UpdateLight(void);
D3DLIGHT9 *GetLight(void);
D3DXVECTOR3 *GetLightDir(void);

#endif