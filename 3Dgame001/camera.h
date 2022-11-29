//=============================================================================
//
// �J�������� [camera.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _CAMERA_H_		//���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�J�����̍\����
typedef struct
{
	D3DXVECTOR3 posV;		//���_(�J�����̈ʒu)
	D3DXVECTOR3 posR;		//�����_(�������ꏊ)
	D3DXVECTOR3 posVDest;	//�ڕW�̎��_
	D3DXVECTOR3 posRDest;	//�ڕW�̒����_
	D3DXVECTOR3 vecU;		//������x�N�g��
	D3DXMATRIX	mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	mtxView;	//�r���[�}�g���b�N�X
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 rotVDest;	//�ڕW�̎��_�̌���
	float rotVDiff;			//�ڕW�̎��_�̍���
	float fDistance;		//����
	int nCntBackFollow;		//�w�ʕ␳����܂ł̎���
	bool bBackFollow;		//�w�ʒǏ]���邩�ǂ���
	bool bFollow;			//�Ǐ]���邩�ǂ���
}Camera;

//�v���g�^�C�v�錾
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera GetCamera(void);
void SetBackFollowCamera(void);

#endif