//=============================================================================
//
// ���b�V���V�����_�[���� [meshcylinder.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "meshcylinder.h"
#include "input.h"

//�}�N����`
#define POS_MESHCYLINDER	(5000.0f)
#define POS_MESHCYLINDER_Y	(1000.0f)
#define MOVE		(0.0f)
#define WIDTH		(16)
#define HEIGHT		(1)

//�O���[�o���ϐ��錾
const char *c_apFilenameMeshCylinder[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\field002.png",
};
//LPDIRECT3DTEXTURE9 g_apTextureMeshCylinder[(sizeof c_apFilenameMeshCylinder) / sizeof(*c_apFilenameMeshCylinder)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MESHCYLINDER g_aMeshCylinder;									//���b�V���V�����_�[�̏��

//==================================================================================
//���b�V���V�����_�[�̏���������
//==================================================================================
void InitMeshCylinder(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\mountain000.png",
		&g_pTextureMeshCylinder);

	//�e�v�f������
	g_aMeshCylinder.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.nNumIndex = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//�C���f�b�N�X��
	g_aMeshCylinder.nNumVertex = (HEIGHT + 1) * (WIDTH + 1);	//���_��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshCylinder.nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[128];		//�v�Z�p�̍��W
	float fRot = (D3DX_PI * 2) / (float)(WIDTH);	//1���Ƃ̊p�x�����߂�

	D3DXVECTOR3 posTest = D3DXVECTOR3(1000.0f, 500.0f, 2000.0f);
	D3DXVECTOR3 NormalizeNor[128];

	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * POS_MESHCYLINDER,
				(POS_MESHCYLINDER_Y * HEIGHT) - ((POS_MESHCYLINDER_Y * nCntHeight)),
				cosf(nCntWidth % WIDTH * fRot) * POS_MESHCYLINDER
			);

			pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * POS_MESHCYLINDER,
				(POS_MESHCYLINDER_Y * HEIGHT) - ((POS_MESHCYLINDER_Y * (nCntHeight + 1))),
				cosf(nCntWidth % WIDTH * fRot) * POS_MESHCYLINDER
			);

			//�e���_���猴�_������
			NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aMeshCylinder.pos;
			NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aMeshCylinder.pos;

			//�o�������̒l�𐳋K������
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))],				&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))],	&NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);

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

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2
			(
				nCntWidth * (1.0f / (float)(WIDTH / 4)),
				nCntHeight * (1.0f / (float)(HEIGHT))
			);

			pVtx += 1;
		}

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshCylinder->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshCylinder.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�o�b�t�@�̗v�f�ԍ�
	int nCntIdx = 0;

	//���_�ԍ��f�[�^�̐ݒ�
	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//�����̕��������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			pIdx[nCntIdx + 0] = nCntWidth + (WIDTH + 1) * (nCntHeight + 1);
			pIdx[nCntIdx + 1] = nCntWidth + ((WIDTH + 1) * nCntHeight);
			
			nCntIdx += 2;	//2�����Ă邩��
		}

		if (nCntHeight + 1 < HEIGHT)
		{//�Ō�̂����͑ł��Ȃ�

			pIdx[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (WIDTH + 1) * (nCntHeight + 2);

			nCntIdx += 2;	//2�����Ă邩��
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshCylinder->Unlock();




	//for (int nCntWidth = 0; nCntWidth < g_aMeshWall[nCntWall].nWidth + 1; nCntWidth++)
	//{//���̒��_�����J��Ԃ�

	//	pIdx[nCntIdx + 0] = nCntWidth + (WIDTH + 1) * (nCntHeight + 1);
	//	pIdx[nCntIdx + 1] = nCntWidth + ((WIDTH + 1) * nCntHeight);
	//	nCntIdx += 2;	//2�����Ă邩��
	//}

	//if (nCntHeight + 1 < g_aMeshWall[nCntWall].nHeight)
	//{//�Ō�̂����͑ł��Ȃ�

	// //��ł�2��
	//	pIdx[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 1)) - 1;
	//	pIdx[nCntIdx + 1] = (WIDTH + 1) * (nCntHeight + 2);
	//	nCntIdx += 2;	//2�����Ă邩��
	//}






}

//==================================================================================
//���b�V���V�����_�[�̏I������
//==================================================================================
void UninitMeshCylinder(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	//���_�C���f�b�N�X�̔j��
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}

}

//==================================================================================
//���b�V���V�����_�[�̍X�V����
//==================================================================================
void UpdateMeshCylinder(void)
{

}

//==================================================================================
//���b�V���V�����_�[�̕`�揈��
//==================================================================================
void DrawMeshCylinder(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aMeshCylinder.mtxWorld);

	//�����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshCylinder.rot.y, g_aMeshCylinder.rot.x, g_aMeshCylinder.rot.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.mtxWorld, &g_aMeshCylinder.mtxWorld, &mtxRot);

	//�ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, g_aMeshCylinder.pos.x, g_aMeshCylinder.pos.y, g_aMeshCylinder.pos.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.mtxWorld, &g_aMeshCylinder.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_aMeshCylinder.nNumVertex,
		0,
		g_aMeshCylinder.nNumIndex - 2);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}