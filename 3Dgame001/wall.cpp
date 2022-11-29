//=============================================================================
//
// �Ǐ��� [wall.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "wall.h"
#include "meshwall.h"
#include "input.h"

//�}�N����`
#define MOVE		(0.0f)
#define MAX_TOP		(4)

//�O���[�o���ϐ��錾
const char *c_apFilenameWall[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\Wall2.png",
};
LPDIRECT3DTEXTURE9 g_apTextureWall[(sizeof c_apFilenameWall) / sizeof(*c_apFilenameWall)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//���_�o�b�t�@�ւ̃|�C���^
WALL g_aWall[MAX_WALL];									//�ǂ̏��

//==================================================================================
//�ǂ̏���������
//==================================================================================
void InitWall(void)
{

	////�f�o�C�X�̎擾
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////�e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\TEXTURE\\wall000.jpg",
	//	&g_pTextureWall);

	////�e�v�f������
	//for (int nCount = 0; nCount < MAX_WALL; nCount++)
	//{
	//	g_aWall[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	g_aWall[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	g_aWall[nCount].bUse = false;
	//}

	////���_�o�b�t�@�̐���
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_WALL,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_3D,
	//	D3DPOOL_MANAGED,
	//	&g_pVtxBuffWall,
	//	NULL);

	//VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	////���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCount = 0; nCount < MAX_WALL; nCount++)
	//{
	//	//���_���W�̐ݒ�
	//	pVtx[0].pos = D3DXVECTOR3(-1000.0f, 1000.0f, 0.0f);
	//	pVtx[1].pos = D3DXVECTOR3(1000.0f, 1000.0f, 0.0f);
	//	pVtx[2].pos = D3DXVECTOR3(-1000.0f, 0.0f, 0.0f);
	//	pVtx[3].pos = D3DXVECTOR3(1000.0f, 0.0f, 0.0f);

	//	//�@���x�N�g���̐ݒ�
	//	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);


	//	//���_�J���[�̐ݒ�
	//	pVtx[0].col =
	//	pVtx[1].col =
	//	pVtx[2].col =
	//	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//	//�e�N�X�`�����W�̐ݒ�
	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	pVtx += MAX_TOP;
	//}

	////���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffWall->Unlock();

	//SetWall(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

}

//==================================================================================
//�ǂ̏I������
//==================================================================================
void UninitWall(void)
{
	////�e�N�X�`���̔j��
	//if (g_pTextureWall != NULL)
	//{
	//	g_pTextureWall->Release();
	//	g_pTextureWall = NULL;
	//}

	////���_�o�b�t�@�̔j��
	//if (g_pVtxBuffWall != NULL)
	//{
	//	g_pVtxBuffWall->Release();
	//	g_pVtxBuffWall = NULL;
	//}
}

//==================================================================================
//�ǂ̍X�V����
//==================================================================================
void UpdateWall(void)
{
	//if (GetKeyboardPress(DIK_1) == true)
	//{//���L�[�������ꂽ,���ړ�

	//	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	//	{
	//		if (g_aWall[nCntWall].bUse == true)
	//		{//�g�p���Ă�����
	//			g_aWall[nCntWall].rot.y += 0.05f;
	//		}
	//	}
	//}
}

//==================================================================================
//�ǂ̕`�揈��
//==================================================================================
void DrawWall(void)
{

	////�f�o�C�X�̎擾
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾

	//for (int nCount = 0; nCount < MAX_WALL; nCount++)
	//{
	//	if (g_aWall[nCount].bUse == true)
	//	{//�g�p���Ă�����

	//		//���[���h�}�g���b�N�X�̏�����
	//		D3DXMatrixIdentity(&g_aWall[nCount].mtxWorld);

	//		//�����𔽉f����
	//		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCount].rot.y, g_aWall[nCount].rot.x, g_aWall[nCount].rot.z);
	//		D3DXMatrixMultiply(&g_aWall[nCount].mtxWorld, &g_aWall[nCount].mtxWorld, &mtxRot);

	//		//�ʒu�𔽉f����
	//		D3DXMatrixTranslation(&mtxTrans, g_aWall[nCount].pos.x, g_aWall[nCount].pos.y, g_aWall[nCount].pos.z);
	//		D3DXMatrixMultiply(&g_aWall[nCount].mtxWorld, &g_aWall[nCount].mtxWorld, &mtxTrans);

	//		//���[���h�}�g���b�N�X�̐ݒ�
	//		pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCount].mtxWorld);

	//		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//		pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

	//		//���_�t�H�[�}�b�g�̐ݒ�
	//		pDevice->SetFVF(FVF_VERTEX_3D);

	//		//�e�N�X�`���̐ݒ�
	//		pDevice->SetTexture(0, g_pTextureWall);

	//		//�ǂ̕`��
	//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);

	//	}

	//}

}

//==================================================================================
//�ǂ̐ݒ菈��
//==================================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{

	//VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	////���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCount = 0; nCount < MAX_WALL; nCount++)
	//{
	//	if (g_aWall[nCount].bUse == false)
	//	{//�g�p���Ă��Ȃ�������

	//		g_aWall[nCount].pos = pos;	//�ʒu
	//		g_aWall[nCount].rot = rot;	//����

	//		g_aWall[nCount].bUse = true;	//�g�p���Ă���󋵂ɂ���

	//		break;
	//	}

	//	pVtx += MAX_TOP;
	//}

	////���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffWall->Unlock();

}

//==================================================================================
//�ǂ̏��擾
//==================================================================================
//WALL *GetWall(void)
//{
	//return &g_aWall[0];
//}