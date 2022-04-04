//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

#define NUM_MODELPARTS (2)		//プレイヤーのモデルパーツ数

//===================================================================
//モデル構造体
//===================================================================

typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//回転
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	LPD3DXMESH pMesh;			//メッシュ
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	int IdxNumber;
	int nldxModelParent;		//親のインデックス		↓のaModel[]の番号を入れる変数
}ModelParts;

typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posOld;		
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 size;			//プレイヤーのサイズ
	D3DXVECTOR3 rot;			//方向
	D3DXVECTOR3 rotDest;		//プレイヤーの角度調整
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 vtxMinPlayer;	//posの最小値
	D3DXVECTOR3 vtxMaxPlayer;	//posの最大値
	ModelParts aModel[NUM_MODELPARTS];
	int nScoreCnt;
	int nTime;					//プレイヤーの加速する間隔タイマー
	int nTimeCoolDown;
	int IdxShadow;
}Player;

//===================================================================
//プロトタイプ宣言
//===================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif