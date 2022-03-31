//=============================================================================
//
// �A�C�e������ [item.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

#define MAX_ITEM	(100)		//�A�C�e���ő吔
#define	ITEM_SIZE	(21.0f)	//�A�C�e���̉��̑傫��

//�G�\���̂̒�`
typedef struct
{
D3DXVECTOR3	move;	//�ړ���
D3DXVECTOR3 pos;	//�ʒu
bool bUse;			//�g�p���Ă��邩�ǂ���
}ITEM;

//�v���g�^�C�v�錾
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos);
void CollisionItem(D3DXVECTOR3 *pPos, float fWidth, float fHeight);
//int GetItem(void);

#endif