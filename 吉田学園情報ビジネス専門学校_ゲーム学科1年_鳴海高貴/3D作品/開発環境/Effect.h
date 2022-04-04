#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//エフェクトの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR col;			//色
	D3DXMATRIX mtxWorld;	//ワールドマトリックス			
	float fRadius;			//半径(大きい)
	int nCounterAnim;		//カウンター
	int nPatternAnim;		//パターン番号
	int nLife;				//寿命(表示時間)
	int nType;				//エフェクトの種類
	bool bUse;				//使用しているかどうか
}Effect;

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife);
#endif
