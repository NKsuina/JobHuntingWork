//=============================================================================
//
// ���f���\���v���O���� [model.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

typedef struct
{
	LPD3DXMESH		pMeshModel;
	LPD3DXBUFFER	pBuffMatModel;
	DWORD			nNumMatModel;
	D3DXVECTOR3		vtxMinModel;
	D3DXVECTOR3		vtxMaxModel;
	D3DXVECTOR3		pos;			//�ʒu
	D3DXVECTOR3		move;			//�ړ���
	D3DXVECTOR3		rot;			//����
	D3DXVECTOR3		size;			//���f���̃T�C�Y
	D3DXMATRIX		mtxWorld;		//���[���h�}�g���b�N�X
	bool bUse;						//�g�p���Ă��邩�ǂ���
	int nType;
	int IdxShadow;					//�e�̐ݒu
}Model;

//�v���g�^�C�v�錾
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetModel(D3DXVECTOR3 pos , D3DXVECTOR3 rot, int nType, char *FileName);
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *size);
Model *GetModel(void);

#endif