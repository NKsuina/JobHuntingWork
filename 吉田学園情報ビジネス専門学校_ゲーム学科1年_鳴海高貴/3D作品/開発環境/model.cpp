//=============================================================================
//
// ���f������ [model.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "Effect.h"
#include "main.h"
#include "input.h"
#include "model.h"
#include "score.h"
#include "camera.h"
#include "player.h"
#include "loadfile.h"
#include "sound.h"
#include "shadow.h"
#include <stdio.h>
#include <math.h>

//�}�N���ϐ�
#define NUM_MODEL (8)				//���f���̎��
#define MAX_MODEL (200)				//���f���̍ő吔
#define VTX	(10000)

//�O���[�o���ϐ��錾
typedef struct
{
	char aFileName[128];
}ModelSub;

Model g_model[MAX_MODEL];	
ModelSub g_modelsub[NUM_MODEL];
D3DXMATRIX g_mtxWorldModel;						//���[���h�}�g���b�N�X

//==============================================
//���f���̏���������
//==============================================
void InitModel(void)
{
	int nCntModel;		//���f���̎�ޗp�̕ϐ�

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	memset(&g_modelsub[0], 0, sizeof(g_modelsub)); 
	strcpy(&g_modelsub[0].aFileName[0], "data/MODEL/house.x");
	strcpy(&g_modelsub[1].aFileName[0], "data/MODEL/biru.x");
	strcpy(&g_modelsub[2].aFileName[0], "data/MODEL/dentyu.x");
	strcpy(&g_modelsub[3].aFileName[0], "data/MODEL/colorcone.x");
	strcpy(&g_modelsub[4].aFileName[0], "data/MODEL/kouzi_mokuzai.x");
	strcpy(&g_modelsub[5].aFileName[0], "data/MODEL/buranko.x");
	strcpy(&g_modelsub[6].aFileName[0], "data/MODEL/tree.x");

	for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_model[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_model[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_model[nCntModel].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_model[nCntModel].nType = 0;
		g_model[nCntModel].bUse = false;							//�g���Ă邩�ǂ���
	}
}

//====================================
//���f���̏I������
//====================================
void UninitModel(void)
{
	int nCntNullModel;		//�I�������Ɏg���ϐ�
	
	for (nCntNullModel = 0; nCntNullModel < NUM_MODEL; nCntNullModel++)
	{
		//���b�V���̔j��
		if (g_model[nCntNullModel].pBuffMatModel != NULL)
		{
			g_model[nCntNullModel].pMeshModel->Release();
			g_model[nCntNullModel].pMeshModel = NULL;
		}

		//�}�e���A���̔j��
		if (g_model[nCntNullModel].pBuffMatModel != NULL)
		{
			g_model[nCntNullModel].pBuffMatModel->Release();
			g_model[nCntNullModel].pBuffMatModel = NULL;
		}
	}
}

//================================
//���f���̍X�V����
//================================
void UpdateModel(void)
{
	
}

//================================
//���f���̕`�揈��
//================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�̃}�g���b�N�X
	D3DMATERIAL9 matDef;								//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;									//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntModelMtx = 0; nCntModelMtx < MAX_MODEL; nCntModelMtx++)
	{
		if (g_model[nCntModelMtx].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldModel);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_model[nCntModelMtx].rot.y, g_model[nCntModelMtx].rot.x, g_model[nCntModelMtx].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_model[nCntModelMtx].pos.x, g_model[nCntModelMtx].pos.y, g_model[nCntModelMtx].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

			//���݂̃}�e���A����ێ�
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_model[nCntModelMtx].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_model->nNumMatModel; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//���f���p�[�c�̕`��
				g_model[nCntModelMtx].pMeshModel->DrawSubset(nCntMat);
			}
			//�ێ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}

	}
}

//====================================
//���f���̔z�u����
//====================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, char *FileName)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BYTE *pVtxBuff;								//���_�o�b�t�@�ւ̃|�C���^
	DWORD sizeFVF;								//���_�t�H�[�}�b�g�̃T�C�Y

	int nNumVtx;								//���_��

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_model[nCntModel].bUse == false)
		{
			g_model[nCntModel].pos = pos;
			g_model[nCntModel].rot = rot;
			g_model[nCntModel].nType = nType;
			g_model[nCntModel].bUse = true;

			//x�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(&FileName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_model[nCntModel].pBuffMatModel,
				NULL,
				&g_model[nCntModel].nNumMatModel,
				&g_model[nCntModel].pMeshModel);

			g_model[nCntModel].vtxMaxModel = D3DXVECTOR3(-100000, -100000, -100000);
			g_model[nCntModel].vtxMinModel = D3DXVECTOR3(100000, 100000, 100000);

			//���_���̎擾
			nNumVtx = g_model[nCntModel].pMeshModel->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(g_model[nCntModel].pMeshModel->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_model[nCntModel].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//���_���W���r���ă��f���̍ŏ��l�A�ő�l���擾
				//x���W
				if (vtx.x < g_model[nCntModel].vtxMinModel.x)
				{
					g_model[nCntModel].vtxMinModel.x = floorf(vtx.x);
				}
				if (vtx.x > g_model[nCntModel].vtxMaxModel.x)
				{
					g_model[nCntModel].vtxMaxModel.x = floorf(vtx.x);
				}
				//y���W
				if (vtx.y < g_model[nCntModel].vtxMinModel.y)
				{
					g_model[nCntModel].vtxMinModel.y = floorf(vtx.y);
				}
				if (vtx.y > g_model[nCntModel].vtxMaxModel.y)
				{
					g_model[nCntModel].vtxMaxModel.y = floorf(vtx.y);
				}
				//z���W
				if (vtx.z < g_model[nCntModel].vtxMinModel.z)
				{
					g_model[nCntModel].vtxMinModel.z = floorf(vtx.z);
				}
				if (vtx.z > g_model[nCntModel].vtxMaxModel.z)
				{
					g_model[nCntModel].vtxMaxModel.z = floorf(vtx.z);
				}
				pVtxBuff += sizeFVF;

				g_model[nCntModel].size.x = g_model[nCntModel].vtxMaxModel.x - g_model[nCntModel].vtxMinModel.x;
				g_model[nCntModel].size.y = g_model[nCntModel].vtxMaxModel.y - g_model[nCntModel].vtxMinModel.y;
				g_model[nCntModel].size.z = g_model[nCntModel].vtxMaxModel.z - g_model[nCntModel].vtxMinModel.z;
			}
			//���_�o�b�t�@�̃��b�N
			g_model[nCntModel].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			//���_�o�b�t�@�̃A�����b�N
			g_model[nCntModel].pMeshModel->UnlockVertexBuffer();
			break;
		}
	}
}
//====================================
//���f���̓����蔻�菈��
//====================================
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *size)
{
	bool bIsLanding = false;		//���n���Ă��邩�ǂ���

	//Player *pPlayer;

	//�v���C���[�̓����蔻�菈��
	//pPlayer = GetPlayer();
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_model[nCntModel].bUse == true)
		{
			//���f���̍��������蔻��
			if (pPos->z - size->z / 2 < g_model[nCntModel].pos.z + g_model[nCntModel].vtxMaxModel.z
				&& pPos->z + size->z / 2 > g_model[nCntModel].pos.z + g_model[nCntModel].vtxMinModel.z
				&& pPosOld->x + size->x / 2 <= g_model[nCntModel].pos.x + g_model[nCntModel].vtxMinModel.x
				&& pPos->x + size->x / 2 > g_model[nCntModel].pos.x + g_model[nCntModel].vtxMinModel.x)
			{
				pPos->x = g_model[nCntModel].pos.x - size->x / 2.0f + g_model[nCntModel].vtxMinModel.x;
				SetEffect(g_model[nCntModel].pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 25.0f), 40, 50);

				if (g_model[nCntModel].nType == 0)		//�Ƃ��󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(300000);
				}
				if (g_model[nCntModel].nType == 1)		//�r�����󂵂���
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(1000000);
				}
				if (g_model[nCntModel].nType == 2)		//�d�����󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 3)		//�O�p�R�[�����󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(30000);
				}
				if (g_model[nCntModel].nType == 4)		//�H������̖؍ނ��󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 5)		//�u�����R���󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(100000);
				}
				if (g_model[nCntModel].nType == 6)		//�؂�j�󂵂���
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(20000);
				}
				g_model[nCntModel].bUse = false;
			}

			//���f���̉E�������蔻��
			else if(pPos->z - size->z / 2 < g_model[nCntModel].pos.z + g_model[nCntModel].vtxMaxModel.z
				&& pPos->z + size->z / 2 > g_model[nCntModel].pos.z + g_model[nCntModel].vtxMinModel.z
				&& pPosOld->x - size->x / 2 >= g_model[nCntModel].pos.x - g_model[nCntModel].vtxMinModel.x
				&& pPos->x - size->x / 2 < g_model[nCntModel].pos.x - g_model[nCntModel].vtxMinModel.x)
			{
				pPos->x = g_model[nCntModel].pos.x + size->x / 2.0f - g_model[nCntModel].vtxMinModel.x;
				SetEffect(g_model[nCntModel].pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 25.0f), 40, 50);

				if (g_model[nCntModel].nType == 0)		//�Ƃ��󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(300000);
				}
				if (g_model[nCntModel].nType == 1)		//�r�����󂵂���
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(1000000);
				}
				if (g_model[nCntModel].nType == 2)		//�d�����󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 3)		//�O�p�R�[�����󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(30000);
				}
				if (g_model[nCntModel].nType == 4)		//�H������̖؍ނ��󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 5)		//�u�����R���󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(100000);
				}
				if (g_model[nCntModel].nType == 6)		//�؂�j�󂵂���
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(20000);
				}

				g_model[nCntModel].bUse = false;
			}

			//���f���̉��������蔻��
			else if (pPos->x - size->x / 2 < g_model[nCntModel].pos.x + g_model[nCntModel].vtxMaxModel.x
				&& pPos->x + size->x / 2 > g_model[nCntModel].pos.x + g_model[nCntModel].vtxMinModel.x
				&& pPosOld->z - size->z / 2 >= g_model[nCntModel].pos.z - g_model[nCntModel].vtxMinModel.z
				&& pPos->z - size->z / 2 < g_model[nCntModel].pos.z - g_model[nCntModel].vtxMinModel.z)
			{
				pPos->z = g_model[nCntModel].pos.z + size->z / 2.0f - g_model[nCntModel].vtxMinModel.z;
				SetEffect(g_model[nCntModel].pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 25.0f), 40, 50);

				if (g_model[nCntModel].nType == 0)		//�Ƃ��󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(300000);
				}
				if (g_model[nCntModel].nType == 1)		//�r�����󂵂���
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(1000000);
				}
				if (g_model[nCntModel].nType == 2)		//�d�����󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 3)		//�O�p�R�[�����󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(30000);
				}
				if (g_model[nCntModel].nType == 4)		//�H������̖؍ނ��󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 5)		//�u�����R���󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(100000);
				}
				if (g_model[nCntModel].nType == 6)		//�؂�j�󂵂���
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(20000);
				}
				g_model[nCntModel].bUse = false;
			}

			//���f���̎�O�������蔻��
			else if (pPos->x - size->x / 2 < g_model[nCntModel].pos.x + g_model[nCntModel].vtxMaxModel.x
				&& pPos->x + size->x / 2 > g_model[nCntModel].pos.x + g_model[nCntModel].vtxMinModel.x
				&& pPosOld->z + size->z / 2 <= g_model[nCntModel].pos.z + g_model[nCntModel].vtxMinModel.z
				&& pPos->z + size->z / 2 > g_model[nCntModel].pos.z + g_model[nCntModel].vtxMinModel.z)
			{
				pPos->z = g_model[nCntModel].pos.z - size->z / 2.0f + g_model[nCntModel].vtxMinModel.z;
				SetEffect(g_model[nCntModel].pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 25.0f), 40, 50);

if (g_model[nCntModel].nType == 0)		//�Ƃ��󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(300000);
				}
				if (g_model[nCntModel].nType == 1)		//�r�����󂵂���
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(1000000);
				}
				if (g_model[nCntModel].nType == 2)		//�d�����󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 3)		//�O�p�R�[�����󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(30000);
				}
				if (g_model[nCntModel].nType == 4)		//�H������̖؍ނ��󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 5)		//�u�����R���󂵂��Ƃ�
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(100000);
				}
				if (g_model[nCntModel].nType == 6)		//�؂�j�󂵂���
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(20000);
				}
				g_model[nCntModel].bUse = false;
			}
		}
	}
	return bIsLanding;
}