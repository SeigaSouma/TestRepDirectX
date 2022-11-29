//=============================================================================
//
// ���f������ [model.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "enemy.h"

//�}�N����`
#define MOVE		(1.0f)
#define MAX_TX		(20)

//�v���g�^�C�v�錾
void ControllModel(void);
void CheckVtxModel(void);
void CollisionEnemy(void);

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureModel[MAX_TX] = {};	//�e�N�X�`���ւ̃|�C���^
LPD3DXMESH g_pMeshModel = NULL;			//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel = NULL;	//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatModel = 0;				//�}�e���A���̐�
Model g_aModel;							//���f���̏��

//==================================================================================
//���f���̏���������
//==================================================================================
void InitModel(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�v�f������
	g_aModel.pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
	g_aModel.posOld = g_aModel.pos;
	g_aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aModel.nIdxShadow = -1;

	g_aModel.nNumVtx = 0;	//���_��
	g_aModel.vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	g_aModel.vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\cube.x",
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_pBuffMatModel,
						NULL,
						&g_dwNumMatModel,
						&g_pMeshModel);

	D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	//���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{

		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�t�@�C�������݂���

			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureModel[nCntMat]);

		}

	}

	//���f���̑S���_�`�F�b�N
	CheckVtxModel();

	g_aModel.nIdxShadow = SetShadow(g_aModel.vtxMax.x, g_aModel.vtxMax.z);	//�e��ݒ�
}

//==================================================================================
//���f���̏I������
//==================================================================================
void UninitModel(void)
{

	for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
	{

		//�e�N�X�`���̔j��
		if (g_pTextureModel[nCntTex] != NULL)
		{
			g_pTextureModel[nCntTex]->Release();
			g_pTextureModel[nCntTex] = NULL;
		}
	}

	//���b�V���̔j��
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//==================================================================================
//���f���̍X�V����
//==================================================================================
void UpdateModel(void)
{
	//�O��̈ʒu���擾
	g_aModel.posOld = g_aModel.pos;

	//���f���̑���
	ControllModel();

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{//SPACE�������ꂽ,�e����

		SetBullet(D3DXVECTOR3(
			g_aModel.pos.x - sinf(g_aModel.rot.y) * (POS_BULLET * 2.0f),
			g_aModel.pos.y + POS_BULLET,
			g_aModel.pos.z - cosf(g_aModel.rot.y) * (POS_BULLET * 2.0f)),	//�G�̍��W
			D3DXVECTOR3(
				sinf(D3DX_PI + g_aModel.rot.y), 
				0.0f,
				cosf(D3DX_PI + g_aModel.rot.y)),	//�ړ�����
			BULLETTYPE_PLAYER);	//�e�̎��
	}

	//�ړ�����
	if (g_aModel.pos.x + g_aModel.vtxMin.x <= -1000.0f && g_aModel.posOld.x + g_aModel.vtxMin.x >= -1000.0f)
	{//����-1000�ȉ�, �O��-1000�ȏ�

		g_aModel.pos.x = -1000.0f - g_aModel.vtxMin.x;
	}
	if (g_aModel.pos.x + g_aModel.vtxMax.x >= 1000.0f && g_aModel.posOld.x + g_aModel.vtxMax.x <= 1000.0f)
	{//����1000�ȏ�, �O��1000�ȉ�

		g_aModel.pos.x = 1000.0f - g_aModel.vtxMax.x;
	}
	if (g_aModel.pos.z + g_aModel.vtxMax.z >= 1000.0f && g_aModel.posOld.z + g_aModel.vtxMax.z <= 1000.0f)
	{//����1000�ȏ�, �O��1000�ȉ�

		g_aModel.pos.z = 1000.0f - g_aModel.vtxMax.z;
	}
	if (g_aModel.pos.z + g_aModel.vtxMin.z <= -1000.0f && g_aModel.posOld.z + g_aModel.vtxMin.z >= -1000.0f)
	{//����-1000�ȉ�, �O��-1000�ȏ�

		g_aModel.pos.z = -1000.0f - g_aModel.vtxMin.z;
	}

	//�G�Ƃ̓����蔻��
	CollisionEnemy();

	//�e�̈ʒu�X�V
	SetPositionShadow(g_aModel.nIdxShadow, g_aModel.pos);

}

//==================================================================================
//���f���̑���
//==================================================================================
void ControllModel(void)
{
	//�J�����̏��擾
	Camera aCamera = GetCamera();

	float fRotMove = g_aModel.rot.y;	//���݂̊p�x
	float fRotDest = g_aModel.rot.y;	//�ڕW�̊p�x
	float fRotDiff = 0.0f;				//�␳

	if (GetKeyboardPress(DIK_LEFT) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
	{//���L�[�������ꂽ,���ړ�

		if (GetKeyboardPress(DIK_UP) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
		{//�� + ��,����ړ�

			g_aModel.move.x += sinf(-D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(-D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
			fRotDest = D3DX_PI * MOVE_LRUP + aCamera.rot.y;
		}
		else if (GetKeyboardPress(DIK_DOWN) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
		{//�� + ��,�����ړ�

			g_aModel.move.x += sinf(-D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(-D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
			fRotDest = D3DX_PI * MOVE_LRDW + aCamera.rot.y;
		}
		else
		{//���ړ�

			g_aModel.move.x += sinf(-D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(-D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
			fRotDest = D3DX_PI * MOVE_LR + aCamera.rot.y;
		}

	}
	else if (GetKeyboardPress(DIK_RIGHT) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
	{//���L�[�������ꂽ,�E�ړ�

		if (GetKeyboardPress(DIK_UP) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
		{//�� + ��,�E��ړ�

			g_aModel.move.x += sinf(D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(D3DX_PI * MOVE_LRDW + aCamera.rot.y) * MOVE;
			fRotDest = -D3DX_PI * MOVE_LRUP + aCamera.rot.y;
		}
		else if (GetKeyboardPress(DIK_DOWN) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
		{//�� + ��,�E���ړ�

			g_aModel.move.x += sinf(D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(D3DX_PI * MOVE_LRUP + aCamera.rot.y) * MOVE;
			fRotDest = -D3DX_PI * MOVE_LRDW + aCamera.rot.y;
		}
		else
		{//�E�ړ�

			g_aModel.move.x += sinf(D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
			g_aModel.move.z += cosf(D3DX_PI * MOVE_LR + aCamera.rot.y) * MOVE;
			fRotDest = -D3DX_PI * MOVE_LR + aCamera.rot.y;
		}

	}
	else if (GetKeyboardPress(DIK_UP) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
	{//���L�[�������ꂽ,��ړ�

		g_aModel.move.x += sinf(aCamera.rot.y) * MOVE;
		g_aModel.move.z += cosf(aCamera.rot.y) * MOVE;
		fRotDest = D3DX_PI + aCamera.rot.y;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
	{//���L�[�������ꂽ,���ړ�

		g_aModel.move.x += sinf(D3DX_PI + aCamera.rot.y) * MOVE;
		g_aModel.move.z += cosf(D3DX_PI + aCamera.rot.y) * MOVE;
		fRotDest = aCamera.rot.y;
	}

	if (GetKeyboardPress(DIK_I) == true)
	{//I�L�[�������ꂽ,�㏸

		g_aModel.pos.y += MOVE;
	}
	else if (GetKeyboardPress(DIK_K) == true)
	{//K�L�[�������ꂽ,���~

		g_aModel.pos.y -= MOVE;
	}

	//���݂ƖڕW�̍��������߂�
	fRotDiff = fRotDest - fRotMove;

	//1�����z������␳����
	if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2.0f;
	}
	else if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2.0f;
	}

	//�p�x�̕␳������
	g_aModel.rot.y += fRotDiff * 0.1f;

	//1�����z������␳����
	if (g_aModel.rot.y < -D3DX_PI)
	{
		g_aModel.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_aModel.rot.y > D3DX_PI)
	{
		g_aModel.rot.y -= D3DX_PI * 2.0f;
	}

	//�ʒu�X�V
	g_aModel.pos += g_aModel.move;

	//��������
	g_aModel.move.x += (0.0f - g_aModel.move.x) * 0.1f;
	g_aModel.move.z += (0.0f - g_aModel.move.z) * 0.1f;

}

//==================================================================================
//�G�Ƃ̓����蔻��
//==================================================================================
void CollisionEnemy(void)
{
	//�G�̏��擾
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//�G���g�p����Ă�����

			//���̔���
			if (g_aModel.pos.z + g_aModel.vtxMax.z > pEnemy->pos.z + pEnemy->vtxMin.z &&	//�G�̍ŏ��l�ɓ����Ă�
				g_aModel.pos.z + g_aModel.vtxMin.z < pEnemy->pos.z + pEnemy->vtxMax.z)		//�G�̍ő�l�ɓ����Ă�
			{//�G��Z���̒��ɂ���Ƃ�

				if (g_aModel.pos.x + g_aModel.vtxMin.x <= pEnemy->pos.x + pEnemy->vtxMax.x &&	//���݂̍ŏ��l���G�̍ő�l����������
					g_aModel.posOld.x + g_aModel.vtxMin.x >= pEnemy->pos.x + pEnemy->vtxMax.x)	//�O��̍ŏ��l���G�̍ő�l�����傫��
				{//�G�̈ʒu�ɕ␳

					g_aModel.pos.x = (pEnemy->pos.x + pEnemy->vtxMax.x) - g_aModel.vtxMin.x;
				}
				if (g_aModel.pos.x + g_aModel.vtxMax.x >= pEnemy->pos.x + pEnemy->vtxMin.x &&	//���݂̍ő�l���G�̍ŏ��l�����傫��
					g_aModel.posOld.x + g_aModel.vtxMax.x <= pEnemy->pos.x + pEnemy->vtxMin.x)	//�O��̍ő�l���G�̍ŏ��l����������
				{//�G�̈ʒu�ɕ␳

					g_aModel.pos.x = (pEnemy->pos.x + pEnemy->vtxMin.x) - g_aModel.vtxMax.x;
				}
			}


			//�c�̔���
			if (g_aModel.pos.x + g_aModel.vtxMax.x > pEnemy->pos.x + pEnemy->vtxMin.x &&	//�G�̍ŏ��l�ɓ����Ă�
				g_aModel.pos.x + g_aModel.vtxMin.x < pEnemy->pos.x + pEnemy->vtxMax.x)		//�G�̍ő�l�ɓ����Ă�
			{//�G��X���̒��ɂ���Ƃ�

				if (g_aModel.pos.z + g_aModel.vtxMin.z <= pEnemy->pos.z + pEnemy->vtxMax.z &&	//���݂̍ŏ��l���G�̍ő�l����������
					g_aModel.posOld.z + g_aModel.vtxMin.z >= pEnemy->pos.z + pEnemy->vtxMax.z)	//�O��̍ŏ��l���G�̍ő�l�����傫��
				{//�G�̈ʒu�ɕ␳

					g_aModel.pos.z = (pEnemy->pos.z + pEnemy->vtxMax.z) - g_aModel.vtxMin.z;
				}
				if (g_aModel.pos.z + g_aModel.vtxMax.z >= pEnemy->pos.z + pEnemy->vtxMin.z &&	//���݂̍ő�l���G�̍ŏ��l�����傫��
					g_aModel.posOld.z + g_aModel.vtxMax.z <= pEnemy->pos.z + pEnemy->vtxMin.z)	//�O��̍ő�l���G�̍ŏ��l����������
				{//�G�̈ʒu�ɕ␳

					g_aModel.pos.z = (pEnemy->pos.z + pEnemy->vtxMin.z) - g_aModel.vtxMax.z;
				}

			}

		}

	}

}

//==================================================================================
//���f���̕`�揈��
//==================================================================================
void DrawModel(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aModel.mtxWorld);

	//�����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel.rot.y, g_aModel.rot.x, g_aModel.rot.z);
	D3DXMatrixMultiply(&g_aModel.mtxWorld, &g_aModel.mtxWorld, &mtxRot);

	//�ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, g_aModel.pos.x, g_aModel.pos.y, g_aModel.pos.z);
	D3DXMatrixMultiply(&g_aModel.mtxWorld, &g_aModel.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aModel.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	//���_�����J��Ԃ�
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{

		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureModel[nCntMat]);

		//���f��(�p�[�c)�̕`��
		g_pMeshModel->DrawSubset(nCntMat);

	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

}

//==================================================================================
//���f���̑S���_�`�F�b�N
//==================================================================================
void CheckVtxModel(void)
{
	//���f���̒��_�����擾
	g_aModel.nNumVtx = g_pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	g_aModel.dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aModel.pVtxBuff);

	//�S�Ă̒��_���`�F�b�N����
	for (int nCntVtx = 0; nCntVtx < g_aModel.nNumVtx; nCntVtx++)
	{
		//����̃f�[�^
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aModel.pVtxBuff;	//���_���W�̑��

		//X
		if (g_aModel.vtxMin.x > vtx.x)
		{//���̍ŏ��l��������̒l����������������

			g_aModel.vtxMin.x = vtx.x;	//����ւ�
		}
		if (g_aModel.vtxMax.x < vtx.x)
		{//���̍ő�l��������̒l���傫��������

			g_aModel.vtxMax.x = vtx.x;	//����ւ�
		}

		//Y
		if (g_aModel.vtxMin.y > vtx.y)
		{//���̍ŏ��l��������̒l����������������

			g_aModel.vtxMin.y = vtx.y;	//����ւ�
		}
		if (g_aModel.vtxMax.y < vtx.y)
		{//���̍ő�l��������̒l���傫��������

			g_aModel.vtxMax.y = vtx.y;	//����ւ�
		}

		//Z
		if (g_aModel.vtxMin.z > vtx.z)
		{//���̍ŏ��l��������̒l����������������

			g_aModel.vtxMin.z = vtx.z;	//����ւ�
		}
		if (g_aModel.vtxMax.z < vtx.z)
		{//���̍ő�l��������̒l���傫��������

			g_aModel.vtxMax.z = vtx.z;	//����ւ�
		}

		g_aModel.pVtxBuff += g_aModel.dwSizeFVF;	//�T�C�Y���|�C���^�ړ�
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshModel->UnlockVertexBuffer();
}

//==================================================================================
//���f���̏��擾
//==================================================================================
Model GetModel(void)
{
	return g_aModel;
}
