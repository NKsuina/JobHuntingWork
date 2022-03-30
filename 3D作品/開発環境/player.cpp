//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "Effect.h"
#include "main.h"
#include "input.h"
#include "model.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "wall.h"

//�O���[�o���ϐ��錾
Player g_Player;	
//ModelParts g_ModelParts;
//LPD3DXMESH g_pMeshPlayer = NULL;		//���b�V�����ւ̃|�C���^
//LPD3DXBUFFER g_pBuffMatPlayer = NULL;	//�}�e���A�����ւ̃|�C���^
//DWORD g_nNumMatPlayer = 0;				//�}�e���A�����̐�
//==============================================
//�v���C���[�̏���������
//==============================================
void InitPlayer(void)
{
	int nNumVtx;		//���_��

	g_Player.vtxMaxPlayer = D3DXVECTOR3(-10000, -10000, -10000);		//�v���C���[�̍ő�l��p��

	g_Player.vtxMinPlayer = D3DXVECTOR3(10000, 10000, 10000);			//�v���C���[�̍ŏ��l��p��

	//�v���C���[�̏���������
	g_Player.pos = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	g_Player.nTime = 0;									//�v���C���[�������ł���܂ł̎���
	g_Player.nTimeCoolDown = 0;							//�������Ă���̌v���p�ϐ�

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/player1.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[0].pBuffMat,		//�v���C���[�̃}�e���A��
		NULL,
		&g_Player.aModel[0].nNumMat,
		&g_Player.aModel[0].pMesh);

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/player2.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[1].pBuffMat,		//�v���C���[�̃}�e���A��
		NULL,
		&g_Player.aModel[1].nNumMat,
		&g_Player.aModel[1].pMesh);


	g_Player.aModel[0].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);		//�ʒu�̏����ݒ�
	g_Player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����̏����ݒ�
	g_Player.aModel[0].IdxNumber = 0;							//�C���f�b�N�X�ԍ�
	g_Player.aModel[0].nldxModelParent = -1;					//�e�̃C���f�b�N�X

	g_Player.aModel[1].pos = D3DXVECTOR3(0.0f, -3.0f, -17.0f);	//�ʒu�̏����ݒ�
	g_Player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����̏����ݒ�
	g_Player.aModel[1].IdxNumber = 1;							//�C���f�b�N�X�ԍ�
	g_Player.aModel[1].nldxModelParent = 0;						//�e�̃C���f�b�N�X

	////x�t�@�C���̓ǂݍ���
	//D3DXLoadMeshFromX("data/MODEL/airplane000.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatPlayer,
	//	NULL,
	//	&g_nNumMatPlayer,
	//	&g_pMeshPlayer);

	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y

	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntPlayer = 0; nCntPlayer < NUM_MODELPARTS; nCntPlayer++)
	{
		//���_���̎擾
		nNumVtx = g_Player.aModel[nCntPlayer].pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_Player.aModel[nCntPlayer].pMesh->GetFVF());

		//���_�o�b�t�@�̃��b�N
		g_Player.aModel[nCntPlayer].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//���_���W���r���ă��f���̍ŏ��l�A�ő�l���擾
			//x���W
			if (vtx.x < g_Player.vtxMinPlayer.x)
			{
				g_Player.vtxMinPlayer.x = floorf(vtx.x);
			}
			if (vtx.x > g_Player.vtxMaxPlayer.x)
			{
				g_Player.vtxMaxPlayer.x = floorf(vtx.x);
			}
			//y���W
			if (vtx.y < g_Player.vtxMinPlayer.y)
			{
				g_Player.vtxMinPlayer.y = floorf(vtx.y);
			}
			if (vtx.y > g_Player.vtxMaxPlayer.y)
			{
				g_Player.vtxMaxPlayer.y = floorf(vtx.y);
			}
			//z���W
			if (vtx.z < g_Player.vtxMinPlayer.z)
			{
				g_Player.vtxMinPlayer.z = floorf(vtx.z);
			}
			if (vtx.z > g_Player.vtxMaxPlayer.z)
			{
				g_Player.vtxMaxPlayer.z = floorf(vtx.z);
			}
			pVtxBuff += sizeFVF;
		}
		//���_�o�b�t�@�̃A�����b�N
		g_Player.aModel[nCntPlayer].pMesh->UnlockVertexBuffer();
	}
		g_Player.size.x = g_Player.vtxMaxPlayer.x - g_Player.vtxMinPlayer.x;
		g_Player.size.z = g_Player.vtxMaxPlayer.z - g_Player.vtxMinPlayer.z;

	//�ŏ��Ƀv���C���[���o��������W
	g_Player.pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e���v���C���[�ɃZ�b�g
	g_Player.IdxShadow = SetShadow(D3DXVECTOR3(g_Player.pos.x, 0.1f, g_Player.pos.z), g_Player.rot);
}

//====================================
//�v���C���[�̏I������
//====================================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < NUM_MODELPARTS; nCntPlayer++)
	{
		//���b�V���̔j��
		if (g_Player.aModel[nCntPlayer].pMesh != NULL)
		{
			g_Player.aModel[nCntPlayer].pMesh->Release();
			g_Player.aModel[nCntPlayer].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_Player.aModel[nCntPlayer].pBuffMat != NULL)
		{
			g_Player.aModel[nCntPlayer].pBuffMat->Release();
			g_Player.aModel[nCntPlayer].pBuffMat = NULL;
		}
	}
}

//================================
//�v���C���[�̍X�V����
//================================
void UpdatePlayer(void)
{
	Wall *Wall = GetWall();			//�ǂ̃|�C���^
	g_Player.nTime++;				
	Camera *pCamera = GetCamera();	//�J�����̃|�C���^

	//�O��̈ʒu��ۑ�
	g_Player.posOld = g_Player.pos;

	//���ړ�
	if (GetKeyboardPress(DIK_W) == true)
	{//���ړ����Ɂ��ړ�����ꂽ��
		if (GetKeyboardPress(DIK_D) == true)
		{
			g_Player.pos.x -= sinf(pCamera->rot.y + (D3DX_PI / 4 * -4.0f)) * 0.7f;
			g_Player.pos.z -= cosf(pCamera->rot.y + (D3DX_PI / 4 * -4.0f)) * 0.7f;
			g_Player.rotDest.y = (-D3DX_PI / 1.5f) + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//���ړ����Ɂ��ړ�����ꂽ��
			g_Player.pos.x -= sinf(pCamera->rot.y + (D3DX_PI / 4 * -4.0f)) * 0.7f;
			g_Player.pos.z -= cosf(pCamera->rot.y + (D3DX_PI / 4 * -4.0f)) * 0.7f;
			g_Player.rotDest.y = (D3DX_PI / 1.5f) + pCamera->rot.y;
		}
		else
		{//���ړ�����
			g_Player.pos.x += sinf(pCamera->rot.y) * 0.7f;
			g_Player.pos.z += cosf(pCamera->rot.y) * 0.7f;
			g_Player.rotDest.y = D3DX_PI + pCamera->rot.y;
		}
	}
	//���ړ�
	if (GetKeyboardPress(DIK_S) == true)
	{//���ړ����Ɂ��ړ�����ꂽ��
		if (GetKeyboardPress(DIK_D) == true)
		{
			g_Player.pos.x += sinf(pCamera->rot.y - (D3DX_PI / 4 * 4.0f)) * 0.7f;
			g_Player.pos.z += cosf(pCamera->rot.y - (D3DX_PI / 4 * 4.0f)) * 0.7f;
			g_Player.rotDest.y = (-D3DX_PI / 4) + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//���ړ����Ɂ��ړ�����ꂽ��
			g_Player.pos.x += sinf(pCamera->rot.y + (D3DX_PI / 4 * 4.0f)) * 0.7f;
			g_Player.pos.z += cosf(pCamera->rot.y + (D3DX_PI / 4 * 4.0f)) * 0.7f;
			g_Player.rotDest.y = (D3DX_PI / 4) + pCamera->rot.y;
		}
		else
		{//���ړ��̂�
			g_Player.pos.x += sinf(pCamera->rot.y + D3DX_PI) * 0.7f;
			g_Player.pos.z += cosf(pCamera->rot.y + D3DX_PI) * 0.7f;
			g_Player.rotDest.y = 0.0f + pCamera->rot.y;
		}
	}
	//���ړ�
	if (GetKeyboardPress(DIK_A) == true)
	{
		g_Player.pos.x += sinf(pCamera->rot.y - (D3DX_PI / 2)) * 0.7f;
		g_Player.pos.z += cosf(pCamera->rot.y - (D3DX_PI / 2)) * 0.7f;
		g_Player.rotDest.y = (D3DX_PI / 2) + pCamera->rot.y;
	}
	//���ړ�
	if (GetKeyboardPress(DIK_D) == true)
	{
		g_Player.pos.x += sinf(pCamera->rot.y + (D3DX_PI / 2)) * 0.7f;
		g_Player.pos.z += cosf(pCamera->rot.y + (D3DX_PI / 2)) * 0.7f;
		g_Player.rotDest.y = (-D3DX_PI / 2) + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		g_Player.pos.x += sinf(pCamera->rot.y) * 5.0f;
		g_Player.pos.z += cosf(pCamera->rot.y) * 5.0f;
		g_Player.rot.y = pCamera->rot.y + D3DX_PI;
	}

	for (int nCntWall = 0; nCntWall < 4; nCntWall++)
	{
		//�ǂ̉��ɍs���Ǝ~�܂�
		if (g_Player.pos.z >= Wall->pos.z + 500)
		{
			g_Player.pos.z = Wall->pos.z + 500;
		}
		//�ǂ̎�O�ɗ���Ǝ~�܂�
		if (g_Player.pos.z <= Wall->pos.z - 500)
		{
			g_Player.pos.z = Wall->pos.z - 500;
		}
		//�ǂ̉E���ɍs���Ǝ~�܂�
		if (g_Player.pos.x >= Wall->pos.x + 500)
		{
			g_Player.pos.x = Wall->pos.x + 500;
		}
		//�ǂ̍����ɍs���Ǝ~�܂�
		if (g_Player.pos.x <= Wall->pos.x - 500)
		{
			g_Player.pos.x = Wall->pos.x - 500;
		}
	}

	//�p�x�̍X�V
	if (g_Player.rotDest.y - g_Player.rot.y > D3DX_PI)
	{
		g_Player.rotDest.y -= D3DX_PI * 2;
	}
	if (g_Player.rotDest.y - g_Player.rot.y < -D3DX_PI)
	{
		g_Player.rotDest.y += D3DX_PI * 2;
	}
	g_Player.rot.y += (g_Player.rotDest.y - g_Player.rot.y) * 0.1f;

	//�p�x�̐��K��
	if (g_Player.rot.y > D3DX_PI)
	{
		g_Player.rot.y -= D3DX_PI * 2;
	}
	if (g_Player.rot.y < -D3DX_PI)
	{
		g_Player.rot.y += D3DX_PI * 2;
	}
	//�v���C���[�̃p�[�c���W
	D3DXVECTOR3 partsPos = D3DXVECTOR3(g_Player.aModel[1].mtxWorld._41, g_Player.aModel[1].mtxWorld._42, g_Player.aModel[1].mtxWorld._43);
	
	//�v���C���[�̃p�[�c�T�C�Y[41]��x���W[42]��y���W[43]��z���W
	D3DXVECTOR3 partsSize = D3DXVECTOR3(g_Player.aModel[1].mtxWorld._41 = 20, g_Player.aModel[1].mtxWorld._42 = 1, g_Player.aModel[1].mtxWorld._43 = 20);
	
	CollisionModel(&partsPos, &g_Player.posOld, &partsSize);

	//�v���C���[�ɉe�����鏈��
	SetPositionShadow(g_Player.IdxShadow, D3DXVECTOR3(g_Player.pos.x, 0.1f, g_Player.pos.z));
}

//================================
//�v���C���[�̕`�揈��
//================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�̃}�g���b�N�X
	D3DMATERIAL9 matDef;								//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;									//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);

	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);

	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

	for (int nCntPlayerModel = 0; nCntPlayerModel < NUM_MODELPARTS; nCntPlayerModel++)
	{
		//���f���p�[�c�̍\���̂̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Player.aModel[nCntPlayerModel].mtxWorld);

		//���f���p�[�c�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.aModel[nCntPlayerModel].rot.y, g_Player.aModel[nCntPlayerModel].rot.x, g_Player.aModel[nCntPlayerModel].rot.z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayerModel].mtxWorld, &g_Player.aModel[nCntPlayerModel].mtxWorld, &mtxRot);

		//���f���p�[�c�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Player.aModel[nCntPlayerModel].pos.x, g_Player.aModel[nCntPlayerModel].pos.y, g_Player.aModel[nCntPlayerModel].pos.z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayerModel].mtxWorld, &g_Player.aModel[nCntPlayerModel].mtxWorld, &mtxTrans);

		//�����̐e�̃��f���̃}�g���b�N�X�Ƃ̊|���Z
		D3DXMATRIX mtxParent;
		if (g_Player.aModel[nCntPlayerModel].nldxModelParent == -1)
		{
			mtxParent = g_Player.mtxWorld;
		}
		else
		{
			mtxParent = g_Player.aModel[g_Player.aModel[nCntPlayerModel].nldxModelParent].mtxWorld;
		}

		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayerModel].mtxWorld, &g_Player.aModel[nCntPlayerModel].mtxWorld, &mtxParent);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntPlayerModel].mtxWorld);

		//���݂̃}�e���A����ێ�
		pDevice->GetMaterial(&matDef);

		//���݂̃}�e���A����ێ�
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_Player.aModel[nCntPlayerModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntPlayerModel].nNumMat; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//���f���p�[�c�̕`��
			g_Player.aModel[nCntPlayerModel].pMesh->DrawSubset(nCntMat);
		}
		//�ێ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}
//=================================
//�v���C���[�̎擾
//=================================
Player* GetPlayer(void)
{
	return &g_Player;
}