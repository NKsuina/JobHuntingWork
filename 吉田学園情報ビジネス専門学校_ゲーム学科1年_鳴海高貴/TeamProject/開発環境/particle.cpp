//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "particle.h"

//=============================================================================
// �X�^�e�B�b�N�ϐ�
//=============================================================================
static LPDIRECT3DTEXTURE9 g_pTexture = NULL;				//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static Particle g_aParticle[MAX_PARTICLE];					//�G�t�F�N�g�̏��

//=============================================================================
// ����������
//=============================================================================
void InitParticle(void)
{
	//�����̏�����
	srand((unsigned int)time(0));

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Particle01.png",
		&g_pTexture);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuff,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���̏�����
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//���_���W�̏�����
		pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);

		//RHW�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̏�����
		pVtx[0].col = g_aParticle[nCntParticle].col;
		pVtx[1].col = g_aParticle[nCntParticle].col;
		pVtx[2].col = g_aParticle[nCntParticle].col;
		pVtx[3].col = g_aParticle[nCntParticle].col;

		//�e�N�X�`���̍��W������
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//�\���̂̏�����
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
		g_aParticle[nCntParticle].fRadius = 0;								//���a
		g_aParticle[nCntParticle].fAngle = 0;								//�p�x
		g_aParticle[nCntParticle].fLength = 0;								//�ړ���
		g_aParticle[nCntParticle].fSubtractionRadius = 0;					//���a���Z��
		g_aParticle[nCntParticle].fSubtraction�� = 0;						//���l���Z��
		g_aParticle[nCntParticle].nLife = 0;								//����
		g_aParticle[nCntParticle].nTipe = 0;								//�^�C�v
		g_aParticle[nCntParticle].bUse = false;								//�g�p���Ă邩�ǂ���

		//���_�|�C���g��4�i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuff->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexture != NULL)
	{
		g_pTexture->Release();
		g_pTexture = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuff != NULL)
	{
		g_pVtxBuff->Release();
		g_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{//�p�[�e�B�N�����g�p����Ă�������s
			//�ʒu�̍X�V
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			//���_�J���[�̃��l�������_���l�ŉ����Ă���
			g_aParticle[nCntParticle].col.a -= g_aParticle[nCntParticle].fSubtraction��;

			//���������X�ɍ��
			g_aParticle[nCntParticle].nLife--;

			//������0�ɂȂ������ɏ���
			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);

			//���_�J���[�̍X�V
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;
		}
		//���_�f�[�^��4�i�߂�
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, g_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//// ���u�����f�B���O�����Z�����ɐݒ�
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexture);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParticle, 2);
		}
	}

	//a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// �Z�b�g����
//=============================================================================
void SetParticle(D3DXVECTOR3 pos ,int nTipe)
{
	//���_���ւ̃|�C���^
	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			if (nTipe == 0)
			{//�^�C�g�����̃p�[�e�B�N��
				g_aParticle[nCntParticle].pos = pos;
				g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314)) * ((float)(rand() % 15 + 1) / 10);
				g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314)) * ((float)(rand() % 15 + 1) / 10);
				g_aParticle[nCntParticle].move.z = 0.0f;
				g_aParticle[nCntParticle].nTipe = nTipe;
				g_aParticle[nCntParticle].fSubtraction�� = (float)(rand() % 5 + 3) / 1000;	
				g_aParticle[nCntParticle].fRadius = (float)(rand() % 150 + 50) / 10;		
				g_aParticle[nCntParticle].nLife = 3000;								
				g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);			
				g_aParticle[nCntParticle].bUse = true;										
				break;
			}

			if (nTipe == 1)
			{//�������̂̃p�[�e�B�N��
				g_aParticle[nCntParticle].pos = pos;
				g_aParticle[nCntParticle].move.x = (float)(rand() % 21 - 10) / 10;
				g_aParticle[nCntParticle].move.y = -((float)(rand() % 3 + 1)) * ((float)(rand() % 15 + 1) / 10);
				g_aParticle[nCntParticle].move.z = 0.0f;
				g_aParticle[nCntParticle].nTipe = nTipe;
				g_aParticle[nCntParticle].fSubtraction�� = (float)(rand() % 5 + 3) / 1000;
				g_aParticle[nCntParticle].fRadius = (float)(rand() % 150 + 80) / 10;
				g_aParticle[nCntParticle].nLife = 3000;
				g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);
				g_aParticle[nCntParticle].bUse = true;
				break;
			}
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuff->Unlock();
}