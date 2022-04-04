//=============================================================================
//
// スコア処理 [score.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore, D3DXVECTOR3 pos);
void AddScore(int nValue);
int GetScore(void);

#endif