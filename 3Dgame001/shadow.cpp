//=============================================================================
//
// �e���� [shadow.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "shadow.h"
#include "input.h"

//�}�N����`
#define POS_SHADOW	(50.0f)
#define MOVE		(0.0f)
#define MAX_TOP		(4)

//�O���[�o���ϐ��錾
const char *c_apFilenameShadow[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\Shadow2.png",
};
//LPDIRECT3DTEXTURE9 g_apTextureShadow[(sizeof c_apFilenameShadow) / sizeof(*c_apFilenameShadow)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
SHADOW g_aShadow[MAX_SHADOW];						//�e�̏��

//==================================================================================
//�e�̏���������
//==================================================================================
void InitShadow(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShadow);

	//�e�v�f������
	for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
	{
		g_aShadow[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCount].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-POS_SHADOW, 0, POS_SHADOW);
		pVtx[1].pos = D3DXVECTOR3(POS_SHADOW, 0, POS_SHADOW);
		pVtx[2].pos = D3DXVECTOR3(-POS_SHADOW, 0, -POS_SHADOW);
		pVtx[3].pos = D3DXVECTOR3(POS_SHADOW, 0, -POS_SHADOW);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//==================================================================================
//�e�̏I������
//==================================================================================
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//==================================================================================
//�e�̍X�V����
//==================================================================================
void UpdateShadow(void)
{

}

//==================================================================================
//�e�̕`�揈��
//==================================================================================
void DrawShadow(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
	{

		if (g_aShadow[nCount].bUse == true)
		{//�g�p���Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCount].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCount].rot.y, g_aShadow[nCount].rot.x, g_aShadow[nCount].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCount].mtxWorld, &g_aShadow[nCount].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCount].pos.x, g_aShadow[nCount].pos.y, g_aShadow[nCount].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCount].mtxWorld, &g_aShadow[nCount].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCount].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�e�̕`��
 			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);

		}

	}

	//���Z���������Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//==================================================================================
//�e�̐ݒ菈��
//==================================================================================
int SetShadow(float fWidth, float fHeight)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	int nCntShadow = -1;

	for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
	{
		if (g_aShadow[nCount].bUse == false)
		{//�g�p���Ă��Ȃ�������

			nCntShadow = nCount;			//�C���f�b�N�X�ԍ��擾
			g_aShadow[nCount].bUse = true;	//�g�p���Ă���󋵂ɂ���

			fWidth *= 1.5f;
			fHeight *= 1.5f;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-fWidth, 0, fWidth);
			pVtx[1].pos = D3DXVECTOR3(fWidth, 0, fWidth);
			pVtx[2].pos = D3DXVECTOR3(-fWidth, 0, -fWidth);
			pVtx[3].pos = D3DXVECTOR3(fWidth, 0, -fWidth);
			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//==================================================================================
//�e�̗L���ݒ菈��
//==================================================================================
void SetEnableShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}

//==================================================================================
//�e�̈ʒu�ݒ菈��
//==================================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{

	g_aShadow[nIdxShadow].pos = D3DXVECTOR3(pos.x, 0.0f, pos.z);
}