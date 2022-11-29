//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _EFFECT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _EFFECT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�C���N���[�h
#include "main.h"

//�}�N����`
#define EFFECT_LIFE		(30)

//�񋓌^��`
typedef enum
{
	MOVEEFFECT_ADD = 0,	//���Z
	MOVEEFFECT_SUB,		//���Z
	MOVEEFFECT_MAX
}MOVEEFFECT;

typedef enum
{
	EFFECTTYPE_NORMAL = 0,	//�ʏ�G�t�F�N�g
	EFFECTTYPE_SMOKE,		//���G�t�F�N�g
	EFFECTTYPE_SMOKEBLACK,	//����
	EFFECTTYPE_BLACK,		//���G�t�F�N�g
	EFFECTTYPE_MAX
}EFFECTTYPE;

//�G�t�F�N�g�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXCOLOR col;		//�F
	float fRadius;		//���a
	int nLife;			//����
	int nMaxLife;		//�ő����(�Œ�)
	int moveType;		//�ړ��̎��
	int nType;			//���
	bool bUse;			//�g�p���Ă��邩�ǂ���
	bool bAddAlpha;		//���Z�����̔���
}Effect;

//�v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType);
int GetEffectNum(void);
bool GetEffectUse(void);

#endif