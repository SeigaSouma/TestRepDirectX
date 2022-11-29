//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : ���n�Ή�
//
//=============================================================================
//�C���N���[�h
#include "main.h"
#include "light.h"
#include "input.h"

//�O���[�o���ϐ��錾
D3DLIGHT9 g_light[MAX_LIGHT];	//���C�g�̏��
D3DXVECTOR3 g_vecDir[MAX_LIGHT];//�ݒ�p�����x�N�g��

//==================================================================================
//���C�g�̏���������
//==================================================================================
void InitLight(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR3 vecDir;	//�ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{

		//���C�g�̏����N���A����
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntLight)
		{
		case 0:
			//���C�g�̊g�U����ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���C�g�̕�����ݒ�
			g_vecDir[nCntLight] = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			break;

		case 1:
			//���C�g�̊g�U����ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			//���C�g�̕�����ݒ�
			g_vecDir[nCntLight] = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			break;

		case 2:
			//���C�g�̊g�U����ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			//���C�g�̕�����ݒ�
			g_vecDir[nCntLight] = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			break;
		}

		D3DXVec3Normalize(&g_vecDir[nCntLight], &g_vecDir[nCntLight]);	//�x�N�g���𐳋K������(1.0�ɂ���)
		g_light[nCntLight].Direction = g_vecDir[nCntLight];

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);

	}


}

//==================================================================================
//���C�g�̏I������
//==================================================================================
void UninitLight(void)
{

}

//==================================================================================
//���C�g�̍X�V����
//==================================================================================
void UpdateLight(void)
{

}

//==================================================================================
//���C�g�̏��擾
//==================================================================================
D3DLIGHT9 *GetLight(void)
{
	return &g_light[0];
}

//==================================================================================
// ���C�g�̊g�U��
//==================================================================================
D3DXVECTOR3 *GetLightDir(void)
{
	return &g_vecDir[0];
}