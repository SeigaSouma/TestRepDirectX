//=============================================================================
//
// �������� [explosion.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _EXPLOSION_H_		//���̃}�N����`������Ă��Ȃ�������
#define _EXPLOSION_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�񋓌^��`
#define MAX_EXPLOSION	(128)	//�����̍ő吔
#define POS_EXPLOSION	(20.0f)	//�����̑傫��

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR col;			//�F
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p���Ă��邩�ǂ���
	int nPatternAnim;		//�A�j���[�V�����p�^�[��No.
	int nCntAnim;			//�A�j���[�V�����J�E���^�[
	int nLife;				//����
	int nMaxLife;			//�ő����
}Explosion;

//�v���g�^�C�v�錾
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos);
Explosion *GetExplosion(void);

#endif