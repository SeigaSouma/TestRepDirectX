//=============================================================================
//
// ���b�V���h�[������ [meshdome.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "meshdome.h"
#include "input.h"

//�}�N����`
#define POS_MESHDOME	(8000.0f)
#define POS_MESHDOME_Y	(POS_MESHDOME)
#define MOVE		(0.0f)
#define WIDTH		(16)
#define HEIGHT		(8)

//�v���g�^�C�v�錾
void InitMeshDomeLid(LPDIRECT3DDEVICE9 pDevice);

//�O���[�o���ϐ��錾
const char *c_apFilenameMeshDome[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\field002.png",
};
//LPDIRECT3DTEXTURE9 g_apTextureMeshDome[(sizeof c_apFilenameMeshDome) / sizeof(*c_apFilenameMeshDome)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureMeshDome = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshDome = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshDomeLid = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̊W�|�C���^
MESHDOME g_aMeshDome;									//���b�V���h�[���̏��

//==================================================================================
//���b�V���h�[���̏���������
//==================================================================================
void InitMeshDome(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sky000.jpg",
		&g_pTextureMeshDome);

	//�e�v�f������
	g_aMeshDome.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshDome.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshDome.nNumIndexLid = ((WIDTH + 1) + 1);	//�W�̃C���f�b�N�X��
	g_aMeshDome.nNumIndex = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//�W�Ȃ��̃C���f�b�N�X��
	g_aMeshDome.nNumVertex = (HEIGHT + 1) * (WIDTH + 1);	//���_��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshDome.nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshDome,
		NULL);

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshDome->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[256];		//�v�Z�p�̍��W
	D3DXVECTOR3 NormalizeNor[256];
	float fRotWidth = (D3DX_PI * 2) / (float)(WIDTH);	//1���Ƃ̊p�x�����߂�, ����
	float fRotHeight = (D3DX_PI * 0.5f) / (float)(HEIGHT);	//1���Ƃ̊p�x�����߂�, ����
	float fRotCalW = 0.0f;
	float fRotCalH = 0.0f;

	pos[WIDTH + (HEIGHT * (WIDTH + 1))] = D3DXVECTOR3(0.0f, POS_MESHDOME_Y, 0.0f);

	for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			//����̊p�x������o��
			fRotCalH = (float)(nCntHeight % (HEIGHT +1 )) * fRotHeight;
			fRotCalW = (fRotWidth * WIDTH) - ((float)(nCntWidth % (WIDTH + 1)) * fRotWidth);

			////�p�x�̐��K��
			fRotCalW = RotNormalize(fRotCalW);
			fRotCalH = RotNormalize(fRotCalH);

			////���W����o��
			pos[nCntWidth + (nCntHeight * (WIDTH + 1))].x = cosf(fRotCalH) * sinf(fRotCalW) * POS_MESHDOME;
			pos[nCntWidth + (nCntHeight * (WIDTH + 1))].z = cosf(fRotCalH) * cosf(fRotCalW) * POS_MESHDOME;
			pos[nCntWidth + (nCntHeight * (WIDTH + 1))].y = sinf(fRotCalH) * POS_MESHDOME_Y;

			//�e���_���猴�_������
			NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aMeshDome.pos;

			//�o�������̒l�𐳋K������
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))], &NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))]);

		}
	}

	//���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			//���_���W�̐ݒ�
			pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

			//�@���x�N�g���̐ݒ�
			pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))];

			if (nCntHeight < HEIGHT - 1)
			{
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.3f, 1.0f);
			}


			pVtx[0].tex = -D3DXVECTOR2
			(
				nCntWidth * (1.0f / (float)(WIDTH)),
				nCntHeight * (1.0f / (float)(HEIGHT))
			);

			pVtx += 1;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshDome->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshDome.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshDome,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�o�b�t�@�̗v�f�ԍ�
	int nCntIdx = 0;
	int aaaa[512];

	//���_�ԍ��f�[�^�̐ݒ�
	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//�����̕��������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			pIdx[nCntIdx + 0] = nCntWidth + (WIDTH + 1) * (nCntHeight + 1);
			pIdx[nCntIdx + 1] = nCntWidth + ((WIDTH + 1) * nCntHeight);

			aaaa[nCntIdx + 0] = nCntWidth + (WIDTH + 1) * (nCntHeight + 1);
			aaaa[nCntIdx + 1] = nCntWidth + ((WIDTH + 1) * nCntHeight);
			
			nCntIdx += 2;	//2�����Ă邩��
		}

		if (nCntHeight + 1 < HEIGHT)
		{//�Ō�̂����͑ł��Ȃ�

			pIdx[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (WIDTH + 1) * (nCntHeight + 2);

			aaaa[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 1)) - 1;
			aaaa[nCntIdx + 1] = (WIDTH + 1) * (nCntHeight + 2);

			nCntIdx += 2;	//2�����Ă邩��
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshDome->Unlock();

	//�W�̃C���f�b�N�X
	InitMeshDomeLid(pDevice);
	

}

//==================================================================================
//���b�V���h�[���̏I������
//==================================================================================
void UninitMeshDome(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshDome != NULL)
	{
		g_pTextureMeshDome->Release();
		g_pTextureMeshDome = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshDome != NULL)
	{
		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}

	//���_�C���f�b�N�X�̔j��
	if (g_pIdxBuffMeshDome != NULL)
	{
		g_pIdxBuffMeshDome->Release();
		g_pIdxBuffMeshDome = NULL;
	}

}

//==================================================================================
//���b�V���h�[���̍X�V����
//==================================================================================
void UpdateMeshDome(void)
{

}

//==================================================================================
//���b�V���h�[���̕`�揈��
//==================================================================================
void DrawMeshDome(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aMeshDome.mtxWorld);

	//�����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshDome.rot.y, g_aMeshDome.rot.x, g_aMeshDome.rot.z);
	D3DXMatrixMultiply(&g_aMeshDome.mtxWorld, &g_aMeshDome.mtxWorld, &mtxRot);

	//�ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, g_aMeshDome.pos.x, g_aMeshDome.pos.y, g_aMeshDome.pos.z);
	D3DXMatrixMultiply(&g_aMeshDome.mtxWorld, &g_aMeshDome.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshDome.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshDome, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshDome);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_aMeshDome.nNumVertex,
		0,
		g_aMeshDome.nNumIndex - 2);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshDome);

	// �W�̃C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshDomeLid);

	// �|���S���̕`��(TRIANGLEFAN)
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN,
		0,
		0,
		25,
		0,
		g_aMeshDome.nNumIndexLid - 2);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==================================================================================
//���b�V���h�[���̊W�̃C���f�b�N�X������
//==================================================================================
void InitMeshDomeLid(LPDIRECT3DDEVICE9 pDevice)
{
	//�W�̃C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshDome.nNumIndexLid,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshDomeLid,
		NULL);

	WORD *pIdx;	//�W�̃C���f�b�N�X���ւ̃|�C���^

				//�W�̃C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshDomeLid->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�o�b�t�@�̗v�f�ԍ�
	int nCntIdx = 0;
	int aaaa[512];

	//���_��1��
	pIdx[0] = HEIGHT * (WIDTH + 1) + WIDTH - (WIDTH);
	aaaa[0] = HEIGHT * (WIDTH + 1) + WIDTH - (WIDTH);

	//119����136�܂�
	for (int nCntIdx = 0; nCntIdx < WIDTH + 1; nCntIdx++)
	{//�W�̒��_���J��Ԃ�

		pIdx[nCntIdx + 1] = (HEIGHT * (WIDTH + 1) + nCntIdx) - (WIDTH + 1);
		aaaa[nCntIdx + 1] = (HEIGHT * (WIDTH + 1) + nCntIdx) - (WIDTH + 1);
	}


	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshDomeLid->Unlock();
}