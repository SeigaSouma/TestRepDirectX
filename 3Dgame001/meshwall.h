//=============================================================================
//
// ���b�V���E�H�[������ [meshwall.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MESHWALL_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MESHWALL_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_WALL	(128)	//�ǂ̍ő吔

//�}�N����`
#define POS_MESHWALL	(250.0f)


//���b�V���t�B�[���h�\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nNumIndex;			//�C���f�b�N�X��
	int nNumVertex;			//���_��
	bool bUse;				//�g�p���Ă��邩
	int nWidth;				//��������
	int nHeight;			//�c������
}MESHWALL;

//�v���g�^�C�v�錾
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Width, int Height);
MESHWALL *GetMeshWall(void);
LPDIRECT3DVERTEXBUFFER9 GetBuffMeshWall(void);

#endif