//=============================================================================
//
// ���f������ [model.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MODEL_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MODEL_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define MAX_MODEL	(128)	//���f���̍ő吔

typedef struct
{
	D3DXVECTOR3 pos;		//���݂̈ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 move;		//�ړ���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nIdxShadow;			//�e�̃C���f�b�N�X�ԍ�

	int nNumVtx;			//���_��
	DWORD dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			//���_�o�b�t�@�̃|�C���^
	D3DXVECTOR3 vtxMin;		//���f���̍ŏ��l
	D3DXVECTOR3	vtxMax;		//���f���̍ő�l
}Model;

//�v���g�^�C�v�錾
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model GetModel(void);

#endif