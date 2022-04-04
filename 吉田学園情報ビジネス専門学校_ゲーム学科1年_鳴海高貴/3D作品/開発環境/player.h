//=============================================================================
//
// �v���C���[���� [player.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

#define NUM_MODELPARTS (2)		//�v���C���[�̃��f���p�[�c��

//===================================================================
//���f���\����
//===================================================================

typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//��]
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	LPD3DXMESH pMesh;			//���b�V��
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	int IdxNumber;
	int nldxModelParent;		//�e�̃C���f�b�N�X		����aModel[]�̔ԍ�������ϐ�
}ModelParts;

typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 posOld;		
	D3DXVECTOR3 move;			//�ړ���
	D3DXVECTOR3 size;			//�v���C���[�̃T�C�Y
	D3DXVECTOR3 rot;			//����
	D3DXVECTOR3 rotDest;		//�v���C���[�̊p�x����
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 vtxMinPlayer;	//pos�̍ŏ��l
	D3DXVECTOR3 vtxMaxPlayer;	//pos�̍ő�l
	ModelParts aModel[NUM_MODELPARTS];
	int nScoreCnt;
	int nTime;					//�v���C���[�̉�������Ԋu�^�C�}�[
	int nTimeCoolDown;
	int IdxShadow;
}Player;

//===================================================================
//�v���g�^�C�v�錾
//===================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif