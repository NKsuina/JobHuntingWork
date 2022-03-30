//=============================================================================
//
// �|���S������ [polygon.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------

#include "polygon.h"
#include "disc.h"
#include "input.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------

#define POLYGON_MAX				(256)	//�|���S���̍ő吔
#define POLYGON_TEXTURE_MAX		(16)	//�e�N�X�`���̍ő吔

//-----------------------------------------------------------------------------
//�\���̂̒�`
//-----------------------------------------------------------------------------

//�|���S���\����
typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 rot;					//����
	D3DXCOLOR col;						//�J���[
	float fRadiusX; float fRadiusZ;		//�e���c��
	D3DXMATRIX mtxWorld;				//���[���h�}�g���b�N�X
	TexturetPolygon Texturetstate;		//�e�N�X�`���̏��
	bool bUse;							//�g�p���Ă��邩�ǂ���

}PolygonStructure;

//-----------------------------------------------------------------------------
//�O���[�o���ϐ��錾
//-----------------------------------------------------------------------------
static int count;

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePolygon[TEXTURE_MAX] = { NULL };//�e�N�X�`���ւ̃|�C���^
PolygonStructure g_aPolygon[POLYGON_MAX];//�|���S���\����


//*****************************************************************************
//�|���S��������
//*****************************************************************************

void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�|���S���̃e�N�X�`���̏�����
	ZeroMemory(&g_pTexturePolygon[0], sizeof(g_pTexturePolygon));

	//�e�N�X�`���̓ǂݍ���
	//���Ӂ@�O�Ԗڂ̃e�N�X�`����NULL�ŁI�I
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/yuka.png",
		&g_pTexturePolygon[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/�^�C�g���w�i.png",
		&g_pTexturePolygon[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/kannkyakuyuka.png",
		&g_pTexturePolygon[3]);

	//---------------------------------
	//3�|�C���g
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/R_3PTS.jpg",
		&g_pTexturePolygon[TEXTURE_3PT_R]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/3PTS_L.jpg",
		&g_pTexturePolygon[TEXTURE_3PT_L]);
	//---------------------------------
	//5�|�C���g
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/5PTS_R.png",
		&g_pTexturePolygon[TEXTURE_5PT_R]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/5PTS_L.png",
		&g_pTexturePolygon[TEXTURE_5PT_L]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * POLYGON_MAX,//�m�ۂ��钸�_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	//�|���S���\���̂̏�����
	ZeroMemory(&g_aPolygon[0], sizeof(g_aPolygon));

	count = 0;

	//�����|���S���̔z�u
	SetPolygon(D3DXVECTOR3(SCREEN_STAGE_WIDTH * 2, 0.0f, SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(-SCREEN_STAGE_WIDTH * 2, 0.0f, SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(SCREEN_STAGE_WIDTH * 2, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(-SCREEN_STAGE_WIDTH * 2, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(0.0f, 0.0f, SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(0.0f, 0.0f, -SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(SCREEN_STAGE_WIDTH * 2, 0.0f, -SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(-SCREEN_STAGE_WIDTH * 2, 0.0f, -SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);

	SetPolygon(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_1);
}
//*****************************************************************************
//�|���S���̏I������
//*****************************************************************************

void UninitPolygon(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTexturePolygon[nCnt] != NULL)
		{
			g_pTexturePolygon[nCnt]->Release();
			g_pTexturePolygon[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}

}

//*****************************************************************************
//�|���S���̍X�V����
//*****************************************************************************
void UpdatePolygon(void)	
{

}
//*****************************************************************************
//�|���S���̕`�揈��
//*****************************************************************************

void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		if (g_aPolygon[nCnt].bUse)
		{
			D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPolygon[nCnt].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aPolygon[nCnt].rot.y, g_aPolygon[nCnt].rot.x, g_aPolygon[nCnt].rot.z);

			D3DXMatrixMultiply(&g_aPolygon[nCnt].mtxWorld, &g_aPolygon[nCnt].mtxWorld, &mtxRot);

			//�ʒu�̔��f
			D3DXMatrixTranslation(&mtxTrans, g_aPolygon[nCnt].pos.x, g_aPolygon[nCnt].pos.y, g_aPolygon[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aPolygon[nCnt].mtxWorld, &g_aPolygon[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPolygon[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturePolygon[g_aPolygon[nCnt].Texturetstate]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCnt * 4,						//�`�悷��ŏ��̒��_�C���f�b�N�X
				2);		//�v���~�e�B�u�i�|���S���j��
		}
	}
}

//-----------------------------------------------------------------------------
//�Z�b�g�|���S������
//-----------------------------------------------------------------------------

void SetPolygon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadiusX, float fRadiusZ, TexturetPolygon Texturetstate)
{
	VERTEX_3D *pVtx = NULL;		//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{

		if (!g_aPolygon[nCnt].bUse)
		{//�������U��������

			//�\���̂��g�p��Ԃɂ���
			g_aPolygon[nCnt].bUse = true;
			g_aPolygon[nCnt].pos = pos;
			g_aPolygon[nCnt].rot = rot;
			g_aPolygon[nCnt].col = col;
			g_aPolygon[nCnt].fRadiusX = fRadiusX;
			g_aPolygon[nCnt].fRadiusZ = fRadiusZ;
			g_aPolygon[nCnt].Texturetstate = Texturetstate;

			//���_���W�̐ݒ�//���[�J�����W��
			pVtx[0].pos = D3DXVECTOR3(-fRadiusX, 0.0f, fRadiusZ);
			pVtx[1].pos = D3DXVECTOR3(fRadiusX, 0.0f, fRadiusZ);
			pVtx[2].pos = D3DXVECTOR3(-fRadiusX, 0.0f, -fRadiusZ);
			pVtx[3].pos = D3DXVECTOR3(fRadiusX, 0.0f, -fRadiusZ);

			//�e���_�̖@���̐ݒ�i�x�N�g���̑傫���͂P�ɂ���K�v������j
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aPolygon[nCnt].col;
			pVtx[1].col = g_aPolygon[nCnt].col;
			pVtx[2].col = g_aPolygon[nCnt].col;
			pVtx[3].col = g_aPolygon[nCnt].col;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			break;
		}

		pVtx += 4; //�f�[�^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPolygon->Unlock();
}
void FalseSet(void)
{
	for (int Count = 0; Count < POLYGON_MAX; Count++)
	{
		if (g_aPolygon[Count].Texturetstate >= TEXTURE_3PT_R)
		{
			g_aPolygon[Count].bUse = false;
		}
	}
}
void TrueSet(void)
{
	for (int a = 0; a < 2; a++)
	{
		for (int i = 0; i < 2; i++)
		{
			SetPolygon(D3DXVECTOR3(-500.0f + 1000 * a, 50.0f, -250.0f + 500 * i), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100, 100, TEXTURE_3PT_R);
			SetPolygon(D3DXVECTOR3(-500.0f + 1000 * a, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100, 100, TEXTURE_5PT_R);
		}
	}
}