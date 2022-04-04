//=============================================================================
//
// ゴール時の処理 [goal.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"

#define GOAL_WIDTH	(25.0f)
#define GOAL_HEIGHT (50.0f)

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	bool bUse;			//使用しているかどうか
}Goal;

//プロトタイプ宣言
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void CollisionGoal(D3DXVECTOR3 *pPos, float fWidth, float fHeight);
void SetGoal(D3DXVECTOR3 pos);
#endif