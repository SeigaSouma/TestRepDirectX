//=============================================================================
//
// ���b�V���E�H�[������ [meshwall.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "meshwall.h"
#include "input.h"

//�O���[�o���ϐ��錾
const char *c_apFilenameMeshWall[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\field002.png",
};
//LPDIRECT3DTEXTURE9 g_apTextureMeshWall[(sizeof c_apFilenameMeshWall) / sizeof(*c_apFilenameMeshWall)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshWall = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MESHWALL g_aMeshWall[MAX_WALL];						//���b�V���E�H�[���̏��
int g_nNumIndexaMeshWall;			//�C���f�b�N�X��
int g_nNumVertexaMeshWall;			//���_��

//==================================================================================
//���b�V���E�H�[���̏���������
//==================================================================================
void InitMeshWall(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall000.jpg",
		&g_pTextureMeshWall);

	//�e�v�f������
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aMeshWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].nNumVertex = 0;
		g_aMeshWall[nCntWall].nNumIndex = 0;
		g_aMeshWall[nCntWall].nWidth = 0;
		g_aMeshWall[nCntWall].nHeight = 0;
		g_aMeshWall[nCntWall].bUse = false;
	}
	g_nNumIndexaMeshWall = 0;
	g_nNumVertexaMeshWall = 0;

	//�ǂ̐ݒu
	SetMeshWall(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), 8, 2);
	SetMeshWall(D3DXVECTOR3(1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 8, 2);
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 8, 2);
	SetMeshWall(D3DXVECTOR3(0.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 8, 2);

	//�C���f�b�N�X�E���_�������߂�
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//�g�p���Ă�����

			g_nNumIndexaMeshWall += g_aMeshWall[nCntWall].nNumIndex;	//�C���f�b�N�X��
			g_nNumVertexaMeshWall += g_aMeshWall[nCntWall].nNumVertex;	//���_��
		}
	}


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexaMeshWall,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//�g�p���Ă�����

			//���_���̐ݒ�
			for (int nCntHeight = 0; nCntHeight < g_aMeshWall[nCntWall].nHeight + 1; nCntHeight++)
			{//�c�̒��_�����J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < g_aMeshWall[nCntWall].nWidth + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3
					(
						(POS_MESHWALL * nCntWidth) - ((POS_MESHWALL * g_aMeshWall[nCntWall].nWidth) * 0.5f),
						(POS_MESHWALL * g_aMeshWall[nCntWall].nHeight) - ((POS_MESHWALL * nCntHeight)),
						0.0f
					);

					//�@���x�N�g���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2
					(
						((float)nCntWidth / (1.0f / (float)(g_aMeshWall[nCntWall].nWidth + 1))) * (1.0f / (float)(g_aMeshWall[nCntWall].nWidth + 1)),
						((float)nCntHeight / (1.0f / (float)(g_aMeshWall[nCntWall].nHeight + 1))) * (1.0f / (float)(g_aMeshWall[nCntWall].nHeight + 1))
					);

					pVtx += 1;
				}
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshWall->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexaMeshWall,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshWall,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�o�b�t�@�̗v�f�ԍ�
	int nCntIdx = 0;
	int nVtxPoint = 0;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//�g�p���Ă�����

			//���_�ԍ��f�[�^�̐ݒ�
			for (int nCntHeight = 0; nCntHeight < g_aMeshWall[nCntWall].nHeight; nCntHeight++)
			{//�����̕��������J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < g_aMeshWall[nCntWall].nWidth + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

					pIdx[nCntIdx + 0] = (nCntWidth + (g_aMeshWall[nCntWall].nWidth + 1) * (nCntHeight + 1)) + nVtxPoint;
					pIdx[nCntIdx + 1] = (nCntWidth + ((g_aMeshWall[nCntWall].nWidth + 1) * nCntHeight)) + nVtxPoint;
					nCntIdx += 2;	//2�����Ă邩��
				}

				if (nCntHeight + 1 < g_aMeshWall[nCntWall].nHeight)
				{//�Ō�̂����͑ł��Ȃ�

					//��ł�2��
					pIdx[nCntIdx + 0] = (((g_aMeshWall[nCntWall].nWidth + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
					pIdx[nCntIdx + 1] = ((g_aMeshWall[nCntWall].nWidth + 1) * (nCntHeight + 2)) + nVtxPoint;
					nCntIdx += 2;	//2�����Ă邩��
				}
			}

			nVtxPoint += g_aMeshWall[nCntWall].nNumVertex;	//����̃C���f�b�N�X�������Z
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshWall->Unlock();

}

//==================================================================================
//���b�V���E�H�[���̏I������
//==================================================================================
void UninitMeshWall(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	//���_�C���f�b�N�X�̔j��
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
	}

}

//==================================================================================
//���b�V���E�H�[���̍X�V����
//==================================================================================
void UpdateMeshWall(void)
{
	if (GetKeyboardPress(DIK_1) == true)
	{//���L�[�������ꂽ,���ړ�

		for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
		{
			if (g_aMeshWall[nCntWall].bUse == true)
			{//�g�p���Ă�����
				g_aMeshWall[nCntWall].rot.y += 0.05f;
			}
		}
	}
}

//==================================================================================
//���b�V���E�H�[���̕`�揈��
//==================================================================================
void DrawMeshWall(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	int nIdxPoint = 0;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//�g�p���Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshWall[nCntWall].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[nCntWall].rot.y, g_aMeshWall[nCntWall].rot.x, g_aMeshWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntWall].mtxWorld, &g_aMeshWall[nCntWall].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[nCntWall].pos.x, g_aMeshWall[nCntWall].pos.y, g_aMeshWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aMeshWall[nCntWall].mtxWorld, &g_aMeshWall[nCntWall].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[nCntWall].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshWall);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMeshWall);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aMeshWall[nCntWall].nNumVertex,
				nIdxPoint,
				g_aMeshWall[nCntWall].nNumIndex - 2);
		}

		nIdxPoint += g_aMeshWall[nCntWall].nNumIndex;	//����̃C���f�b�N�X�������Z
	}

}

//==================================================================================
//���b�V���E�H�[���̐ݒ菈��
//==================================================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Width, int Height)
{

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == false)
		{//�g�p���Ă��Ȃ�������

			g_aMeshWall[nCntWall].pos = pos;	//�ʒu
			g_aMeshWall[nCntWall].rot = rot;	//����
			g_aMeshWall[nCntWall].bUse = true;	//�g�p���Ă���󋵂ɂ���
			g_aMeshWall[nCntWall].nNumVertex = (Width + 1) * (Height + 1);	//���_��
			g_aMeshWall[nCntWall].nNumIndex = (Height * ((Width + 1) * 2)) + (2 * (Height - 1));	//�C���f�b�N�X��
			g_aMeshWall[nCntWall].nWidth = Width;
			g_aMeshWall[nCntWall].nHeight = Height;
			break;
		}
	}

}

//==================================================================================
//���b�V���E�H�[���̏��擾
//==================================================================================
MESHWALL *GetMeshWall(void)
{
	return &g_aMeshWall[0];
}

//==================================================================================
//���b�V���E�H�[���̒��_���擾
//==================================================================================
LPDIRECT3DVERTEXBUFFER9 GetBuffMeshWall(void)
{
	return g_pVtxBuffMeshWall;
}