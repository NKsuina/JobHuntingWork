//=============================================================================
//
// メイン処理 [mein.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _MAIN_H_			//このマクロ定義がされなかったら
#define _MAIN_H_			//二重インクルード防止のマクロ定義

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------

#include<windows.h>
#include"d3dx9.h"											//描画に必要
#define	 DIRECTINPUT_VERSION (0x0800)						//ビルド時の警告対策用マクロ(この位置から動かさない)
#include "dinput.h"											//入力処理に必要
#include "xaudio2.h"										//サウンド処理に必要
#include "Xinput.h"											//ジョイパット処理
#include <time.h>

//-----------------------------------------------------------------------------
//ライブラリのリンク
//-----------------------------------------------------------------------------

#pragma comment(lib,"d3d9.lib")			//描画に必要
#pragma comment(lib,"d3dx9.lib")		//d3d9.libの拡張ライブラリ
#pragma comment(lib,"dxguid.lib")		//DirectXコンポーネントに使用
#pragma comment(lib,"winmm.lib")		//システム時刻取得に必要


//-----------------------------------------------------------------------------
//マクロ定義x1280y720
//-----------------------------------------------------------------------------

#define SCREEN_WIDTH			(1280)												//ウインドウの幅
#define SCREEN_HEIGHT			(720)												//ウインドウの高さ
#define SCREEN_STAGE_WIDTH		(640)												//ゲームステージの幅5
#define SCREEN_STAGE_HEIGHT		(500)												//ゲームステージの高さ
#define SCREEN_STAGE_DEPTH		(360)												//ゲームステージの奥行き
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				//座標、カラー 、テクスチャ
#define FVF_VERTEX_LINE	(D3DFVF_XYZ | D3DFVF_DIFFUSE)								//座標、カラー
#define CLASS_NAME			"WindowClass"											//ウインドウクラス名
#define WINDOW_NAWE			"3DA"													//ウインドウの名前（キャプション表示）
#define SELECT_GAME_MODE (6) //ゲームモードの数
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE  | D3DFVF_TEX1)	//座標、法線、カラー 、テクスチャ

//-----------------------------------------------------------------------------
//構造体の定義
//-----------------------------------------------------------------------------

// 頂点情報[ライン]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DCOLOR col;			//頂点カラー
}VERTEX_LINE;

// 頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	float rhw;				//座標変換用係数（1.0fで固定）
	D3DCOLOR col;			//頂点カラー
	D3DXVECTOR2 tex;		//テクスチャ座標

}VERTEX_2D;

// 頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 nor;		//法線ベクトル
	D3DCOLOR col;			//頂点カラー
	D3DXVECTOR2 tex;		//テクスチャ座標
}VERTEX_3D;

//画面(モード)の種類
typedef enum
{
	MODE_TITLE = 0,		//タイトル画面
	MODE_TUTORIAL,		//チュートリアル画面
	MODE_GAME,			//ゲーム画面
	MODE_RESULT,		//リザルト画面
	MODE_RANKING,		//ランキング画面
	MODE_MAX
}MODE;

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------

LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif
