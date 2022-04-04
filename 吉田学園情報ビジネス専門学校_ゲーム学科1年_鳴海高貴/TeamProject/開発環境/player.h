//=============================================================================
//
// 処理 []
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _PLAYER_H_			//このマクロ定義がされなかったら
#define _PLAYER_H_			//2重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------

#include "main.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------

#define PARENT_MAX			(30)		//プレイヤーのパーツの最大量
#define MOTION_KEY_MAX		(10)		//モーションのキーの最大量

//-----------------------------------------------------------------------------
//列挙型の定義
//-----------------------------------------------------------------------------

//プレイヤーの状態
typedef enum
{
	PLAYER_NEUTRAL = 0,
	PLAYER_RUN,
	PLAYER_HAVE,
	PLAYER_SLIDING,
	PLAYER_MAX
}PlayerState;

//-----------------------------------------------------------------------------
//構造体
//-----------------------------------------------------------------------------

//モデルパーツ構造体
typedef struct
{
	D3DXVECTOR3	pos;					//位置
	D3DXVECTOR3	rot;					//向き
	D3DXVECTOR3 posMotionset[MOTION_KEY_MAX][MOTION_KEY_MAX];		//モーション中の位置
	D3DXVECTOR3 rotMotionset[MOTION_KEY_MAX][MOTION_KEY_MAX];		//モーション中の向き
	int nIndex;							//このパーツの番号
	int nParent;						//親パーツの番号
	bool bUse;							//使用しているかどうか
}PlayerParts;

//モーション情報構造
typedef struct
{
	int nLoop;
	int nNumKey;
	int nFrame[MOTION_KEY_MAX];
	bool bUse;
}MotionSet;

//プレイヤー構造体
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 posOld;					//過去の位置
	D3DXVECTOR3 posHand;				//位置(右手)
	D3DXVECTOR3 rotHand;				//向き(右手)
	D3DXVECTOR3 move;					//移動量(毎フレーム変動)
	D3DXVECTOR3 rot;					//向き
	D3DXVECTOR3 rotDest;				//目的の角度
	PlayerParts Parts[PARENT_MAX];		//モデルパーツ
	MotionSet Motion[MOTION_KEY_MAX];	//モーション情報
	int nShadow;						//影の番号
	int nNumParts;						//パーツ数
	float fMove;						//移動量(不変)
	float fJump;						//ジャンプ力
	D3DXVECTOR3 vtxMax;					//胴体の最大
	D3DXVECTOR3 vtxMin;					//胴体の最小
	PlayerState PlayerState;			//プレイヤーの状態
	int nNumDisc;						//今持ってるディスクの番号
	bool bDiscHave;						//ディスクを持ってるかどうか
	bool bUes;							//使用しているかどうか
}Player;


//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------

void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
Player GetPlayerData(void);
HRESULT LoadPlayerPattern(char *sXFilePath);
void SetPlayer(Player Player);
D3DXVECTOR3 GetPosPlayer(int nPlayerNum);
#endif