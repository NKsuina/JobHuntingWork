//=============================================================================
//
// タイトル処理 [title.h]
// Author : KADO TAKUMA
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
// 列挙型
//=============================================================================
typedef enum
{
	MENU_NORMAL = 0,
	MENU_GAME,
	MENU_TUTORIAL,	
	MENU_MAX
}MENUSTATE;

//=============================================================================
// 構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fWidth;
	float fHeight;
	float fLength;
	float fAngle;
}TITLE;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void RotateCamera(void);
void TextureUpdate(void);
void MenuSelect(void);

#endif