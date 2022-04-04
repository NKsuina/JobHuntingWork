//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "fade.h"

//=============================================================================
// スタティック変数
//=============================================================================
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffFade = NULL;		//頂点バッファへのポインタ
static FADE s_fade;											//フェードの状態
static MODE s_modeNext;										//次の画面(モード)
static D3DXCOLOR s_colorFade;								//ポリゴン(フェード)の色

//=============================================================================
// 初期化処理
//=============================================================================
void InitFade(MODE modeNext)
{
	s_fade = FADE_IN;									//フェードイン状態に
	s_modeNext = modeNext;								//次の画面(モード)を設定
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒いポリゴン(不透明)にしておく

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffFade,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	s_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(255, 255, 255, 255);
	pVtx[1].col = D3DXCOLOR(255, 255, 255, 255);
	pVtx[2].col = D3DXCOLOR(255, 255, 255, 255);
	pVtx[3].col = D3DXCOLOR(255, 255, 255, 255);

	//頂点バッファをアンロックする
	s_pVtxBuffFade->Unlock();

	//モードの設定
	SetMode(s_modeNext);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFade(void)
{
	//頂点バッファの破棄
	if (s_pVtxBuffFade != NULL)
	{
		s_pVtxBuffFade->Release();
		s_pVtxBuffFade = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFade(void)
{
	VERTEX_2D*pVtx;					//頂点情報へのポインタ
	s_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	if (s_fade != FADE_NONE)
	{
		if (s_fade == FADE_IN)
		{ //フェードイン状態
			s_colorFade.a -= 0.03f;	//ポリゴンを透明にしていく
			if (s_colorFade.a <= 0.0f)
			{
				s_colorFade.a = 0.0f;
				s_fade = FADE_NONE;		//何もしていない状態に
			}
		}

		else if (s_fade == FADE_OUT)
		{ //フェードアウト状態
			s_colorFade.a += 0.03f;	//ポリゴンを不透明にしていく
			if (s_colorFade.a >= 1.0f)
			{
				s_colorFade.a = 1.0f;
				s_fade = FADE_IN;		//フェードイン状態に

				//モード設定(次の画面に移行)
				SetMode(s_modeNext);
			}
		}

		//頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = s_colorFade;
		pVtx[1].col = s_colorFade;
		pVtx[2].col = s_colorFade;
		pVtx[3].col = s_colorFade;

		//頂点バッファをアンロックする
		s_pVtxBuffFade->Unlock();

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画       
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//ポリゴンの形
		0,						//頂点の開始場所
		2);						//プリミティブの数
}

//=============================================================================
// 設定処理
//=============================================================================
void SetFade(MODE modeNext)
{
	s_fade = FADE_OUT;										//フェードアウト状態に
	s_modeNext = modeNext;									//次の画面(モード)を設定
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//黒いポリゴン(透明)にしておく
}

//=============================================================================
// 情報の取得
//=============================================================================
FADE GetFade(void)
{
	return s_fade;
}