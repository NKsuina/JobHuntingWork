//-------------------------------------
//
//�f�B�X�N�̏���[disc.cpp]
//Author:���c�喲
//
//-------------------------------------
#include <time.h>
#include "camera.h"
#include "Input.h"
#include "disc.h"
#include "goal.h"
#include "score1.h"
#include "score2.h"
#include "wallmodel.h"
#include "2dpolyron.h"
#include "sound.h"

//�O���[�o���ϐ�
LPD3DXMESH g_pMeshDisc = NULL;					//���b�V���ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatDisc = NULL;				//���_�o�b�t�@�ւ̃|�C���^
DWORD g_nNumMatDisc= 0;
static Disc g_Disc[MAX_DISC];
static PlayerHaveDisc player;
static int nTime;

static int Count;

static int s_nPtsNum;
static bool s_bHaving;

//�f�B�X�N�̏���������
void InitDisc(void)
{
	s_bHaving = false;
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\�f�B�X�N.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatDisc,
		NULL,
		&g_nNumMatDisc,
		&g_pMeshDisc);

	Count = 0;
	int nNumVtx;			//���_��
	DWORD sizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE * pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVtx = g_pMeshDisc->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshDisc->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshDisc->LockVertexBuffer(D3DLOCK_READONLY, (void **)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		for (int nCntDisc = 0; nCntDisc < MAX_DISC; nCntDisc++)
		{
			//x
			if (vtx.x < g_Disc[nCntDisc].vtxMinDisc.x)
			{
				g_Disc[nCntDisc].vtxMinDisc.x = vtx.x;
			}

			if (vtx.x > g_Disc[nCntDisc].vtxMaxDisc.x)
			{
				g_Disc[nCntDisc].vtxMaxDisc.x = vtx.x;
			}

			//y
			if (vtx.y < g_Disc[nCntDisc].vtxMinDisc.y)
			{
				g_Disc[nCntDisc].vtxMinDisc.y = vtx.y;
			}

			if (vtx.y > g_Disc[nCntDisc].vtxMaxDisc.y)
			{
				g_Disc[nCntDisc].vtxMaxDisc.y = vtx.y;
			}

			//z
			if (vtx.z < g_Disc[nCntDisc].vtxMinDisc.z)
			{
				g_Disc[nCntDisc].vtxMinDisc.z = vtx.z;
			}

			if (vtx.z > g_Disc[nCntDisc].vtxMaxDisc.z)
			{
				g_Disc[nCntDisc].vtxMaxDisc.z = vtx.z;
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pMeshDisc->UnlockVertexBuffer();

	for (int nCntDisc = 0; nCntDisc < MAX_DISC; nCntDisc++)
	{
		g_Disc[nCntDisc].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ʒu�̏�����
		g_Disc[nCntDisc].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ��̏�����
		g_Disc[nCntDisc].size = D3DXVECTOR3(g_Disc[nCntDisc].vtxMaxDisc.x - g_Disc[nCntDisc].vtxMinDisc.x, 0.0f, g_Disc[nCntDisc].vtxMaxDisc.z - g_Disc[nCntDisc].vtxMinDisc.z);
		g_Disc[nCntDisc].rotDisc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����̏�����
		g_Disc[nCntDisc].rotDestDisc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Disc[nCntDisc].vtxMinDisc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Disc[nCntDisc].vtxMaxDisc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Disc[nCntDisc].bUse = false;									//�g���Ă��邩�ǂ���
		g_Disc[nCntDisc].bGoal = false;									//�S�[���������ǂ���
		g_Disc[nCntDisc].nGoal = 0;										//�ǂ����ɃS�[��������
	}
}

//�f�B�X�N�̏I������
void UninitDisc(void)
{
	for (int nCntDisc = 0; nCntDisc < MAX_DISC; nCntDisc++)
	{
		//���b�V���̔j��
		if (g_pMeshDisc != NULL)
		{
			g_pMeshDisc->Release();
			g_pMeshDisc = NULL;
		}
	}
	//�}�e���A���̔j��
	if (g_pBuffMatDisc != NULL)
	{
		g_pBuffMatDisc->Release();
		g_pBuffMatDisc = NULL;
	}
}

//�f�B�X�N�̍X�V����
void UpdateDisc(void)
{
	Camera * pCamera = GetCamera();

	for (int nCntDisc = 0; nCntDisc < MAX_DISC; nCntDisc++)
	{

		//�p�x�̐��K��
		if (g_Disc[nCntDisc].rotDestDisc.y - g_Disc[nCntDisc].rotDisc.y > D3DX_PI)
		{
			g_Disc[nCntDisc].rotDestDisc.y = g_Disc[nCntDisc].rotDestDisc.y - D3DX_PI * 2;
		}
		else if (g_Disc[nCntDisc].rotDestDisc.y - g_Disc[nCntDisc].rotDisc.y < -D3DX_PI)
		{
			g_Disc[nCntDisc].rotDestDisc.y = g_Disc[nCntDisc].rotDestDisc.y + D3DX_PI * 2;
		}

		g_Disc[nCntDisc].rotDisc += (g_Disc[nCntDisc].rotDestDisc - g_Disc[nCntDisc].rotDisc) * 0.08f;

		//�p�x�̐��K��
		if (g_Disc[nCntDisc].rotDisc.y > D3DX_PI)
		{
			g_Disc[nCntDisc].rotDisc.y = g_Disc[nCntDisc].rotDisc.y - D3DX_PI * 2;
		}
		else if (g_Disc[nCntDisc].rotDisc.y < -D3DX_PI)
		{
			g_Disc[nCntDisc].rotDisc.y = g_Disc[nCntDisc].rotDisc.y + D3DX_PI * 2;
		}

		//�O��̈ʒu��ۑ�
		g_Disc[nCntDisc].posOld = g_Disc[nCntDisc].pos;

		//�ʒu���X�V
		g_Disc[nCntDisc].pos.x += g_Disc[nCntDisc].move.x;
		g_Disc[nCntDisc].pos.y += g_Disc[nCntDisc].move.y;
		g_Disc[nCntDisc].pos.z += g_Disc[nCntDisc].move.z;

		//�S�[���̂����蔻��
		if ((g_Disc[nCntDisc].pos.x >= 600.0f && g_Disc[nCntDisc].pos.z >= 180.0f) || (g_Disc[nCntDisc].pos.x >= 600.0f &&g_Disc[nCntDisc].pos.z <= -80.0f) && !s_bHaving)
		{
			g_Disc[nCntDisc].bUse = false;
			g_Disc[nCntDisc].bGoal = true;
			g_Disc[nCntDisc].nGoal = 2;

			if (nTime == 1)
			{
				Polygon_2D Polygon;

				Polygon.fX = 200.0f;
				Polygon.fY = 100.0f;
				Polygon.nPat = 1;
				Polygon.pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f,0.0f);

				s_nPtsNum = Set2DPolygon(Polygon);

				PlaySound(SOUND_LABEL_SE_KANSEI);
				AddScore(3);
			}
		}
		else if (g_Disc[nCntDisc].pos.x >= 600.0f && !s_bHaving)
		{
			g_Disc[nCntDisc].bUse = false;
			g_Disc[nCntDisc].bGoal = true;
			g_Disc[nCntDisc].nGoal = 2;

			if (nTime == 1)
			{
				Polygon_2D Polygon;

				Polygon.fX = 200.0f;
				Polygon.fY = 100.0f;
				Polygon.nPat = 3;
				Polygon.pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);

				s_nPtsNum = Set2DPolygon(Polygon);
				PlaySound(SOUND_LABEL_SE_KANSEI);
				AddScore(5);

			}
		}

		if ((g_Disc[nCntDisc].pos.x <= -600.0f && g_Disc[nCntDisc].pos.z >= 180.0f) || (g_Disc[nCntDisc].pos.x <= -600.0f &&g_Disc[nCntDisc].pos.z <= -80.0f) && !s_bHaving)
		{
			g_Disc[nCntDisc].bUse = false;
			g_Disc[nCntDisc].bGoal = true;
			g_Disc[nCntDisc].nGoal = 1;

			if (nTime == 1)
			{
				Polygon_2D Polygon;

				Polygon.fX = 200.0f;
				Polygon.fY = 100.0f;
				Polygon.nPat = 0;
				Polygon.pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);

				s_nPtsNum = Set2DPolygon(Polygon);
				PlaySound(SOUND_LABEL_SE_KANSEI);
				AddScore2(3);

			}
		}
		else if (g_Disc[nCntDisc].pos.x <= -600.0f && !s_bHaving)
		{
			g_Disc[nCntDisc].bUse = false;
			g_Disc[nCntDisc].bGoal = true;
			g_Disc[nCntDisc].nGoal = 1;

			if (nTime == 1)
			{
			
				Polygon_2D Polygon;

				Polygon.fX = 200.0f;
				Polygon.fY = 100.0f;
				Polygon.nPat = 2;
				Polygon.pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
				PlaySound(SOUND_LABEL_SE_KANSEI);
				s_nPtsNum = Set2DPolygon(Polygon);
				AddScore2(5);

			}
		}

		//2P���S�[��������1P���Ƀf�B�X�N����
		if (g_Disc[nCntDisc].bGoal == true && g_Disc[nCntDisc].nGoal == 1)
		{
			if (nTime >= 60)
			{
				g_Disc[nCntDisc].pos = D3DXVECTOR3(-200.0f, 0.0f, 0.0f);
				g_Disc[nCntDisc].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_Disc[nCntDisc].bUse = true;
				g_Disc[nCntDisc].bGoal = false;
				g_Disc[nCntDisc].nGoal = 0;
				nTime = 0;

				Break2DPolygon(s_nPtsNum);
			}
			nTime++;
		}

		//1P���S�[��������2P���Ƀf�B�X�N����
		if (g_Disc[nCntDisc].bGoal == true && g_Disc[nCntDisc].nGoal == 2)
		{
			if (nTime >= 60)
			{
				g_Disc[nCntDisc].pos = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
				g_Disc[nCntDisc].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_Disc[nCntDisc].bUse = true;
				g_Disc[nCntDisc].bGoal = false;
				g_Disc[nCntDisc].nGoal = 0;
				nTime = 0;

				Break2DPolygon(s_nPtsNum);
			}
			nTime++;
		}

		//�ǂ̓����蔻��
		if (g_Disc[nCntDisc].pos.z >= 320.0f && g_Disc[nCntDisc].bUse)
		{
			g_Disc[nCntDisc].pos.z = 320.0f;
			g_Disc[nCntDisc].move.z *= -1;
			SetVibration(0);
			PlaySound(SOUND_LABEL_SE_FENCE);
		}
		else if (g_Disc[nCntDisc].pos.z <= -320.0f && g_Disc[nCntDisc].bUse)
		{
			g_Disc[nCntDisc].pos.z = -320.0f;
			g_Disc[nCntDisc].move.z *= -1;
			SetVibration(1);
			PlaySound(SOUND_LABEL_SE_FENCE);
		}
	}
}

//�f�B�X�N�̕`�揈��
void DrawDisc(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL * pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntDisc = 0; nCntDisc < MAX_DISC; nCntDisc++)
	{
		if (g_Disc[nCntDisc].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Disc[nCntDisc].mtxWorldDisc);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Disc[nCntDisc].rotDisc.y, g_Disc[nCntDisc].rotDisc.x, g_Disc[nCntDisc].rotDisc.z);
			D3DXMatrixMultiply(&g_Disc[nCntDisc].mtxWorldDisc, &g_Disc[nCntDisc].mtxWorldDisc, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Disc[nCntDisc].pos.x, g_Disc[nCntDisc].pos.y, g_Disc[nCntDisc].pos.z);
			D3DXMatrixMultiply(&g_Disc[nCntDisc].mtxWorldDisc, &g_Disc[nCntDisc].mtxWorldDisc, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Disc[nCntDisc].mtxWorldDisc);

			//���݂̃}�e���A����ێ�
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL *)g_pBuffMatDisc->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatDisc; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�f�B�X�N�p�[�c�̕`��
				g_pMeshDisc->DrawSubset(nCntMat);
			}
			//�ێ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//�f�B�X�N�̐ݒ菈��
void SetDisc(PlayerHaveDisc player)
{
	for (int nCnt = 0; nCnt < MAX_DISC; nCnt++)
	{
		switch (player)
		{
		case E_PLAYER_1:
			if (g_Disc[nCnt].bUse == false)
			{
				g_Disc[nCnt].pos = D3DXVECTOR3(-300.0f, 0.0f, 0.0f);
				g_Disc[nCnt].bUse = true;
				g_Disc[nCnt].bGoal = false;
				g_Disc[nCnt].nGoal = 0;
				g_Disc[nCnt].nDiscNumber = 0;
			}

			break;

		case E_PLAYER_2:
			if (g_Disc[nCnt].bUse == false)
			{
				g_Disc[nCnt].pos = D3DXVECTOR3(300.0f, 0.0f, 0.0f);
				g_Disc[nCnt].bUse = true;
				g_Disc[nCnt].bGoal = false;
				g_Disc[nCnt].nGoal = 0;
				g_Disc[nCnt].nDiscNumber = 1;
			}
			break;
		}
	}
}

//�f�B�X�N�̈ʒu��ς��鏈��
void SetDiscPos(int nDiscNumber, D3DXVECTOR3 pos)
{
	g_Disc[nDiscNumber].pos = pos;
	s_bHaving = true;
}

Disc * GetDisc(void)
{
	return &g_Disc[0];	//��ԍŏ��������Ă����܂���G�̏��������܂��̈Ӗ�
}

//------------------------
//�p�x�ύX
void TherowingDisc(int nRot, int nSpeed, int nNum)
{
	PlaySound(SOUND_LABEL_SE_SHOT);
	g_Disc[nNum].move.x = sinf(D3DXToRadian(nRot))* nSpeed;		//Z�@���s��
	g_Disc[nNum].move.z = cosf(D3DXToRadian(nRot))* nSpeed;		//X�@
	g_Disc[nNum].pos.y = 0.0f;
	s_bHaving = false;
}

void BreakDisc(void)
{
	for (int nCnt = 0; nCnt < MAX_DISC; nCnt++)
	{
		if (g_Disc[nCnt].bUse)
		{
			g_Disc[nCnt].bUse = false;
			g_Disc[nCnt].bGoal = false;
		}
	}
}