//=============================================================================
//
// �v���C���[���� [player.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "model.h"
#include "bullet.h"
#include "particle.h"
#include "enemy.h"

//�}�N����`
#define MOVE		(1.0f)
#define MAX_TX		(30)
#define DMG_TIME	(10)	//�_���[�W��Ԃ̎���

//�v���g�^�C�v�錾
void ControllPlayer(void);
void ReadSetPlayer(void);
void ReadSetMotionPlayer(void);
void UpdateMotionPlayer(void);
void LimitPosPlayer(void);
void CollisionEnemyPlayer(void);
void UpdateStatePlayer(void);

//�O���[�o���ϐ��錾
const char *c_apModelnamePlayer[] =					//���f���f�[�^�ǂݍ���
{
	"data\\MODEL\\00_body.x",		//��
	"data\\MODEL\\01_head.x",		//��
	"data\\MODEL\\02_ninoudeR.x",	//�E��̘r
	"data\\MODEL\\03_udeR.x",		//�E�r
	"data\\MODEL\\04_ThehandR.x",	//�E��
	"data\\MODEL\\05_ninoudeL.x",	//�E����
	"data\\MODEL\\06_udeL.x",		//�E�ӂ���͂�
	"data\\MODEL\\07_ThehandL.x",	//�E����
	"data\\MODEL\\08_futomomoR.x",	//�E��
	"data\\MODEL\\09_fukuraR.x",	//����̘r
	"data\\MODEL\\10_asikubiR.x",	//���r
	"data\\MODEL\\11_footR.x",		//����
	"data\\MODEL\\12_futomomoL.x",	//������
	"data\\MODEL\\13_fukuraL.x",	//���ӂ���͂�
	"data\\MODEL\\14_asikubiL.x",	//������
	"data\\MODEL\\15_footL.x",		//����
	"data\\MODEL\\handgun_01.x",		//�n���h�K��
};

Player g_aPlayer;				//�v���C���[�̏��
SetPartsPlayer g_aSetPartsPlayer[MAX_PLAYER];		//�O���t�@�C���p
MotionData g_aMotionData[MAX_MOTION];				//���[�V�����p�̏��

//�\���̂̓l�X�g���āA���[�V�����̍\���́A�L�[�̍\���́A�p�[�c�̍\���̂��ĕ����Ă���

//==================================================================================
//�v���C���[�̏���������
//==================================================================================
void InitPlayer(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{//���[�V�����̐��J��Ԃ�

		g_aMotionData[nCntMotion].bUse = false;	//�g�p���Ă��邩
		g_aMotionData[nCntMotion].nCntFrame = 0;//�t���[���̃J�E���g
		g_aMotionData[nCntMotion].nLoop = MOTION_LOOP_ON;	//���[�v����
		g_aMotionData[nCntMotion].nNumKey = 0;				//�L�[�̐�
		g_aMotionData[nCntMotion].nPatternKey = 0;			//���A���ڂ̃L�[��

		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{//�L�[�̐��J��Ԃ�

			g_aMotionData[nCntMotion].aKey[nCntKey].nFrame = 0;	//�Đ��t���[��

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{//�p�[�c�̐��J��Ԃ�

				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ڕW�̈ʒu
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu�̍���
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ڕW�̌���
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�����̍���
			}
			
		}

	}

	//�e�v�f������
	g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.posOld = g_aPlayer.pos;
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.nIdxShadow = -1;
	g_aPlayer.nLife = 10;
	g_aPlayer.nState = PLAYERSTATE_NONE;	//���
	g_aPlayer.nCntState = 0;				//��ԃJ�E���g
	g_aPlayer.nCntWalk = 0;
	g_aPlayer.nNowMotionNum = PLAYERMOTION_DEF;
	g_aPlayer.bUse = true;	//�g�p���Ă���󋵂ɂ���
	g_aPlayer.bJump = false;
	g_aPlayer.bATK = false;
	g_aPlayer.bUseGun = false;
	g_aPlayer.nIdxGun = -1;
	g_aPlayer.bMove = false;
	g_aPlayer.nIdxShadow = SetShadow(20.0f, 20.0f);	//�e��ݒ�
	g_aPlayer.nPartsNum = 0;
	g_aPlayer.fRadius = 50.0f;	//���a

	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		g_aSetPartsPlayer[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSetPartsPlayer[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSetPartsPlayer[nCount].nType = -1;	//NULL
		g_aSetPartsPlayer[nCount].nParent = -1;	//NULL
		g_aSetPartsPlayer[nCount].nWeapon = 0;	//���킩�ǂ���
		g_aSetPartsPlayer[nCount].bUse = false;	//�g�p���Ă��邩
		g_aSetPartsPlayer[nCount].posOrigin = g_aSetPartsPlayer[nCount].pos;
	}


	//�O���t�@�C���ǂݍ���
	ReadSetPlayer();
	ReadSetMotionPlayer();
}

//==================================================================================
//�v���C���[�̏I������
//==================================================================================
void UninitPlayer(void)
{


	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//�e�N�X�`���̔j��
			if (g_aSetPartsPlayer[nCount].pTexture[nCntTex] != NULL)
			{
				g_aSetPartsPlayer[nCount].pTexture[nCntTex]->Release();
				g_aSetPartsPlayer[nCount].pTexture[nCntTex] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_aSetPartsPlayer[nCount].pMesh != NULL)
		{
			g_aSetPartsPlayer[nCount].pMesh->Release();
			g_aSetPartsPlayer[nCount].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aSetPartsPlayer[nCount].pBuffMat != NULL)
		{
			g_aSetPartsPlayer[nCount].pBuffMat->Release();
			g_aSetPartsPlayer[nCount].pBuffMat = NULL;
		}
	}

}

//==================================================================================
//�v���C���[�̍X�V����
//==================================================================================
void UpdatePlayer(void)
{

	if (g_aPlayer.bUse == true)
	{//�g�p���Ă�����

		g_aPlayer.posOld = g_aPlayer.pos;

		//�v���C���[�̑���
		ControllPlayer();

		//���W����
		LimitPosPlayer();
			
		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//�ړ����Ă�Ƃ�

			g_aPlayer.nCntWalk++;	//�J�E���g���Z

			if (g_aPlayer.nCntWalk > 5)
			{
				//�ړ��p�[�e�B�N��
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);

				g_aPlayer.nCntWalk = 0;
			}
			

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_WALK)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_WALK);
			}
			
		}
		else if (g_aPlayer.bJump == true)
		{//�W�����v���Ă���

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_JUMP)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_JUMP);
			}
		}
		else
		{//�j���[�g�������[�V����

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_DEF && g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_LAND && g_aPlayer.nNowMotionNum != PLAYERMOTION_SHOT)
			{

				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DEF);

			}
		}

		//���[�V�����̍X�V
		UpdateMotionPlayer();

		//�e�̈ʒu�X�V
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);

		//�G�Ƃ̓����蔻��
		CollisionEnemyPlayer();

		//��ԍX�V
		UpdateStatePlayer();

	}

}

//==================================================================================
//���f���̑���
//==================================================================================
void ControllPlayer(void)
{
	//�J�����̏��擾
	Camera aCamera = GetCamera();

	float fRotMove = g_aPlayer.rot.y;	//���݂̊p�x
	float fRotDest = g_aPlayer.rot.y;	//�ڕW�̊p�x
	float fRotDiff = 0.0f;				//�␳

	if (aCamera.bFollow == true)
	{//�Ǐ]��ON�̎�����

		if (GetKeyboardPress(DIK_A) == true || XGetStickPressR(BUTTON_LX, 0) < 0)
		{//���L�[�������ꂽ,���ړ�

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
			{//A+W,����ړ�

				g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
				fRotDest = D3DX_PI * MOVE_LRUP + aCamera.rot.y;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
			{//A+S,�����ړ�

				g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
				fRotDest = D3DX_PI * MOVE_LRDW + aCamera.rot.y;
			}
			else
			{//A,���ړ�

				g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
				fRotDest = D3DX_PI * MOVE_LR + aCamera.rot.y;
			}

			g_aPlayer.bMove = true;
		}
		else if (GetKeyboardPress(DIK_D) == true || XGetStickPressR(BUTTON_LX, 0) > 0)
		{//D�L�[�������ꂽ,�E�ړ�

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
			{//D+W,�E��ړ�

				g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
				fRotDest = -D3DX_PI * MOVE_LRUP + aCamera.rot.y;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
			{//D+S,�E���ړ�

				g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
				fRotDest = -D3DX_PI * MOVE_LRDW + aCamera.rot.y;
			}
			else
			{//D,�E�ړ�

				g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
				g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
				fRotDest = -D3DX_PI * MOVE_LR + aCamera.rot.y;
			}

			g_aPlayer.bMove = true;
		}
		else if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
		{//W�������ꂽ�A���ړ�

			g_aPlayer.move.x += sinf(aCamera.rot.y) * MOVE;
			g_aPlayer.move.z += cosf(aCamera.rot.y) * MOVE;
			fRotDest = D3DX_PI + aCamera.rot.y;
			g_aPlayer.bMove = true;
		}
		else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
		{//S�������ꂽ�A��O�ړ�

			g_aPlayer.move.x += sinf(D3DX_PI + aCamera.rot.y) * MOVE;
			g_aPlayer.move.z += cosf(D3DX_PI + aCamera.rot.y) * MOVE;
			fRotDest = aCamera.rot.y;
			g_aPlayer.bMove = true;
		}
		else
		{//�Ȃ�̃L�[��������Ă��Ȃ�

			if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f ||
				g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
			{
				g_aPlayer.bMove = false;
			}
		}

		if (GetKeyboardPress(DIK_LSHIFT) == true)
		{//J��]

			g_aPlayer.rot.y += D3DX_PI * 0.05f;
		}
		if (GetKeyboardPress(DIK_RSHIFT) == true)
		{//J��]

			g_aPlayer.rot.y -= D3DX_PI * 0.05f;
		}

		if (GetKeyboardPress(DIK_I) == true)
		{//I�L�[�������ꂽ,�㏸

			g_aPlayer.pos.y += MOVE;
		}
		else if (GetKeyboardPress(DIK_K) == true)
		{//K�L�[�������ꂽ,���~

			g_aPlayer.pos.y -= MOVE;
		}

		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//SPACE�������ꂽ,�W�����v

			//�n���h�K���g�p
			g_aSetPartsPlayer[g_aPlayer.nIdxGun].bUse = true;

			if (g_aPlayer.bJump == false)
			{//�W�����v���ĂȂ��Ƃ�����

				g_aPlayer.move.y += JUMP;
				g_aPlayer.bJump = true;
			}

		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//ENTER�������ꂽ,,�e����

			//�U�����
			g_aPlayer.bATK = true;

			if (g_aSetPartsPlayer[g_aPlayer.nIdxGun].bUse == false)
			{//�n���h�K���Ȃ�

				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_ACTION);
			}
			else if (g_aSetPartsPlayer[g_aPlayer.nIdxGun].bUse == true)
			{//�n���h�K������

				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_SHOT);
			}

			SetBullet(D3DXVECTOR3(
				g_aPlayer.pos.x - sinf(g_aPlayer.rot.y) * (POS_BULLET * 2.0f),
				g_aPlayer.pos.y + POS_BULLET,
				g_aPlayer.pos.z - cosf(g_aPlayer.rot.y) * (POS_BULLET * 2.0f)),	//�G�̍��W
				D3DXVECTOR3(
					sinf(D3DX_PI + g_aPlayer.rot.y),
					0.0f,
					cosf(D3DX_PI + g_aPlayer.rot.y)),	//�ړ�����
				BULLETTYPE_PLAYER);	//�e�̎��

		}

	}

	//���݂ƖڕW�̍��������߂�
	fRotDiff = fRotDest - fRotMove;

	//�p�x�̐��K��
	fRotDiff = RotNormalize(fRotDiff);

	//�p�x�̕␳������
	g_aPlayer.rot.y += fRotDiff * 0.1f;

	//�p�x�̐��K��
	g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);

	//�d�͏���
	g_aPlayer.move.y -= 1.0f;

	//�ʒu�X�V
	g_aPlayer.pos += g_aPlayer.move;

	//��������
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.1f;
	g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * 0.1f;

}

//==================================================================================
//�v���C���[�̍��W����
//==================================================================================
void LimitPosPlayer(void)
{
	if (g_aPlayer.pos.y < 0.0f)
	{//�n�ʂ�艺�ɍs������

		g_aPlayer.pos.y = 0.0f;

		//�d�͏���
		g_aPlayer.move.y = 0.0f;

		if (g_aPlayer.bJump == true)
		{//�W�����v����������

			//���[�V�����̐ݒ�
			SetMotisonPlayer(PLAYERMOTION_LAND);

			//���n�p�[�e�B�N��
			SetParticle(g_aPlayer.pos, PARTICLE_TYPE_LAND);
		}

		g_aPlayer.bJump = false;

	}

	//�ړ�����
	if (g_aPlayer.pos.x + g_aPlayer.fRadius <= -1000.0f && g_aPlayer.posOld.x + g_aPlayer.fRadius >= -1000.0f)
	{//����-1000�ȉ�, �O��-1000�ȏ�

		g_aPlayer.pos.x = -1000.0f - g_aPlayer.fRadius;
	}
	if (g_aPlayer.pos.x + g_aPlayer.fRadius >= 1000.0f && g_aPlayer.posOld.x + g_aPlayer.fRadius <= 1000.0f)
	{//����1000�ȏ�, �O��1000�ȉ�

		g_aPlayer.pos.x = 1000.0f - g_aPlayer.fRadius;
	}
	if (g_aPlayer.pos.z + g_aPlayer.fRadius >= 1000.0f && g_aPlayer.posOld.z + g_aPlayer.fRadius <= 1000.0f)
	{//����1000�ȏ�, �O��1000�ȉ�

		g_aPlayer.pos.z = 1000.0f - g_aPlayer.fRadius;
	}
	if (g_aPlayer.pos.z + g_aPlayer.fRadius <= -1000.0f && g_aPlayer.posOld.z + g_aPlayer.fRadius >= -1000.0f)
	{//����-1000�ȉ�, �O��-1000�ȏ�

		g_aPlayer.pos.z = -1000.0f - g_aPlayer.fRadius;
	}
	
}

//==================================================================================
//�e�̃q�b�g����
//==================================================================================
void HitPlayer(D3DXVECTOR3 move, int nDMG)
{

	g_aPlayer.move = move;	//�ړ��ʉ��Z
	//g_aPlayer.nLife -= nDMG;	//�̗͌��炷

	if (g_aPlayer.nLife <= 0)
	{//�̗͂��Ȃ��Ȃ���

		g_aPlayer.nLife = 0;
		g_aPlayer.bUse = false;
		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���S�̃p�[�e�B�N���Z�b�g
		SetEnableShadow(g_aPlayer.nIdxShadow);	//�e����
		SetParticle(g_aPlayer.pos, PARTICLE_TYPE_DEAD);
	}
	else
	{//�܂��̗͂��c���Ă�

		g_aPlayer.nState = ENEMYSTATE_DMG;	//�_���[�W��Ԃ�
		g_aPlayer.nCntState = DMG_TIME;	//�_���[�W��Ԃ�ۂ��Ԃ�^����

		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
		{//�p�[�c���J��Ԃ�

			D3DXMATERIAL *pMat;	//�}�e���A���f�[�^�ւ̃|�C���^

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aSetPartsPlayer[nCntParts].pBuffMat->GetBufferPointer();

			//���_�����J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)g_aSetPartsPlayer[nCntParts].dwNumMat; nCntMat++)
			{//�}�e���A����Ԃ�����

				pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
		}
	}

}

//==================================================================================
//�v���C���[�̏�ԍX�V
//==================================================================================
void UpdateStatePlayer(void)
{

	switch (g_aPlayer.nState)
	{
	case ENEMYSTATE_NONE:
		break;

	case ENEMYSTATE_DMG:

		//�_���[�W�J�E���^�[���Z
		g_aPlayer.nCntState--;

		if (g_aPlayer.nCntState <= 0)
		{//�_���[�W�J�E���^�[��0�ȉ�

			//��Ԃ����Ƃɖ߂�
			g_aPlayer.nState = ENEMYSTATE_NONE;

			for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
			{//�p�[�c���J��Ԃ�

				D3DXMATERIAL *pMat;	//�}�e���A���f�[�^�ւ̃|�C���^

				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aSetPartsPlayer[nCntParts].pBuffMat->GetBufferPointer();

				//���_�����J��Ԃ�
				for (int nCntMat = 0; nCntMat < (int)g_aSetPartsPlayer[nCntParts].dwNumMat; nCntMat++)
				{
					//���̐F�ɖ߂�
					pMat[nCntMat].MatD3D.Diffuse = g_aSetPartsPlayer[nCntParts].pMatData[nCntMat].MatD3D.Diffuse;
					pMat[nCntMat].MatD3D.Ambient = g_aSetPartsPlayer[nCntParts].pMatData[nCntMat].MatD3D.Ambient;
				}
			}
		}

		break;
	}
}

//==================================================================================
//�G�Ƃ̓����蔻��
//==================================================================================
void CollisionEnemyPlayer(void)
{
	//�G�̏��擾
	Enemy *pEnemy = GetEnemy();

	float fLength = (g_aPlayer.pos.x - pEnemy->pos.x) * (g_aPlayer.pos.x - pEnemy->pos.x) +
					(g_aPlayer.pos.z - pEnemy->pos.z) * (g_aPlayer.pos.y - pEnemy->pos.z);



	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//�G���g�p����Ă�����

			//���̔���
			if (g_aPlayer.pos.z + g_aPlayer.fRadius > pEnemy->pos.z + pEnemy->vtxMin.z &&	//�G�̍ŏ��l�ɓ����Ă�
				g_aPlayer.pos.z - g_aPlayer.fRadius < pEnemy->pos.z + pEnemy->vtxMax.z)		//�G�̍ő�l�ɓ����Ă�
			{//�G��Z���̒��ɂ���Ƃ�

				if (g_aPlayer.pos.x - g_aPlayer.fRadius <= pEnemy->pos.x + pEnemy->vtxMax.x &&	//���݂̍ŏ��l���G�̍ő�l����������
					g_aPlayer.posOld.x - g_aPlayer.fRadius >= pEnemy->pos.x + pEnemy->vtxMax.x)	//�O��̍ŏ��l���G�̍ő�l�����傫��
				{//�E����

					g_aPlayer.pos.x = (pEnemy->pos.x + pEnemy->vtxMax.x) + g_aPlayer.fRadius;
				}
				if (g_aPlayer.pos.x + g_aPlayer.fRadius >= pEnemy->pos.x + pEnemy->vtxMin.x &&	//���݂̍ő�l���G�̍ŏ��l�����傫��
					g_aPlayer.posOld.x + g_aPlayer.fRadius <= pEnemy->pos.x + pEnemy->vtxMin.x)	//�O��̍ő�l���G�̍ŏ��l����������
				{//������

					g_aPlayer.pos.x = (pEnemy->pos.x + pEnemy->vtxMin.x) - g_aPlayer.fRadius;
				}

			}


			//�c�̔���
			if (g_aPlayer.pos.x + g_aPlayer.fRadius > pEnemy->pos.x + pEnemy->vtxMin.x &&	//�G�̍ŏ��l�ɓ����Ă�
				g_aPlayer.pos.x - g_aPlayer.fRadius < pEnemy->pos.x + pEnemy->vtxMax.x)		//�G�̍ő�l�ɓ����Ă�
			{//�G��X���̒��ɂ���Ƃ�

				if (g_aPlayer.pos.z - g_aPlayer.fRadius <= pEnemy->pos.z + pEnemy->vtxMax.z &&	//���݂̍ŏ��l���G�̍ő�l����������
					g_aPlayer.posOld.z - g_aPlayer.fRadius >= pEnemy->pos.z + pEnemy->vtxMax.z)	//�O��̍ŏ��l���G�̍ő�l�����傫��
				{//�G�̈ʒu�ɕ␳

					g_aPlayer.pos.z = (pEnemy->pos.z + pEnemy->vtxMax.z) + g_aPlayer.fRadius;
				}
				if (g_aPlayer.pos.z + g_aPlayer.fRadius >= pEnemy->pos.z + pEnemy->vtxMin.z &&	//���݂̍ő�l���G�̍ŏ��l�����傫��
					g_aPlayer.posOld.z + g_aPlayer.fRadius <= pEnemy->pos.z + pEnemy->vtxMin.z)	//�O��̍ő�l���G�̍ŏ��l����������
				{//�G�̈ʒu�ɕ␳

					g_aPlayer.pos.z = (pEnemy->pos.z + pEnemy->vtxMin.z) - g_aPlayer.fRadius;
				}

			}

		}

	}


}

//==================================================================================
//�v���C���[�̃��[�V��������
//==================================================================================
void SetMotisonPlayer(int nMotionType)
{

	if (g_aPlayer.nNowMotionNum != nMotionType)
	{

		for (int nCntKey = 0; nCntKey < g_aMotionData[nMotionType].nNumKey; nCntKey++)
		{//�L�[�����J��Ԃ�

			for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
			{//�p�[�c���J��Ԃ�

				g_aSetPartsPlayer[nCntParts].rot = g_aMotionData[nMotionType].aKey[nCntKey].aParts[nCntParts].rot;
				g_aSetPartsPlayer[nCntParts].pos = g_aSetPartsPlayer[nCntParts].posOrigin;

			}

			g_aMotionData[nMotionType].nCntFrame = 0;	//�t���[���J�E���g���Z�b�g
		}

		//���̃��[�V�����̍X�V
		g_aPlayer.nNowMotionNum = nMotionType;

		g_aMotionData[nMotionType].nPatternKey = 0;	//���݂̃L�[�ԍ����Z�b�g
	}


	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

		//���̌����ƌ��݂̌����ŖڕW�̌��������߂�
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDest = g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rot;
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].posDest = g_aSetPartsPlayer[nCntParts].posOrigin + g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].pos;

		//���݂ƖڕW�̍��������߂�
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff = g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDest - g_aSetPartsPlayer[nCntParts].rot;
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].posDiff = g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].posDest - g_aSetPartsPlayer[nCntParts].pos;

		//�p�x�̐��K��
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.x = RotNormalize(g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.x);
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.y = RotNormalize(g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.y);
		g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.z = RotNormalize(g_aMotionData[nMotionType].aKey[g_aMotionData[nMotionType].nPatternKey].aParts[nCntParts].rotDiff.z);

	}

}

//==================================================================================
//�v���C���[�̃��[�V�����X�V����
//==================================================================================
void UpdateMotionPlayer(void)
{
	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

		//���������߂ċK��t���[���ŏI���悤�ɕ␳
		g_aSetPartsPlayer[nCntParts].rot +=
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey].aParts[nCntParts].rotDiff /	//�ړI�ƌ��݂̍���
			(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey].nFrame;					//�Đ��t���[���Ŋ���

		//�ʒu
		g_aSetPartsPlayer[nCntParts].pos +=
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey].aParts[nCntParts].posDiff /	//�ړI�ƌ��݂̍���
			(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey].nFrame;					//�Đ��t���[���Ŋ���

		//�p�x�̐��K��
		g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.x = RotNormalize(g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.x);
		g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.y = RotNormalize(g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.y);
		g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.z = RotNormalize(g_aSetPartsPlayer[g_aPlayer.nNowMotionNum].rot.z);

	}

	//�t���[���̃J�E���g�����Z
	g_aMotionData[g_aPlayer.nNowMotionNum].nCntFrame++;

	if (g_aMotionData[g_aPlayer.nNowMotionNum].nCntFrame >= g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey].nFrame)
	{//�t���[���̃J�E���g���t���[�����ɒB������

		//�t���[���̃J�E���g���[���ɖ߂�
		g_aMotionData[g_aPlayer.nNowMotionNum].nCntFrame = 0;

		//�p�^�[��NO.�X�V
		g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey = (g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey + 1) % g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey;

		if (g_aMotionData[g_aPlayer.nNowMotionNum].nPatternKey == 0)
		{//�L�[���[���̎�

			g_aPlayer.bATK = false;

			if (g_aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{

				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
			else
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(g_aPlayer.nNowMotionNum);
			}

		}
		else
		{
			//���[�V�����̐ݒ�
			SetMotisonPlayer(g_aPlayer.nNowMotionNum);
		}

	}

}

//==================================================================================
//�v���C���[�̕`�揈��
//==================================================================================
void DrawPlayer(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxnParent;			//�e�̌v�Z�p�}�g���b�N�X

	if (g_aPlayer.bUse == true)
	{//�g�p���Ă�����

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

		//�����𔽉f����
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

		//�ʒu�𔽉f����
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.mtxWorld);

		for (int nCount = 0; nCount < g_aPlayer.nPartsNum; nCount++)
		{

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aSetPartsPlayer[nCount].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSetPartsPlayer[nCount].rot.y, g_aSetPartsPlayer[nCount].rot.x, g_aSetPartsPlayer[nCount].rot.z);
			D3DXMatrixMultiply(&g_aSetPartsPlayer[nCount].mtxWorld, &g_aSetPartsPlayer[nCount].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aSetPartsPlayer[nCount].pos.x, g_aSetPartsPlayer[nCount].pos.y, g_aSetPartsPlayer[nCount].pos.z);
			D3DXMatrixMultiply(&g_aSetPartsPlayer[nCount].mtxWorld, &g_aSetPartsPlayer[nCount].mtxWorld, &mtxTrans);


			if (g_aSetPartsPlayer[nCount].nParent == -1)
			{//NULL�̏ꍇ,�e

				//�v���C���[�̌����𔽉f����
				mtxnParent = g_aPlayer.mtxWorld;
			}
			else
			{//�p�[�c

				//�e�̌����𔽉f����
				mtxnParent = g_aSetPartsPlayer[g_aSetPartsPlayer[nCount].nParent].mtxWorld;
			}

			//�����̂�ɐe�̃��[���h�}�g���b�N�X���|���Z
			D3DXMatrixMultiply(&g_aSetPartsPlayer[nCount].mtxWorld, &g_aSetPartsPlayer[nCount].mtxWorld, &mtxnParent);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aSetPartsPlayer[nCount].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aSetPartsPlayer[nCount].pBuffMat->GetBufferPointer();

			if (g_aSetPartsPlayer[nCount].bUse == true)
			{//�p�[�c���g�p����Ă�����

				//���_�����J��Ԃ�
				for (int nCntMat = 0; nCntMat < (int)g_aSetPartsPlayer[nCount].dwNumMat; nCntMat++)
				{

					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_aSetPartsPlayer[nCount].pTexture[nCntMat]);

					//�v���C���[(�p�[�c)�̕`��
					g_aSetPartsPlayer[nCount].pMesh->DrawSubset(nCntMat);

				}

			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}

}

//==================================================================================
//�v���C���[�̏��擾
//==================================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer;
}

//==================================================================================
//�v���C���[�̐ݒ菈��
//==================================================================================
void SetPlayer(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(c_apModelnamePlayer[nCntParts],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aSetPartsPlayer[nCntParts].pBuffMat,
			NULL,
			&g_aSetPartsPlayer[nCntParts].dwNumMat,
			&g_aSetPartsPlayer[nCntParts].pMesh);

		D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aSetPartsPlayer[nCntParts].pBuffMat->GetBufferPointer();

		//���_�����J��Ԃ�
		for (int nCntMat = 0; nCntMat < (int)g_aSetPartsPlayer[nCntParts].dwNumMat; nCntMat++)
		{
			//�}�e���A���̃f�[�^�擾
			g_aSetPartsPlayer[nCntParts].pMatData[nCntMat] = pMat[nCntMat];

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�t�@�C�������݂���

				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aSetPartsPlayer[nCntParts].pTexture[nCntMat]);

			}
		}

		if (g_aSetPartsPlayer[nCntParts].nWeapon != 0)
		{//����̎d�l���肪0�ȊO�Ȃ�

			//�p�[�c�͎g�p���Ȃ�
			g_aSetPartsPlayer[nCntParts].bUse = false;

			//�v�f�ԍ��ۑ�
			g_aPlayer.nIdxGun = nCntParts;
		}
		else
		{//����̎d�l���肪0

			//�p�[�c���g�p���
			g_aSetPartsPlayer[nCntParts].bUse = true;
		}

	}
}

//==================================================================================
//�v���C���[�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetPlayer(void)
{
	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen("data\\TEXT\\player_set.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		char aComment[MAX_COMMENT];	//�R�����g�A�m�F��

		int nCntSetParts = 0;

		while (1)
		{//END_SCRIPT������܂ŌJ��Ԃ�

			//������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			if (strcmp(aComment, "NUM_MODEL") == 0)
			{//���f���̐��ǂݍ���

				//������̓ǂݍ���
				fscanf(pFile, "%s", &aComment[0]);	//=�̕�
				fscanf(pFile, "%d", &g_aPlayer.nPartsNum);	//�p�[�c��

				/*for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCount++)
				{*/

			}


			//�e�p�[�c�̐ݒ�
			if (strcmp(aComment, "CHARACTERSET") == 0)
			{//�L�����N�^�[���̓ǂݍ��݂��J�n

				while (strcmp(aComment, "END_CHARACTERSET") != 0)
				{//END_CHARACTERSET������܂ŌJ��Ԃ�

					fscanf(pFile, "%s", &aComment[0]);	//�m�F����

					if (strcmp(aComment, "PARTSSET") == 0)
					{//PARTSSET�Ńp�[�c���̓ǂݍ��݊J�n

						while (strcmp(aComment, "END_PARTSSET") != 0)
						{//END_PARTSSET������܂ŌJ��Ԃ�

							fscanf(pFile, "%s", &aComment[0]);	//�m�F����

							if (strcmp(aComment, "INDEX") == 0)
							{//INDEX�������烂�f���ԍ��ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%d", &g_aSetPartsPlayer[nCntSetParts].nType);	//���f����ނ̗�

							}

							if (strcmp(aComment, "PARENT") == 0)
							{//PARENT��������e�̔ԍ��ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%d", &g_aSetPartsPlayer[nCntSetParts].nParent);	//�e�̔ԍ�

							}

							if (strcmp(aComment, "POS") == 0)
							{//POS��������ʒu�ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].pos.x);	//X���W
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].pos.y);	//Y���W
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].pos.z);	//Z���W

								//���̈ʒu���蓖��
								g_aSetPartsPlayer[nCntSetParts].posOrigin = g_aSetPartsPlayer[nCntSetParts].pos;

							}

							if (strcmp(aComment, "ROT") == 0)
							{//ROT������������ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].rot.x);	//X�̊p�x
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].rot.y);	//Y�̊p�x
								fscanf(pFile, "%f", &g_aSetPartsPlayer[nCntSetParts].rot.z);	//Z�̊p�x

							}

							if (strcmp(aComment, "WEAPON") == 0)
							{//WEAPON�������畐�킩�ǂ����ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%d", &g_aSetPartsPlayer[nCntSetParts].nWeapon);	//���킩�ǂ���

							}

						}//END_PARTSSET�̂�����

						nCntSetParts++;	//�p�[�c�̃J�E���g�����Z
					}

				}//END_CHARACTERSET�̂�����

			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//�I�������Ń��[�v�𔲂���

				break;
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}

	//�v���C���[�̔z�u
	SetPlayer();

}

//==================================================================================
//�v���C���[�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetMotionPlayer(void)
{

	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

	char aComment[MAX_COMMENT];	//�R�����g�p
	int nCntMotion = 0, nCntKey = 0, nCntParts = 0;

	//�t�@�C�����J��
	pFile = fopen("data\\TEXT\\motion_set_player.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		while (1)
		{//END_SCRIPT������܂ŌJ��Ԃ�

			//������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			//==================================================
			//�e���[�V�����̐ݒ�
			//==================================================
			if (strcmp(aComment, "MOTIONSET") == 0)
			{//���[�V�������̓ǂݍ��݂��J�n

				while (strcmp(aComment, "END_MOTIONSET") != 0)
				{//END_MOTIONSET������܂ŌJ��Ԃ�

					fscanf(pFile, "%s", &aComment[0]);	//�m�F����

					if (strcmp(aComment, "LOOP") == 0)
					{//LOOP�������烋�[�vON/OFF�ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aMotionData[nCntMotion].nLoop);	//���[�v0��1

					}

					if (strcmp(aComment, "NUM_KEY") == 0)
					{//NUM_KEY��������L�[���ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aMotionData[nCntMotion].nNumKey);	//�L�[��

					}

					//==================================================
					//�e�L�[�̐ݒ�
					//==================================================
					if (strcmp(aComment, "KEYSET") == 0)
					{//KEYSET�ŃL�[���̓ǂݍ��݊J�n

						while (strcmp(aComment, "END_KEYSET") != 0)
						{//END_KEYSET������܂ŌJ��Ԃ�

							fscanf(pFile, "%s", &aComment[0]);	//�m�F����

							if (strcmp(aComment, "FRAME") == 0)
							{//FRAME��������Đ��t���[�����ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%d", &g_aMotionData[nCntMotion].aKey[nCntKey].nFrame);	//�Đ��t���[��

							}

							//==================================================
							//�p�[�c�̐ݒ�
							//==================================================
							if (strcmp(aComment, "PARTS") == 0)
							{//PARTS�Ńp�[�c���̓ǂݍ��݊J�n

								while (strcmp(aComment, "END_PARTS") != 0)
								{//END_PARTS������܂ŌJ��Ԃ�

									fscanf(pFile, "%s", &aComment[0]);	//�m�F����

									if (strcmp(aComment, "POS") == 0)
									{//POS��������ʒu�ǂݍ���

										fscanf(pFile, "%s", &aComment[0]);		//=�̕�
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	//X���W
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	//Y���W
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	//Z���W
									}

									if (strcmp(aComment, "ROT") == 0)
									{//ROT��������p�x�ǂݍ���

										fscanf(pFile, "%s", &aComment[0]);		//=�̕�
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	//X�p�x
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	//Y�p�x
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	//Z�p�x
									}

								}//END_PARTS�̂�����

								nCntParts++;	//�p�[�c�̃J�E���g���Z
							}


						}//END_KEYSET�̂�����

						nCntParts = 0;	//�p�[�c�̃J�E���g���Z�b�g
						nCntKey++;	//�L�[�̃J�E���g���Z
					}

				}//END_MOTIONSET�̂�����

				nCntKey = 0;	//�L�[�̃J�E���g���Z�b�g
				nCntMotion++;	//���[�V�����̃J�E���g���Z

			}


			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//�I�������Ń��[�v�𔲂���

				break;
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
}
