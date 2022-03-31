//=============================================================================
//
// ブロック処理 [block.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

#define BLOCK_SIZE (21.0f)		//ブロックのサイズ

typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bIsLanding;		//
	bool bUse;				//使用しているかどうか				
}BLOCK;

//プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
BLOCK *GetBlock(void);

#endif
