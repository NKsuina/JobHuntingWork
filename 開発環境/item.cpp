//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "score.h"
#include "item.h"
#include "sound.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureItem= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;						
D3DXVECTOR3 g_posItem;							//�ʒu
D3DXVECTOR3 g_moveItem;							//�ړ���		
ITEM g_aItem[MAX_ITEM];
//==============================
//�A�C�e���̏���������
//==============================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntItem;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/coin000.png",
		&g_apTextureItem);

	for (int nCntItem = 0; nCntItem <MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posItem.x - ITEM_SIZE/2, g_posItem.y - ITEM_SIZE/2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posItem.x + ITEM_SIZE/2, g_posItem.y - ITEM_SIZE/2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posItem.x - ITEM_SIZE/2, g_posItem.y + ITEM_SIZE/2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posItem.x + ITEM_SIZE/2, g_posItem.y + ITEM_SIZE/2, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();
}
//============================
//�A�C�e���̏I������
//============================
void UninitItem(void)
{
	//�e�N�X�`���̔j��
	if (g_apTextureItem != NULL)
	{
		g_apTextureItem->Release();
		g_apTextureItem = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}
//====================================
//�A�C�e���̍X�V����
//====================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			g_aItem[nCntItem].pos.x += g_aItem[nCntItem].move.x;

			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_SIZE/2, g_aItem[nCntItem].pos.y - ITEM_SIZE/2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_SIZE/2, g_aItem[nCntItem].pos.y - ITEM_SIZE/2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_SIZE/2, g_aItem[nCntItem].pos.y + ITEM_SIZE/2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_SIZE/2, g_aItem[nCntItem].pos.y + ITEM_SIZE/2, 0.0f);
		}
		pVtx += 4;  //���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffItem->Unlock();
}

//=================================
//�A�C�e���̕`�揈��
//=================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntItem;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{//�A�C�e�����g�p����Ă���
		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureItem);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntItem * 4,
				2);
		}
	}
}

//===================================
//�A�C�e���̓����蔻�菈��
//===================================
void CollisionItem(D3DXVECTOR3 *pPos, float fWidth, float fHeight)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//��̓����蔻�菈��
		if (g_aItem[nCntItem].bUse)
		{//�A�C�e�����g�p����Ă���
			if (pPos->x + (fWidth / 2) >= g_aItem[nCntItem].pos.x - (ITEM_SIZE / 2)
				&& pPos->x - (fWidth / 2) <= g_aItem[nCntItem].pos.x + (ITEM_SIZE / 2)
				&& pPos->y >= g_aItem[nCntItem].pos.y - ITEM_SIZE
				&& pPos->y - fHeight <= g_aItem[nCntItem].pos.y)
			{
				g_aItem[nCntItem].bUse = false;
				PlaySound(SOUND_LABEL_SE_GETCOIN);
				AddScore(200);
			}
		}
	}
}
//==============================
//�A�C�e���̐ݒ菈��
//==============================
void SetItem(D3DXVECTOR3 pos)
{
	int nCntItem;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{//�A�C�e�����g�p����Ă��Ȃ��ꍇ
			g_aItem[nCntItem].pos = pos;

			//���_���W�̐ݒ菈��
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_SIZE/2, g_aItem[nCntItem].pos.y - ITEM_SIZE/2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_SIZE/2, g_aItem[nCntItem].pos.y - ITEM_SIZE/2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_SIZE/2, g_aItem[nCntItem].pos.y + ITEM_SIZE/2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_SIZE/2, g_aItem[nCntItem].pos.y + ITEM_SIZE/2, 0.0f);

			g_aItem[nCntItem].bUse = true;		//�g�p���Ă����Ԃɂ���

			break;		//��for���𔲂��o��
		}
		pVtx += 4;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();
}