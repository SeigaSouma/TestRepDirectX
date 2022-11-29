//=============================================================================
//
// �o���b�g���� [bullet.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "meshwall.h"
#include "wall.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "enemy.h"
#include "player.h"

//�}�N����`
#define MAX_TOP		(4)		//���_��
#define MOVE		(8.0f)	//�e�̈ړ���
#define DEF_LIFE	(120)	//���̎���
#define DMG			(1)		//�U����

//�v���g�^�C�v�錾
void CollisionWall(int nCntBullet);
void CollisionEnemy(int nCntBullet);
void CollisionPlayer(int nCntBullet);

//�O���[�o���ϐ��錾
const char *c_apFilenameBullet[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\bullet000.png",
};
LPDIRECT3DTEXTURE9 g_pTextureBullet[(sizeof c_apFilenameBullet) / sizeof(*c_apFilenameBullet)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];						//�e�̏��

//==================================================================================
//�e�̏���������
//==================================================================================
void InitBullet(void)
{
	
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameBullet) / sizeof(*c_apFilenameBullet); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBullet[nCntTex],
			&g_pTextureBullet[nCntTex]);

	}

	//�e�̏��̏�����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;	//�O��̈ʒu
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
		g_aBullet[nCntBullet].nIdxShadow = -1;						//�e�̃C���f�b�N�X�ԍ�
		g_aBullet[nCntBullet].nType = BULLETTYPE_PLAYER;			//�e�̎��
		g_aBullet[nCntBullet].bUse = false;							//�g�p���Ă��Ȃ���Ԃɂ���
	}


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_BULLET,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffBullet,
								NULL);

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-POS_BULLET, +POS_BULLET, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+POS_BULLET, +POS_BULLET, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-POS_BULLET, -POS_BULLET, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+POS_BULLET, -POS_BULLET, 0.0f);

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

		pVtx += MAX_TOP;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//==================================================================================
//�e�̏I������
//==================================================================================
void UninitBullet(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameBullet) / sizeof(*c_apFilenameBullet); nCount++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBullet[nCount] != NULL)
		{
			g_pTextureBullet[nCount]->Release();
			g_pTextureBullet[nCount] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//==================================================================================
//�e�̍X�V����
//==================================================================================
void UpdateBullet(void)
{

	//�J�����̏��擾
	Camera aCamera = GetCamera();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{

		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă�����

			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos);

			SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f), POS_BULLET * 1.5f, 10,
				-g_aBullet[nCntBullet].move * 0.01f, MOVEEFFECT_SUB, EFFECTTYPE_NORMAL);

			//�p�[�e�B�N���̐ݒ菈��
			SetParticle(g_aBullet[nCntBullet].pos, PARTICLE_TYPE_FIRE);

			//�ǂƂ̓����蔻��
			CollisionWall(nCntBullet);

			//�e�̈ʒu�̍X�V
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;

			if (g_aBullet[nCntBullet].nType != BULLETTYPE_ENEMY)
			{//�e�̎�ނ��G����Ȃ�������

				//�G�ƒe�̓����蔻��
				CollisionEnemy(nCntBullet);
			}
			else if (g_aBullet[nCntBullet].nType != BULLETTYPE_PLAYER)
			{//�e�̎�ނ��v���C���[����Ȃ�������

				//�v���C���[�ƒe�̓����蔻��
				CollisionPlayer(nCntBullet);
			}

			//�����̃J�E���g�_�E��
			g_aBullet[nCntBullet].nLife--;

			if (g_aBullet[nCntBullet].nLife <= 0)
			{//�������s�����ꍇ

				g_aBullet[nCntBullet].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
				SetEnableShadow(g_aBullet[nCntBullet].nIdxShadow);	//�e����
				SetExplosion(g_aBullet[nCntBullet].pos);			//�����Ăяo��

			}
			
		}

	}
}

//==================================================================================
//�e�̕`�揈��
//==================================================================================
void DrawBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p

	
	 //�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCount = 0; nCount < MAX_BULLET; nCount++)
	{

		if (g_aBullet[nCount].bUse == true)
		{//�g�p����Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCount].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBullet[nCount].mtxWorld, NULL, &mtxView);	//�t�z������߂�
			g_aBullet[nCount].mtxWorld._41 = 0.0f;
			g_aBullet[nCount].mtxWorld._42 = 0.0f;
			g_aBullet[nCount].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCount].pos.x, g_aBullet[nCount].pos.y, g_aBullet[nCount].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCount].mtxWorld, &g_aBullet[nCount].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCount].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet[0]);

			//�r���{�[�h�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);

		}

	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==================================================================================
//�e�̐ݒ菈��
//==================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType)
{

	//���f���̏����擾
	Model aModel = GetModel();

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{

		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ�������

			g_aBullet[nCntBullet].pos = pos;	//�ʒu
			g_aBullet[nCntBullet].nIdxShadow = SetShadow(POS_BULLET, POS_BULLET);	//�e��ݒ�
			g_aBullet[nCntBullet].move = move * MOVE;	//�ړ���
			g_aBullet[nCntBullet].nLife = DEF_LIFE;		//����
			g_aBullet[nCntBullet].nType = nType;	//�e�̎��
			g_aBullet[nCntBullet].bUse = true;		//�g�p���Ă����ԂɕύX

			break;
		}

		pVtx += MAX_TOP;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();

}

//==================================================================================
// �e�ƕǂ̓����蔻��
//==================================================================================
void CollisionWall(int nCntBullet)
{

	//�ǂ̏��擾
	MESHWALL *pMeshWall = GetMeshWall();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pMeshWall++)
	{//�ǂ̐����J��Ԃ�

		if (pMeshWall->bUse == true)
		{//�ǂ��g�p����Ă���Ƃ�

			//�ǂ̕�
			float fLen = (pMeshWall->nWidth * POS_MESHWALL) * 0.5f;

			D3DXVECTOR3 vecLine;
			vecLine.x = (pMeshWall->pos.x + cosf(pMeshWall->rot.y) * fLen) - (pMeshWall->pos.x + cosf(pMeshWall->rot.y) * -fLen);
			vecLine.z = (pMeshWall->pos.z + sinf(pMeshWall->rot.y) * -fLen) - (pMeshWall->pos.z + sinf(pMeshWall->rot.y) * fLen);

			D3DXVECTOR3 vecToPos;
			vecToPos.x = g_aBullet[nCntBullet].pos.x - (pMeshWall->pos.x + cosf(pMeshWall->rot.y) * -fLen);
			vecToPos.z = g_aBullet[nCntBullet].pos.z - (pMeshWall->pos.z + sinf(pMeshWall->rot.y) * fLen);

			if((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0)
			{//�ǂɓ���������

				g_aBullet[nCntBullet].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
				SetEnableShadow(g_aBullet[nCntBullet].nIdxShadow);	//�e����
				SetExplosion(g_aBullet[nCntBullet].pos);			//�����Ăяo��
			}

		}

	}

}

//==================================================================================
//�G�ƒe�̓����蔻��
//==================================================================================
void CollisionEnemy(int nCntBullet)
{
	//�G�̏��擾
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{//�G�̐����J��Ԃ�

		if (pEnemy->bUse == true)
		{//�G���g�p����Ă�����

			if (
				g_aBullet[nCntBullet].pos.x - POS_BULLET < pEnemy->pos.x + pEnemy->vtxMax.x &&
				g_aBullet[nCntBullet].pos.x + POS_BULLET > pEnemy->pos.x + pEnemy->vtxMin.x &&
				g_aBullet[nCntBullet].pos.z - POS_BULLET < pEnemy->pos.z + pEnemy->vtxMax.z &&
				g_aBullet[nCntBullet].pos.z + POS_BULLET > pEnemy->pos.z + pEnemy->vtxMin.z &&
				g_aBullet[nCntBullet].pos.y - POS_BULLET < pEnemy->pos.y + pEnemy->vtxMax.y &&
				g_aBullet[nCntBullet].pos.y + POS_BULLET > pEnemy->pos.y + pEnemy->vtxMin.y
				)
			{//�e���G�ɓ���������

				g_aBullet[nCntBullet].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
				SetEnableShadow(g_aBullet[nCntBullet].nIdxShadow);	//�e����
				SetExplosion(g_aBullet[nCntBullet].pos);			//�����Ăяo��

				//�G�̃q�b�g����
				HitEnemy(nCntEnemy, D3DXVECTOR3(g_aBullet[nCntBullet].move.x, 10.0f, g_aBullet[nCntBullet].move.z), DMG);


			}

		}
	}

}

//==================================================================================
//�v���C���[�ƒe�̓����蔻��
//==================================================================================
void CollisionPlayer(int nCntBullet)
{
	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	if (pPlayer->bUse == true)
	{//�v���C���[���g�p����Ă�����

		if (
			g_aBullet[nCntBullet].pos.x - POS_BULLET < pPlayer->pos.x + pPlayer->fRadius &&
			g_aBullet[nCntBullet].pos.x + POS_BULLET > pPlayer->pos.x - pPlayer->fRadius &&
			g_aBullet[nCntBullet].pos.z - POS_BULLET < pPlayer->pos.z + pPlayer->fRadius &&
			g_aBullet[nCntBullet].pos.z + POS_BULLET > pPlayer->pos.z - pPlayer->fRadius &&
			g_aBullet[nCntBullet].pos.y - POS_BULLET < pPlayer->pos.y + pPlayer->fRadius &&
			g_aBullet[nCntBullet].pos.y + POS_BULLET > pPlayer->pos.y - pPlayer->fRadius
			)
		{//�e���v���C���[�ɓ���������

			g_aBullet[nCntBullet].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
			SetEnableShadow(g_aBullet[nCntBullet].nIdxShadow);	//�e����
			SetExplosion(g_aBullet[nCntBullet].pos);			//�����Ăяo��

			//�v���C���[�̃q�b�g����
			HitPlayer(D3DXVECTOR3(g_aBullet[nCntBullet].move.x, 10.0f, g_aBullet[nCntBullet].move.z), DMG);
		}
	}

}

//==================================================================================
//�e�̏��擾
//==================================================================================
Bullet *GetBullet(void)
{
	return &g_aBullet[0];
}
