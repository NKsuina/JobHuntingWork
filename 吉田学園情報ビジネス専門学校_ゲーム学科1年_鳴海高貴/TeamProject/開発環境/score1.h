#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

#define RESULT_SCORE	(45.0f)		//リザルトでのスコアの大きさ
#define RESULT_SCOREX	(80.0f)		//リザルトでX軸どれくらいずらすか

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValse);
void SubScore(int nValse);
int GetScore(void);

#endif