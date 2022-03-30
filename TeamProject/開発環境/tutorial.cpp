//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "tutorial.h"
#include "title.h"
#include "fade.h"
#include "input.h"
#include "game.h"
#include "sound.h"

//=============================================================================
// �X�^�e�B�b�N�ϐ�
//=============================================================================
static LPDIRECT3DTEXTURE9 g_pTextureTutorial[3] = {};				//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static D3DXCOLOR g_EnterColer;										//�uPress Enter�v�̐F
static int g_nCounterEnterMoving;									//�uPress Enter�v�̓_�ł̃J�E���^�[
static bool bEnterFlag2;											//�uPress Enter�v�̓_�ł̃t���O
static int g_EnterCounter;											//�G���^�[�������񐔂̃J�E���^�[

//=============================================================================
// ����������
//=============================================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�O���[�o���ϐ��̏�����
	g_EnterColer = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_nCounterEnterMoving = 0;
	bEnterFlag2 = false;
	g_EnterCounter = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Tutorial.png",
		&g_pTextureTutorial[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial001.jpg",
		&g_pTextureTutorial[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title005.png",
		&g_pTextureTutorial[2]);

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 12,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[8].pos = D3DXVECTOR3(1110.0f - 150.0f, 640.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(1110.0f + 150.0f, 640.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(1110.0f - 150.0f, 710.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(1110.0f + 150.0f, 710.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitTutorial(void)
{
	//�T�E���h�̒�~
	//StopSound();

	//�e�N�X�`���̔j��
	for (int nCntTutorial = 0; nCntTutorial < 3; nCntTutorial++)
	{
		if (g_pTextureTutorial[nCntTutorial] != NULL)
		{
			g_pTextureTutorial[nCntTutorial]->Release();
			g_pTextureTutorial[nCntTutorial] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTutorial(void)
{
	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

	//�t�F�[�h�̏����|�C���^�ɓ����
	FADE pFade;
	pFade = GetFade();

	//�uPress Enter�v�𓮂�������
	g_nCounterEnterMoving++;

	//�uPress Enter�v�̓_�ł��鏈��
	if (g_nCounterEnterMoving == 1)
	{
		bEnterFlag2 = false;
	}
	if (g_nCounterEnterMoving == 21)
	{
		bEnterFlag2 = true;
	}
	if (g_nCounterEnterMoving == 41)
	{
		g_nCounterEnterMoving = 0;
	}
	if (bEnterFlag2 == false)
	{
		g_EnterColer = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}
	if (bEnterFlag2 == true)
	{
		g_EnterColer = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (g_EnterCounter == 2)
	{
		g_EnterColer = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̍X�V
	pVtx[8].col = g_EnterColer;
	pVtx[9].col = g_EnterColer;
	pVtx[10].col = g_EnterColer;
	pVtx[11].col = g_EnterColer;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();

	//����L�[(ENTER�L�[)�������ꂽ���ǂ���
	if ((GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A)) && pFade == FADE_NONE)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_CLICK);;
		g_EnterCounter++;
		if (g_EnterCounter == 1)
		{
			//���[�h�̃Z�b�g����
			SetFade(MODE_TITLE);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_EnterCounter == 0)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTutorial[0]);

		//�|���S���̕`��       
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�|���S���̌`
			0,						//���_�̊J�n�ꏊ
			2);						//�v���~�e�B�u�̐�
	}

	if (g_EnterCounter == 1)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTutorial[1]);

		//�|���S���̕`��       
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�|���S���̌`
			4,						//���_�̊J�n�ꏊ
			2);						//�v���~�e�B�u�̐�
	}

	////�e�N�X�`���̐ݒ�
	//pDevice->SetTexture(0, g_pTextureTutorial[2]);

	////�|���S���̕`��       
	//pDevice->DrawPrimitive(
	//	D3DPT_TRIANGLESTRIP,	//�|���S���̌`
	//	8,						//���_�̊J�n�ꏊ
	//	2);						//�v���~�e�B�u�̐�
}