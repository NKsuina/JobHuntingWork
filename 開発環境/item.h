//=============================================================================
//
// アイテム処理 [item.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

#define MAX_ITEM	(100)		//アイテム最大数
#define	ITEM_SIZE	(21.0f)	//アイテムの横の大きさ

//敵構造体の定義
typedef struct
{
D3DXVECTOR3	move;	//移動量
D3DXVECTOR3 pos;	//位置
bool bUse;			//使用しているかどうか
}ITEM;

//プロトタイプ宣言
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos);
void CollisionItem(D3DXVECTOR3 *pPos, float fWidth, float fHeight);
//int GetItem(void);

#endif