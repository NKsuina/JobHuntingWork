//=============================================================================
//
// �G�̏��� [enemy.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "sound.h"
#include "score.h"
#include "block.h"

//�}�N����`
#define NUM_ENEMY		(4)		//�G�̎��

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
Enemy g_aEnemy[MAX_ENEMY];							//�G�̏��
D3DXVECTOR3 g_posOldEnemy;						
D3DXVECTOR3 g_posEnemy;								//�ʒu
D3DXVECTOR3 g_moveEnemy;							//�ړ���
D3DXVECTOR3 g_rotEnemy;								//����

//==============================
//�G�̏���������
//==============================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy000.png",
		&g_apTextureEnemy[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy001.png",
		&g_apTextureEnemy[1]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy002.png",
		&g_apTextureEnemy[2]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy004.png",
		&g_apTextureEnemy[3]);

	//�G�̏��̏�����
	for (int nCntEnemy = 0; nCntEnemy <MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏�����
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʂ̏�����
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			
		g_aEnemy[nCntEnemy].bIsJumping = false;							
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nTime = 0;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posEnemy.x - ENEMY_WIDTH/2, g_posEnemy.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posEnemy.x + ENEMY_WIDTH/2, g_posEnemy.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posEnemy.x - ENEMY_WIDTH/2, g_posEnemy.y , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posEnemy.x + ENEMY_WIDTH/2, g_posEnemy.y , 0.0f);

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

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

}

//============================
//�G�̏I������
//============================
void UninitEnemy(void)
{
	int nCnt;

	for (nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureEnemy[nCnt] != NULL)
		{
			g_apTextureEnemy[nCnt]->Release();
			g_apTextureEnemy[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//====================================
//�G�̍X�V����
//====================================
void UpdateEnemy(void)
{
	int nCntEnemy;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2, g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2, g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2, g_aEnemy[nCntEnemy].pos.y, 0.0f);

			//�O��̈ʒu��ۑ�
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
			g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;

			g_aEnemy[nCntEnemy].bIsJumping = !CollisionBlock(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, ENEMY_WIDTH, ENEMY_HEIGHT);

		}

		if (g_aEnemy[nCntEnemy].nType == 1)
		{
			g_aEnemy[nCntEnemy].move.y = 1;
		}

		//�E�̕ǂɂԂ�����
		if (g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH)
		{
			g_aEnemy[nCntEnemy].pos.x = SCREEN_WIDTH;
			g_aEnemy[nCntEnemy].move.x = 0.0f;
		}
		//���̕ǂɂԂ�����
		if (g_aEnemy[nCntEnemy].pos.x <= 0)
		{
			g_aEnemy[nCntEnemy].pos.x = 0;
			g_aEnemy[nCntEnemy].move.x = 0.0f;
		}
		pVtx += 4;  //���_�f�[�^�̃|�C���^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();
}
//=================================
//�G�̕`�揈��
//=================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă���
		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntEnemy * 4,
				2);
		}
	}
}
//==============================
//�G�̐ݒ菈��
//==============================
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//�G���g�p����Ă��Ȃ��ꍇ
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nType = nType;

			//���_���W�̐ݒ菈��
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH/2, g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH/2, g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH/2, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH/2, g_aEnemy[nCntEnemy].pos.y, 0.0f);

			g_aEnemy[nCntEnemy].bUse = true;		//�g�p���Ă����Ԃɂ���

			break;		//��for���𔲂��o��
		}
		pVtx += 4;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	Player *pPlayer;

	//�v���C���[�̓����蔻�菈��
	pPlayer = GetPlayer();

	bool bIsLanding = false;		//���n���Ă��邩�ǂ���

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//��̓����蔻�菈��
		if (g_aEnemy[nCntEnemy].bUse)
		{//�G���g�p����Ă���
			if (pPos->x + fWidth / 2.0f > g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2.0f
				&& pPos->x - fWidth / 2.0f < g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2.0f
				&& pPosOld->y <= g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2.0f
				&& pPos->y > g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2.0f)
			{
				pPos->y = g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2.0f;
				if (g_aEnemy[nCntEnemy].nType == 1)
				{
					PlaySound(SOUND_LABEL_SE_STUNP);
					g_aEnemy[nCntEnemy].bUse = false;
					AddScore(200);
				}
				if (g_aEnemy[nCntEnemy].nType == 0)
				{
					PlaySound(SOUND_LABEL_SE_HIT);
					pPlayer->nLife = 0;
				}
			}

			//���̓����蔻�菈��
			if (pPos->x + fWidth / 2.0f > g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2.0f
				&& pPos->x - fWidth / 2.0f < g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2.0f
				&& pPosOld->y - fHeight >= g_aEnemy[nCntEnemy].pos.y + ENEMY_HEIGHT
				&& pPos->y - fHeight < g_aEnemy[nCntEnemy].pos.y + ENEMY_HEIGHT)

			{
				PlaySound(SOUND_LABEL_SE_HIT);
				pPlayer->nLife = 0;
			}

			//���̓����蔻�菈��
			if (pPos->x + fWidth / 2.0f > g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2.0f
				&& pPosOld->x - fWidth / 2.0f <= g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2.0f
				&& pPos->y - fHeight < g_aEnemy[nCntEnemy].pos.y + ENEMY_HEIGHT / 2.0f
				&& pPos->y > g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2.0f)
			{
				PlaySound(SOUND_LABEL_SE_HIT);
				pPlayer->nLife = 0;
			}

			//�E�̓����蔻�菈��
			if (pPos->x - fWidth / 2.0f < g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2.0f
				&& pPosOld->x + fWidth / 2.0f >= g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2.0f
				&& pPos->y - fHeight < g_aEnemy[nCntEnemy].pos.y + ENEMY_HEIGHT / 2.0f
				&& pPos->y > g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2.0f)
			{
				PlaySound(SOUND_LABEL_SE_HIT);
				pPlayer->nLife = 0;
			}
		}
	}
	return bIsLanding;
}

Enemy *GetEnemy(void)
{
	Enemy *pEnemy = g_aEnemy;

	//�Ԃ�l
	return pEnemy;
}