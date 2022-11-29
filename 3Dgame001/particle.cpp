//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "particle.h"
#include "input.h"
#include "effect.h"
#include "camera.h"

//�}�N����`
#define MAX_PARTICLE	(12800)	//�p�[�e�B�N���̍ő吔
#define SMOKE_LIFE		(40)	//���̎���
#define FIRE_LIFE		(20)	//�΂̎���
#define HEAT_LIFE		(20)	//���x�㏸�̎���
#define COOL_LIFE		(30)	//���x�����̎���
#define OFFSETTING_LIFE	(60)	//�ł������̎���

//�v���g�^�C�v�錾
void SmokeParticle(int nCntParticle);
void SmokeParticleRed(int nCntParticle);
void SmokeParticleYel(int nCntParticle);
void FireParticle(int nCntParticle, int nAngle);
void FireParticleYellow(int nCntParticle, int nAngle);
void HeatParticle(int nCntParticle);
void CoalBulletParticle(int nCntParticle);
void CoolParticle(int nCntParticle);
void OffSettingParticle(int nCntParticle);
void LandParticle(int nCntParticle);
void WalkParticle(int nCntParticle);
void DeadParticle(int nCntParticle);

//�O���[�o���ϐ��錾
Particle g_aParticle[MAX_PARTICLE];			//�p�[�e�B�N���̏��
D3DXCOLOR g_Smokecol[MAX_PARTICLE];			//���̐F
int g_nParticleNum;

//==================================================================================
//�p�[�e�B�N���̏���������
//==================================================================================
void InitParticle(void)
{
	int nCntParticle;

	//�p�[�e�B�N���̏��̏�����
	for (nCntParticle =0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
	}
	g_nParticleNum = 0;
}

//==================================================================================
//�p�[�e�B�N���̏I������
//==================================================================================
void UninitParticle(void)
{
	
}

//==================================================================================
//�p�[�e�B�N���̍X�V����
//==================================================================================
void UpdateParticle(void)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{

		if (g_aParticle[nCntParticle].bUse == true)
		{//�p�[�e�B�N�����g�p����Ă�����

			//�����̍X�V
			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
				g_nParticleNum--;
			}

		}

	}
	
}

//==================================================================================
//�G�t�F�N�g�̕`�揈����i�߂�
//==================================================================================
void MoveParticle(float fMove)
{

	for (int nCntEffect = 0; nCntEffect < MAX_PARTICLE; nCntEffect++)
	{

		if (g_aParticle[nCntEffect].bUse == true)
		{//�u���b�N���g�p����Ă�����

			g_aParticle[nCntEffect].pos.x -= fMove;
		}

	}

}

//==================================================================================
//�p�[�e�B�N���̕`�揈��
//==================================================================================
void DrawParticle(void)
{
	
}

//==================================================================================
//�p�[�e�B�N���̐ݒ菈��
//==================================================================================
void SetParticle(D3DXVECTOR3 pos, int nType)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//�p�[�e�B�N�����g�p����Ă��Ȃ�������

			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].nType = nType;
			g_aParticle[nCntParticle].bUse = true;


			switch (nType)
			{
			case PARTICLE_TYPE_EXPLOSION:
				g_aParticle[nCntParticle].nLife = SMOKE_LIFE;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				SmokeParticle(nCntParticle);
				SmokeParticleRed(nCntParticle);
				SmokeParticleYel(nCntParticle);
				break;

			case PARTICLE_TYPE_SMOKE:

				g_aParticle[nCntParticle].nLife = 30;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				SmokeParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_SMOKE_RED:

				g_aParticle[nCntParticle].nLife = SMOKE_LIFE;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				SmokeParticleRed(nCntParticle);
				break;


			case PARTICLE_TYPE_SMOKE_YEL:

				g_aParticle[nCntParticle].nLife = SMOKE_LIFE;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				SmokeParticleYel(nCntParticle);
				break;
					

			case PARTICLE_TYPE_FIRE:

				g_aParticle[nCntParticle].nLife = FIRE_LIFE;
				FireParticle(nCntParticle, 1);
				FireParticleYellow(nCntParticle, 1);
				break;
					
			case PARTICLE_TYPE_FIRE_YEL:

				g_aParticle[nCntParticle].nLife = FIRE_LIFE;
				FireParticleYellow(nCntParticle, 1);
				break;


			case PARTICLE_TYPE_HEAT:

				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aParticle[nCntParticle].nLife = HEAT_LIFE;
				HeatParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_COALBULLET:
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aParticle[nCntParticle].nLife = HEAT_LIFE;
				CoalBulletParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_COOL:
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_aParticle[nCntParticle].nLife = COOL_LIFE;
				CoolParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_OFFSETTING:

				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				OffSettingParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_LAND:
				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				LandParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_WALK:
				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				WalkParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_DEAD:
				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				DeadParticle(nCntParticle);
				break;

			}

			//�p�[�e�B�N���̐����Z
			g_nParticleNum++;

			g_aParticle[nCntParticle].nMaxLife = g_aParticle[nCntParticle].nLife;
			break;
		}

		
	}
}

//==================================================================================
//���̃p�[�e�B�N��
//==================================================================================
void SmokeParticle(int nCntParticle)
{

	float fAngle, fMove;

	//�J�����̏��擾
	Camera aCamera = GetCamera();

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 1.0f;		//�ړ���
		fAngle = (float)(rand() % 101 - 50) / 100.0f;	//�p�x

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, g_Smokecol[nCntParticle].a);
		g_Smokecol[nCntParticle].a += 0.05f;

		g_aParticle[nCntParticle].fRadius = 100.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if(rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);

	}
}



//==================================================================================
//���̃p�[�e�B�N��(��)
//==================================================================================
void SmokeParticleRed(int nCntParticle)
{
	float fAngle, fMove;

	//�J�����̏��擾
	Camera aCamera = GetCamera();

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 2.0f;		//�ړ���
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//�p�x

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.4f, 0.3f, g_Smokecol[nCntParticle].a);
		g_Smokecol[nCntParticle].a -= 0.02f;

		g_aParticle[nCntParticle].fRadius = 30.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//���̃p�[�e�B�N��(��)
//==================================================================================
void SmokeParticleYel(int nCntParticle)
{
	float fAngle, fMove;

	//�J�����̏��擾
	Camera aCamera = GetCamera();

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 2.0f;		//�ړ���
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//�p�x

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.8f, 0.5f, 0.0f, g_Smokecol[nCntParticle].a);
		g_Smokecol[nCntParticle].a -= 0.02f;

		g_aParticle[nCntParticle].fRadius = 30.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//�΂̃p�[�e�B�N��
//==================================================================================
void FireParticle(int nCntParticle, int nAngle)
{
	float fAngle, fMove;

	fMove = (float)(rand() % 10) / 1 + 20.5f;		//�ړ���
	fAngle = (float)(rand() % 41 - 20) / 100.0f;	//�p�x


	for (int nCntUse = 0; nCntUse < 3; nCntUse++)
	{

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 41 - 20) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = sinf((float)(rand() % 41 - 20) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].fRadius = 55.0f;
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.95f, 0.15f, 0.0f, 1.0f);

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//�΂̃p�[�e�B�N��(��)
//==================================================================================
void FireParticleYellow(int nCntParticle, int nAngle)
{
	float fAngle, fMove;

	fMove = (float)(rand() % 10) / 1 + 20.5f;		//�ړ���
	fAngle = (float)(rand() % 41 - 20) / 100.0f;	//�p�x

	for (int nCntUse = 0; nCntUse < 3; nCntUse++)
	{

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 41 - 20) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = sinf((float)(rand() % 41 - 20) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].fRadius = 30.0f;
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//���x�㏸�̃p�[�e�B�N��
//==================================================================================
void HeatParticle(int nCntParticle)
{
	float fAngle, fMove;

	fMove = (float)(rand() % 15) / 10 + 8.5f;		//�ړ���
	fAngle = (float)(rand() % 41 - 20) / 100.0f;	//�p�x

	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf(D3DX_PI * fAngle) * -((float)(rand() % 15) / 10 + 6.5f);
		g_aParticle[nCntParticle].move.y = cosf(D3DX_PI * fAngle) * -((float)(rand() % 15) / 10 + 2.5f);
		g_aParticle[nCntParticle].move.z = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.15f, 0.15f, 0.15f, g_Smokecol[nCntParticle].a);

		g_Smokecol[nCntParticle].a = ((float)g_aParticle[nCntParticle].nLife / (float)g_aParticle[nCntParticle].nMaxLife);

		g_aParticle[nCntParticle].fRadius = 30.0f;

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//���x�㏸�̃p�[�e�B�N��
//==================================================================================
void CoalBulletParticle(int nCntParticle)
{
	float fAngle, fMove;

	fMove = (float)(rand() % 15) / 10 + 8.5f;		//�ړ���
	fAngle = (float)(rand() % 11 - 5) / 100.0f + -1.5f;	//�p�x

	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf(D3DX_PI * fAngle) * -((float)(rand() % 5) / 10 + 0.5f);
		g_aParticle[nCntParticle].move.y = cosf(D3DX_PI * fAngle) * -((float)(rand() % 15) / 10 + 10.5f);
		g_aParticle[nCntParticle].move.z = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.15f, 0.15f, 0.15f, g_Smokecol[nCntParticle].a);

		g_Smokecol[nCntParticle].a = ((float)g_aParticle[nCntParticle].nLife / (float)g_aParticle[nCntParticle].nMaxLife);

		g_aParticle[nCntParticle].fRadius = 40.0f;

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//���x�����̃p�[�e�B�N��
//==================================================================================
void CoolParticle(int nCntParticle)
{
	float fAngle, fMove;

	fMove = (float)(rand() % 15) / 1 + 0.5f;		//�ړ���
	fAngle = (float)(rand() % 629 - 314) / 100.0f;	//�p�x

	for (int nCntUse = 0; nCntUse < 3; nCntUse++)
	{

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf(D3DX_PI * fAngle) * ((float)(rand() % 5) / 10 + 2.5f);
		g_aParticle[nCntParticle].move.y = cosf(D3DX_PI * fAngle) * ((float)(rand() % 5) / 10 + 2.5f);
		g_aParticle[nCntParticle].move.z = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.3f, 0.3f, 1.0f, g_Smokecol[nCntParticle].a);
		g_Smokecol[nCntParticle].a -= 0.025f;

		g_aParticle[nCntParticle].fRadius = 50.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_SMOKE);
	}
}

//==================================================================================
//�ł������̃p�[�e�B�N��
//==================================================================================
void OffSettingParticle(int nCntParticle)
{

	float fAngle, fMove;

	//�J�����̏��擾
	Camera aCamera = GetCamera();

	for (int nCntUse = 0; nCntUse < 20; nCntUse++)
	{

		fMove = (float)(rand() % 10) / 1 + 3.0f;		//�ړ���
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//�p�x

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_aParticle[nCntParticle].fRadius = 40.0f;

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_NORMAL);
	}
}

//==================================================================================
// ���n�̃p�[�e�B�N��
//==================================================================================
void LandParticle(int nCntParticle)
{
	float fMove;

	for (int nCntUse = 0; nCntUse < 20; nCntUse++)
	{
		fMove = (float)(rand() % 61 - 30) / 10 + 1.0f;		//�ړ���

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

		g_aParticle[nCntParticle].fRadius = 10.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
// �ړ��̃p�[�e�B�N��
//==================================================================================
void WalkParticle(int nCntParticle)
{
	float fMove;

	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 1.0f;		//�ړ���

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

		g_aParticle[nCntParticle].fRadius = 5.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
// ���S�̃p�[�e�B�N��
//==================================================================================
void DeadParticle(int nCntParticle)
{
	float fMove;

	for (int nCntUse = 0; nCntUse < 50; nCntUse++)
	{
		fMove = (float)(rand() % 30) / 10 + 1.0f;		//�ړ���

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.1f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);

		g_aParticle[nCntParticle].fRadius = 120.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_NORMAL);
	}

}

//==================================================================================
//�p�[�e�B�N�����擾
//==================================================================================
int GetParticleNum(void)
{
	return g_nParticleNum;
}
