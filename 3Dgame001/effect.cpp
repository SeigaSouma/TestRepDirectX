//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "effect.h"
#include "input.h"
#include "particle.h"
#include "camera.h"

//�}�N����`
#define MAX_TOP			(4)		//���_��
#define MAX_EFFECT		(12800)	//�G�t�F�N�g�̍ő吔
#define EFFECTSIZE1		(0.85f)
#define EFFECTSIZE2		(0.95f)
#define EFFECTSIZE3		(0.9f)


//�v���g�^�C�v�錾
void UpdateSubEffect(int nCntEffect);
void UpdateAddEffect(int nCntEffect);

//�O���[�o���ϐ��錾
const char *c_apFilenameEffect[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect002.jpg",
	"data\\TEXTURE\\effect002.tga",
	"data\\TEXTURE\\effect000.png",
};

LPDIRECT3DTEXTURE9 g_pTextureEffect[(sizeof c_apFilenameEffect) / sizeof(*c_apFilenameEffect)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];						//�G�t�F�N�g�̏��
int g_EffectNum;									//�G�t�F�N�g�̐�
bool g_bEffectUse;

//==================================================================================
//�G�t�F�N�g�̏���������
//==================================================================================
void InitEffect(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameEffect) / sizeof(*c_apFilenameEffect); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameEffect[nCntTex],
			&g_pTextureEffect[nCntTex]);

	}


	//�G�t�F�N�g�̏��̏�����
	for (int nCntEffect =0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].nLife = EFFECT_LIFE;
		g_aEffect[nCntEffect].bAddAlpha = false;
	}
	g_EffectNum = 0;
	g_bEffectUse = true;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_EFFECT,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffEffect,
								NULL);

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//==================================================================================
//�G�t�F�N�g�̏I������
//==================================================================================
void UninitEffect(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameEffect) / sizeof(*c_apFilenameEffect); nCount++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureEffect[nCount] != NULL)
		{
			g_pTextureEffect[nCount]->Release();
			g_pTextureEffect[nCount] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//==================================================================================
//�G�t�F�N�g�̍X�V����
//==================================================================================
void UpdateEffect(void)
{

	//�J�����̏��擾
	Camera aCamera = GetCamera();

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_F5) == true)
	{//F5�������ꂽ,�G�t�F�N�g�g�p�؂�ւ�

		g_bEffectUse = g_bEffectUse ? false : true;
	}

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă�����

			//���_���W�̍X�V(����������)
			if (g_aEffect[nCntEffect].nType == EFFECTTYPE_SMOKEBLACK)
			{//�����������ꍇ

				if (((float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nMaxLife) <= 0.5f)
				{//�ő�̗͂̔����ȉ�

					g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move * 0.001f;
				}
				else
				{//��������

					g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move * (((float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nMaxLife));
				}
			}
			else 
			{//�����ȊO

				g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
			}
			

			//���������Ă��������̏ꍇ
			if (g_aEffect[nCntEffect].moveType == MOVEEFFECT_SUB)
			{//�G�t�F�N�g�����������Ă���

				//�k������
				UpdateSubEffect(nCntEffect);
			}
			else
			{//�G�t�F�N�g��傫�����Ă���

				//�g�又��
				UpdateAddEffect(nCntEffect);
			}


			//�����̍X�V
			g_aEffect[nCntEffect].nLife--;

			//�s�����x�̍X�V
			g_aEffect[nCntEffect].col.a = (float)g_aEffect[nCntEffect].nLife / (float)g_aEffect[nCntEffect].nMaxLife;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{//�������s������

				g_aEffect[nCntEffect].bUse = false;
				g_aEffect[nCntEffect].bAddAlpha = false;
				g_EffectNum--;
			}

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//���_���W�̍X�V(����������)
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

		}

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//==================================================================================
//�G�t�F�N�g�̏k������
//==================================================================================
void UpdateSubEffect(int nCntEffect)
{

	int nEffectType = rand() % 3;

	if (nEffectType == 0)
	{
		g_aEffect[nCntEffect].fRadius *= EFFECTSIZE1;
	}
	else if (nEffectType == 1)
	{
		g_aEffect[nCntEffect].fRadius *= EFFECTSIZE2;
	}
	else if (nEffectType == 2)
	{
		g_aEffect[nCntEffect].fRadius *= EFFECTSIZE3;
	}

}

//==================================================================================
//�G�t�F�N�g�̊g�又��
//==================================================================================
void UpdateAddEffect(int nCntEffect)
{

	int nEffectType = rand() % 3;

	if (nEffectType == 0)
	{
		g_aEffect[nCntEffect].fRadius += 1.8f;
	}
	else if (nEffectType == 1)
	{
		g_aEffect[nCntEffect].fRadius += 0.8f;
	}
	else if (nEffectType == 2)
	{
		g_aEffect[nCntEffect].fRadius += 0.0f;
	}

}

//==================================================================================
//�G�t�F�N�g�̕`�揈��
//==================================================================================
void DrawEffect(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//�|���S���̕`��
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true && g_bEffectUse == true)
		{//�G�t�F�N�g���g�p����Ă���

			//���u�����f�B���O�����Z�����ɐݒ�
			if (g_aEffect[nCntEffect].bAddAlpha == false)
			{
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxView);	//�t�z������߂�
			g_aEffect[nCntEffect].mtxWorld._41 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._42 = 0.0f;
			g_aEffect[nCntEffect].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].nType]);

			//�r���{�[�h�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * MAX_TOP, 2);

			//���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		}

	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==================================================================================
//�G�t�F�N�g�̐ݒ菈��
//==================================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType)
{

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//�G�t�F�N�g���g�p����Ă��Ȃ�������

			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nMaxLife = nLife;
			g_aEffect[nCntEffect].moveType = moveType;
			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].bUse = true;		//�g�p���Ă����ԂɕύX
			g_EffectNum++;

			if (g_aEffect[nCntEffect].nType == EFFECTTYPE_NORMAL)
			{//�ʏ�̎�

				g_aEffect[nCntEffect].bAddAlpha = false;
			}
			else if (g_aEffect[nCntEffect].nType == EFFECTTYPE_SMOKE)
			{//���̃G�t�F�N�g

				g_aEffect[nCntEffect].bAddAlpha = false;
			}
			else if (g_aEffect[nCntEffect].nType == EFFECTTYPE_SMOKEBLACK)
			{//�����̃G�t�F�N�g

				g_aEffect[nCntEffect].bAddAlpha = true;
			}
			else if (g_aEffect[nCntEffect].nType == EFFECTTYPE_BLACK)
			{//���̃G�t�F�N�g

				g_aEffect[nCntEffect].bAddAlpha = true;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			break;
		}

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//==================================================================================
//�G�t�F�N�g�̐��擾
//==================================================================================
int GetEffectNum(void)
{
	return g_EffectNum;
}

//==================================================================================
//�G�t�F�N�g���g�p���邩
//==================================================================================
bool GetEffectUse(void)
{
	return g_bEffectUse;
}