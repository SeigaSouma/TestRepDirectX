//=============================================================================
//
// �J�������� [camera.cpp]
// Author : ���n�Ή�
//
//=============================================================================
//�C���N���[�h
#include "main.h"
#include "camera.h"
#include "input.h"
#include "model.h"
#include "player.h"

//�}�N��
#define MOVE			(5.0f)		//�ړ���
#define START_LENGTH	(560.0f)	//���̋���
#define MAX_LENGTH		(2000.0f)	//�ő勗��
#define MIN_LENGTH		(10.0f)		//�ŏ�����
#define ROT_MOVE		(0.03f)		//��]�ړ���
#define MIN_ROT			(-D3DX_PI * 0.499f)	//�J�����Œ�p
#define MAX_ROT			(D3DX_PI * 0.499f)	//�J�����Œ�p
#define BACKFOLLOW_TIME	(60)		//�w�ʕ␳�܂ł̎���
#define DIS_Y			(50.0f)

//�v���g�^�C�v�錾
void MoveCameraInput(void);
void MoveCameraVR(void);
void MoveCameraV(void);
void MoveCameraR(void);
void MoveCameraDistance(void);
void SetCameraV(void);
void SetCameraR(void);
void SetCameraRot(void);
void MoveCameraFollow(void);

//�O���[�o���ϐ��錾
Camera g_camera;	//�J�����̏��

//==================================================================================
//�J�����̏���������
//==================================================================================
void InitCamera(void)
{

	g_camera.posV = D3DXVECTOR3(0.0f, 200.0f, -400.0f);	//���_
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����_
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//������x�N�g��
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, -0.3f);		//����
	g_camera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	g_camera.fDistance = START_LENGTH;					//����
	g_camera.bFollow = false;							//�Ǐ]
	g_camera.bBackFollow = false;						//�w�ʒǏ]
	g_camera.nCntBackFollow = 0;						//�Ǐ]�J�E���g

	g_camera.posVDest = g_camera.posV;	//�ڕW�̎��_
	g_camera.posRDest = g_camera.posR;	//�ڕW�̒����_

	g_camera.rotVDest = g_camera.rot;	//�ڕW�̌���
	g_camera.rotVDiff = 0.0f;			//�ڕW�̎��_�̍���

}

//==================================================================================
//�J�����̏I������
//==================================================================================
void UninitCamera(void)
{

}

//==================================================================================
//�J�����̍X�V����
//==================================================================================
void UpdateCamera(void)
{

	//���_�E�����_�ړ�
	MoveCameraFollow();
	MoveCameraInput();
	MoveCameraVR();
	MoveCameraR();
	MoveCameraV();
	MoveCameraDistance();

	if (GetKeyboardTrigger(DIK_F6) == true)
	{//F6�������ꂽ,�w�ʒǏ]�؂�ւ�

		g_camera.bBackFollow = g_camera.bBackFollow ? false : true;
	}
	if (GetKeyboardTrigger(DIK_F7) == true)
	{//F7�������ꂽ,�Ǐ]�؂�ւ�

		g_camera.bFollow = g_camera.bFollow ? false : true;
	}

}

//==================================================================================
//���͋@��̃J�����ړ�
//==================================================================================
void MoveCameraInput(void)
{

	if (GetMousePress(MOUSE_BUTTON_LEFT) == true &&
		GetMousePress(MOUSE_BUTTON_RIGHT) == true)
	{//���E��������

		g_camera.move.x += (GetMouseMove().x * sinf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE) -
							(GetMouseMove().y * cosf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE);

		g_camera.move.z += (GetMouseMove().x * cosf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE) +
							(GetMouseMove().y * sinf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE);

		//�ړ��ʕ␳
		MoveCameraVR();

		//��]�̕␳
		SetCameraRot();

		//�����_�ݒ�
		SetCameraR();
	}
	else if (GetMousePress(MOUSE_BUTTON_LEFT) == true)
	{//���N���b�N���Ă�Ƃ�,���_��]

		g_camera.rot.y += GetMouseMove().x * ROT_MOVE;
		g_camera.rot.z += GetMouseMove().y * ROT_MOVE;

		//��]�̕␳
		SetCameraRot();

		//���_�̑������
		SetCameraV();

	}
	else if (GetMousePress(MOUSE_BUTTON_RIGHT) == true)
	{//�E�N���b�N���Ă�Ƃ�,�����_��]

		g_camera.rot.y += GetMouseMove().x * ROT_MOVE;
		g_camera.rot.z += GetMouseMove().y * ROT_MOVE;

		//��]�̕␳
		SetCameraRot();

		//�����_�̈ʒu�X�V
		SetCameraR();
	}

	//�}�E�X�z�C�[���ŋ�������
	g_camera.fDistance += GetMouseMove().z * (MOVE * 0.1f);
	MoveCameraDistance();

	//���_�̑������
	SetCameraV();

}

//==================================================================================
//�J�����̎��_�ړ�
//==================================================================================
void MoveCameraV(void)
{

	//���_�ړ�
	if (GetKeyboardPress(DIK_Y) == true)
	{//Y�L�[�������ꂽ,���_��ړ�

		g_camera.rot.z -= ROT_MOVE;
	}
	else if (GetKeyboardPress(DIK_N) == true)
	{//N�L�[�������ꂽ,���_���ړ�

		g_camera.rot.z += ROT_MOVE;
	}

	if (GetKeyboardPress(DIK_Z) == true)
	{//Z�L�[�������ꂽ

		g_camera.rot.y += ROT_MOVE;

	}
	if (GetKeyboardPress(DIK_C) == true)
	{//C�L�[�������ꂽ

		g_camera.rot.y -= ROT_MOVE;

	}

	//��]�̕␳
	SetCameraRot();

	//���_�̑������
	SetCameraV();

}

//==================================================================================
//�J�����̒����_�ړ�
//==================================================================================
void MoveCameraR(void)
{
	//����
	if (GetKeyboardPress(DIK_Q) == true)
	{//Q�L�[�������ꂽ,������

		
		g_camera.rot.y -= ROT_MOVE;

	}
	else if (GetKeyboardPress(DIK_E) == true)
	{//E�L�[�������ꂽ,������

		g_camera.rot.y += ROT_MOVE;
	}

	//�㏸���~
	if (GetKeyboardPress(DIK_T) == true)
	{//T�L�[�������ꂽ

		g_camera.rot.z += ROT_MOVE;

	}
	else if (GetKeyboardPress(DIK_B) == true)
	{//B�L�[�������ꂽ

		g_camera.rot.z -= ROT_MOVE;

	}

	//��]�̕␳
	SetCameraRot();

	//�����_�̈ʒu�X�V
	SetCameraR();
}

//==================================================================================
//�J�����̎��_�E�����_�ړ�
//==================================================================================
void MoveCameraVR(void)
{

	if (g_camera.bFollow == false)
	{//�Ǐ]��OFF�̎�����

		if (GetKeyboardPress(DIK_A) == true || XGetStickPressR(BUTTON_LX, 0) < 0)
		{//A�L�[�������ꂽ,���_���ړ�

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
			{//A+W,����ړ�

				g_camera.move.x += sinf(-D3DX_PI * MOVE_LRDW + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(-D3DX_PI * MOVE_LRDW + g_camera.rot.y) * MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
			{//A+S,�����ړ�

				g_camera.move.x += sinf(-D3DX_PI * MOVE_LRUP + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(-D3DX_PI * MOVE_LRUP + g_camera.rot.y) * MOVE;
			}
			else
			{//A,���ړ�

				g_camera.move.x += sinf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(-D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE;
			}

		}
		else if (GetKeyboardPress(DIK_D) == true || XGetStickPressR(BUTTON_LX, 0) > 0)
		{//D�L�[�������ꂽ,���_�E�ړ�

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
			{//D+W,�E��ړ�

				g_camera.move.x += sinf(D3DX_PI * MOVE_LRDW + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(D3DX_PI * MOVE_LRDW + g_camera.rot.y) * MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
			{//D+S,�E���ړ�

				g_camera.move.x += sinf(D3DX_PI * MOVE_LRUP + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(D3DX_PI * MOVE_LRUP + g_camera.rot.y) * MOVE;
			}
			else
			{//D,�E�ړ�

				g_camera.move.x += sinf(D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE;
				g_camera.move.z += cosf(D3DX_PI * MOVE_LR + g_camera.rot.y) * MOVE;
			}

		}
		else if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
		{//W�������ꂽ�A���ړ�

			g_camera.move.x += sinf(g_camera.rot.y) * MOVE;
			g_camera.move.z += cosf(g_camera.rot.y) * MOVE;
		}
		else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
		{//S�������ꂽ�A��O�ړ�

			g_camera.move.x += sinf(D3DX_PI + g_camera.rot.y) * MOVE;
			g_camera.move.z += cosf(D3DX_PI + g_camera.rot.y) * MOVE;
		}

	}

	//�ړ��ʕ������Z
	g_camera.posV.x += g_camera.move.x;
	g_camera.posV.z += g_camera.move.z;

	//�ړ��ʂ����Z�b�g
	g_camera.move.x = 0.0f;
	g_camera.move.z = 0.0f;

	//�����_�̈ʒu�X�V
	SetCameraR();
}

//==================================================================================
// ���_�E�����_�ԋ���
//==================================================================================
void MoveCameraDistance(void)
{
	if (GetKeyboardPress(DIK_U) == true)
	{//U�L�[�������ꂽ,�߂Â�

		g_camera.fDistance -= MOVE;
	}
	else if (GetKeyboardPress(DIK_M) == true)
	{//M�L�[�������ꂽ,�߂Â�

		g_camera.fDistance += MOVE;
	}

	//���E�l�␳
	if (g_camera.fDistance >= MAX_LENGTH)
	{
		g_camera.fDistance = MAX_LENGTH;
	}
	else if (g_camera.fDistance <= MIN_LENGTH)
	{
		g_camera.fDistance = MIN_LENGTH;
	}

}

//==================================================================================
//�J�����̎��_�������
//==================================================================================
void SetCameraV(void)
{

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	if (g_camera.bFollow == false)
	{//�Ǐ]���Ȃ��Ƃ�

		//���_�̑������
		g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.z) * sinf(g_camera.rot.y) * -g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.z) * cosf(g_camera.rot.y) * -g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y + sinf(g_camera.rot.z) * -g_camera.fDistance;
	}
	else
	{//�Ǐ]ON

		float fYcamera = 0.0f;

		if (pPlayer->pos.y >= 90.0f)
		{
			fYcamera = pPlayer->pos.y - 90.0f;
		}

		//���_�̑������
		g_camera.posVDest.x = (pPlayer->pos.x + sinf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_camera.rot.z) * sinf(g_camera.rot.y) * -g_camera.fDistance;
		g_camera.posVDest.z = (pPlayer->pos.z + cosf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_camera.rot.z) * cosf(g_camera.rot.y) * -g_camera.fDistance;
		g_camera.posVDest.y = fYcamera + DIS_Y + sinf(g_camera.rot.z) * -g_camera.fDistance;

		//�␳����
		g_camera.posV += (g_camera.posVDest - g_camera.posV) * 0.12f;
	}

}

//==================================================================================
//�J�����̒����_�������
//==================================================================================
void SetCameraR(void)
{

	if (g_camera.bFollow == false)
	{//�Ǐ]���Ȃ��Ƃ�

		//�����_�̑������
		g_camera.posR.x = g_camera.posV.x + cosf(g_camera.rot.z) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.z) * cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.y = g_camera.posV.y + sinf(g_camera.rot.z) * g_camera.fDistance;
	}
	else
	{//�Ǐ]ON

		//�����_�̑������
		g_camera.posRDest.x = g_camera.posV.x + cosf(g_camera.rot.z) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posRDest.z = g_camera.posV.z + cosf(g_camera.rot.z) * cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posRDest.y = g_camera.posV.y + sinf(g_camera.rot.z) * g_camera.fDistance;

		//�␳����
		g_camera.posR += (g_camera.posRDest - g_camera.posR) * 0.08f;

	}

}

//==================================================================================
//�J�����̉�]�␳
//==================================================================================
void SetCameraRot(void)
{
	//Y���̕␳
	if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y = -D3DX_PI;
	}
	else if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y = D3DX_PI;
	}

	//Z���̌Œ�
	if (g_camera.rot.z > MAX_ROT)
	{
		g_camera.rot.z = MAX_ROT;
	}
	else if (g_camera.rot.z < MIN_ROT)
	{
		g_camera.rot.z = MIN_ROT;
	}

}

//==================================================================================
//�J�����̔w�ʎ����Ǐ]
//==================================================================================
void MoveCameraFollow(void)
{

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();
	g_camera.rotVDest.y = (D3DX_PI + pPlayer->rot.y);

	if (g_camera.bBackFollow == true)
	{//�Ǐ]����Ƃ�

		if (pPlayer->move.x > 1.0f || pPlayer->move.x < -1.0f || pPlayer->move.z > 1.0f || pPlayer->move.z < -1.0f)
		{//�ړ����Ă�Ƃ�

			g_camera.nCntBackFollow = 0;	//�J�E���g���Z�b�g
		}
		else
		{
			g_camera.nCntBackFollow++;	//�J�E���g���Z

		}

		if (g_camera.nCntBackFollow > BACKFOLLOW_TIME)
		{//�w�ʕ␳�̃J�E���g���K��l��������

			//�␳����
			g_camera.rotVDiff = (g_camera.rotVDest.y - g_camera.rot.y);

			if (g_camera.rotVDiff > D3DX_PI)
			{
				g_camera.rotVDiff += (-D3DX_PI * 2.0f);
			}
			else if (g_camera.rotVDiff < -D3DX_PI)
			{
				g_camera.rotVDiff += (D3DX_PI * 2.0f);
			}

			//�����ŕ␳����
			g_camera.rot.y += g_camera.rotVDiff * 0.025f;

			if (g_camera.rot.y > D3DX_PI)
			{
				g_camera.rot.y += (-D3DX_PI * 2.0f);
			}
			else if (g_camera.rot.y < -D3DX_PI)
			{
				g_camera.rot.y += (D3DX_PI * 2.0f);
			}

			//��]�̕␳
			SetCameraRot();

			//�����_�̈ʒu�X�V
			SetCameraR();

			////���_�̑������
			SetCameraV();

			//���_�̑������
			g_camera.posV.x = g_camera.posR.x + cosf(g_camera.rot.z) * sinf(g_camera.rot.y) * -g_camera.fDistance;
			g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.z) * cosf(g_camera.rot.y) * -g_camera.fDistance;
			g_camera.posV.y = g_camera.posR.y + sinf(g_camera.rot.z) * -g_camera.fDistance;

		}

	}

}


//==================================================================================
//�J�����̐ݒ菈��
//==================================================================================
void SetCamera(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								D3DXToRadian(45.0f),	//����p
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//�A�X�y�N�g��
								10.0f,		//���s���̐���
								10000.0f);	//���s���̐���

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
						&g_camera.posV,	//���_
						&g_camera.posR,	//�����_
						&g_camera.vecU);//������x�N�g��

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

}

//==================================================================================
//�J�����̏��擾
//==================================================================================
Camera GetCamera(void)
{
	return g_camera;
}