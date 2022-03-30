//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "game.h"
#include "input.h"
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "pause.h"
#include "result.h"
#include "player.h"
#include "disc.h"
#include "load.h"
#include "goal.h"

#include "wallmodel.h"
#include <time.h>
#include "billboard.h"
#include "particle.h"
#include "score2.h"
#include "score1.h"

#include "countdown.h"
#include "2dpolyron.h"
#include "sound.h"



//=============================================================================
// スタティック変数
//=============================================================================
static bool g_bPause = false;		//ポーズ中かどうか

//=============================================================================
// 初期化処理
//=============================================================================
void InitGame(void)
{
	PlaySound(SOUND_LABEL_BGM001);
	//乱数の初期化
	srand((unsigned int)time(0));

	Init2DPolygon();
	Initcountdown();

	//プレイヤー初期化
	InitPlayer();

	//ロード処理（プレイヤー）
	LoadSetPlayer();

	//ポリゴンの初期化処理
	InitPolygon();

	//ビルボードの初期化処理
	InitBillboard();

	//スコア1の初期化
	InitScore();

	//スコア2の初期化
	InitScore2();

	//ゴールの初期化処理
	InitGoal();

	//壁モデルの初期化処理
	InitWallModel();

	//ディスクの初期化処理
	InitDisc();

	//ディスクの位置を強制的に変える
	//SetDiscPos(0, D3DXVECTOR3(500.0f, 0.0f, 0.0f));

	//パーティクルの初期化処理
	InitParticle();

	//カメラの初期化処理
	InitCamera();

	//ビルボードファイルの読み込み処理
	InputBillboard();

	//ライトの初期化処理
	InitLight();

	//リザルトの初期化処理
	InitResult();

	//ポーズ画面の初期化処理
	InitPause();		

	//ポーズ解除
	g_bPause = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	Uninitcountdown();

	//ポリゴンの終了処理
	UninitPolygon();

	Uninit2DPolygon();
	//スコア1の終了処理
	UninitScore();

	//スコア2の終了処理
	UninitScore2();

	//ビルボードの終了処理
	UninitBillboard();

	//ディスクの終了処理
	UninitDisc();

	//パーティクルの終了処理
	UninitParticle();

	//壁モデルの終了処理
	UninitWallModel();

	//ゴールの終了処理
	UninitGoal();

	//プレイヤー終了処理
	UninitPlayer();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//リザルトの終了処理
	UninitResult();

	//ポーズ画面の終了処理
	UninitPause();		
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	//ポーズ情報の取得
	PAUSE *pPause = GetPause();

	if ((GetKeyboardTrigger(DIK_P) || GetJoypadAllTrigger(JOYKEY_START)) && g_bPause == false)
	{//ポーズキー(Pキー)が押された
		g_bPause = g_bPause ? false : true;		//trueならfalse,falseならtrue 三項演算子
	}

	if (g_bPause == true)
	{//ポーズ中
		//ポーズの更新処理
		UpdatePause();
	}

	if (g_bPause == false /*&& nTimer != 0*/)
	{//ポーズ中でなければ

		Updatecountdown();
		Update2DPolygon();
		//ポリゴンの更新処理
		UpdatePolygon();

		//スコア1の更新処理
		UpdateScore();

		//スコア２の更新処理
		UpdateScore2();

		//ディスクの更新処理
		UpdateDisc();

		//パーティクルの更新処理
		UpdateParticle();

		//壁モデルの更新処理
		UpdateWallModel();

		//ゴールの更新処理
		UpdateGoal();

		//プレイヤーの更新処理
		UpdatePlayer();

		//ビルボードの更新処理
		UpdatePlayer();

		//カメラの更新処理
		UpdateCamera();

		//ライトの更新処理
		UpdateLight();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{

	//カメラの設定処理
	SetCamera();

	//ポリゴンの描画処理
	DrawPolygon();

	//ディスクの描画処理
	DrawDisc();

	//ビルボードの描画処理
	DrawBillboard();

	//スコア1の描画処理
	DrawScore();

	//スコア2の描画処理
	DrawScore2();

	//壁モデルの描画処理
	DrawWallModel();

	Drawcountdown();

	//ゴールの描画処理
	DrawGoal();

	//プレイヤーの描画処理
	DrawPlayer();

	Draw2DPolygon();

	//パーティクルの描画処理
	DrawParticle();

	//リザルトの描画処理
	DrawResult();

	if (g_bPause == true)
	{//ポーズ中
		//ポーズの描画処理
		DrawPause();
	}
}

//=============================================================================
// ポーズの有効無効設定
//=============================================================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}