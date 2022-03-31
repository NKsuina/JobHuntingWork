//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "block.h"
#include "item.h"
#include "sound.h"
#include "enemy.h"
#include "fade.h"
#include "goal.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Player g_player;

//==============================
//�v���C���[�̏���������
//==============================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player000.png",
		&g_pTexturePlayer);

	g_player.nDirectionMove = 0;								//������������
	g_player.nCounterAnim = 0;									//�J�E���^�[������������
	g_player.nPatternAnim = 0;									//�p�^�[��No.������������
	g_player.nLife = 1;											//�v���C���[�̗̑�
	g_player.nCounterState = 0;									//�v���C���[�̏��
	g_player.bUse = true;
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//�v���C���[�̈ړ���
	g_player.pos = D3DXVECTOR3(100.0f, 700.0f, 0.0f);			//�v���C���[�̍��W

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH /2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH /2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH /2, g_player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH /2, g_player.pos.y, 0.0f);

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
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//==============================
//�v���C���[�̏I������
//==============================
void UninitPlayer(void)
{

	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}
//==============================
//�v���C���[�̍X�V����
//==============================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	BLOCK *g_aBlock;
	FADE g_fade;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH / 2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH / 2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH / 2, g_player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH / 2, g_player.pos.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_player.state)
	{
	case PLAYER_STAY:
		if (g_player.nDirectionMove == 0)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
		}

		//���ړ�
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_player.state = PLAYER_RUN;
		}

		//�E�ړ�
		if (GetKeyboardPress(DIK_D) == true)
		{
			g_player.state = PLAYER_RUN;
		}

		if (g_player.pos.y == SCREEN_HEIGHT)
		{
			g_player.bIsJumping = false;
		}

		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			if (g_player.bIsJumping == false)
			{
				g_player.move.y -= 10.5f;
				g_player.bIsJumping = true;
			}
		}
		break;

	case PLAYER_RUN:

		g_player.nCounterAnim++;		//�J�E���^�[�����Z
		if (g_player.nCounterAnim >= 8)
		{
			g_player.nCounterAnim = 0;		//�J�E���^�[�������l�ɖ߂�
			g_player.nPatternAnim++;			//�p�^�[��No.���X�V����

			if (g_player.nPatternAnim >= 8)
			{
				g_player.nPatternAnim = 0;		//�p�^�[��No,�������l�ɖ߂�
			}
		}
		if (g_player.nDirectionMove == 0)
		{
			if (g_player.nCounterAnim <= 4)
			{
				pVtx[0].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim + 0.25f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim, 0.5f);
				pVtx[3].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim + 0.25f, 0.5f);
			}
		}

		if (g_player.nDirectionMove == 1)
		{
			if (g_player.nCounterAnim <= 4)
			{
				pVtx[0].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim, 0.5f);
				pVtx[1].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim + 0.25f, 0.5f);
				pVtx[2].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim + 0.25f, 1.0f);
			}
		}

		if (g_player.bIsJumping == false)
		{
			if (g_player.move.y > 0)
			{
				g_player.nPatternAnim;
			}
		}

		//��i�W�����v�ł��Ȃ��悤��
		if (g_player.pos.y == SCREEN_HEIGHT)
		{
			g_player.bIsJumping = false;
		}

		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			if (g_player.bIsJumping == false)
			{
				g_player.move.y -= 10.5f;
				g_player.bIsJumping = true;
			}
		}

		//���ړ�
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_player.nDirectionMove = 1;
			g_player.move.x += sinf(-D3DX_PI * 0.5f) *0.5f;
			g_player.move.y += cosf(-D3DX_PI * 0.5f) *0.5f;
		}

		//�E�ړ�
		if (GetKeyboardPress(DIK_D) == true)
		{
			g_player.nDirectionMove = 0;
			g_player.move.x += sinf(D3DX_PI * 0.5f) *0.5f;
			g_player.move.y += cosf(D3DX_PI * 0.5f) *0.5f;
		}

		if (GetKeyboardPress(DIK_A) == false && GetKeyboardPress(DIK_D) == false)
		{
			g_player.state = PLAYER_STAY;
		}
		break;
	}

	g_player.move.y += 0.6f;			//�d�͉��Z

	g_aBlock = GetBlock();

	g_fade = GetFade();

	//�O��̈ʒu��ۑ�
	g_player.posOld = g_player.pos;

	//�ʒu���X�V
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;

	//�ړ��ʂ��X�V(����������)
	g_player.move.x += (0.0f - g_player.move.x)* 0.1f;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH / 2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH / 2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH / 2, g_player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH / 2, g_player.pos.y, 0.0f);

	//�u���b�N�̓����蔻��
	g_player.bIsJumping =! CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, PLAYER_WIDTH, PLAYER_HEIGHT);

	//�G�̓����蔻��
	CollisionEnemy(&g_player.pos, &g_player.posOld, &g_player.move, PLAYER_WIDTH, PLAYER_HEIGHT);

	//�A�C�e���̓����蔻��
	CollisionItem(&g_player.pos, PLAYER_WIDTH, PLAYER_HEIGHT);

	//�S�[���̓����蔻��
	CollisionGoal(&g_player.pos, PLAYER_WIDTH, PLAYER_HEIGHT);

	if (g_player.nLife == 0)
	{
		g_player.bUse = false;
		if (g_fade == FADE_NONE)
		{
			StopSound();
			PlaySound(SOUND_LABEL_SE_DETH);
			SetFade(MODE_RESULT);
		}
	}

		if (g_player.pos.y >= 1200)
		{//�n�_�����W����ʉ��[�ɓ�������
			if (g_fade == FADE_NONE)
			{
				StopSound();
				PlaySound(SOUND_LABEL_SE_DETH);
				SetFade(MODE_RESULT);
			}
		}
		if (g_player.pos.x >= SCREEN_WIDTH)
		{
			g_player.pos.x = SCREEN_WIDTH;
			g_player.move.x = 0.0f;
		}
		if (g_player.pos.x <= 0)
		{
			g_player.pos.x = 0;
			g_player.move.x = 0.0f;
		}
}

//==============================
//�|���S���̕`�揈��
//==============================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.bUse == true)
	{//�|���S�����g�p����Ă���
	 //�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0, 2);
	}
}
//===============================
//Player�̎擾
//===============================
Player *GetPlayer(void)
{
	return &g_player;
}
