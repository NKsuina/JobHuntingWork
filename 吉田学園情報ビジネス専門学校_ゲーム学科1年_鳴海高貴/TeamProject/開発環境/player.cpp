//=============================================================================
//
// ���f������ [model.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------

#include "player.h"
#include "camera.h"
#include "input.h"
#include "calculation.h"
#include "polygon.h"
#include "disc.h"
#include "collision.h"

#include <assert.h>
#include "sound.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------

#define ANGLE_DAMPING_COEFFICIENT	(0.07f)					//�p�x�̌����W��
#define SCALE_DOWN					(1.0f)					//�X�P�[���T�C�Y�̕ύX
#define ANCHOR_POINT_MAX			(120)					//�J�����̈ړ��_
#define GRAVITY_MAX					(3.0f)					//�d��
#define ACCELERATION_MAX			(1.0f)					//�����̌��E
#define PLAYER_SPEED				(7.0f)
#define PLAYER_MAX					(2)


//-----------------------------------------------------------------------------
//�\����
//-----------------------------------------------------------------------------

//���[�V�����Đ����ɕK�v�ȏ��\����
typedef struct
{
	int nFrameCnt;					//���݂̃t���[����
	int nNowRebirthKeySet;			//�Đ����̃L�[�Z�b�g�ԍ�
	int nNowRebirthMotion;			//�Đ����̃��[�V�����ԍ�
}PlayerMotionData;

//���f���p�^�[���\����
typedef struct
{
	LPD3DXMESH pMesh = NULL;			//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;		//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat = 0;					//�}�e���A�����̐�
	bool bUse;							//�g�p���Ă��邩�ǂ���
}PlayerPartsPattern;

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------

D3DXMATRIX g_PlayerPartsmtxWorld[PARENT_MAX];				//���[���h�}�g���b�N�X(�v���C���[�p�[�c)
PlayerPartsPattern g_PartsPattern[PARENT_MAX];				//���f���p�^�[���\����
Player g_Player[PLAYER_MAX];											//�v���C���[�̍\����
char g_sPlayerModelFilename[PARENT_MAX][256];				//�t�@�C����
PlayerMotionData g_MotionPlayer[PLAYER_MAX];							//���[�V�����ɕK�v�ȃf�[�^�\����
float g_fDeceleration;										//�v���C���[�ړ��ʂ̌��������i�Q�[�����p�j
D3DXVECTOR3 g_PlayerPos[PLAYER_MAX];									//�Q�[�����̉��ړ��Ȃǂ���邽�߂�POS
int g_nLineNmn = 0;

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------

void PlayerMove(int nPlayerNum);
void MotionsetPlayer(int nPlayerNum);
void PlayerAction(int nPlayerNum);
void PlayerDraw(int nPlayerNum);
void ChangeMotion(int nPlayerNum,int nMotion, int nKey);
bool MoveKeyPlayer(int nPlayerNum, JOYKEY_CROSS Key);

//*****************************************************************************
//�v���C���[�̏�����
//*****************************************************************************

void InitPlayer(void)
{
	//�v���C���[�ɕK�v�ȑS���[���h�}�g���b�N�X�̏�����
	ZeroMemory(&g_PlayerPartsmtxWorld, sizeof(g_PlayerPartsmtxWorld));
	//�v���C���[�̍\���̂̏�����
	ZeroMemory(&g_PartsPattern, sizeof(g_PartsPattern));
	ZeroMemory(&g_Player, sizeof(g_Player));
	//���[�V�����ɕK�v�ȃf�[�^�\���̂̏�����
	ZeroMemory(&g_MotionPlayer, sizeof(g_MotionPlayer));
	//POS������
	ZeroMemory(&g_PlayerPos, sizeof(g_PlayerPos));

	g_PlayerPos[0] = D3DXVECTOR3(-400.0f, 0.0f, 0.0f);
	g_PlayerPos[1] = D3DXVECTOR3(400.0f, 0.0f, 0.0f);

	g_Player[0].nNumDisc = -1;
	g_Player[1].nNumDisc = -1;
	//�v���C���[�ړ��ʂ̌��������̏�����
	g_fDeceleration = 0.0f;
}

//*****************************************************************************
//�v���C���[�̏I������
//*****************************************************************************

void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < PARENT_MAX; nCnt++)
	{
		//���b�V���̔j��
		if (g_PartsPattern[nCnt].pMesh != NULL)
		{
			g_PartsPattern[nCnt].pMesh->Release();
			g_PartsPattern[nCnt].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_PartsPattern[nCnt].pBuffMat != NULL)
		{
			g_PartsPattern[nCnt].pBuffMat->Release();
			g_PartsPattern[nCnt].pBuffMat = NULL;
		}

	}
}

//*****************************************************************************
//�v���C���[�̍X�V����
//*****************************************************************************

void UpdatePlayer(void)
{
	////�J�����̏��̎擾�i�|�C���^�j
	//Camera *pCamera = GetCamera();

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//�v���C���[�̈ړ�
		PlayerMove(nCnt);
		//�v���C���[�̍s��
		PlayerAction(nCnt);
		//���[�V����
		if (g_MotionPlayer[nCnt].nNowRebirthMotion == g_Player[nCnt].PlayerState)
		{
			MotionsetPlayer(nCnt);
		}
	}
}

//*****************************************************************************
//�v���C���[�̕`�揈��
//*****************************************************************************

void DrawPlayer(void)
{
	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		PlayerDraw(nCnt);
	}
}

//-----------------------------------------------------------------------------
//�v���C���[�̕`�揈���i���g�j
//-----------------------------------------------------------------------------
void PlayerDraw(int nPlayerNum)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

									//�f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXMATRIX mtxRootFirst;			//�匳�̐e�̐e���[���h�}�g���b�N�X
	D3DXMATRIX mtxRoot;					//�匳�̐e�̃��[���h�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

										//���[���h�}�g���b�N�X�̏������i�匳�̐e�̐e�j
	D3DXMatrixIdentity(&mtxRootFirst);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player[nPlayerNum].rot.y, g_Player[nPlayerNum].rot.x, g_Player[nPlayerNum].rot.z);
	D3DXMatrixMultiply(&mtxRootFirst, &mtxRootFirst, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, g_PlayerPos[nPlayerNum].x, g_PlayerPos[nPlayerNum].y, g_PlayerPos[nPlayerNum].z);
	D3DXMatrixMultiply(&mtxRootFirst, &mtxRootFirst, &mtxTrans);

	//���[���h�}�g���b�N�X�̏������i�匳�̐e�j
	D3DXMatrixIdentity(&mtxRoot);

	D3DXMatrixScaling(&mtxRot,
		SCALE_DOWN, SCALE_DOWN, SCALE_DOWN);

	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxRot);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);

	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, g_Player[nPlayerNum].pos.x, g_Player[nPlayerNum].pos.y, g_Player[nPlayerNum].pos.z);

	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxTrans);

	//���f���̃}�g���b�N�X�@���@�e�̃��[���h�}�g���b�N�X
	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxRootFirst);

	for (int nCnt = 0; nCnt < PARENT_MAX; nCnt++)
	{
		if (g_Player[nPlayerNum].Parts[nCnt].bUse)
		{
			D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ�
			D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

											//���[���h�}�g���b�N�X�̏������i�q�j
			D3DXMatrixIdentity(&g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex]);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_Player[nPlayerNum].Parts[nCnt].rot.y, g_Player[nPlayerNum].Parts[nCnt].rot.x, g_Player[nPlayerNum].Parts[nCnt].rot.z);

			D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &mtxRot);

			//�ʒu�̔��f
			D3DXMatrixTranslation(&mtxTrans, g_Player[nPlayerNum].Parts[nCnt].pos.x, g_Player[nPlayerNum].Parts[nCnt].pos.y, g_Player[nPlayerNum].Parts[nCnt].pos.z);
			D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &mtxTrans);

			

			if (g_Player[nPlayerNum].Parts[nCnt].nParent < 0)
			{
				//���f���̃}�g���b�N�X�@���@�e�̃��[���h�}�g���b�N�X
				D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &mtxRoot);
			}
			else
			{
				//���f���̃}�g���b�N�X�@���@�e�̃��[���h�}�g���b�N�X
				D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nParent]);
			}

			if (nCnt == 4)
			{
				//�K�v���̃|�C���^���擾
				D3DXMATRIX *pMtxWorld = &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex];
  				g_Player[nPlayerNum].posHand = D3DXVECTOR3(pMtxWorld->_41, pMtxWorld->_42, pMtxWorld->_43);
			}

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex]);

			//���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_PartsPattern[g_Player[nPlayerNum].Parts[nCnt].nIndex].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_PartsPattern[g_Player[nPlayerNum].Parts[nCnt].nIndex].nNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);

				//���f���p�[�c�̕`��
				g_PartsPattern[g_Player[nPlayerNum].Parts[nCnt].nIndex].pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-----------------------------------------------------------------------------
//�v���C���[�̈ړ�
//-----------------------------------------------------------------------------
void PlayerMove(int nPlayerNum)
{
	//�ߋ��̈ʒu�̕ۑ�
	g_Player[nPlayerNum].posOld = g_PlayerPos[nPlayerNum] + g_Player[nPlayerNum].pos;

	//�J�����̏��̎擾�i�|�C���^�j
	Camera *pCamera = GetCamera();

	//���_�ړ�
	g_Player[nPlayerNum].PlayerState = PLAYER_RUN;
	g_MotionPlayer[nPlayerNum].nNowRebirthMotion = PLAYER_RUN;
	if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_UP))
	{//��L�[�������ꂽ
		if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_LEFT))
		{
			g_Player[nPlayerNum].rotDest.y = D3DX_PI * 0.75f + pCamera->rot.y;
			g_Player[nPlayerNum].move.x -= sinf(pCamera->rot.y + D3DX_PI * 0.75f) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z -= cosf(pCamera->rot.y + D3DX_PI * 0.75f) * g_Player[nPlayerNum].fMove;
		}
		else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_RIGHT))
		{
			g_Player[nPlayerNum].rotDest.y = D3DX_PI * -0.75f + pCamera->rot.y;
			g_Player[nPlayerNum].move.x -= sinf(pCamera->rot.y + D3DX_PI * -0.75f) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z -= cosf(pCamera->rot.y + D3DX_PI * -0.75f) * g_Player[nPlayerNum].fMove;
		}
		else
		{
			g_Player[nPlayerNum].rotDest.y = D3DX_PI + pCamera->rot.y;
			g_Player[nPlayerNum].move.x += sinf(pCamera->rot.y) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z += cosf(pCamera->rot.y) * g_Player[nPlayerNum].fMove;
		}
	}
	else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_DOWN))
	{//���L�[�������ꂽ
		if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_LEFT))
		{
			g_Player[nPlayerNum].rotDest.y = D3DX_PI * 0.25f + pCamera->rot.y;
			g_Player[nPlayerNum].move.x -= sinf(pCamera->rot.y + D3DX_PI * 0.25f) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z -= cosf(pCamera->rot.y + D3DX_PI * 0.25f) * g_Player[nPlayerNum].fMove;
		}
		else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_RIGHT))
		{
			g_Player[nPlayerNum].rotDest.y = D3DX_PI * -0.25f + pCamera->rot.y;
			g_Player[nPlayerNum].move.x -= sinf(pCamera->rot.y + D3DX_PI * -0.25f) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z -= cosf(pCamera->rot.y + D3DX_PI * -0.25f) * g_Player[nPlayerNum].fMove;
		}
		else
		{
			g_Player[nPlayerNum].rotDest.y = pCamera->rot.y;
			g_Player[nPlayerNum].move.x += sinf(pCamera->rot.y + D3DX_PI) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z += cosf(pCamera->rot.y + D3DX_PI) * g_Player[nPlayerNum].fMove;
		}
	}
	else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_LEFT))
	{//���L�[�������ꂽ
		g_Player[nPlayerNum].rotDest.y = D3DX_PI * 0.5f + pCamera->rot.y;
		g_Player[nPlayerNum].move.x += sinf(pCamera->rot.y + D3DX_PI * -0.5f) * g_Player[nPlayerNum].fMove;
		g_Player[nPlayerNum].move.z += cosf(pCamera->rot.y + D3DX_PI * -0.5f) * g_Player[nPlayerNum].fMove;
	}
	else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_RIGHT))
	{//�E�L�[�������ꂽ
		g_Player[nPlayerNum].rotDest.y = D3DX_PI * -0.5f + pCamera->rot.y;
		g_Player[nPlayerNum].move.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * g_Player[nPlayerNum].fMove;
		g_Player[nPlayerNum].move.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * g_Player[nPlayerNum].fMove;
	}
	else
	{
		ZeroMemory(&g_Player[nPlayerNum].move, sizeof(g_Player[nPlayerNum].move));
		
			g_Player[nPlayerNum].PlayerState = PLAYER_NEUTRAL;
			g_MotionPlayer[nPlayerNum].nNowRebirthMotion = PLAYER_NEUTRAL;
			//���[�V�����̏������ƃL�[�̃Z�b�g�ύX
			//ChangeMotion(g_MotionPlayer.nNowRebirthMotion, 1);
		
	}

	//rot���K�萔��蒴�����Ƃ��̕␳����
	g_Player[nPlayerNum].rot.y += Normalization(g_Player[nPlayerNum].rotDest.y/*�ړI�̊p�x*/ - g_Player[nPlayerNum].rot.y/*���݂̊p�x*/)
		* ANGLE_DAMPING_COEFFICIENT;

	//rot���K�萔��蒴�����Ƃ��̕␳
	g_Player[nPlayerNum].rot.y = Normalization(g_Player[nPlayerNum].rot.y);

	//�v���C���[POS�̍X�V
	g_PlayerPos[nPlayerNum] += g_Player[nPlayerNum].move * 1.0f;
	g_Player[nPlayerNum].move.x += (0.0f - g_Player[nPlayerNum].move.x) * 0.2f;
	g_Player[nPlayerNum].move.z += (0.0f - g_Player[nPlayerNum].move.z) * 0.2f;

	//�v���C���[�̓��̂�POS�X�V(�W�����v����)
	g_Player[nPlayerNum].pos.y -= (70.0f - g_Player[nPlayerNum].pos.y) * 0.1f;
	

	//��������
	if (g_fDeceleration < 0.9f)
	{
		g_fDeceleration += (ACCELERATION_MAX - g_fDeceleration) * 0.01f;
	}

	//��SCREEN_STAGE_WIDTH
	if (g_Player[nPlayerNum].pos.y <= 0.0f)
	{
		g_Player[nPlayerNum].pos.y = 0.0f;
	}

	if (g_PlayerPos[nPlayerNum].x > -100.0f
		&& nPlayerNum == 0)
	{
		g_PlayerPos[nPlayerNum].x = -100.0f;
	}
	else if (g_PlayerPos[nPlayerNum].x  < -SCREEN_STAGE_WIDTH + 100.0f
		&& nPlayerNum == 0)
	{
		g_PlayerPos[nPlayerNum].x = -SCREEN_STAGE_WIDTH + 100.0f;
	}

	if (g_PlayerPos[nPlayerNum].x > SCREEN_STAGE_WIDTH - 100.0f
		&& nPlayerNum == 1)
	{
		g_PlayerPos[nPlayerNum].x = SCREEN_STAGE_WIDTH - 100.0f;
	}
	else if (g_PlayerPos[nPlayerNum].x  < 100.0f
		&& nPlayerNum == 1)
	{
		g_PlayerPos[nPlayerNum].x = 100.0f;
	}

	if (g_PlayerPos[nPlayerNum].z > SCREEN_STAGE_DEPTH - 20.0f)
	{
		g_PlayerPos[nPlayerNum].z = SCREEN_STAGE_DEPTH - 20.0f;
	}
	else if (g_PlayerPos[nPlayerNum].z < -SCREEN_STAGE_DEPTH + 20.0f)
	{
		g_PlayerPos[nPlayerNum].z = -SCREEN_STAGE_DEPTH + 20.0f;
	}

}

//-----------------------------------------------------------------------------
//�v���C���[�s��
//-----------------------------------------------------------------------------
void PlayerAction(int nPlayerNum)
{
	//�f�B�X�N�̏��̎擾
	Disc *pDisc = GetDisc();
	//������
	for (int nCnt = 0; nCnt < MAX_DISC; nCnt++)
	{
		if (pDisc[nCnt].bUse
			&& CollisionCircle(pDisc[nCnt].pos, 20.0f, g_PlayerPos[nPlayerNum], 20.0f)
			&& !g_Player[nPlayerNum].bDiscHave
			&& MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_MAX))
		{
			PlaySound(SOUND_LABEL_SE_HIT);
			g_Player[nPlayerNum].nNumDisc = nCnt;
			g_Player[nPlayerNum].bDiscHave = true;
			break;
		}
	}

	if (g_Player[nPlayerNum].bDiscHave)
	{// 
		SetDiscPos(g_Player[nPlayerNum].nNumDisc, g_Player[nPlayerNum].posHand);
	}

	if (!MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_MAX)
		&& g_Player[nPlayerNum].bDiscHave)
	{//�E�L�[�������ꂽ
		g_Player[nPlayerNum].bDiscHave = false;
		int nRot = 1;

		if (nPlayerNum == 1)
		{
			nRot = -1;
		}

		if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_UP))
		{
			TherowingDisc(45 * nRot, 20, g_Player[nPlayerNum].nNumDisc);
		}
		else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_DOWN))
		{
			TherowingDisc(135 * nRot, 20, g_Player[nPlayerNum].nNumDisc);
		}
		else
		{
			TherowingDisc(90 * nRot, 20, g_Player[nPlayerNum].nNumDisc);
		}
	}
}

//-----------------------------------------------------------------------------
//�v���C���[���̎擾�i�|�C���^�j
//-----------------------------------------------------------------------------
Player *GetPlayer(void)
{
	return &g_Player[0];
}

//-----------------------------------------------------------------------------
//�v���C���[���̎擾�i�|�C���^�ł͂Ȃ��j
//-----------------------------------------------------------------------------
Player GetPlayerData(void)
{
	return g_Player[0];
}

//-----------------------------------------------------------------------------
//���f���^�C�v�ǂݍ��ݏ���
//-----------------------------------------------------------------------------

HRESULT LoadPlayerPattern(char *sXFilePath)
{

	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

									//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntPatn = 0; nCntPatn < PARENT_MAX; nCntPatn++)
	{
		if (!g_PartsPattern[nCntPatn].bUse)
		{

			//X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(sXFilePath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_PartsPattern[nCntPatn].pBuffMat,
				NULL,
				&g_PartsPattern[nCntPatn].nNumMat,
				&g_PartsPattern[nCntPatn].pMesh);

			g_PartsPattern[nCntPatn].bUse = true;

			return S_OK;
		}
	}

	assert(false);
//	MessageBox(hWnd, "�ǂݍ��݉\���f�����𒴂��Ă��܂�", "�x���I", MB_ICONWARNING);
	return E_FAIL;
}

//-----------------------------------------------------------------------------
//�v���C���[�̃Z�b�g����
//-----------------------------------------------------------------------------

void SetPlayer(Player Player)
{
	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		if (!g_Player[nCnt].bUes)
		{
			g_Player[nCnt] = Player;

			g_Player[nCnt].rotDest.y = D3DX_PI;
			g_Player[nCnt].rot.y = D3DX_PI;
			//g_PlayerPos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Player[nCnt].PlayerState = PLAYER_NEUTRAL;
			g_MotionPlayer[nCnt].nNowRebirthMotion = PLAYER_NEUTRAL;
			g_Player[nCnt].bUes = true;
			//���[�V�����̏������ƃL�[�̃Z�b�g�ύX
			ChangeMotion(nCnt,0, 1);

			int nNumVix;		//���_��
			DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
			BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

								//���f���̃T�C�Y�̔�r�p�����l
			g_Player[nCnt].vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
			g_Player[nCnt].vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

			//���_���̎擾
			nNumVix = g_PartsPattern[0].pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g�擾
			sizeFVF = D3DXGetFVFVertexSize(g_PartsPattern[0].pMesh->GetFVF());

			//���_���b�N
			g_PartsPattern[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			//���ׂĂ̒��_POS�̎擾
			for (int nCntVtx = 0; nCntVtx < nNumVix; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				if (g_Player[nCnt].vtxMax.x < vtx.x)
				{//X
					g_Player[nCnt].vtxMax.x = vtx.x;
				}
				if (g_Player[nCnt].vtxMin.x > vtx.x)
				{
					g_Player[nCnt].vtxMin.x = vtx.x;
				}

				if (g_Player[nCnt].vtxMax.y < vtx.y)
				{//Y
					g_Player[nCnt].vtxMax.y = vtx.y;
				}
				if (g_Player[nCnt].vtxMin.y > vtx.y)
				{
					g_Player[nCnt].vtxMin.y = vtx.y;
				}

				if (g_Player[nCnt].vtxMax.z < vtx.z)
				{//Z
					g_Player[nCnt].vtxMax.z = vtx.z;
				}
				if (g_Player[nCnt].vtxMin.z > vtx.z)
				{
					g_Player[nCnt].vtxMin.z = vtx.z;
				}

				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			g_Player[nCnt].vtxMax *= SCALE_DOWN;
			g_Player[nCnt].vtxMin *= SCALE_DOWN;


			//���_�A�����b�Na
			g_PartsPattern[0].pMesh->UnlockVertexBuffer();
		}
	}
}

//-----------------------------------------------------------------------------
//���[�V�����Đ�����
//-----------------------------------------------------------------------------

void MotionsetPlayer(int nPlayerNum)
{
	int nMotion = g_MotionPlayer[nPlayerNum].nNowRebirthMotion;
	int nKey = g_MotionPlayer[nPlayerNum].nNowRebirthKeySet;
	int nFrame = g_Player[nPlayerNum].Motion[nMotion].nFrame[nMotion];
	int nNumKey = g_Player[nPlayerNum].Motion[nMotion].nNumKey;

	if (nFrame <= 0)
	{//�e�L�X�g�ݒ�t���[�������O�ȉ���������
		nFrame = 1;
	}


	//���[�V�����Đ��i�S�p�[�c�jD3DXVec3Normalize
	for (int nCnt = 0; nCnt < PARENT_MAX; nCnt++)
	{
		if (g_Player[nPlayerNum].Parts[nCnt].bUse)
		{
			D3DXVECTOR3 move, rot;


			move = (g_Player[nPlayerNum].Parts[nCnt].posMotionset[nMotion][nKey] - g_Player[nPlayerNum].Parts[nCnt].posMotionset[nMotion][((nKey - 1) + nNumKey) % nNumKey]) / (float)g_Player[nPlayerNum].Motion[nMotion].nFrame[nKey];
			//POS�̍Đ��i���[�J�����W�j
			g_Player[nPlayerNum].Parts[nCnt].pos += move;

			rot = (g_Player[nPlayerNum].Parts[nCnt].rotMotionset[nMotion][nKey] - g_Player[nPlayerNum].Parts[nCnt].rotMotionset[nMotion][((nKey - 1) + nNumKey) % nNumKey]) / (float)g_Player[nPlayerNum].Motion[nMotion].nFrame[nKey];

			//ROT�̍Đ�
			g_Player[nPlayerNum].Parts[nCnt].rot += rot;
		}
	}


	//�t���[���̉��Z
	g_MotionPlayer[nPlayerNum].nFrameCnt++;

	if (g_Player[nPlayerNum].Motion[nMotion].nFrame[nKey] <= g_MotionPlayer[nPlayerNum].nFrameCnt)
	{//�t���[�������ݒ�̒l�𒴂�����

	 //���[�V�����̏������ƃL�[�̃Z�b�g�ύX
		ChangeMotion(nPlayerNum,nMotion, nKey);

		//�Đ����̃L�[���̉��Z
		g_MotionPlayer[nPlayerNum].nNowRebirthKeySet++;
		//�t���[���̏�����
		g_MotionPlayer[nPlayerNum].nFrameCnt = 0;

		if (g_Player[nPlayerNum].Motion[nMotion].nNumKey <= g_MotionPlayer[nPlayerNum].nNowRebirthKeySet)
		{//�Đ����̃L�[�����ݒ�̒l�𒴂�����
			if (g_Player[nPlayerNum].Motion[nMotion].nLoop == 1)
			{
				g_MotionPlayer[nPlayerNum].nNowRebirthKeySet = 0;

				//���[�V�����̏������ƃL�[�̃Z�b�g�ύX
				ChangeMotion(nPlayerNum,g_MotionPlayer[nPlayerNum].nNowRebirthMotion, 1);
			}
			else if (nMotion == PLAYER_HAVE)
			{
				g_MotionPlayer[nPlayerNum].nNowRebirthKeySet = 0;
				g_MotionPlayer[nPlayerNum].nNowRebirthMotion = 1;

				//���[�V�����̏������ƃL�[�̃Z�b�g�ύX
				//ChangeMotion(g_MotionPlayer.nNowRebirthMotion, g_MotionPlayer.nNowRebirthKeySet);
			}
			else
			{//���ݍĐ����̃��[�V�������烉�����[�V�����ɕύX
				g_MotionPlayer[nPlayerNum].nNowRebirthKeySet = 0;
				g_MotionPlayer[nPlayerNum].nNowRebirthMotion = 1;
				//���[�V�����̏������ƃL�[�̃Z�b�g�ύX
				ChangeMotion(nPlayerNum,g_MotionPlayer[nPlayerNum].nNowRebirthMotion, 1);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//���[�V�����̏������ƃL�[�̃Z�b�g�ύX
//-----------------------------------------------------------------------------
void ChangeMotion(int nPlayerNum,int nMotion, int nKey)
{
	for (int nCnt = 0; nCnt < PARENT_MAX; nCnt++)
	{
		if (g_Player[nPlayerNum].Parts[nCnt].bUse)
		{
			//g_Player.Parts[nCnt].pos = g_Player.Parts[nCnt].posMotionset[nMotion][nKey];
			g_Player[nPlayerNum].Parts[nCnt].rot = g_Player[nPlayerNum].Parts[nCnt].rotMotionset[nMotion][nKey];

		}
	}


}

//-----------------------------------------------------------------------------
//�v���C���[�̈ʒu�̎Q��
//-----------------------------------------------------------------------------

D3DXVECTOR3 GetPosPlayer(int nPlayerNum)
{
	return g_PlayerPos[nPlayerNum];
}

//-----------------------------------------------------------------------------
//�v���C���[�̃L�[�̓ǂݎ��
//-----------------------------------------------------------------------------

bool MoveKeyPlayer(int nPlayerNum, JOYKEY_CROSS Key)
{

	if (Key == JOYKEY_CROSS_UP)
	{
		if (GetJoypadPress(JOYKEY_UP, nPlayerNum)
			|| GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP, nPlayerNum)
			|| (GetKeyboardPress(DIK_W) && nPlayerNum == 0)
			|| (GetKeyboardPress(DIK_UP) && nPlayerNum == 1))
		{
			return true;
		}
	}
	else if (Key == JOYKEY_CROSS_LEFT)
	{
		if (GetJoypadPress(JOYKEY_LEFT, nPlayerNum)
			|| GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_LEFT, nPlayerNum)
			|| (GetKeyboardPress(DIK_A) && nPlayerNum == 0)
			|| (GetKeyboardPress(DIK_LEFT) && nPlayerNum == 1))
		{
			return true;
		}
	}
	else if (Key == JOYKEY_CROSS_RIGHT)
	{
		if (GetJoypadPress(JOYKEY_RIGHT, nPlayerNum)
			|| GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_RIGHT, nPlayerNum)
			|| (GetKeyboardPress(DIK_D) && nPlayerNum == 0)
			|| (GetKeyboardPress(DIK_RIGHT) && nPlayerNum == 1))
		{
			return true;
		}
	}
	else if (Key == JOYKEY_CROSS_DOWN)
	{
		if (GetJoypadPress(JOYKEY_DOWN, nPlayerNum)
			|| GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN, nPlayerNum)
			|| (GetKeyboardPress(DIK_S) && nPlayerNum == 0)
			|| (GetKeyboardPress(DIK_DOWN) && nPlayerNum == 1))
		{
			return true;
		}
	}
	else if (Key == JOYKEY_CROSS_MAX)
	{
		if (GetJoypadPress(JOYKEY_A, nPlayerNum)
			|| (GetKeyboardPress(DIK_SPACE) && nPlayerNum == 0)
			|| (GetKeyboardPress(DIK_RETURN) && nPlayerNum == 1))
		{
			return true;
		}
	}

	return false;
}