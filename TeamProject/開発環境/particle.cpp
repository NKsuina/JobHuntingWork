//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "particle.h"

//=============================================================================
// スタティック変数
//=============================================================================
static LPDIRECT3DTEXTURE9 g_pTexture = NULL;				//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff = NULL;			//頂点バッファへのポインタ
static Particle g_aParticle[MAX_PARTICLE];					//エフェクトの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitParticle(void)
{
	//乱数の初期化
	srand((unsigned int)time(0));

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Particle01.png",
		&g_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuff,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//頂点座標の初期化
		pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);

		//RHWの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの初期化
		pVtx[0].col = g_aParticle[nCntParticle].col;
		pVtx[1].col = g_aParticle[nCntParticle].col;
		pVtx[2].col = g_aParticle[nCntParticle].col;
		pVtx[3].col = g_aParticle[nCntParticle].col;

		//テクスチャの座標初期化
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//構造体の初期化
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
		g_aParticle[nCntParticle].fRadius = 0;								//半径
		g_aParticle[nCntParticle].fAngle = 0;								//角度
		g_aParticle[nCntParticle].fLength = 0;								//移動量
		g_aParticle[nCntParticle].fSubtractionRadius = 0;					//半径減算量
		g_aParticle[nCntParticle].fSubtractionα = 0;						//α値減算量
		g_aParticle[nCntParticle].nLife = 0;								//寿命
		g_aParticle[nCntParticle].nTipe = 0;								//タイプ
		g_aParticle[nCntParticle].bUse = false;								//使用してるかどうか

		//頂点ポイントを4つ進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuff->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	//テクスチャの破棄
	if (g_pTexture != NULL)
	{
		g_pTexture->Release();
		g_pTexture = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuff != NULL)
	{
		g_pVtxBuff->Release();
		g_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	//頂点情報へのポインタ
	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{//パーティクルが使用されていたら実行
			//位置の更新
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			//頂点カラーのα値をランダム値で下げていく
			g_aParticle[nCntParticle].col.a -= g_aParticle[nCntParticle].fSubtractionα;

			//寿命を徐々に削る
			g_aParticle[nCntParticle].nLife--;

			//寿命が0になった時に消す
			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);

			//頂点カラーの更新
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;
		}
		//頂点データを4つ進める
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, g_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//// αブレンディングを減算合成に設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexture);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParticle, 2);
		}
	}

	//aブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// セット処理
//=============================================================================
void SetParticle(D3DXVECTOR3 pos ,int nTipe)
{
	//頂点情報へのポインタ
	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			if (nTipe == 0)
			{//タイトル中のパーティクル
				g_aParticle[nCntParticle].pos = pos;
				g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314)) * ((float)(rand() % 15 + 1) / 10);
				g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314)) * ((float)(rand() % 15 + 1) / 10);
				g_aParticle[nCntParticle].move.z = 0.0f;
				g_aParticle[nCntParticle].nTipe = nTipe;
				g_aParticle[nCntParticle].fSubtractionα = (float)(rand() % 5 + 3) / 1000;	
				g_aParticle[nCntParticle].fRadius = (float)(rand() % 150 + 50) / 10;		
				g_aParticle[nCntParticle].nLife = 3000;								
				g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);			
				g_aParticle[nCntParticle].bUse = true;										
				break;
			}

			if (nTipe == 1)
			{//勝利時ののパーティクル
				g_aParticle[nCntParticle].pos = pos;
				g_aParticle[nCntParticle].move.x = (float)(rand() % 21 - 10) / 10;
				g_aParticle[nCntParticle].move.y = -((float)(rand() % 3 + 1)) * ((float)(rand() % 15 + 1) / 10);
				g_aParticle[nCntParticle].move.z = 0.0f;
				g_aParticle[nCntParticle].nTipe = nTipe;
				g_aParticle[nCntParticle].fSubtractionα = (float)(rand() % 5 + 3) / 1000;
				g_aParticle[nCntParticle].fRadius = (float)(rand() % 150 + 80) / 10;
				g_aParticle[nCntParticle].nLife = 3000;
				g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);
				g_aParticle[nCntParticle].bUse = true;
				break;
			}
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuff->Unlock();
}