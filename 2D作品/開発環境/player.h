#ifndef _POLY_H_			
#define _POLY_H_			

#include "main.h"

#define PLAYER_WIDTH (30.0f)	//プレイヤーのYサイズ
#define PLAYER_HEIGHT (30.0f)	//プレイヤーのXサイズ

typedef enum
{
	PLAYER_STAY = 0,		//止まっている時
	PLAYER_RUN,				//動いているとき
	PLAYER_JUMP,			//飛んだ時
	PLAYER_ATTACK,			//攻撃したとき
	PLAYER_DETH				//死んだとき
}E_PLAYER;

typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//過去のプレイヤー位置
	D3DXVECTOR3 move;		//移動量
	int nLife;				//体力
	int nCounterAnim;		//カウンター
	int nPatternAnim;		//パターン番号
	int nDirectionMove;		//向き
	int nCounterState;
	bool bUse;
	bool bDisp;				//表示するかしないか
	bool bIsJumping;		//ジャンプ中
	E_PLAYER state;
}Player;		

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif
