//=============================================================================
//
// ビルボード処理 [billboard.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"

typedef enum
{
	NULLTEXTURE = 0,
	TEXTURE_AudienceMan,		//Type1
	TEXTURE_ElectricBulletin,	//Type2
	TEXTURE_AudienceWo,			//Type3
	TEXTURE_AudienceSara,		//Type4
	TEXTURE_AudienceAlien,		//Type5
	TEXTURE_AudienceBack,		//Type6
	TEXTURE_AudienceBro,		//type7
	TEXTURE_AudienceDog,		//type8
	BILLBOARD_TEXTURE_MAX
}E_BILLBOARD;

typedef struct
{
	D3DXVECTOR3 pos;		//ビルボードの座標
	D3DXVECTOR2 size;		//ビルボードのサイズ
	D3DXMATRIX mtxWorld;	//マトリックスワールド
	E_BILLBOARD nType;	//使用するテクスチャ
	bool bUse;				//使用しているかどうか
	int IdxShadow;			//影の設置に使う変数
}Billboard;

//プロトタイプ宣言
void InitBillboard(void);
void UninitBillboard(void);
void UpdataBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR2 size, E_BILLBOARD nType, char *FileName);
void InputBillboard(void);
//E_BILLBOARD texture		//←これ気にしないで

#endif