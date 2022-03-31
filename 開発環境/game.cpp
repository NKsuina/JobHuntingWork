//=============================================================================
//
// ゲーム部分まとめ [game.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "Player.h"
#include "main.h"
#include "input.h"
#include "bg.h"
#include "item.h"
#include "block.h"
#include "Enemy.h"
#include "score.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "map.h"
#include "goal.h"

void InitGame(void)
{
	//サウンドの取得
	PlaySound(SOUND_LABEL_BGM001);

	//背景の初期化処理
	InitBG();

	//スコアの初期化処理
	InitScore();

	//アイテムの初期化処理
	InitItem();

	//敵の初期化処理
	InitEnemy();

	//ポリゴンの初期化処理
	InitPlayer();

	//ゴールの初期化処理
	InitGoal();

	//ブロックの初期化処理
	InitBlock();

	InitMap();

}

void UninitGame(void)
{

	//サウンドの停止
	StopSound();

	//背景の終了処理
	UninitBG();

	//スコアの終了処理
	UninitScore();
	
	//アイテムの終了処理
	UninitItem();

	//敵の終了処理
	UninitEnemy();

	//ポリゴンの終了処理
	UninitPlayer();

	//ゴールの終了処理
	UninitGoal();

	//ブロックの初期化処理
	UninitBlock();
}

void UpdateGame(void)
{

	//背景の更新処理
	UpdateBG();

	//スコアの更新処理
	UpdateScore();

	//アイテムの更新処理
	UpdateItem();
	
	//敵の更新処理
	UpdateEnemy();

	//ポリゴンの更新処理
	UpdatePlayer();

	//ゴールの更新処理
	UpdateGoal();
	
	//ブロックの更新処理
	UpdateBlock();
}

void DrawGame(void)
{

	//背景の描画処理
	DrawBG();

	//スコアの描画処理
	DrawScore();

	//アイテムの描画処理
	DrawItem();

	//敵の描画処理
	DrawEnemy();

	//ポリゴンの描画処理
	DrawPlayer();

	//ゴールの描画処理
	DrawGoal();

	//ブロックの描画処理
	DrawBlock();
}
