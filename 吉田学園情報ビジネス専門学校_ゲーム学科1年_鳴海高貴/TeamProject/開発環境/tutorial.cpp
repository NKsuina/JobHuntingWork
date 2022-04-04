//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "tutorial.h"
#include "title.h"
#include "fade.h"
#include "input.h"
#include "game.h"
#include "sound.h"

//=============================================================================
// スタティック変数
//=============================================================================
static LPDIRECT3DTEXTURE9 g_pTextureTutorial[3] = {};				//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;			//頂点バッファへのポインタ
static D3DXCOLOR g_EnterColer;										//「Press Enter」の色
static int g_nCounterEnterMoving;									//「Press Enter」の点滅のカウンター
static bool bEnterFlag2;											//「Press Enter」の点滅のフラグ
static int g_EnterCounter;											//エンター押した回数のカウンター

//=============================================================================
// 初期化処理
//=============================================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//グローバル変数の初期化
	g_EnterColer = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_nCounterEnterMoving = 0;
	bEnterFlag2 = false;
	g_EnterCounter = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Tutorial.png",
		&g_pTextureTutorial[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial001.jpg",
		&g_pTextureTutorial[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title005.png",
		&g_pTextureTutorial[2]);

	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 12,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[8].pos = D3DXVECTOR3(1110.0f - 150.0f, 640.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(1110.0f + 150.0f, 640.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(1110.0f - 150.0f, 710.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(1110.0f + 150.0f, 710.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
{
	//サウンドの停止
	//StopSound();

	//テクスチャの破棄
	for (int nCntTutorial = 0; nCntTutorial < 3; nCntTutorial++)
	{
		if (g_pTextureTutorial[nCntTutorial] != NULL)
		{
			g_pTextureTutorial[nCntTutorial]->Release();
			g_pTextureTutorial[nCntTutorial] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTutorial(void)
{
	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//フェードの情報をポインタに入れる
	FADE pFade;
	pFade = GetFade();

	//「Press Enter」を動かす処理
	g_nCounterEnterMoving++;

	//「Press Enter」の点滅する処理
	if (g_nCounterEnterMoving == 1)
	{
		bEnterFlag2 = false;
	}
	if (g_nCounterEnterMoving == 21)
	{
		bEnterFlag2 = true;
	}
	if (g_nCounterEnterMoving == 41)
	{
		g_nCounterEnterMoving = 0;
	}
	if (bEnterFlag2 == false)
	{
		g_EnterColer = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}
	if (bEnterFlag2 == true)
	{
		g_EnterColer = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (g_EnterCounter == 2)
	{
		g_EnterColer = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の更新
	pVtx[8].col = g_EnterColer;
	pVtx[9].col = g_EnterColer;
	pVtx[10].col = g_EnterColer;
	pVtx[11].col = g_EnterColer;

	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();

	//決定キー(ENTERキー)が押されたかどうか
	if ((GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A)) && pFade == FADE_NONE)
	{
		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_CLICK);;
		g_EnterCounter++;
		if (g_EnterCounter == 1)
		{
			//モードのセット処理
			SetFade(MODE_TITLE);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_EnterCounter == 0)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[0]);

		//ポリゴンの描画       
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//ポリゴンの形
			0,						//頂点の開始場所
			2);						//プリミティブの数
	}

	if (g_EnterCounter == 1)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[1]);

		//ポリゴンの描画       
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//ポリゴンの形
			4,						//頂点の開始場所
			2);						//プリミティブの数
	}

	////テクスチャの設定
	//pDevice->SetTexture(0, g_pTextureTutorial[2]);

	////ポリゴンの描画       
	//pDevice->DrawPrimitive(
	//	D3DPT_TRIANGLESTRIP,	//ポリゴンの形
	//	8,						//頂点の開始場所
	//	2);						//プリミティブの数
}