//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "bg.h"
#include "main.h"
#include "input.h"

//�}�N����`
#define NUM_BG		(2)	//�w�i�̐�

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBG[NUM_BG] = { NULL,NULL };			//�e�N�X�`��(3����)�̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;						//���_�o�b�t�@�ւ̃|�C���^	
D3DXVECTOR2 g_aPosTexV[NUM_BG];
D3DXVECTOR2 g_aMoveTexV;

//========================================
//  �w�i�̏������ݒ菈��
//========================================
void InitBG(void)
{
	int nCnt;

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/sky000.jpg",
		&g_pTextureBG[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\mountain000.png",
		&g_pTextureBG[1]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL
	);

	//�ʒu�̏�����
	for (nCnt = 0; nCnt < NUM_BG; nCnt++)
	{
		g_aPosTexV[nCnt] = D3DXVECTOR2(0.0f, 0.0f);
	}

	g_aMoveTexV = D3DXVECTOR2(0.0f, 0.0f);

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		pVtx[4].pos = D3DXVECTOR3(0.0f, 500.0f, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH, 500.0f, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 8;		//���_�f�[�^�̃|�C���^��8�i�߂�

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();
}
//===================================
//�w�i�̏I������
//===================================
void UninitBG(void)
{
	int nCnt;

	for (nCnt = 0; nCnt < NUM_BG; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBG[nCnt] != NULL)
		{
			g_pTextureBG[nCnt]->Release();
			g_pTextureBG[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//==================================
//�w�i�̍X�V����
//==================================
void UpdateBG(void)
{
	VERTEX_2D * pVtx;

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

		//�ړ��ʂ��X�V(����)
		g_aPosTexV[0].x += (0.0001f);

		//���_���W�̍X�V
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexV[0].x + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexV[0].x + 0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexV[0].x + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexV[0].x + 0.25f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();
}
//==================================
//�w�i�̕`�揈��
//==================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBG[nCntBG]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			nCntBG * 4,
			2);
	}
}
