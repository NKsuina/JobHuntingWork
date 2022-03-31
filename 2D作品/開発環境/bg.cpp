//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "bg.h"
#include "main.h"
#include "input.h"

//マクロ定義
#define NUM_BG		(2)	//背景の数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBG[NUM_BG] = { NULL,NULL };			//テクスチャ(3枚分)のポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;						//頂点バッファへのポインタ	
D3DXVECTOR2 g_aPosTexV[NUM_BG];
D3DXVECTOR2 g_aMoveTexV;

//========================================
//  背景の初期化設定処理
//========================================
void InitBG(void)
{
	int nCnt;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/sky000.jpg",
		&g_pTextureBG[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\mountain000.png",
		&g_pTextureBG[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL
	);

	//位置の初期化
	for (nCnt = 0; nCnt < NUM_BG; nCnt++)
	{
		g_aPosTexV[nCnt] = D3DXVECTOR2(0.0f, 0.0f);
	}

	g_aMoveTexV = D3DXVECTOR2(0.0f, 0.0f);

	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		pVtx[4].pos = D3DXVECTOR3(0.0f, 500.0f, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH, 500.0f, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 8;		//頂点データのポインタを8つ進める

	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}
//===================================
//背景の終了処理
//===================================
void UninitBG(void)
{
	int nCnt;

	for (nCnt = 0; nCnt < NUM_BG; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureBG[nCnt] != NULL)
		{
			g_pTextureBG[nCnt]->Release();
			g_pTextureBG[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//==================================
//背景の更新処理
//==================================
void UpdateBG(void)
{
	VERTEX_2D * pVtx;

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

		//移動量を更新(加速)
		g_aPosTexV[0].x += (0.0001f);

		//頂点座標の更新
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexV[0].x + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexV[0].x + 0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexV[0].x + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexV[0].x + 0.25f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}
//==================================
//背景の描画処理
//==================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBG[nCntBG]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			nCntBG * 4,
			2);
	}
}
