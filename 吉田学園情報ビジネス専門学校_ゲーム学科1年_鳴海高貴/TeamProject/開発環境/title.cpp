//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "title.h"
#include "camera.h"
#include "light.h"
#include "polygon.h"
#include "fade.h"
#include "result.h"
#include "particle.h"
#include "goal.h"
#include "wallmodel.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TITLE_TEXTURE	(3)		//使用するテクスチャの枚数

//=============================================================================
// スタティック変数
//=============================================================================
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_TITLE_TEXTURE] = { NULL };		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;					//頂点バッファへのポインタ
static TITLE s_Title[MAX_TITLE_TEXTURE];									//構造体
static int s_MenuSelect;											//ポーズ選択用の変数
static int s_MenuCol;												//メニューカラー変更用の変数
static int s_MoveSwitchCounter1;									//移動量スイッチのカウンター1
static int s_MoveSwitchCounter2;									//移動量スイッチのカウンター2

//=============================================================================
// 初期化処理
//=============================================================================
void InitTitle(void)
{
	PlaySound(SOUND_LABEL_BGM000);

	//ポリゴンの初期化処理
	InitPolygon();

	//パーティクルの初期化処理
	InitParticle();

	//カメラの初期化処理
	InitCamera();

	//ライトの初期化処理
	InitLight();

	//ゴールの初期化処理
	InitGoal();

	//壁モデルの初期化処理
	InitWallModel();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title000.png",
		&s_pTexture[0]);

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title001.png",
		&s_pTexture[1]);

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title002.png",
		&s_pTexture[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//変数の初期化
	s_MenuSelect = 0;

	//構造体の初期化
	s_Title[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, -300.0f, 0.0f);
	s_Title[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_Title[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 / 2, 600.0f, 0.0f);
	s_Title[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	s_Title[1].move = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	s_Title[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 2 / 2 + 90.0f), 600.0f, 0.0f);
	s_Title[2].move = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//頂点情報へのポインタ
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_TITLE_TEXTURE; i++)
	{
		//頂点座標の初期化
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの初期化
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの初期化
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の初期化
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点データを4つ進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	//サウンド停止
	StopSound();
	//ポリゴンの終了処理
	UninitPolygon();

	//パーティクルの終了処理
	UninitParticle();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//ゴールの終了処理
	UninitGoal();

	//壁モデルの終了処理
	UninitWallModel();

	for (int i = 0; i < MAX_TITLE_TEXTURE; i++)
	{
		//テクスチャの破棄
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}

		//頂点バッファの破棄
		if (s_pVtxBuff != NULL)
		{
			s_pVtxBuff->Release();
			s_pVtxBuff = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	//カメラの更新処理
	UpdateCamera();

	//カメラの自動旋回処理
	RotateCamera();

	//ポリゴンの更新処理
	UpdatePolygon();

	//パーティクルの更新処理
	UpdateParticle();

	//メニューセレクト処理
	MenuSelect();

	//テクスチャ更新処理
	TextureUpdate();

	//ライトの更新処理
	UpdateLight();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	//カメラの設定処理
	SetCamera();

	//壁モデルの描画処理
	DrawWallModel();
	//ゴールの描画処理
	DrawGoal();

	//ポリゴンの描画処理
	DrawPolygon();

	//パーティクルの描画処理
	DrawParticle();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_TITLE_TEXTURE; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[i]);

		//ポリゴンの描画処理
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//ポリゴンの形
			4 * i,						//頂点の開始場所
			2);							//プリミティブの数
	}
}

//=============================================================================
// カメラ自動旋回処理
//=============================================================================
void RotateCamera(void)
{
	//カメラの情報を取得
	Camera *pCamera = GetCamera();

	//カメラ旋回
	pCamera->rot.y -= 0.007f;
	pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
}

//=============================================================================
// テクスチャ,頂点情報更新処理
//=============================================================================
void TextureUpdate(void)
{
	//移動量設定
	if (s_Title[0].pos.y < 230.0f)
	{
		//移動量代入
		s_Title[0].move.y = 5.0f;
	}
	else
	{
		//静止させる
		s_Title[0].move.y = 0.0f;

		//パーティクルのセット処理
		SetParticle(D3DXVECTOR3(SCREEN_WIDTH / 2, 250.0f, 0.0f), 0);
	}

	//位置更新
	s_Title[0].pos += s_Title[0].move;

	//頂点情報へのポインタ
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(s_Title[0].pos.x - 350.0f, s_Title[0].pos.y - 150.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(s_Title[0].pos.x + 350.0f, s_Title[0].pos.y - 150.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(s_Title[0].pos.x - 350.0f, s_Title[0].pos.y + 150.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(s_Title[0].pos.x + 350.0f, s_Title[0].pos.y + 150.5f, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(s_Title[1].pos.x - 150.0f, s_Title[1].pos.y - 45.5f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(s_Title[1].pos.x + 150.0f, s_Title[1].pos.y - 45.5f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(s_Title[1].pos.x - 150.0f, s_Title[1].pos.y + 45.5f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(s_Title[1].pos.x + 150.0f, s_Title[1].pos.y + 45.5f, 0.0f);

	pVtx[8].pos = D3DXVECTOR3(s_Title[2].pos.x - 180.0f, s_Title[2].pos.y - 45.5f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(s_Title[2].pos.x + 180.0f, s_Title[2].pos.y - 45.5f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(s_Title[2].pos.x - 180.0f, s_Title[2].pos.y + 45.5f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(s_Title[2].pos.x + 180.0f, s_Title[2].pos.y + 45.5f, 0.0f);

	//位置と頂点カラーの更新
	if (s_MenuSelect == 0)
	{
		//位置更新
		s_Title[1].pos += s_Title[1].move;

		//色を付けて分かりやすくする
		pVtx[4].col = s_Title[1].col;
		pVtx[5].col = s_Title[1].col;
		pVtx[6].col = s_Title[1].col;
		pVtx[7].col = s_Title[1].col;

		//半透明にする
		pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	}
	else if (s_MenuSelect == 1)
	{
		//位置更新
		s_Title[2].pos += s_Title[2].move;

		//半透明にする
		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		//色を付けて分かりやすくする
		pVtx[8].col = s_Title[1].col;
		pVtx[9].col = s_Title[1].col;
		pVtx[10].col = s_Title[1].col;
		pVtx[11].col = s_Title[1].col;
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=============================================================================
// メニューセレクト処理
//=============================================================================
void MenuSelect(void)
{
	//フェードの情報を取得
	FADE pFade = GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_A) || GetJoypadAllTrigger(JOYKEY_LEFT) || GetJoypadStickAllTrigger(JOYKEY_LEFT_STICK, JOYKEY_CROSS_LEFT))
		{//Wキーが入力されたとき
			s_MenuSelect--;
			if (s_MenuSelect < 0)
			{
				s_MenuSelect = 1;
			}
			PlaySound(SOUND_LABEL_SE_SELECTION);
		}
		else if (GetKeyboardTrigger(DIK_D) || GetJoypadAllTrigger(JOYKEY_RIGHT) || GetJoypadStickAllTrigger(JOYKEY_LEFT_STICK, JOYKEY_CROSS_RIGHT))
		{//Sキーが入力されたとき
			s_MenuSelect++;
			if (s_MenuSelect >= 2)
			{
				s_MenuSelect = 0;
			}
			PlaySound(SOUND_LABEL_SE_SELECTION);
		}
	}

	//メニュー選択でフェード移行
	switch (s_MenuSelect)
	{
	case 0:
		if ((GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A)) && pFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_CLICK);
			//モードのセット処理
			SetFade(MODE_GAME);
			break;
		}
	case 1:
		if ((GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A)) && pFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_CLICK);
			//モードのセット処理
			SetFade(MODE_TUTORIAL);
			break;
		}
	}

	//カラースイッチ
	if (s_Title[1].col.b >= 1.0f)
	{
		s_MenuCol = 1;
	}
	else if (s_Title[1].col.b <= 0.0f)
	{
		s_MenuCol = 2;
	}

	//カラーの変更
	if (s_MenuCol == 1)
	{
		s_Title[1].col.b -= 0.02f;
	}
	else
	{
		s_Title[1].col.b += 0.02f;
	}

	//移動量スイッチ
	if (s_MenuSelect == 0)
	{
		s_Title[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 2 / 2 + 90.0f), 600.0f, 0.0f);
		s_MoveSwitchCounter1++;
		if (s_MoveSwitchCounter1 == 20)
		{
			s_Title[1].move *= -1;
			s_MoveSwitchCounter1 = 0;
		}
	}
	else if (s_MenuSelect == 1)
	{
		s_Title[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 / 2, 600.0f, 0.0f);
		s_MoveSwitchCounter2++;
		if (s_MoveSwitchCounter2 == 20)
		{
			s_Title[2].move *= -1;
			s_MoveSwitchCounter2 = 0;
		}
	}
}
