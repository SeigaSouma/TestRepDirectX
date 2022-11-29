//=============================================================================
//
// �r���{�[�h���� [billboard.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "billboard.h"
#include "input.h"

//�}�N����`
#define POS_BILLBOARD	(50.0f)
#define MOVE		(0.0f)
#define MAX_TOP		(4)

//�O���[�o���ϐ��錾
const char *c_apFilenameBillboard[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\char_02.png",
};
LPDIRECT3DTEXTURE9 g_pTextureBillboard[(sizeof c_apFilenameBillboard) / sizeof(*c_apFilenameBillboard)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Billboard g_aBillboard[MAX_BILLBOARD];				//�r���{�[�h�̏��
bool g_bZFunc;			//Z�e�X�g
bool g_bAlphaFunc;		//�A���t�@�e�X�g

//==================================================================================
//�r���{�[�h�̏���������
//==================================================================================
void InitBillboard(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameBillboard) / sizeof(*c_apFilenameBillboard); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBillboard[nCntTex],
			&g_pTextureBillboard[nCntTex]);

	}

	//�e�v�f������
	for (int nCount = 0; nCount < MAX_BILLBOARD; nCount++)
	{
		g_aBillboard[nCount].pos = D3DXVECTOR3(0.0f, POS_BILLBOARD, 0.0f);
		g_aBillboard[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCount].bUse = false;
	}
	g_bZFunc = true;
	g_bAlphaFunc = true;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BILLBOARD; nCount++)
	{

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-POS_BILLBOARD, POS_BILLBOARD, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(POS_BILLBOARD, POS_BILLBOARD, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-POS_BILLBOARD, -POS_BILLBOARD, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(POS_BILLBOARD, -POS_BILLBOARD, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		pVtx += MAX_TOP;

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();

	//�r���{�[�h�̐ݒ菈��
	SetBillboard(D3DXVECTOR3(0.0f, POS_BILLBOARD, 0.0f));
	SetBillboard(D3DXVECTOR3(30.0f, POS_BILLBOARD, POS_BILLBOARD));
	SetBillboard(D3DXVECTOR3(970.0f, POS_BILLBOARD, POS_BILLBOARD));
}

//==================================================================================
//�r���{�[�h�̏I������
//==================================================================================
void UninitBillboard(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameBillboard) / sizeof(*c_apFilenameBillboard); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBillboard[nCntTex] != NULL)
		{
			g_pTextureBillboard[nCntTex]->Release();
			g_pTextureBillboard[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//==================================================================================
//�r���{�[�h�̍X�V����
//==================================================================================
void UpdateBillboard(void)
{
	if (GetKeyboardTrigger(DIK_F8) == true)
	{//F8�������ꂽ,Z�e�X�g�؂�ւ�

		g_bZFunc = g_bZFunc ? false : true;
	}
	if (GetKeyboardTrigger(DIK_F9) == true)
	{//F9�������ꂽ,Z�e�X�g�؂�ւ�

		g_bAlphaFunc = g_bAlphaFunc ? false : true;
	}

}

//==================================================================================
//�r���{�[�h�̕`�揈��
//==================================================================================
void DrawBillboard(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p

	if (g_bZFunc == false)
	{//����OFF�̂Ƃ�

		//Z�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	}

	if (g_bAlphaFunc == true)
	{//����ON�̂Ƃ�

		//�A���t�@�e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	}

	for (int nCount = 0; nCount < MAX_BILLBOARD; nCount++)
	{

		if (g_aBillboard[nCount].bUse == true)
		{//�g�p����Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBillboard[nCount].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBillboard[nCount].mtxWorld, NULL, &mtxView);	//�t�z������߂�

#if 0

	//X���ɉ����Ă�
			g_aBillboard[nCount].mtxWorld._12 = 0.0f;
			g_aBillboard[nCount].mtxWorld._13 = 0.0f;
			g_aBillboard[nCount].mtxWorld._14 = 0.0f;

#else

	//��ŏ�����
			g_aBillboard[nCount].mtxWorld._21 = 0.0f;
			g_aBillboard[nCount].mtxWorld._23 = 0.0f;
			g_aBillboard[nCount].mtxWorld._24 = 0.0f;

			//���C�g
			/*g_aBillboard[nCount].mtxWorld._31 = 0.0f;
			g_aBillboard[nCount].mtxWorld._32 = 0.0f;
			g_aBillboard[nCount].mtxWorld._34 = 0.0f;*/

#endif

			g_aBillboard[nCount].mtxWorld._41 = 0.0f;
			g_aBillboard[nCount].mtxWorld._42 = 0.0f;
			g_aBillboard[nCount].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCount].pos.x, g_aBillboard[nCount].pos.y, g_aBillboard[nCount].pos.z);
			D3DXMatrixMultiply(&g_aBillboard[nCount].mtxWorld, &g_aBillboard[nCount].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCount].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBillboard[0]);

			//�r���{�[�h�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);

		}

	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

}

//==================================================================================
//�r���{�[�h�̐ݒ菈��
//==================================================================================
void SetBillboard(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BILLBOARD; nCount++)
	{

		if (g_aBillboard[nCount].bUse == false)
		{//�g�p����Ă��Ȃ�������

			g_aBillboard[nCount].pos = pos;
			g_aBillboard[nCount].bUse = true;
			break;
		}

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//==================================================================================
//Z�e�X�g�̔���
//==================================================================================
bool GetZTest(void)
{
	return g_bZFunc;
}

//==================================================================================
//�A���t�@�e�X�g�̔���
//==================================================================================
bool GetAlphaTest(void)
{
	return g_bAlphaFunc;
}