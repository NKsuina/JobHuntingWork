//=============================================================================
//
// �G�t�F�N�g���� [camera.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "Effect.h"

//�}�N����`
#define MAX_EFFECT		(10000)
#define EFFECT_WIDTH	(20.0f)
#define EFFECT_HEIGHT	(20.0f)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureEffect;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
Effect g_aEffect[MAX_EFFECT];

//=================================
//�G�t�F�N�g�̏���������
//=================================
void InitEffect(void)
{
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bakuha.png",
		&g_pTextureEffect);

	//�e�̏��̏�����
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f ,0.0f);
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].nCounterAnim = 0;									//�J�E���^�[������������
		g_aEffect[nCntEffect].nPatternAnim = 0;									//�p�^�[��No.������������
		g_aEffect[nCntEffect].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-30.0f, 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(30.0f, 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(30.0f, 0.0f, 0.0f);

		//�e���_�̖@���̐ݒ�i���x�N�g���̑傫����1�ɂ���K�v������j
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.3f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.3f);
		pVtx[3].tex = D3DXVECTOR2(0.3f, 0.3f);

		pVtx += 4;		//���_�f�[�^�̃|���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//==============================
//�G�t�F�N�g�̏I������
//==============================
void UninitEffect(void)
{
		//�e�N�X�`���j��
		if (g_pTextureEffect != NULL)
		{
			g_pTextureEffect->Release();
			g_pTextureEffect = NULL;
		}
		//�o�b�t�@�j��
		if (g_pVtxBuffEffect != NULL)
		{
			g_pVtxBuffEffect->Release();
			g_pVtxBuffEffect = NULL;
		}
}

//=================================
//�G�t�F�N�g�̍X�V����
//=================================
void UpdateEffect(void)
{
	int nCntEffect;
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���

		    //���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//�G�t�F�N�g�̍��W�X�V 
			pVtx[0].pos = D3DXVECTOR3(-30.0f, 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(30.0f, 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(30.0f, 0.0f, 0.0f);

			g_aEffect[nCntEffect].nCounterAnim++;		//�J�E���^�[�����Z
			if (g_aEffect[nCntEffect].nCounterAnim >= 3)
			{
				pVtx[0].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.3f);
				pVtx[3].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.3f);
			}

			if (g_aEffect[nCntEffect].nCounterAnim >= 6)
			{
				pVtx[0].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.3f);
				pVtx[1].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.3f);
				pVtx[2].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.6f);
				pVtx[3].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.6f);
			}

			if (g_aEffect[nCntEffect].nCounterAnim >= 9)
			{
				pVtx[0].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.6f);
				pVtx[1].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.6f);
				pVtx[2].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.9f);
				pVtx[3].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.9f);
				g_aEffect[nCntEffect].nCounterAnim = 0;			//�J�E���g��0�ɂ���
			}

			g_aEffect[nCntEffect].nLife--;

			//�������s����
			g_aEffect[nCntEffect].nLife--;

			g_aEffect[nCntEffect].fRadius--;

			g_aEffect[nCntEffect].fRadius -= 0.25f;

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].fRadius = 0;
			}

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
		pVtx += 4;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//============================
//�G�t�F�N�g�̕`�揈��
//============================
void DrawEffect(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DXMATRIX  mtxView;

	//�r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEffect = 0; nCntEffect< MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);

			//���e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			//�J�����̋t�s���ݒ�
			g_aEffect[nCntEffect].mtxWorld._11 = mtxView._11;
			g_aEffect[nCntEffect].mtxWorld._12 = mtxView._21;
			g_aEffect[nCntEffect].mtxWorld._13 = mtxView._31;
			g_aEffect[nCntEffect].mtxWorld._21 = mtxView._12;
			g_aEffect[nCntEffect].mtxWorld._22 = mtxView._22;
			g_aEffect[nCntEffect].mtxWorld._23 = mtxView._32;
			g_aEffect[nCntEffect].mtxWorld._31 = mtxView._13;
			g_aEffect[nCntEffect].mtxWorld._32 = mtxView._23;
			g_aEffect[nCntEffect].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			//���e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);

			//���C�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//���e�X�g�𖳌���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);
		}
	}

	//���u�����f�B���O�����Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//===================================
//�G�t�F�N�g�̐ݒ菈��
//===================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntEffect;
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].bUse = true;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-30.0f,30.0f,0.0f);
			pVtx[1].pos = D3DXVECTOR3(30.0f, 30.0f,0.0f);
			pVtx[2].pos = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(30.0f, 0.0f,0.0f);

			//�e���_�̖@���̐ݒ�i���x�N�g���̑傫����1�ɂ���K�v������j
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.3f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.3f);
			pVtx[3].tex = D3DXVECTOR2(0.3f, 0.3f);
			break;
		}
		pVtx += 4;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}