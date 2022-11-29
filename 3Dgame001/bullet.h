//=============================================================================
//
// �o���b�g���� [bullet.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _BULLET_H_		//���̃}�N����`������Ă��Ȃ�������
#define _BULLET_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�񋓌^��`
#define MAX_BULLET	(128)	//�e�̍ő吔
#define POS_BULLET	(20.0f)	//�e�̑傫��

//�e�̎��
typedef enum
{
	BULLETTYPE_PLAYER = 0,	//�v���C���[�̒e
	BULLETTYPE_ENEMY,		//�G�̒e
	BULLETTYPE_MAX
}BULLETTYPE;

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nIdxShadow;			//�e�̃C���f�b�N�X�ԍ�
	int nLife;				//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
	int nType;				//�e�̎��
}Bullet;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType);
Bullet *GetBullet(void);

#endif