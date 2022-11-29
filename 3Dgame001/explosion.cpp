//=============================================================================
//
// �o���b�g���� [explosion.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "explosion.h"
#include "input.h"
#include "particle.h"

//�}�N����`
#define MAX_TOP		(4)			//���_��
#define MOVE		(10.0f)		//�����̈ړ���
#define MOVE_U		(0.125f)	//U���W�ړ���
#define MOVE_V		(1.0f)		//V���W�ړ���
#define PAUSE_TIME	(8)			//�A�j���[�V�������x
#define MAX_CUT		(8)			//U������
#define MOVE_SIZE	(0.3f)		//�T�C�Y�ړ���

//�O���[�o���ϐ��錾
const char *c_apFilenameExplosion[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\explosion000.png",
};
LPDIRECT3DTEXTURE9 g_pTextureExplosion[(sizeof c_apFilenameExplosion) / sizeof(*c_apFilenameExplosion)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];						//�����̏��

//==================================================================================
//�����̏���������
//==================================================================================
void InitExplosion(void)
{
	
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameExplosion) / sizeof(*c_apFilenameExplosion); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameExplosion[nCntTex],
			&g_pTextureExplosion[nCntTex]);

	}

	//�����̏��̏�����
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].nLife = PAUSE_TIME * MAX_CUT;
		g_aExplosion[nCntExplosion].nLife = g_aExplosion[nCntExplosion].nLife;
		g_aExplosion[nCntExplosion].nCntAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���

	}


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_EXPLOSION,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffExplosion,
								NULL);

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-POS_EXPLOSION, +POS_EXPLOSION, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+POS_EXPLOSION, +POS_EXPLOSION, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-POS_EXPLOSION, -POS_EXPLOSION, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+POS_EXPLOSION, -POS_EXPLOSION, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = g_aExplosion[nCntExplosion].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(MOVE_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(MOVE_U, 1.0f);

		pVtx += MAX_TOP;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//==================================================================================
//�����̏I������
//==================================================================================
void UninitExplosion(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameExplosion) / sizeof(*c_apFilenameExplosion); nCount++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureExplosion[nCount] != NULL)
		{
			g_pTextureExplosion[nCount]->Release();
			g_pTextureExplosion[nCount] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//==================================================================================
//�����̍X�V����
//==================================================================================
void UpdateExplosion(void)
{

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�������g�p����Ă�����

			g_aExplosion[nCntExplosion].nCntAnim++;	//�J�E���^�[�����Z

			if ((g_aExplosion[nCntExplosion].nCntAnim % PAUSE_TIME) == 0)
			{//�K��J�E���g�i�񂾂�

				//�p�^�[��No.���X�V
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % MAX_CUT;
				g_aExplosion[nCntExplosion].nCntAnim = 0;
			}

			if (g_aExplosion[nCntExplosion].nPatternAnim >= MAX_CUT - 1)
			{//���p�^�[�����𒴂�����

				g_aExplosion[nCntExplosion].bUse = false;
			}

			//���������炷
			g_aExplosion[nCntExplosion].nLife--;

			//�s�����x�������Ō��炷
			g_aExplosion[nCntExplosion].col.a = (float)g_aExplosion[nCntExplosion].nLife / (float)g_aExplosion[nCntExplosion].nMaxLife;

			//���_���W�̍X�V
			pVtx[0].pos.x -= MOVE_SIZE;
			pVtx[0].pos.y += MOVE_SIZE;

			pVtx[1].pos.x += MOVE_SIZE;
			pVtx[1].pos.y += MOVE_SIZE;

			pVtx[2].pos.x -= MOVE_SIZE;
			pVtx[2].pos.y -= MOVE_SIZE;

			pVtx[3].pos.x += MOVE_SIZE;
			pVtx[3].pos.y -= MOVE_SIZE;

			//���_�J���[�̐ݒ�
			pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = g_aExplosion[nCntExplosion].col;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % MAX_CUT) * MOVE_U, (g_aExplosion[nCntExplosion].nPatternAnim / MAX_CUT) * MOVE_V);
			pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % MAX_CUT + 1) * MOVE_U, (g_aExplosion[nCntExplosion].nPatternAnim / MAX_CUT) * MOVE_V);
			pVtx[2].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % MAX_CUT) * MOVE_U, (g_aExplosion[nCntExplosion].nPatternAnim / MAX_CUT) * MOVE_V + MOVE_V);
			pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim % MAX_CUT + 1) * MOVE_U, (g_aExplosion[nCntExplosion].nPatternAnim / MAX_CUT) * MOVE_V + MOVE_V);
			
		}

		pVtx += MAX_TOP;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();

}

//==================================================================================
//�����̕`�揈��
//==================================================================================
void DrawExplosion(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��
	
	 //�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCount = 0; nCount < MAX_EXPLOSION; nCount++)
	{

		if (g_aExplosion[nCount].bUse == true)
		{//�g�p����Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aExplosion[nCount].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aExplosion[nCount].mtxWorld, NULL, &mtxView);	//�t�z������߂�
			g_aExplosion[nCount].mtxWorld._41 = 0.0f;
			g_aExplosion[nCount].mtxWorld._42 = 0.0f;
			g_aExplosion[nCount].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCount].pos.x, g_aExplosion[nCount].pos.y, g_aExplosion[nCount].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCount].mtxWorld, &g_aExplosion[nCount].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCount].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExplosion[0]);

			//�r���{�[�h�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);

		}

	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==================================================================================
//�����̐ݒ菈��
//==================================================================================
void SetExplosion(D3DXVECTOR3 pos)
{


	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{

		if (g_aExplosion[nCntExplosion].bUse == false)
		{//�������g�p����Ă��Ȃ�������

			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].nLife = PAUSE_TIME * MAX_CUT;
			g_aExplosion[nCntExplosion].nMaxLife = g_aExplosion[nCntExplosion].nLife;
			g_aExplosion[nCntExplosion].nCntAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;		//�g�p���Ă����ԂɕύX
			
			//�p�[�e�B�N���̐ݒ菈��
			SetParticle(g_aExplosion[nCntExplosion].pos, PARTICLE_TYPE_SMOKE);
			SetParticle(g_aExplosion[nCntExplosion].pos, PARTICLE_TYPE_SMOKE_RED);
			SetParticle(g_aExplosion[nCntExplosion].pos, PARTICLE_TYPE_SMOKE_YEL);

			
			SetParticle(g_aExplosion[nCntExplosion].pos, PARTICLE_TYPE_OFFSETTING);


			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-POS_EXPLOSION, +POS_EXPLOSION, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+POS_EXPLOSION, +POS_EXPLOSION, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-POS_EXPLOSION, -POS_EXPLOSION, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+POS_EXPLOSION, -POS_EXPLOSION, 0.0f);
			break;
		}

		pVtx += MAX_TOP;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();

}

//==================================================================================
//�����̏��擾
//==================================================================================
Explosion *GetExplosion(void)
{
	return &g_aExplosion[0];
}
