//===================================================================
//
//タイム処理[time.h]
//Author : NARUMI KOUKI
//
//===================================================================
#ifndef _TIME_H_
#define _TIME_H_ 
#include "main.h"

//タイム構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 tex;
	int nTime;
}Time;

//マクロ定義
#define TIME_HEIGHT	(80.0f)
#define TIME_WIDTH	(40.0f)
#define TIME_NUM	(2)

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);
#endif