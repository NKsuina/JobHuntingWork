//=============================================================================
//
// ゲーム部分まとめ [game.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================

#include "Effect.h"
#include "camera.h"
#include "light.h"
#include "main.h"
#include "loadfile.h"
#include "meshfield.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "score.h"
#include "sound.h"
#include "time.h"
#include "wall.h"

//初期化処理関連
void InitGame(void)
{
	//サウンドの取得
	PlaySound(SOUND_LABEL_BGM001);

	//影の初期化
	InitShadow();

	//モデルの初期化
	InitModel();

	//メッシュフィールドの初期化
	InitMeshfield();

	//プレイヤーの初期化
	InitPlayer();

	//ライトの初期化
	InitLight();

	//カメラの初期化
	InitCamera();

	//壁の初期化
	InitWall();
	
	//スコアの初期化
	InitScore();

	//エフェクトの初期化
	InitEffect();

	//タイムの初期化
	InitTime();

	//モデルファイルの初期化
	InitLoadFile();
}

//終了処理関連
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//影の終了処理
	UninitShadow();

	//メッシュフィールドの終了処理
	UninitMeshfield();

	//モデルの終了処理
	UninitModel();

	//プレイヤーの終了処理
	UninitPlayer();

	//ライトの終了処理
	UninitLight();

	//カメラの終了処理
	UninitCamera();

	//壁の終了処理
	UninitWall();

	//エフェクトの終了処理
	UninitEffect();

	//スコアの終了処理
	UninitScore();

	//タイムの終了処理
	UninitTime();
}

//更新処理関連
void UpdateGame(void)
{
	//モデルの更新処理
	UpdateModel();

	//メッシュフィールドの更新処理
	UpdateMeshfield();

	//壁の更新処理
	UpdataWall();

	//プレイヤーの更新処理
	UpdatePlayer();

	//影の更新処理
	UpdataShadow();

	//エフェクトの更新処理
	UpdateEffect();

	//カメラの更新処理
	UpdateCamera();

	//ライトの更新処理
	UpdateLight();

	//スコアの更新処理
	UpdateScore();

	//タイムの更新処理
	UpdateTime();
}

//描画処理関連
void DrawGame(void)
{
	//カメラの描画処理
	SetCamera();

	//モデルの描画処理
	DrawModel();

	//メッシュフィールドの描画処理
	DrawMeshfield();

	//影の描画処理
	DrawShadow();

	//モデルの描画処理
	DrawModel();

	//プレイヤーの描画処理
	DrawPlayer();

	//壁の描画処理
	DrawWall();

	//エフェクトの描画処理
	DrawEffect();

	//スコアの描画処理
	DrawScore();

	//タイムの描画処理
	DrawTime();
}
