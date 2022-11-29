//==================================================================================
//
//3D����[main.cpp]
//Author:���n�Ή�
//
//==================================================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "shadow.h"
#include "enemy.h"
#include "wall.h"
#include "billboard.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "meshdome.h"

//�}�N����`
#define CLASS_NAME			"WindowsClass"		//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME			"����"	//�E�C���h�E�̖��O(�L���v�V����)

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);
void DrawFPS(void);
void DrawControll(void);
void DrawPosCamera(void);
void DrawPosPlayer(void);
void DrawPosLight(void);
void DrawBillboardTest(void);

//�O���[�o���ϐ��錾
LPDIRECT3D9 g_pD3D = NULL;					//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		//Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont = NULL;					//�t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;						//FPS�J�E���^
MODE g_mode = MODE_TITLE;					//���݂̃��[�h

//==================================================================================
//���C���֐�
//==================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmbLine, int nCmbShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASS�̃������T�C�Y
		CS_CLASSDC,							//�E�C���h�E�̃X�^�C��
		WindowProc,							//�E�C���h�E�v���V�[�W��
		0,									//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,									//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,							//�C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),	//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL, IDC_ARROW),		//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			//�N���C�A���g�̈�̔w�i�F
		NULL,								//���j���[�o�[
		CLASS_NAME,							//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL, IDI_APPLICATION)		//�t�@�C���̃A�C�R��
	};

	HWND hWnd;								//�E�C���h�E�n���h��(���ʎq)
	MSG msg;								//���b�Z�[�W���i�[����ϐ�

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	//��ʃT�C�Y�̍\����

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�̐���
	hWnd = CreateWindowEx(
		0,									//�g���E�C���h�E�X�^�C��
		CLASS_NAME,							//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,						//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,				//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,						//�E�C���h�E�̍���X���W
		CW_USEDEFAULT,						//�E�C���h�E�̍���Y���W
		(rect.right - rect.left),			//�E�C���h�E�̕�
		(rect.bottom - rect.top),			//�E�C���h�E�̍���
		NULL,								//�e�E�C���h�E�̃n���h��
		NULL,								//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,							//�C���X�^���X�n���h��
		NULL								//�E�C���h�E�쐬�f�[�^
	);

	DWORD dwCurrentTime;					//���ݎ���
	DWORD dwExecLastTime;					//�Ō�ɏ�����������
	DWORD dwFrameCount;						//�t���[���J�E���g
	DWORD dwFPSLastTime;					//�Ō��FPS���v����������

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ

		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;					//����������
	dwExecLastTime = timeGetTime();		//���ݎ������擾(�ۑ�)

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmbShow);				//�E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);						//�N���C�A���g�̈���X�V

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//���b�Z�[�W���[�v
	while(1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Window�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);				//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);				//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���

			dwCurrentTime = timeGetTime();	//���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;		//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					//�t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;		//�����J�n�̎���[���ݎ���]��ۑ�

				//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;						//�t���[���J�E���g�����Z
			}
		}
	}

	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);
		
	return (int)msg.wParam;
}

//==================================================================================
//�E�C���h�E�v���V�[�W��
//==================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;	//�Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY:				//�E�C���h�E�j���̃��b�Z�[�W
		//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:				//ESC�L�[����

			////���b�Z�[�W�{�b�N�X��\��
			//nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			//if (nID == IDYES)
			//{
				//�E�C���h�E��j������(VK_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			//}
			break;
		}
		break;

	case WM_CLOSE:				//�o�c��
		//���b�Z�[�W�{�b�N�X��\��
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{
			//�E�C���h�E��j������(VK_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;			//0��Ԃ��ďI����h��
		}
		break;

	case WM_LBUTTONDOWN:		//�}�E�X���N���b�N
		//�ΏۃE�C���h�E�Ƀt�H�[�J�X�����킹��
		SetFocus(hWnd);
		break;

	case WM_TIMER:				//�^�C�}�[���s�̃��b�Z�[�W
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//����̏�����Ԃ�
}
//==================================================================================
//����������
//==================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�������[�h

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{//���s�����Ƃ�
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D -> GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{//���s�����Ƃ�
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));		//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;		//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;		//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;					//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;		//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//�f�o�C�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;					//�E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���
	if (FAILED(g_pD3D ->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//���߂郂�[�h

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//���͋@��̏���������
	if (FAILED (InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�e�̏���������
	InitShadow();

	//�|���S���̏���������
	InitPolygon();

	//�ǂ̏���������
	InitWall();

	//�J�����̏���������
	InitCamera();

	//���C�g�̏���������
	InitLight();

	//���f���̏���������
	InitModel();

	//�G�̏���������
	InitEnemy();

	//�v���C���[�̏���������
	InitPlayer();

	//�r���{�[�h�̏���������
	InitBillboard();

	//�e�̏���������
	InitBullet();

	//�����̏���������
	InitExplosion();

	//�f�o�b�O�\���̏���������
	InitDebugProc();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	//���b�V���E�H�[���̏���������
	InitMeshWall();

	//���b�V���V�����_�[�̏���������
	InitMeshCylinder();

	//���b�V���E�H�[���̏���������
	InitMeshDome();

	//�����̎��ݒ�
	srand((unsigned int)time(0));

	return S_OK;
}
//==================================================================================
//�I������
//==================================================================================
void Uninit(void)
{

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//���͋@��̏I������
	UninitInput();

	//�|���S���̏I������
	UninitPolygon();

	//�ǂ̏I������
	UninitWall();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//���f���̏I������
	UninitModel();

	//�e�̏I������
	UninitShadow();

	//�G�̏I������
	UninitEnemy();

	//�v���C���[�̏I������
	UninitPlayer();

	//�r���{�[�h�̏I������
	UninitBillboard();

	//�e�̏I������
	UninitBullet();

	//�����̏I������
	UninitExplosion();

	//�f�o�b�O�\���̏I������
	UninitDebugProc();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	//���b�V���E�H�[���̏I������
	UninitMeshWall();

	//���b�V���V�����_�[�̏I������
	UninitMeshCylinder();

	//���b�V���E�H�[���̏I������
	UninitMeshDome();


	//�f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}
//==================================================================================
//�X�V����
//==================================================================================
void Update(void)
{


	if (GetKeyboardPress(DIK_F2) == true)
	{//F9�������ꂽ,Z�e�X�g�؂�ւ�

		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//���߂郂�[�h
	}
	else
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//���߂郂�[�h
	}

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�p�[�e�B�N���̍X�V����
	UpdateParticle();

	//���͋@��̍X�V����
	UpdateInput();

	//�|���S���̍X�V����
	UpdatePolygon();

	//�ǂ̍X�V����
	UpdateWall();

	//�r���{�[�h�̍X�V����
	UpdateBillboard();

	//�J�����̍X�V����
	UpdateCamera();

	//���C�g�̍X�V����
	UpdateLight();

	//���f���̍X�V����
	UpdateModel();

	//�e�̍X�V����
	UpdateShadow();

	//�G�̍X�V����
	UpdateEnemy();

	//�v���C���[�̍X�V����
	UpdatePlayer();

	//�e�̍X�V����
	UpdateBullet();

	//�����̍X�V����
	UpdateExplosion();

	//�f�o�b�O�\���̍X�V����
	UpdateDebugProc();

	//���b�V���t�B�[���h�̍X�V����
	UpdateMeshField();

	//���b�V���E�H�[���̍X�V����
	UpdateMeshWall();

	//���b�V���V�����_�[�̍X�V����
	UpdateMeshCylinder();

	//���b�V���E�H�[���̍X�V����
	UpdateMeshDome();

	
}
//==================================================================================
//�`�揈��
//==================================================================================
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear
	(	0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0
	);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice -> BeginScene()))
	{//�`�悪���������Ƃ�

		//�J�����̐ݒ菈��
		SetCamera();

		//�|���S���̕`�揈��
		//DrawPolygon();

		//���b�V���t�B�[���h�̕`�揈��
		DrawMeshField();

		//���b�V���V�����_�[�̕`�揈��
		DrawMeshCylinder();

		//���b�V���E�H�[���̕`�揈��
		DrawMeshWall();

		//���b�V���E�H�[���̕`�揈��
		DrawMeshDome();


		//�ǂ̕`�揈��
		DrawWall();

		//���f���̕`�揈��
		DrawModel();

		//�e�̕`�揈��
		DrawShadow();

		//�G�̕`�揈��
		DrawEnemy();

		//�v���C���[�̕`�揈��
		DrawPlayer();

		//�r���{�[�h�̕`�揈��
		DrawBillboard();

		//�e�̕`�揈��
		DrawBullet();

		//�����̕`�揈��
		DrawExplosion();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�p�[�e�B�N���̕`�揈��
		DrawParticle();

		//�f�o�b�O�\�L
		DrawDebug();

		//�f�o�b�O�\���̕`�揈��
		DrawDebugProc();

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==================================================================================
//���[�h�̐ݒ�
//==================================================================================
void SetMode(MODE mode)
{
	//���݂̉��(���[�h)�̏I������
	//switch (g_mode)
	{
	

	}

	//�V�������(���[�h)�̏���������
	//switch (mode)
	{
	

	}

	g_mode = mode;		//���[�h�̐ݒ�
}

//==================================================================================
//���[�h�̎擾
//==================================================================================
MODE GetMode(void)
{
	return g_mode;
}

//==================================================================================
//�p�x�̐��K������
//==================================================================================
float RotNormalize(float fRot)
{
	//1�����z������␳����
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2.0f;
	}
	else if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2.0f;
	}

	return fRot;
}

//==================================================================================
// �I�u�W�F�N�g�Ԃ̋����Z�o
//==================================================================================
float CircleRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float fLength = (pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z);

	return fLength;
}

//==================================================================================
//�f�o�C�X�̎擾
//==================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==================================================================================
//�t�H���g�̎擾
//==================================================================================
LPD3DXFONT GetFont(void)
{
	return g_pFont;
}

//==================================================================================
//�f�o�b�O�\�L
//==================================================================================
void DrawDebug(void)
{

//#ifdef _DEBUG

	//FPS�\��
	DrawFPS();

	//�������
	DrawControll();

	//�J�����̈ʒu���
	DrawPosCamera();

	//���f���̏��
	DrawPosPlayer();

	//���C�g�̏��
	DrawPosLight();

	DrawBillboardTest();

//#endif
}

//==================================================================================
//FPS�̕\��
//==================================================================================
void DrawFPS(void)
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char aStr[256];

	//������ɑ��
	wsprintf(&aStr[0],	"FPS�F%d\n", g_nCountFPS);

	//�e�L�X�g�̕`�揈��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//==================================================================================
//������@�̕\��
//==================================================================================
void DrawControll(void)
{
	RECT rect = { 0, 150, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//������ɑ��
	wsprintf(&aStr[0], "------------- �J�����ړ� -------------\n"
		"�����_�ړ����@�@�@�@ [Y/N/Z/C]\n"
		"�������_�ړ����@�@�@ [T/B/Q/E]\n"
		"�����_ & �����_�ړ���[W/A/S/D]\n\n"
		"���_�ԋ����ړ��@ �@�@�@�@ �F[U/M]\n"
		"���J�����̈ʒu��񃊃Z�b�g�F[SPACE]\n\n");

	//�e�L�X�g�̕`�揈��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

}

//==================================================================================
// �J�����̈ʒu���\��
//==================================================================================
void DrawPosCamera(void)
{
	RECT rect = { 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//�J�����̏��擾
	Camera aCamera = GetCamera();

	//������ɑ��
	sprintf(&aStr[0],
		"------------- �J�����̈ʒu��� -------------\n"
		"�����_���@�@�@�@�@�@�@(%.2f, %.2f, %.2f)\n"
		"�������_���@�@�@�@�@�@(%.2f, %.2f, %.2f)\n"
		"���J�����̌������@�@�@(%.2f, %.2f, %.2f))\n"
		"�����_�ƒ����_�̋�����(%.2f)", aCamera.posV.x, aCamera.posV.y, aCamera.posV.z, aCamera.posR.x, aCamera.posR.y, aCamera.posR.z, aCamera.rot.x, aCamera.rot.y, aCamera.rot.z, aCamera.fDistance);

	//�e�L�X�g�̕`�揈��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//==================================================================================
// �v���C���[�̈ʒu���\��
//==================================================================================
void DrawPosPlayer(void)
{
	RECT rect = { 0, 300, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[512];

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	Model aModel = GetModel();

	//������ɑ��
	sprintf(&aStr[0], "------------- ���f����� -------------\n"
		"���v���C���[���W��(%.2f, %.2f, %.2f)\n"
		"���v���C���[������(%.2f, %.2f, %.2f)\n"
		"���v���C���[�ړ���[W/A/S/D]�@�@�@���Ǐ]��Ԃ�\n"
		"�v���C���[���ӂɃ��f���z�u�F[F2]\n"
		"���f��[X�F%.2f, %.2f  Y�F%.2f, %.2f  Z�F%.2f, %.2f}]", pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z, pPlayer->rot.x, pPlayer->rot.y, pPlayer->rot.z,
		aModel.vtxMin.x, aModel.vtxMax.x, aModel.vtxMin.y, aModel.vtxMax.y, aModel.vtxMin.z, aModel.vtxMax.z);

	//�e�L�X�g�̕`�揈��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//==================================================================================
// ���C�g�̏��
//==================================================================================
void DrawPosLight(void)
{
	RECT rect = { 0, 420, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//���C�g�̏��
	D3DLIGHT9 *pLight = GetLight();
	D3DXVECTOR3 *pLightDir = GetLightDir();

	//������ɑ��
	wsprintf(&aStr[0], "------------- ���C�g��� -------------\n");

	//�e�L�X�g�̕`�揈��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//���`��
	char aStrLight[256];

	for (int nCount = 0; nCount < MAX_LIGHT; nCount++, pLightDir++, pLight++)
	{
		rect = { 0, 440 + 20 * nCount, SCREEN_WIDTH, SCREEN_HEIGHT };

		//������ɑ��
		sprintf(&aStrLight[0], "�����C�g%d�������F(%.2f, %.2f, %.2f)�@�g�U���F(%.2f, %.2f, %.2f)\n", nCount,
			pLightDir->x, pLightDir->y, pLightDir->z,
			pLight->Diffuse.r, pLight->Diffuse.g, pLight->Diffuse.b);

		//�e�L�X�g�̕`�揈��
		g_pFont->DrawText(NULL, &aStrLight[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	}

}

//==================================================================================
// �e��ݒ�
//==================================================================================
void DrawBillboardTest(void)
{
	RECT rect = { 0, 520, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	bool bZTest = GetZTest();
	bool bAlphaTest = GetAlphaTest();
	Camera aCamera = GetCamera();
	bool bEffectUse = GetEffectUse();

	char aBackFollow[256], aFollow[256], aZStr[256], aAlphaStr[256], aEffectUse[256];

	//�w�ʒǏ]����
	if (aCamera.bBackFollow == true){ strcpy(&aBackFollow[0], "ON"); }
	else{ strcpy(&aBackFollow[0], "OFF"); }

	//�Ǐ]����
	if (aCamera.bFollow == true){ strcpy(&aFollow[0], "ON"); }
	else{ strcpy(&aFollow[0], "OFF"); }

	//Z�e�X�g
	if (bZTest == false) { strcpy(&aZStr[0], "ON"); }
	else { strcpy(&aZStr[0], "OFF"); }

	//�A���t�@�e�X�g
	if (bAlphaTest == true) { strcpy(&aAlphaStr[0], "ON"); }
	else { strcpy(&aAlphaStr[0], "OFF"); }

	//�G�t�F�N�g
	if (bEffectUse == true) { strcpy(&aEffectUse[0], "ON"); }
	else { strcpy(&aEffectUse[0], "OFF"); }

	//������ɑ��
	wsprintf(&aStr[0], "------------- �e��ݒ� -------------\n"
		"�G�t�F�N�g�@�@�F[F5]�y %s �z\n"
		"�w�ʒǏ]�@�@�@�F[F6]�y %s �z\n"
		"�Ǐ]�J����	�@ �F[F7]�y %s �z\n"
		"Z�e�X�g �@�@�@�F[F8]�y %s �z\n"
		"�A���t�@�e�X�g�F[F9]�y %s �z\n", aEffectUse, aBackFollow, aFollow, aZStr, aAlphaStr);

	//�e�L�X�g�̕`�揈��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
