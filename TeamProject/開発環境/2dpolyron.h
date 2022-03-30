#ifndef _2DPOLYGON_H_			//このマクロ定義がされなかったら
#define _2DPOLYGON_H_			//2重インクルード防止のマクロ定義

#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;
	float fX;
	float fY;
	int nPat;
	bool bUse;
}Polygon_2D;

// プロトタイプ宣言
void Init2DPolygon(void);
void Uninit2DPolygon(void);
void Update2DPolygon(void);
void Draw2DPolygon(void);

int Set2DPolygon(Polygon_2D Polygon_2D);
void Break2DPolygon(int nNum);
#endif