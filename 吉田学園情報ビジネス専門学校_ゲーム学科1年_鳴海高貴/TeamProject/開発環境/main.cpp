//=============================================================================
//
// ���C������ [main.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "main.h"
#include <stdio.h>
#include "input.h"
#include "light.h"
#include "load.h"
#include "player.h"
#include "camera.h"
#include "polygon.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "pause.h"
#include "fade.h"
#include "disc.h"
#include "goal.h"
#include "billboard.h"
#include "tutorial.h"
#include "sound.h"

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------

LPDIRECT3D9		g_pD3D = NULL;						//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;			//Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont = NULL;							//�t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;								//FPS�J�E���^
int g_nCheck[8][8] = { { NULL } };
int g_nCntHil[2];
MODE g_mode = MODE_TITLE;							//���݂̃��[�h

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//*****************************************************************************
//���C���֐�
//*****************************************************************************

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,								//�E�C���h�E�̃X�^�C��
		WindowProc,								//�E�C���h�E�v���V�[�W��
		0,										//0�ɂ���i�ʏ�͎g�p���Ȃ��j
		0,										//0�ɂ���i�ʏ�͎g�p���Ȃ��j
		hInstance,								//�C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),		//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),				//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),				//�N���C�A���g�̈�̔w�i�F
		NULL,									//���j���[�o�[
		CLASS_NAME,								//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)			//�t�@�C���̃A�C�R��
	};
	DWORD dwCurrentTime;		//���ݎ���

	DWORD dwExecLastTime;		//�Ō�ɏ�����������

	DWORD dwFrameCount;			//�t���[���J�E���g

	DWORD dwFPSLastTime;		//�Ō��FPS���v����������


	HWND hWnd;		//�E�C���h�n���h���i���ʎq�j
	MSG msg;		//���b�Z�[�W���i�[����ϐ�

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };//��ʍ\����

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);


	//�N���C�A���g�̈���w��T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(
		0,						//�g���E�C���h�E�X�^�C��
		CLASS_NAME,				//�E�C���h�E�N���X�̖��O
		WINDOW_NAWE,			//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,	//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,			//�E�C���h�E�̍���X���W
		CW_USEDEFAULT,			//�E�C���h�E�̍���Y���W
		(rect.right - rect.left),//�E�C���h�E�̕�
		(rect.bottom - rect.top),//�E�C���h�E�̍���
		NULL,					//�e�E�C���h�E�̃n���h��
		NULL,					//���j���[�n���h���܂��͎q�E�B���h�E��ID
		hInstance,
		NULL);

	//����������FALSE
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������sTRUE
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();
	dwCurrentTime = 0;				//����������
	dwExecLastTime = timeGetTime(); //���ݎ������擾�i�ۑ��j

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);		//�E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);				//�N���C�A���g�̈���X�V

									//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();		//���ݎ������擾
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
				//FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;			//FPS���v������������ۑ�
				dwFrameCount = 0;						//�t���[���J�E���g���N���A
			}
			if ((dwCurrentTime - dwExecLastTime)>=(1000/60))
			{//120����1�b�o��
				dwExecLastTime = dwCurrentTime;		//�����J�n�̎���[���ݎ���]��ۑ�

				//�X�V����
				Update();
				

				//�`�揈��
				Draw();

				dwFrameCount++;		//�t���[���J�E���g�����Z
			}
		}
	}

	//�I������
	Uninit();
	//�E�C���h�E�N���X�̓o�^����
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;

}

//*****************************************************************************
//�E�C���h�E�v���V�[�W��
//*****************************************************************************

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:		//�E�C���h�E�j��̃��b�Z�[�W
							//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			//�E�C���h�E�̔j��
			DestroyWindow(hWnd);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//*****************************************************************************
//����������
//*****************************************************************************

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;		//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

									//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));	//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;	//�Q�[����ʃT�C�Y�i���j
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;	//�Q�[����ʃT�C�Y�i�����j
	d3dpp.BackBufferFormat = d3ddm.Format;	//�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;	//�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�_�u���o�b�t�@�̐؂�ւ��i�f���M���ɓ����j
	d3dpp.EnableAutoDepthStencil = TRUE;	//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//�f�v�X�o�b�t�@�Ƃ��ĂP�UBIT���g��
	d3dpp.Windowed = bWindow;	//�E�C���h���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��

	//Direct3D�f�o�C�X�̐����i�`�揈���ƒ��_�����̓n�[�h�E�F�A�ōs���j
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs��)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_������CPU�ōs��)
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

	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);

	//�����_�[�X�e�[�g�̐ݒ�D3DCULL_CCW
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//���b�V����
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//���͏����̏���������
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�T�E���h�̏�����
	InitSound(hWnd);

	//�t�F�[�h�̐ݒ�
	InitFade(g_mode);

	//���[�h�̐ݒ�
	SetMode(g_mode);

	//�����_����̏�����
	srand((unsigned int)time(0));

	 g_nCntHil[0] = 0;
	 g_nCntHil[1] = 0;

	return S_OK;
}

//*****************************************************************************
//�I������
//*****************************************************************************

void Uninit(void)
{
	
	//����\��߂�
	timeEndPeriod(1);

	//���͏����̏I������
	UninitInput();

	//�^�C�g����ʂ̏I������
	UninitTitle();

	////�`���[�g���A����ʂ̏I������
	//UninitTutorial();

	//�Q�[����ʂ̏I������
	UninitGame();

	//���U���g��ʂ̏I������
	UninitResult();
	//�T�E���h�I������
	UninitSound();
	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	//�f�o�b�N�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//*****************************************************************************
//�X�V����
//*****************************************************************************

void Update(void)
{
	//���͏����̍X�V����
	UpdateInput();

	//���[�h�̍X�V����
	switch (g_mode)
	{
		case MODE_TITLE:		//�^�C�g�����
			UpdateTitle();
			break;

		case MODE_TUTORIAL:		//�`���[�g���A�����
			UpdateTutorial();
			break;

		case MODE_GAME:			//�Q�[�����
			UpdateGame();
			break;

		case MODE_RESULT:		//���U���g���
			UpdateResult();
			break;

		//case MODE_RANKING:		//�����L���O���
		//	UpdateRanking();
	}

	//�t�F�[�h�̍X�V����
	UpdateFade();

	/* if (GetMouseWheel() < 0)
	 {
		 int nData = 0;
	 }*/
}

//*****************************************************************************
//�`�揈��
//*****************************************************************************

void Draw(void)
{
	//��ʃN���A�i�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A�j
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n������������

	 //���[�h�̍X�V����
		switch (g_mode)
		{
			case MODE_TITLE:		//�^�C�g�����
				DrawTitle();
				break;

			case MODE_TUTORIAL:		//�`���[�g���A�����
				DrawTutorial();
				break;

			case MODE_GAME:			//�Q�[�����
				DrawGame();
				break;

			case MODE_RESULT:		//���U���g���
				DrawResult();
				break;

			//case MODE_RANKING:		//�����L���O���
			//	DrawRanking();
		}

#ifdef _DEBUG
		//FPS�̕\��
		//DrawFPS();
#endif

		//�t�F�[�h�̕`��
		DrawFade();

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

//==========================================================================================================
//���[�h�̐ݒ�
//==========================================================================================================
void SetMode(MODE mode)
{
	//���݂̉��(���[�h)�̏I������
	switch (g_mode)
	{
		case MODE_TITLE:		//�^�C�g�����
			UninitTitle();
			break;

		case MODE_TUTORIAL:		//�`���[�g���A�����
			UninitTutorial();
			break;

		case MODE_GAME:			//�Q�[�����
			UninitGame();
			break;

		case MODE_RESULT:		//���U���g���
			UninitResult();
			break;

		//case MODE_RANKING:		//�����L���O���
		//	UninitRanking();
		//	break;
	}

	g_mode = mode;		//���݂̉��(���[�h)��؂�ւ���

	//�V�������(���[�h)�̏���������
	switch (g_mode)
	{
		case MODE_TITLE:		//�^�C�g�����
			InitTitle();
			break;

		case MODE_TUTORIAL:		//�`���[�g���A�����
			InitTutorial();
			break;

		case MODE_GAME:			//�Q�[�����
			InitGame();
			break;

		case MODE_RESULT:		//���U���g���
			InitResult();
			break;

		//case MODE_RANKING:		//�����L���O���
		//	InitRanking();
		//	RoadRanking();
		//	SetRanking(GetScore());
		//	break;
	}
}

//-----------------------------------------------------------------------------
//�f�o�C�X�̎擾
//-----------------------------------------------------------------------------

LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-----------------------------------------------------------------------------
//FPS�̕\���̕`�揈��
//-----------------------------------------------------------------------------

void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[20][256];
	
	/*if (g_nCntHil[0] < GetMouseWheel())
	{
		g_nCntHil[0] = GetMouseWheel();
	}
	if (g_nCntHil[1] > GetMouseWheel())
	{
		g_nCntHil[1] = GetMouseWheel();
	}*/
	

	//������ɑ��
	sprintf(&aStr[0][0], "����:%f\n�ő�:%f\n�ŏ�:%f �@[�ǉ�] O�L�[�ŃX�R�A���Z", GetJoypadStick(JOYKEY_LEFT_STICK,0).x, GetJoypadStick(JOYKEY_LEFT_STICK, 0).y, GetJoypadStick(JOYKEY_LEFT_STICK, 0).z);
	
	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	
}

//==========================================================================================================
//���[�h�̎擾
//==========================================================================================================
MODE GetMode(void)
{
	return g_mode;
}