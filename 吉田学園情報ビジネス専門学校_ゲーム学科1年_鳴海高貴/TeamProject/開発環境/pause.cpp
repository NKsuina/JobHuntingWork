//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "pause.h"
#include "sound.h"

//=============================================================================
// スタティック変数
//=============================================================================
static LPDIRECT3DTEXTURE9 g_pTexturePause[MAX_PAUSE] = {};		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			//頂点バッファへのポインタ
static PAUSE_MENU g_PauseMenu;									//ポーズメニュー列挙型
static PAUSE g_Pause;											//ポーズメニュー構造体
static int nPauseSelect;										//ポーズ選択用の変数

//=============================================================================
// 初期化処理
//=============================================================================
void InitPause(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9  pDevice;		

	//デバイスの取得
	pDevice = GetDevice();

	//変数の初期化
	g_Pause.bPause = false;
	nPauseSelect = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pause100.png",
		&g_pTexturePause[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pause000.png",
		&g_pTexturePause[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pause001.png",
		&g_pTexturePause[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pause002.png",
		&g_pTexturePause[3]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_PAUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	//頂点へのポインタ
	VERTEX_2D*pVtx;		

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//ポーズの情報の初期化
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - MAX_RADIUS, SCREEN_HEIGHT / 2 - MAX_RADIUS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + MAX_RADIUS, SCREEN_HEIGHT / 2 - MAX_RADIUS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - MAX_RADIUS, SCREEN_HEIGHT / 2 + MAX_RADIUS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + MAX_RADIUS, SCREEN_HEIGHT / 2 + MAX_RADIUS, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 250.0f - MAX_SELECT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 250.0f - MAX_SELECT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 250.0f + MAX_SELECT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 250.0f + MAX_SELECT, 0.0f);

	pVtx[8].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 360.0f - MAX_SELECT, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 360.0f - MAX_SELECT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 360.0f + MAX_SELECT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 360.0f + MAX_SELECT, 0.0f);

	pVtx[12].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 470.0f - MAX_SELECT, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 470.0f - MAX_SELECT, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 470.0f + MAX_SELECT, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 470.0f + MAX_SELECT,0.0f);

	//RHWの設定
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

	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.7f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.7f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.7f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.7f);

	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	//テクスチャの座標設定
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

	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	int nCntPause;		//整数をカウントする用の変数

	for (nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{//テクスチャの破棄
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}

	if (g_pVtxBuffPause != NULL)
	{//頂点バッファの破棄
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{
	//フェードの情報をポインタに入れる
	FADE pFade;
	pFade = GetFade();

	if ((GetKeyboardTrigger(DIK_P) || GetJoypadAllTrigger(JOYKEY_START) )&& g_Pause.bPause == false)
	{//Pが入力された時
		////サウンドの再生
		PlaySound(SOUND_LABEL_SE_CLICK);
		nPauseSelect = 0;
		g_Pause.bPause = true;
	}

	else if ((GetKeyboardTrigger(DIK_P) || GetJoypadAllTrigger(JOYKEY_START)) && g_Pause.bPause == true)
	{//Pが入力された時
		////サウンドの再生
		PlaySound(SOUND_LABEL_SE_CLICK);
		g_Pause.bPause = false;
	}

	if (g_Pause.bPause == true)
	{
		if (pFade == FADE_NONE)
		{
			if (GetKeyboardTrigger(DIK_W) || GetJoypadAllTrigger(JOYKEY_UP) || GetJoypadStickAllTrigger(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP))
			{//Wキーが入力されたとき
				nPauseSelect--;
				////サウンドの再生
				PlaySound(SOUND_LABEL_SE_SELECTION);
				if (nPauseSelect < 0)
				{
					nPauseSelect = 2;
				}
			}
			else if (GetKeyboardTrigger(DIK_S) || GetJoypadAllTrigger(JOYKEY_DOWN) || GetJoypadStickAllTrigger(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN))
			{//Sキーが入力されたとき
				nPauseSelect++;
				////サウンドの再生
				PlaySound(SOUND_LABEL_SE_SELECTION);
				if (nPauseSelect >= 3)
				{
					nPauseSelect = 0;
				}
			}
		}
	}

	//ポーズ画面でのモード選択
	switch (nPauseSelect)
	{
	case 0:
		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A))
		{//CONTINUE
			////サウンドの再生
			PlaySound(SOUND_LABEL_SE_CLICK);
			g_Pause.bPause = false;
		}
		break;
	case 1:
		if ((GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A)) && pFade == FADE_NONE)
		{//RETRY
			////サウンドの再生
			PlaySound(SOUND_LABEL_SE_CLICK);
			SetFade(MODE_GAME);
		}
		break;
	case 2:
		if ((GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A)) && pFade == FADE_NONE)
		{//QUIT
			////サウンドの再生
			PlaySound(SOUND_LABEL_SE_CLICK);
			SetFade(MODE_TITLE);
		}
		break;
	}


	//ポーズ画面での選択肢の色の更新
	VERTEX_2D*pVtx;		//頂点へのポインタ

	//頂点バッファをロックする
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Pause.bPause == true)
	{

		if (nPauseSelect == 0)
		{
			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (nPauseSelect == 1)
		{
			pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (nPauseSelect == 2)
		{
			pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (nPauseSelect == 1 || nPauseSelect == 2)
		{
			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
		if (nPauseSelect == 0 || nPauseSelect == 1)
		{
			pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
		if (nPauseSelect == 0 || nPauseSelect == 2)
		{
			pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

	}

	//頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();

	//ポーズの有効無効設定
	SetEnablePause(g_Pause.bPause);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ
	int nCntPause;

	pDevice = GetDevice();		//代入

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		if (g_Pause.bPause == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

			//ポリゴンの描画処理
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//ポリゴンの形
				nCntPause * 4,				//頂点の開始場所
				2);							//プリミティブの数
		}
	}
}

//=============================================================================
// ポーズの情報を取得
//=============================================================================
PAUSE *GetPause(void)
{
	return &g_Pause;
}