//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "block.h"
#include "item.h"
#include "sound.h"
#include "enemy.h"
#include "fade.h"
#include "goal.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;		//頂点バッファへのポインタ
Player g_player;

//==============================
//プレイヤーの初期化処理
//==============================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player000.png",
		&g_pTexturePlayer);

	g_player.nDirectionMove = 0;								//向きを初期化
	g_player.nCounterAnim = 0;									//カウンターを初期化する
	g_player.nPatternAnim = 0;									//パターンNo.を初期化する
	g_player.nLife = 1;											//プレイヤーの体力
	g_player.nCounterState = 0;									//プレイヤーの状態
	g_player.bUse = true;
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//プレイヤーの移動量
	g_player.pos = D3DXVECTOR3(100.0f, 700.0f, 0.0f);			//プレイヤーの座標

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH /2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH /2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH /2, g_player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH /2, g_player.pos.y, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);


	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//==============================
//プレイヤーの終了処理
//==============================
void UninitPlayer(void)
{

	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}
//==============================
//プレイヤーの更新処理
//==============================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	BLOCK *g_aBlock;
	FADE g_fade;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH / 2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH / 2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH / 2, g_player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH / 2, g_player.pos.y, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_player.state)
	{
	case PLAYER_STAY:
		if (g_player.nDirectionMove == 0)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
		}

		//左移動
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_player.state = PLAYER_RUN;
		}

		//右移動
		if (GetKeyboardPress(DIK_D) == true)
		{
			g_player.state = PLAYER_RUN;
		}

		if (g_player.pos.y == SCREEN_HEIGHT)
		{
			g_player.bIsJumping = false;
		}

		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			if (g_player.bIsJumping == false)
			{
				g_player.move.y -= 10.5f;
				g_player.bIsJumping = true;
			}
		}
		break;

	case PLAYER_RUN:

		g_player.nCounterAnim++;		//カウンターを加算
		if (g_player.nCounterAnim >= 8)
		{
			g_player.nCounterAnim = 0;		//カウンターを初期値に戻す
			g_player.nPatternAnim++;			//パターンNo.を更新する

			if (g_player.nPatternAnim >= 8)
			{
				g_player.nPatternAnim = 0;		//パターンNo,を初期値に戻す
			}
		}
		if (g_player.nDirectionMove == 0)
		{
			if (g_player.nCounterAnim <= 4)
			{
				pVtx[0].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim + 0.25f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim, 0.5f);
				pVtx[3].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim + 0.25f, 0.5f);
			}
		}

		if (g_player.nDirectionMove == 1)
		{
			if (g_player.nCounterAnim <= 4)
			{
				pVtx[0].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim, 0.5f);
				pVtx[1].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim + 0.25f, 0.5f);
				pVtx[2].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.25f*g_player.nPatternAnim + 0.25f, 1.0f);
			}
		}

		if (g_player.bIsJumping == false)
		{
			if (g_player.move.y > 0)
			{
				g_player.nPatternAnim;
			}
		}

		//二段ジャンプできないように
		if (g_player.pos.y == SCREEN_HEIGHT)
		{
			g_player.bIsJumping = false;
		}

		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			if (g_player.bIsJumping == false)
			{
				g_player.move.y -= 10.5f;
				g_player.bIsJumping = true;
			}
		}

		//左移動
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_player.nDirectionMove = 1;
			g_player.move.x += sinf(-D3DX_PI * 0.5f) *0.5f;
			g_player.move.y += cosf(-D3DX_PI * 0.5f) *0.5f;
		}

		//右移動
		if (GetKeyboardPress(DIK_D) == true)
		{
			g_player.nDirectionMove = 0;
			g_player.move.x += sinf(D3DX_PI * 0.5f) *0.5f;
			g_player.move.y += cosf(D3DX_PI * 0.5f) *0.5f;
		}

		if (GetKeyboardPress(DIK_A) == false && GetKeyboardPress(DIK_D) == false)
		{
			g_player.state = PLAYER_STAY;
		}
		break;
	}

	g_player.move.y += 0.6f;			//重力加算

	g_aBlock = GetBlock();

	g_fade = GetFade();

	//前回の位置を保存
	g_player.posOld = g_player.pos;

	//位置を更新
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;

	//移動量を更新(減衰させる)
	g_player.move.x += (0.0f - g_player.move.x)* 0.1f;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH / 2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH / 2, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH / 2, g_player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH / 2, g_player.pos.y, 0.0f);

	//ブロックの当たり判定
	g_player.bIsJumping =! CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, PLAYER_WIDTH, PLAYER_HEIGHT);

	//敵の当たり判定
	CollisionEnemy(&g_player.pos, &g_player.posOld, &g_player.move, PLAYER_WIDTH, PLAYER_HEIGHT);

	//アイテムの当たり判定
	CollisionItem(&g_player.pos, PLAYER_WIDTH, PLAYER_HEIGHT);

	//ゴールの当たり判定
	CollisionGoal(&g_player.pos, PLAYER_WIDTH, PLAYER_HEIGHT);

	if (g_player.nLife == 0)
	{
		g_player.bUse = false;
		if (g_fade == FADE_NONE)
		{
			StopSound();
			PlaySound(SOUND_LABEL_SE_DETH);
			SetFade(MODE_RESULT);
		}
	}

		if (g_player.pos.y >= 1200)
		{//始点ｙ座標が画面下端に当たった
			if (g_fade == FADE_NONE)
			{
				StopSound();
				PlaySound(SOUND_LABEL_SE_DETH);
				SetFade(MODE_RESULT);
			}
		}
		if (g_player.pos.x >= SCREEN_WIDTH)
		{
			g_player.pos.x = SCREEN_WIDTH;
			g_player.move.x = 0.0f;
		}
		if (g_player.pos.x <= 0)
		{
			g_player.pos.x = 0;
			g_player.move.x = 0.0f;
		}
}

//==============================
//ポリゴンの描画処理
//==============================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.bUse == true)
	{//ポリゴンが使用されている
	 //テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0, 2);
	}
}
//===============================
//Playerの取得
//===============================
Player *GetPlayer(void)
{
	return &g_player;
}
