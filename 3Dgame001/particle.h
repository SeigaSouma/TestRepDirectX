//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _PARTICLE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _PARTICLE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�񋓌^��`
typedef enum
{
	PARTICLE_TYPE_SMOKE = 0,	//���̃p�[�e�B�N��
	PARTICLE_TYPE_SMOKE_RED,	//���̃p�[�e�B�N����
	PARTICLE_TYPE_SMOKE_YEL,	//���̃p�[�e�B�N����
	PARTICLE_TYPE_EXPLOSION,	//�����̃p�[�e�B�N��
	PARTICLE_TYPE_FIRE,			//���M�̉��p�[�e�B�N��
	PARTICLE_TYPE_FIRE_YEL,		//���M�̉��p�[�e�B�N��
	PARTICLE_TYPE_HEAT,			//���x�㏸�̃p�[�e�B�N��
	PARTICLE_TYPE_COALBULLET,	//���x�㏸�̃p�[�e�B�N��
	PARTICLE_TYPE_COOL,			//���x�����̃p�[�e�B�N��
	PARTICLE_TYPE_OFFSETTING,	//�ł������̃p�[�e�B�N��
	PARTICLE_TYPE_LAND,			//���n�̃p�[�e�B�N��
	PARTICLE_TYPE_WALK,			//�ړ��̃p�[�e�B�N��
	PARTICLE_TYPE_DEAD,			//���S���̃p�[�e�B�N��
}PARTICLE_TYPE;

//�p�[�e�B�N���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col;		//�F
	float fRadius;		//���a
	int nLife;			//����
	int nMaxLife;		//�ő����
	int nType;			//���
	bool bUse;			//�g�p���Ă��邩�ǂ���
}Particle;

//�v���g�^�C�v�錾
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, int nType);
int GetParticleNum(void);

#endif