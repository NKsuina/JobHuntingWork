//-----------------------------------------------------------------------------------------------------------
//
//ゴール	
//Author;takano　minto
//
//-----------------------------------------------------------------------------------------------------------
#ifndef _GOAL_H_													//このマクロ定義がなされなっかたら
#define _GOAL_H_													//二重インクルード防止のマクロ定義

#include"main.h"

//-----------------
//マクロ定義
//-----------------
#define MAX_TEXTURE		(5)						//使用するテクスチャの数

//-----------------
//ゴール構造体
//-----------------
typedef struct
{
	D3DXVECTOR3 pos;								//視点
	D3DXVECTOR3 rot;								//向き
	D3DXVECTOR3 rotDest;							//角度の宛先
	D3DXMATRIX mtxWorld;							//ワールドマトリックス
	D3DXVECTOR3 Min;								//モデルの最小値
	D3DXVECTOR3 Max;								//モデルの最大値
	DWORD nNumMat;									//マテリアル情報の数
	LPD3DXMESH pMesh;								//メッシュ情報
	LPD3DXBUFFER pBuffMat;							//マテリアル情報
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEXTURE];		//テクスチャ情報
	D3DXVECTOR3 size;								//モデルサイズ
	bool bUse;										//使用しているか
}GOAL;

//-----------------
//プロトタイプ宣言
//-----------------
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void SetGoal(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void CollitionGoal(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size);
GOAL *GetGoal(void);

#endif // !GOAL_H_

