//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "score.h"

//�}�N����`
#define NUM_SCORE (8)
#define SCORE_HEIGHT (25.0f)	//�X�R�A��Y�T�C�Y
#define SCORE_WIDTH (25.0f)		//�X�R�A��X�T�C�Y

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;			//���_�o�b�t�@�ւ̃|�C���^	
D3DXVECTOR3 g_posScore;									//�X�R�A�̈ʒu
int g_nScore;											//�X�R�A�̒l

//===========================
//�X�R�A�̏���������
//===========================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureScore);

	g_posScore = D3DXVECTOR3(900.0f, 30.0f, 0.0f);		//�ʒu��������
	g_nScore = 0;										//�l������������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - SCORE_WIDTH, g_posScore.y - SCORE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + SCORE_WIDTH, g_posScore.y - SCORE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x - SCORE_WIDTH, g_posScore.y + SCORE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + SCORE_WIDTH, g_posScore.y + SCORE_HEIGHT, 0.0f);

		g_posScore += D3DXVECTOR3(50.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();

}
//===========================
//�X�R�A�̐ݒ菈��
//===========================
void SetScore(int nScore)
{
	int aPosTexU[NUM_SCORE];		//�e���̐������i�[

	g_nScore = g_nScore;

	aPosTexU[0] = g_nScore % 100000000 / 10000000;
	aPosTexU[1] = g_nScore % 10000000 / 1000000;
	aPosTexU[2] = g_nScore % 1000000 / 100000;
	aPosTexU[3] = g_nScore % 100000 / 10000;
	aPosTexU[4] = g_nScore % 10000 / 1000;
	aPosTexU[5] = g_nScore % 1000 / 100;
	aPosTexU[6] = g_nScore % 100 / 10;
	aPosTexU[7] = g_nScore % 10 / 1;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//���_���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//==========================
//�X�R�A�̏I������
//==========================
void UninitScore(void)
{
	int nCnt;

	for (nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pVtxBuffScore != NULL)
		{
			g_pVtxBuffScore->Release();
			g_pVtxBuffScore = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}
//===========================
//�X�R�A�̍X�V����
//===========================
void UpdateScore(void)
{

}
//===========================
//�X�R�A�̕`�揈��
//===========================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntScore * 4,
			2);
	}
}

//==========================
//�X�R�A�̉��Z����
//==========================
void AddScore(int nValue)
{
	int aPosTexU[NUM_SCORE];		//�e���̐������i�[
	VERTEX_2D *pVtx;
	int nCnt;

	g_nScore += nValue;

	aPosTexU[0] = g_nScore % 100000000 / 10000000;
	aPosTexU[1] = g_nScore % 10000000 / 1000000;
	aPosTexU[2] = g_nScore % 1000000 / 100000;
	aPosTexU[3] = g_nScore % 100000 / 10000;
	aPosTexU[4] = g_nScore % 10000 / 1000;
	aPosTexU[5] = g_nScore % 1000 / 100;
	aPosTexU[6] = g_nScore % 100 / 10;
	aPosTexU[7] = g_nScore % 10;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCnt] + 0.1f, 1.0f);

		pVtx += 4;
	}
	//���_���A�����b�N����
	g_pVtxBuffScore->Unlock();
}
int GetScore(void)
{
	return g_nScore;
}