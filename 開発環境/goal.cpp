//=============================================================================
//
// �S�[�����̏��� [goal.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "goal.h"
#include "player.h"
#include "fade.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureGoal = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGoal = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Goal g_aGoal;

//========================================
//  �S�[���̏������ݒ菈��
//========================================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/hataa.png",
		&g_pTextureGoal);

	g_aGoal.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGoal.bUse = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGoal,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aGoal.pos.x - GOAL_WIDTH, g_aGoal.pos.y - GOAL_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aGoal.pos.x + GOAL_WIDTH, g_aGoal.pos.y - GOAL_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aGoal.pos.x - GOAL_WIDTH, g_aGoal.pos.y + GOAL_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aGoal.pos.x + GOAL_WIDTH, g_aGoal.pos.y + GOAL_HEIGHT, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	SetGoal(D3DXVECTOR3(80.0f, 50.0f, 0.0f));

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGoal->Unlock();
}
//========================================
//  �S�[���̏I������
//========================================
void UninitGoal(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureGoal != NULL)
	{			  
		g_pTextureGoal->Release();
		g_pTextureGoal = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGoal != NULL)
	{			
		g_pVtxBuffGoal->Release();
		g_pVtxBuffGoal = NULL;
	}
}
//========================================
//  �S�[���̍X�V����
//========================================
void UpdateGoal(void)
{

}
//========================================
//  �S�[���̕`�揈��
//========================================
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGoal, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aGoal.bUse == true)
	{//�S�[�����g�p����Ă���
	 //�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureGoal);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,2);
	}
}
//===================================
//�S�[���̓����蔻�菈��
//===================================
void CollisionGoal(D3DXVECTOR3 *pPos, float fWidth, float fHeight)
{
	//��̓����蔻�菈��
	if (g_aGoal.bUse)
	{//�S�[�����g�p����Ă���
		if (pPos->x + (fWidth / 2) >= g_aGoal.pos.x - (GOAL_WIDTH / 2)
			&& pPos->x - (fWidth / 2) <= g_aGoal.pos.x + (GOAL_WIDTH / 2)
			&& pPos->y >= g_aGoal.pos.y - GOAL_HEIGHT
			&& pPos->y - fHeight <= g_aGoal.pos.y)
		{
			g_aGoal.bUse = false;
			SetFade(MODE_CLEAR);
		}

	}
}
//==============================
//�S�[���̐ݒ菈��
//==============================
void SetGoal(D3DXVECTOR3 pos)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aGoal.bUse == false)
	{//�S�[�����g�p����Ă��Ȃ��ꍇ
		g_aGoal.pos = pos;

		//���_���W�̐ݒ菈��
		pVtx[0].pos = D3DXVECTOR3(g_aGoal.pos.x - GOAL_WIDTH / 2, g_aGoal.pos.y - GOAL_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGoal.pos.x + GOAL_WIDTH / 2, g_aGoal.pos.y - GOAL_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGoal.pos.x - GOAL_WIDTH / 2, g_aGoal.pos.y + GOAL_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGoal.pos.x + GOAL_WIDTH / 2, g_aGoal.pos.y + GOAL_HEIGHT / 2, 0.0f);

		g_aGoal.bUse = true;		//�g�p���Ă����Ԃɂ���
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGoal->Unlock();
}