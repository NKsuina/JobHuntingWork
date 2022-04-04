//=============================================================================
//
// 敵処理 [enemy.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _ENEMY_H_
#define	_ENEMY_H_

#include "main.h"

//マクロ定義
#define MAX_ENEMY		(128)		//敵の最大数
#define ENEMY_HEIGHT	(21.0f)		//敵のYサイズ
#define ENEMY_WIDTH		(21.0f)		//敵のXサイズ

//敵構造体の定義
typedef struct
{
	D3DXVECTOR3	move;	//移動量
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posOld;		//過去のプレイヤー位置
	int nTime;			//カウントタイマー
	int nType;			//種類
	bool bIsCollide;
	bool bIsJumping;		//ジャンプしているかどうか
	bool bUse;			//使用しているかどうか
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
Enemy *GetEnemy(void);

#endif