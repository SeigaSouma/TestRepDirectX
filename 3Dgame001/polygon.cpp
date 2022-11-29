//=============================================================================
//
// �|���S������ [polygon.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "polygon.h"
#include "input.h"

//�}�N����`
#define POS_POLYGON	(1000.0f)
#define MOVE		(0.0f)
#define MAX_TOP		(4)

//�O���[�o���ϐ��錾
const char *c_apFilenamePolygon[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\Polygon2.png",
};
//LPDIRECT3DTEXTURE9 g_apTexturePolygon[(sizeof c_apFilenamePolygon) / sizeof(*c_apFilenamePolygon)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	//���_�o�b�t�@�ւ̃|�C���^
POLYGON g_aPolygon;									//�|���S���̏��

//==================================================================================
//�|���S���̏���������
//==================================================================================
void InitPolygon(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\field002.jpg",
		&g_pTexturePolygon);

	//�e�v�f������
	g_aPolygon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPolygon.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-POS_POLYGON, 0.0f, POS_POLYGON);
	pVtx[1].pos = D3DXVECTOR3(POS_POLYGON, 0.0f, POS_POLYGON);
	pVtx[2].pos = D3DXVECTOR3(-POS_POLYGON, 0.0f, -POS_POLYGON);
	pVtx[3].pos = D3DXVECTOR3(POS_POLYGON, 0.0f, -POS_POLYGON);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//==================================================================================
//�|���S���̏I������
//==================================================================================
void UninitPolygon(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//==================================================================================
//�|���S���̍X�V����
//==================================================================================
void UpdatePolygon(void)
{

}

//==================================================================================
//�|���S���̕`�揈��
//==================================================================================
void DrawPolygon(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aPolygon.mtxWorld);

	//�����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPolygon.rot.y, g_aPolygon.rot.x, g_aPolygon.rot.z);
	D3DXMatrixMultiply(&g_aPolygon.mtxWorld, &g_aPolygon.mtxWorld, &mtxRot);

	//�ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, g_aPolygon.pos.x, g_aPolygon.pos.y, g_aPolygon.pos.z);
	D3DXMatrixMultiply(&g_aPolygon.mtxWorld, &g_aPolygon.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aPolygon.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePolygon);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}