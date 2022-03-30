//=============================================================================
//
// �r���{�[�h���� [billboard.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "billboard.h"
#include "input.h"

#define NUM_BILLBOARD (256)			//�r���{�[�h�̍ő吔

//�O���[�o���ϐ��錾
static Billboard s_aBillboard[NUM_BILLBOARD];
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffBillboard = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 s_pTextureBillboard[BILLBOARD_TEXTURE_MAX];		//�e�N�X�`���ւ̃|�C���^

//==============================================
//�r���{�[�h�̏���������
//==============================================
void InitBillboard(void)
{
	//�O���[�o���ϐ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_pTextureBillboard[NULLTEXTURE] = NULL;

	//�e�N�X�`���̓ǂݍ���1
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ouen_man.png",
		&s_pTextureBillboard[TEXTURE_AudienceMan]);

	//�e�N�X�`���̓ǂݍ���2
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ElectricBulletin2.jpg",
		&s_pTextureBillboard[TEXTURE_ElectricBulletin]);

	//�e�N�X�`���̓ǂݍ���3
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ouen_woman.png",
		&s_pTextureBillboard[TEXTURE_AudienceWo]);

	//�e�N�X�`���̓ǂݍ���4
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/sarari-man_ouen.png",
		&s_pTextureBillboard[TEXTURE_AudienceSara]);

	//�e�N�X�`���̓ǂݍ���5
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/alien_ouen.png",
		&s_pTextureBillboard[TEXTURE_AudienceAlien]);

	//�e�N�X�`���̓ǂݍ���6
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/audience.png",
		&s_pTextureBillboard[TEXTURE_AudienceBack]);

	//�e�N�X�`���̓ǂݍ���7
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bro.png",
		&s_pTextureBillboard[TEXTURE_AudienceBro]);

	//�e�N�X�`���̓ǂݍ���8
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/mukimuki_Dog.png",
		&s_pTextureBillboard[TEXTURE_AudienceDog]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBillboard,
		NULL);

	VERTEX_3D* pVtx = NULL;

	//���_�o�b�t�@�����b�N
	s_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < NUM_BILLBOARD; nCntBillboard++)
	{

		s_aBillboard[nCntBillboard].bUse = false;

		//���_���W�̐ݒ�i���[���h���W�ł͂Ȃ����[�J�����W���w�肷��j
		pVtx[0].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x - 0.0f, s_aBillboard[nCntBillboard].pos.y - 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x + 0.0f, s_aBillboard[nCntBillboard].pos.y - 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x - 0.0f, s_aBillboard[nCntBillboard].pos.y + 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x + 0.0f, s_aBillboard[nCntBillboard].pos.y + 0.0f, 0.0f);

		//�e���_�̖@���̐ݒ�i���x�N�g���̑傫����1�ɂ���K�v������j
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	//���_�o�b�t�@�̃A�����b�N
	s_pVtxBuffBillboard->Unlock();

	//�r���{�[�h�ɉe��t����\�肾�������́�
	//SetShadow(D3DXVECTOR3(g_posBillboard.x, 0.1f, g_posBillboard.z), g_rotBillboard);
}

//==============================================
//�r���{�[�h�̏I������
//==============================================
void UninitBillboard(void)
{
	//�o�b�t�@�j��
	if (s_pVtxBuffBillboard != NULL)
	{
		s_pVtxBuffBillboard->Release();
		s_pVtxBuffBillboard = NULL;
	}

	for (int nCnt = 0; nCnt < BILLBOARD_TEXTURE_MAX; nCnt++)
	{
		//�e�N�X�`���j��
		if (s_pTextureBillboard[nCnt] != NULL)
		{
			s_pTextureBillboard[nCnt]->Release();
			s_pTextureBillboard[nCnt] = NULL;
		}
	}
}

//==============================================
//�r���{�[�h�̍X�V����
//==============================================
void UpdataBillboard(void)
{
}

//==============================================
//�r���{�[�h�̕`�揈��
//==============================================
void DrawBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX  mtxTrans;							//�v�Z�p�}�g���b�N�X

	for (int nCntBillboard = 0; nCntBillboard < NUM_BILLBOARD; nCntBillboard++)
	{
		if (s_aBillboard[nCntBillboard].bUse == true)
		{
			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, s_pTextureBillboard[s_aBillboard[nCntBillboard].nType]);

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&s_aBillboard[nCntBillboard].mtxWorld);

			D3DXMATRIX  mtxView;

			//�r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			s_aBillboard[nCntBillboard].mtxWorld._11 = mtxView._11;
			s_aBillboard[nCntBillboard].mtxWorld._12 = mtxView._21;
			s_aBillboard[nCntBillboard].mtxWorld._13 = mtxView._31;
			s_aBillboard[nCntBillboard].mtxWorld._21 = mtxView._12;
			s_aBillboard[nCntBillboard].mtxWorld._22 = mtxView._22;
			s_aBillboard[nCntBillboard].mtxWorld._23 = mtxView._32;
			s_aBillboard[nCntBillboard].mtxWorld._31 = mtxView._13;
			s_aBillboard[nCntBillboard].mtxWorld._32 = mtxView._23;
			s_aBillboard[nCntBillboard].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, s_aBillboard[nCntBillboard].pos.x, s_aBillboard[nCntBillboard].pos.y, s_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&s_aBillboard[nCntBillboard].mtxWorld, &s_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, s_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &s_aBillboard[nCntBillboard].mtxWorld);

			//���e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);

			//���C�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//���e�X�g�𖳌���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//===============================
//�r���{�[�h�̐ݒ菈��
//===============================
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR2 size, E_BILLBOARD nType, char *FileName)
{
	VERTEX_3D* pVtx = NULL;

	//���_�o�b�t�@�����b�N
	s_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < NUM_BILLBOARD; nCntBillboard++)
	{
		if (s_aBillboard[nCntBillboard].bUse == false)
		{
			s_aBillboard[nCntBillboard].pos = pos;
			s_aBillboard[nCntBillboard].size = size;
			s_aBillboard[nCntBillboard].nType = nType;
			s_aBillboard[nCntBillboard].bUse = true;

			//���_���W�̐ݒ�i���[���h���W�ł͂Ȃ����[�J�����W���w�肷��j
			pVtx[0].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x - s_aBillboard[nCntBillboard].size.x, s_aBillboard[nCntBillboard].pos.y + s_aBillboard[nCntBillboard].size.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x + s_aBillboard[nCntBillboard].size.x, s_aBillboard[nCntBillboard].pos.y + s_aBillboard[nCntBillboard].size.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x - s_aBillboard[nCntBillboard].size.x, s_aBillboard[nCntBillboard].pos.y - s_aBillboard[nCntBillboard].size.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x + s_aBillboard[nCntBillboard].size.x, s_aBillboard[nCntBillboard].pos.y - s_aBillboard[nCntBillboard].size.y, 0.0f);

			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@�̃A�����b�N
	s_pVtxBuffBillboard->Unlock();
}
//===================================
//�r���{�[�h�̃t�@�C���ǂݍ��ݏ���
//===================================
void InputBillboard(void)
{
	VERTEX_3D* pVtx = NULL;
	FILE *pFile;							//�t�@�C���̃|�C���^��ǂݍ���
	static char a[128];						//�������̕������ۑ�����
	static char savefile[128][256];			//�t�@�C���̏ꏊ��ۑ�����
	static int	sCnt = 0;					//�����Ă邾���񂵂�������g���ϐ�

	//�t�@�C�����J��
	pFile = fopen("data/Text/loadfile.txt", "r");

	if (pFile != NULL)
	{
		while (fscanf(pFile, "%s", &a[0]) != EOF)	//EOF = EndOfFile
		{
			if (strcmp(&a[0], "TEXTURE_NAME") == 0)
			{
				fscanf(pFile, "%s", &a[0]);					//�u=�v�����鏊
				fscanf(pFile, "%s", &savefile[sCnt][0]);	//�e�N�X�`���̃\�[�X�ꏊ�����鏊
				sCnt++;
			}
			if (strcmp(&a[0], "BILLBOARD_SET") == 0)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f);
				E_BILLBOARD nType;

				while (1)		//END_MODELSET������܂ŉ�
				{
					fscanf(pFile, "%s", &a[0]);

					if ((strcmp(&a[0], "POS")) == 0)
					{//�r���{�[�h�̏ꏊ���w��
						fscanf(pFile, "%s", &a[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}
					if ((strcmp(&a[0], "SIZE")) == 0)
					{//�r���{�[�h�̃T�C�Y���w��(2D�Ȃ̂�X,Y�̂ݎw��)
						fscanf(pFile, "%s", &a[0]);
						fscanf(pFile, "%f", &size.x);
						fscanf(pFile, "%f", &size.y);
					}
					if ((strcmp(&a[0], "TYPE")) == 0)
					{
						int i;
						fscanf(pFile, "%s", &a[0]);
						fscanf(pFile, "%d", &i);
						nType = (E_BILLBOARD)i;
					}
					if ((strcmp(&a[0], "END_BILLBOARDSET")) == 0)
					{
						SetBillboard(pos, size, nType, &savefile[sCnt][0]);
						break;
					}
				}
			}
		}
	}
}