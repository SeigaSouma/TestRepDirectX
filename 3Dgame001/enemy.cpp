//=============================================================================
//
// �G���� [enemy.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "particle.h"
#include "bullet.h"

//�}�N����`
#define MOVE		(3.0f)
#define MAX_TX		(30)
#define MAX_LENGTH	(100.0f)	//���̋���
#define ROT_MOVE	(0.03f)	//��]�ړ���
#define DMG_TIME	(20)	//�_���[�W��Ԃ̎���
#define ATK_INTERVAL	(120)

//�v���g�^�C�v�錾
void ControllEnemy(int nCntEnemy);
void ReadSetEnemy(void);
void UpdateStateEnemy(int nCntEnemy);
void LimitPosEnemy(int nCntEnemy);
void CheckVtxEnemy(int nCntEnemy);
void RotFacePlayerEnemy(int nCntEnemy);
void ATKEnemy(int nCntEnemy);

//�O���[�o���ϐ��錾
const char *c_apModelname[] =					//���f���f�[�^�ǂݍ���
{
	"data\\MODEL\\car_01.x",			//��
	"data\\MODEL\\KiteShield.x",	//�V�[���h
	"data\\MODEL\\wagon_01.x",		//���S��
	"data\\MODEL\\01_head.x",		//��
};

Enemy g_aEnemy[MAX_ENEMY];				//�G�̏��

//==================================================================================
//�G�̏���������
//==================================================================================
void InitEnemy(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�v�f������
	for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
	{
		g_aEnemy[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCount].Startpos = g_aEnemy[nCount].pos;
		g_aEnemy[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCount].nIdxShadow = -1;
		g_aEnemy[nCount].bUse = false;
		g_aEnemy[nCount].state = ENEMYSTATE_NONE;		//���
		g_aEnemy[nCount].nLife = 9999;		//�̗�
		g_aEnemy[nCount].nATKInterval = 9999;		//�U���Ԋu
		g_aEnemy[nCount].nCntATK = 0;				//�U���̊Ԋu�J�E���g
		g_aEnemy[nCount].nCntState = 0;			//��ԃJ�E���^�[
		g_aEnemy[nCount].nType = -1;			//���

		g_aEnemy[nCount].pTexture[30] = {};	//�e�N�X�`���ւ̃|�C���^
		g_aEnemy[nCount].pMesh = {};		//���b�V��(���_���)�ւ̃|�C���^
		g_aEnemy[nCount].pBuffMat = {};		//�}�e���A���ւ̃|�C���^
		g_aEnemy[nCount].dwNumMat = {};			//�}�e���A���̐�

		g_aEnemy[nCount].nNumVtx = 0;	//���_��
		g_aEnemy[nCount].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_aEnemy[nCount].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aEnemy[nCount].pMatData[nCntMat] = {};		//�}�e���A���̃f�[�^
		}

	}

	//�O���t�@�C���ǂݍ���
	ReadSetEnemy();

}

//==================================================================================
//�G�̏I������
//==================================================================================
void UninitEnemy(void)
{


	for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
	{

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//�e�N�X�`���̔j��
			if (g_aEnemy[nCount].pTexture[nCntTex] != NULL)
			{
				g_aEnemy[nCount].pTexture[nCntTex]->Release();
				g_aEnemy[nCount].pTexture[nCntTex] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_aEnemy[nCount].pMesh != NULL)
		{
			g_aEnemy[nCount].pMesh->Release();
			g_aEnemy[nCount].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aEnemy[nCount].pBuffMat != NULL)
		{
			g_aEnemy[nCount].pBuffMat->Release();
			g_aEnemy[nCount].pBuffMat = NULL;
		}
	}

}

//==================================================================================
//�G�̍X�V����
//==================================================================================
void UpdateEnemy(void)
{
	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	if (GetKeyboardTrigger(DIK_F2) == true)
	{//F2�������ꂽ,�X�|�[��

		SetEnemy(pPlayer->pos, pPlayer->rot, MODELTYPE_WAGON, 3);
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{

		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�g�p���Ă�����

			//�G�̑���
			ControllEnemy(nCntEnemy);

			//�v���C���[�̕�������
			RotFacePlayerEnemy(nCntEnemy);

			//�e���ˏ���
			ATKEnemy(nCntEnemy);

			//���W����
			LimitPosEnemy(nCntEnemy);

			//��ԍX�V
			UpdateStateEnemy(nCntEnemy);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);
		}

	}

}

//==================================================================================
//�G�̑���
//==================================================================================
void ControllEnemy(int nCntEnemy)
{

	//�J�����̏��擾
	Camera aCamera = GetCamera();

	//�d�͏���
	g_aEnemy[nCntEnemy].move.y -= GRAVITY;

	//�ʒu�X�V
	g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;
	g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * 0.1f;
	g_aEnemy[nCntEnemy].move.z += (0.0f - g_aEnemy[nCntEnemy].move.z) * 0.1f;

}

//==================================================================================
//�G�̍U������
//==================================================================================
void ATKEnemy(int nCntEnemy)
{

	//�U���Ԋu�J�E���g���Z
	//g_aEnemy[nCntEnemy].nCntATK++;

	if (g_aEnemy[nCntEnemy].nCntATK >= g_aEnemy[nCntEnemy].nATKInterval)
	{//�J�E���g���K��l��������

		//�J�E���g���Z�b�g
		g_aEnemy[nCntEnemy].nCntATK = 0;

		SetBullet(D3DXVECTOR3(
			g_aEnemy[nCntEnemy].pos.x - sinf(g_aEnemy[nCntEnemy].rot.y) * (POS_BULLET * 2.0f),
			g_aEnemy[nCntEnemy].pos.y + POS_BULLET,
			g_aEnemy[nCntEnemy].pos.z - cosf(g_aEnemy[nCntEnemy].rot.y) * (POS_BULLET * 2.0f)),	//�G�̍��W
			D3DXVECTOR3(
				sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y),
				0.0f,
				cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y)),	//�ړ�����
			BULLETTYPE_ENEMY);	//�e�̎��

	}

}

//==================================================================================
//�v���C���[�̕������鏈��
//==================================================================================
void RotFacePlayerEnemy(int nCntEnemy)
{

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	float fRotMove = g_aEnemy[nCntEnemy].rot.y;	//���݂̊p�x
	float fRotDest = g_aEnemy[nCntEnemy].rot.y;	//�ڕW�̊p�x
	float fRotDiff = 0.0f;				//�␳

	//�ڕW�̊p�x�����߂�
	fRotDest = atan2f((g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x), (g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z));
	fRotDiff = fRotDest - fRotMove;

	//�p�x�̐��K��
	fRotDiff = RotNormalize(fRotDiff);

	//�p�x�̕␳������
	g_aEnemy[nCntEnemy].rot.y += fRotDiff * 0.1f;

	//�p�x�̐��K��
	g_aEnemy[nCntEnemy].rot.y = RotNormalize(g_aEnemy[nCntEnemy].rot.y);

}

//==================================================================================
//�G�̍��W����
//==================================================================================
void LimitPosEnemy(int nCntEnemy)
{

	if (g_aEnemy[nCntEnemy].pos.y < 0.0f)
	{//�n�ʂ�艺�ɍs������

		g_aEnemy[nCntEnemy].pos.y = 0.0f;

		//�d�͏���
		g_aEnemy[nCntEnemy].move.y = 0.0f;

		if (g_aEnemy[nCntEnemy].bJump == true)
		{//�W�����v����������

			//���n�p�[�e�B�N��
			SetParticle(g_aEnemy[nCntEnemy].pos, PARTICLE_TYPE_LAND);
		}

		g_aEnemy[nCntEnemy].bJump = false;

	}

	//�ړ�����
	if (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x < -1000.0f) { g_aEnemy[nCntEnemy].pos.x = -1000.0f - g_aEnemy[nCntEnemy].vtxMin.x; }
	if (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x > 1000.0f) { g_aEnemy[nCntEnemy].pos.x = 1000.0f - g_aEnemy[nCntEnemy].vtxMax.x; }
	if (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z > 1000.0f) { g_aEnemy[nCntEnemy].pos.z = 1000.0f - g_aEnemy[nCntEnemy].vtxMax.z; }
	if (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z < -1000.0f) { g_aEnemy[nCntEnemy].pos.z = -1000.0f - g_aEnemy[nCntEnemy].vtxMin.z; }

}


//==================================================================================
//�e�̃q�b�g����
//==================================================================================
void HitEnemy(int nCntEnemy, D3DXVECTOR3 move, int nDMG)
{

	D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

	g_aEnemy[nCntEnemy].move = move;	//�ړ��ʉ��Z
	g_aEnemy[nCntEnemy].nLife -= nDMG;	//�̗͌��炷

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{//�̗͂��Ȃ��Ȃ���

		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���S�̃p�[�e�B�N���Z�b�g
		SetEnableShadow(g_aEnemy[nCntEnemy].nIdxShadow);	//�e����
		SetParticle(g_aEnemy[nCntEnemy].pos, PARTICLE_TYPE_DEAD);
	}
	else
	{//�܂��̗͂��c���Ă�

		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DMG;	//�_���[�W��Ԃ�
		g_aEnemy[nCntEnemy].nCntState = DMG_TIME;	//�_���[�W��Ԃ�ۂ��Ԃ�^����

		//���_�����J��Ԃ�
		for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].dwNumMat; nCntMat++)
		{//�}�e���A����Ԃ�����

			pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

}

//==================================================================================
//�G�̏�ԍX�V����
//==================================================================================
void UpdateStateEnemy(int nCntEnemy)
{

	D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

	switch (g_aEnemy[nCntEnemy].state)
	{
	case ENEMYSTATE_NONE:
		break;

	case ENEMYSTATE_DMG:

		//�_���[�W�J�E���^�[���Z
		g_aEnemy[nCntEnemy].nCntState--;

		if (g_aEnemy[nCntEnemy].nCntState <= 0)
		{//�_���[�W�J�E���^�[��0�ȉ�

			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NONE;

			//���_�����J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].dwNumMat; nCntMat++)
			{
				//���̐F�ɖ߂�
				pMat[nCntMat].MatD3D.Diffuse = g_aEnemy[nCntEnemy].pMatData[nCntMat].MatD3D.Diffuse;
				pMat[nCntMat].MatD3D.Ambient = g_aEnemy[nCntEnemy].pMatData[nCntMat].MatD3D.Ambient;
			}

		}
		break;
	}
}

//==================================================================================
//�G�̕`�揈��
//==================================================================================
void DrawEnemy(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
	{

		if (g_aEnemy[nCount].bUse == true)
		{//�g�p���Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCount].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCount].rot.y, g_aEnemy[nCount].rot.x, g_aEnemy[nCount].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCount].mtxWorld, &g_aEnemy[nCount].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCount].pos.x, g_aEnemy[nCount].pos.y, g_aEnemy[nCount].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCount].mtxWorld, &g_aEnemy[nCount].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCount].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCount].pBuffMat->GetBufferPointer();

			//���_�����J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCount].dwNumMat; nCntMat++)
			{

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aEnemy[nCount].pTexture[nCntMat]);

				//�G(�p�[�c)�̕`��
				g_aEnemy[nCount].pMesh->DrawSubset(nCntMat);

			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);

		}

	}

}

//==================================================================================
//�G�̏��擾
//==================================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==================================================================================
//�G�̐ݒ菈��
//==================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nLife)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
	{
		if (g_aEnemy[nCount].bUse == false)
		{//�g�p���Ă��Ȃ�������

			g_aEnemy[nCount].pos = pos;	//�ʒu
			g_aEnemy[nCount].rot = rot;	//����
			g_aEnemy[nCount].nLife = nLife;	//����
			g_aEnemy[nCount].bUse = true;	//�g�p���Ă���󋵂ɂ���
			g_aEnemy[nCount].nATKInterval = ATK_INTERVAL;		//�U���Ԋu

			//X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(c_apModelname[nType],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aEnemy[nCount].pBuffMat,
				NULL,
				&g_aEnemy[nCount].dwNumMat,
				&g_aEnemy[nCount].pMesh);

			D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCount].pBuffMat->GetBufferPointer();

			//���_�����J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCount].dwNumMat; nCntMat++)
			{
				//�}�e���A���̃f�[�^�擾
				g_aEnemy[nCount].pMatData[nCntMat] = pMat[nCntMat];

				if (pMat[nCntMat].pTextureFilename != NULL)
				{//�t�@�C�������݂���

					//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_aEnemy[nCount].pTexture[nCntMat]);
				}
			}

			//�G�̑S���_�`�F�b�N
			CheckVtxEnemy(nCount);

			g_aEnemy[nCount].nIdxShadow = SetShadow(g_aEnemy[nCount].vtxMax.x, g_aEnemy[nCount].vtxMax.z);	//�e��ݒ�

			break;
		}
	}
}

//==================================================================================
//�G�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetEnemy(void)
{
	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen("data\\TEXT\\model_set.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		char aComment[MAX_COMMENT];	//�R�����g�p
		int nCntEnemy = 0;

		while (1)
		{//END_SCRIPT������܂ŌJ��Ԃ�

			//������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			//�G�̐ݒ�
			if (strcmp(aComment, "MODELSET") == 0)
			{//���f���̓ǂݍ��݂��J�n

				while (strcmp(aComment, "END_MODELSET"))
				{//END_MODELSET������܂ŌJ��Ԃ�

					fscanf(pFile, "%s", &aComment[0]);	//�m�F����

					if (strcmp(aComment, "TYPE") == 0)
					{//TYPE���������ޓǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aEnemy[nCntEnemy].nType);	//���f����ނ̗�
					}

					if (strcmp(aComment, "POS") == 0)
					{//POS��������ʒu�ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.x);	//X���W
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.y);	//Y���W
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.z);	//Z���W
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROT������������ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.x);	//X�̌���
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.y);	//Y�̌���
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.z);	//Z�̌���
					}

					if (strcmp(aComment, "LIFE") == 0)
					{//LIFE��������̗͓ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aEnemy[nCntEnemy].nLife);	//�̗�
					}

				}//END_MODELSET�̂�����

				 //�G�̔z�u
				 SetEnemy(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].nType, g_aEnemy[nCntEnemy].nLife);

				//�G�̔ԍ����Z
				nCntEnemy++;
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

//==================================================================================
//�G�̑S���_�`�F�b�N
//==================================================================================
void CheckVtxEnemy(int nCntEnemy)
{
	//���f���̒��_�����擾
	g_aEnemy[nCntEnemy].nNumVtx = g_aEnemy[nCntEnemy].pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	g_aEnemy[nCntEnemy].dwSizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMesh->GetFVF());

	//���_�o�b�t�@�����b�N
	g_aEnemy[nCntEnemy].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aEnemy[nCntEnemy].pVtxBuff);

	//�S�Ă̒��_���`�F�b�N����
	for (int nCntVtx = 0; nCntVtx < g_aEnemy[nCntEnemy].nNumVtx; nCntVtx++)
	{
		//����̃f�[�^
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aEnemy[nCntEnemy].pVtxBuff;	//���_���W�̑��

		//X
		if (g_aEnemy[nCntEnemy].vtxMin.x > vtx.x)
		{//���̍ŏ��l��������̒l����������������

			g_aEnemy[nCntEnemy].vtxMin.x = vtx.x;	//����ւ�
		}
		if (g_aEnemy[nCntEnemy].vtxMax.x < vtx.x)
		{//���̍ő�l��������̒l���傫��������

			g_aEnemy[nCntEnemy].vtxMax.x = vtx.x;	//����ւ�
		}

		//Y
		if (g_aEnemy[nCntEnemy].vtxMin.y > vtx.y)
		{//���̍ŏ��l��������̒l����������������

			g_aEnemy[nCntEnemy].vtxMin.y = vtx.y;	//����ւ�
		}
		if (g_aEnemy[nCntEnemy].vtxMax.y < vtx.y)
		{//���̍ő�l��������̒l���傫��������

			g_aEnemy[nCntEnemy].vtxMax.y = vtx.y;	//����ւ�
		}

		//Z
		if (g_aEnemy[nCntEnemy].vtxMin.z > vtx.z)
		{//���̍ŏ��l��������̒l����������������

			g_aEnemy[nCntEnemy].vtxMin.z = vtx.z;	//����ւ�
		}
		if (g_aEnemy[nCntEnemy].vtxMax.z < vtx.z)
		{//���̍ő�l��������̒l���傫��������

			g_aEnemy[nCntEnemy].vtxMax.z = vtx.z;	//����ւ�
		}

		g_aEnemy[nCntEnemy].pVtxBuff += g_aEnemy[nCntEnemy].dwSizeFVF;	//�T�C�Y���|�C���^�ړ�
	}

	//���_�o�b�t�@���A�����b�N
	g_aEnemy[nCntEnemy].pMesh->UnlockVertexBuffer();
}
