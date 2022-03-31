//=============================================================================
//
// �u���b�N���� [block.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

#define BLOCK_SIZE (21.0f)		//�u���b�N�̃T�C�Y

typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	float fWidth;			//��
	float fHeight;			//����
	bool bIsLanding;		//
	bool bUse;				//�g�p���Ă��邩�ǂ���				
}BLOCK;

//�v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
BLOCK *GetBlock(void);

#endif
