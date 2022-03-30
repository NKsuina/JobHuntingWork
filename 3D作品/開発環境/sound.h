//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// タイトルBGM
	SOUND_LABEL_BGM001,			// ゲームBGM
	SOUND_LABEL_BGM002,			// リザルトBGM
	SOUND_LABEL_SE_PUSH,		// エンターキーを押したときの音
	SOUND_LABEL_SE_BREAK,		// 建物を破壊した時の音
	SOUND_LABEL_SE_TIMEUP,		// 時間切れになった時の音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif