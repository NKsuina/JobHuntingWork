//=============================================================================
//
// モデル表示プログラム [model.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

typedef struct
{
	LPD3DXMESH		pMeshModel;
	LPD3DXBUFFER	pBuffMatModel;
	DWORD			nNumMatModel;
	D3DXVECTOR3		vtxMinModel;
	D3DXVECTOR3		vtxMaxModel;
	D3DXVECTOR3		pos;			//位置
	D3DXVECTOR3		move;			//移動量
	D3DXVECTOR3		rot;			//方向
	D3DXVECTOR3		size;			//モデルのサイズ
	D3DXMATRIX		mtxWorld;		//ワールドマトリックス
	bool bUse;						//使用しているかどうか
	int nType;
	int IdxShadow;					//影の設置
}Model;

//プロトタイプ宣言
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetModel(D3DXVECTOR3 pos , D3DXVECTOR3 rot, int nType, char *FileName);
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *size);
Model *GetModel(void);

#endif