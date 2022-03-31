//=============================================================================
//
// �u���b�N���� [block.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "player.h"
#include "main.h"
#include "block.h"
#include"player.h"

//�}�N����`
#define MAX_BLOCK	(1096)		//�u���b�N�̍ő吔

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBlock = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;			//���_�o�b�t�@�ւ̃|�C���^	
BLOCK g_aBlock[MAX_BLOCK];								//�u���b�N�̏��

//==============================
//�u���b�N�̏���������
//==============================
void InitBlock(void)
{
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block000.jpg",
		&g_pTextureBlock);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//�u���b�N�̏��̏�����
		g_aBlock[MAX_BLOCK].bUse = false;
		g_aBlock[MAX_BLOCK].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[MAX_BLOCK].fWidth = 0;
		g_aBlock[MAX_BLOCK].fHeight = 0;
	}
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL
	);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - 21.0f / 2, g_aBlock[nCntBlock].pos.y - 21.0f /2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + 21.0f / 2, g_aBlock[nCntBlock].pos.y - 21.0f /2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - 21.0f / 2, g_aBlock[nCntBlock].pos.y + 21.0f /2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + 21.0f / 2, g_aBlock[nCntBlock].pos.y + 21.0f /2, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//==============================
//�u���b�N�̏I������
//==============================
void UninitBlock(void)
{

	//�e�N�X�`���̔j��
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}
//==============================
//�u���b�N�̍X�V����
//==============================
void UpdateBlock(void)
{
	
}

//==============================
//�u���b�N�̕`�揈��
//==============================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBlock);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//�u���b�N���g�p����Ă���
		 //�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntBlock * 4,
				2);
		}
	}
}

//===================================
//�u���b�N�̓����蔻�菈��
//===================================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	bool bIsLanding = false;		//���n���Ă��邩�ǂ���

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//��̓����蔻�菈��
		if (g_aBlock[nCntBlock].bUse)
		{//�u���b�N���g�p����Ă���
			if (pPos->x + fWidth / 2.0f > g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2.0f
				&& pPos->x - fWidth / 2.0f < g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2.0f
				&& pPosOld->y <= g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2.0f
				&& pPos->y > g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2.0f)
			{
				bIsLanding = true;
				pPos->y = g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2.0f;
				pMove->y = 0.0f;
			}

			//���̓����蔻�菈��
			if (pPos->x + fWidth / 2.0f > g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2.0f
				&& pPos->x - fWidth / 2.0f < g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2.0f
				&& pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + BLOCK_SIZE
				&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + BLOCK_SIZE)

			{
				pPos->y = g_aBlock[nCntBlock].pos.y + (BLOCK_SIZE / 2.0f) + fHeight;
				pMove->y = 0.0f;
			}

			//���̓����蔻�菈��
			if (pPos->x + fWidth / 2.0f > g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2.0f
				&& pPosOld->x - fWidth / 2.0f <= g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2.0f
				&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + BLOCK_SIZE / 2.0f
				&& pPos->y > g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2.0f)
			{
				pPos->x = g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2.0f - fWidth / 2.0f;
				pMove->x = 0.0f;
			}

			//�E�̓����蔻�菈��
			if (pPos->x - fWidth / 2.0f < g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2.0f
				&& pPosOld->x + fWidth / 2.0f >= g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2.0f
				&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + BLOCK_SIZE / 2.0f
				&& pPos->y > g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2.0f)
			{
				pPos->x = g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2.0f + fWidth / 2.0f;
				pMove->x = 0.0f;
			}
		}
	}
	return bIsLanding;
}

//===============================
//�u���b�N�̐ݒ菈��
//===============================
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	int nCntBlock;
	VERTEX_2D *pVtx;

	//���_�������b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			g_aBlock[nCntBlock].fHeight = fHeight;
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].pos = pos;

			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2, g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2, g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2, g_aBlock[nCntBlock].pos.y + BLOCK_SIZE / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2, g_aBlock[nCntBlock].pos.y + BLOCK_SIZE / 2, 0.0f);

			g_aBlock[nCntBlock].bUse = true;
			break;
		}
		pVtx += 4;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//===============================
//Block�̎擾
//===============================
BLOCK *GetBlock(void)
{
	return &g_aBlock[MAX_BLOCK];
}