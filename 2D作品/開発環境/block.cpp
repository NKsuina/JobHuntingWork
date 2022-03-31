//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "player.h"
#include "main.h"
#include "block.h"
#include"player.h"

//マクロ定義
#define MAX_BLOCK	(1096)		//ブロックの最大数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBlock = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;			//頂点バッファへのポインタ	
BLOCK g_aBlock[MAX_BLOCK];								//ブロックの情報

//==============================
//ブロックの初期化処理
//==============================
void InitBlock(void)
{
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block000.jpg",
		&g_pTextureBlock);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//ブロックの情報の初期化
		g_aBlock[MAX_BLOCK].bUse = false;
		g_aBlock[MAX_BLOCK].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[MAX_BLOCK].fWidth = 0;
		g_aBlock[MAX_BLOCK].fHeight = 0;
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL
	);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - 21.0f / 2, g_aBlock[nCntBlock].pos.y - 21.0f /2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + 21.0f / 2, g_aBlock[nCntBlock].pos.y - 21.0f /2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - 21.0f / 2, g_aBlock[nCntBlock].pos.y + 21.0f /2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + 21.0f / 2, g_aBlock[nCntBlock].pos.y + 21.0f /2, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//頂点データのポンタを４つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//==============================
//ブロックの終了処理
//==============================
void UninitBlock(void)
{

	//テクスチャの破棄
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}
//==============================
//ブロックの更新処理
//==============================
void UpdateBlock(void)
{
	
}

//==============================
//ブロックの描画処理
//==============================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBlock);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//ブロックが使用されている
		 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntBlock * 4,
				2);
		}
	}
}

//===================================
//ブロックの当たり判定処理
//===================================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	bool bIsLanding = false;		//着地しているかどうか

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//上の当たり判定処理
		if (g_aBlock[nCntBlock].bUse)
		{//ブロックが使用されている
			if (pPos->x + fWidth / 2.0f > g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2.0f
				&& pPos->x - fWidth / 2.0f < g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2.0f
				&& pPosOld->y <= g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2.0f
				&& pPos->y > g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2.0f)
			{
				bIsLanding = true;
				pPos->y = g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2.0f;
				pMove->y = 0.0f;
			}

			//下の当たり判定処理
			if (pPos->x + fWidth / 2.0f > g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2.0f
				&& pPos->x - fWidth / 2.0f < g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2.0f
				&& pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + BLOCK_SIZE
				&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + BLOCK_SIZE)

			{
				pPos->y = g_aBlock[nCntBlock].pos.y + (BLOCK_SIZE / 2.0f) + fHeight;
				pMove->y = 0.0f;
			}

			//左の当たり判定処理
			if (pPos->x + fWidth / 2.0f > g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2.0f
				&& pPosOld->x - fWidth / 2.0f <= g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2.0f
				&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + BLOCK_SIZE / 2.0f
				&& pPos->y > g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2.0f)
			{
				pPos->x = g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2.0f - fWidth / 2.0f;
				pMove->x = 0.0f;
			}

			//右の当たり判定処理
			if (pPos->x - fWidth / 2.0f < g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2.0f
				&& pPosOld->x + fWidth / 2.0f >= g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2.0f
				&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + BLOCK_SIZE / 2.0f
				&& pPos->y > g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2.0f)
			{
				pPos->x = g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2.0f + fWidth / 2.0f;
				pMove->x = 0.0f;
			}
		}
	}
	return bIsLanding;
}

//===============================
//ブロックの設定処理
//===============================
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	int nCntBlock;
	VERTEX_2D *pVtx;

	//頂点情報をロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			g_aBlock[nCntBlock].fHeight = fHeight;
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].pos = pos;

			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2, g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2, g_aBlock[nCntBlock].pos.y - BLOCK_SIZE / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - BLOCK_SIZE / 2, g_aBlock[nCntBlock].pos.y + BLOCK_SIZE / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + BLOCK_SIZE / 2, g_aBlock[nCntBlock].pos.y + BLOCK_SIZE / 2, 0.0f);

			g_aBlock[nCntBlock].bUse = true;
			break;
		}
		pVtx += 4;			//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//===============================
//Blockの取得
//===============================
BLOCK *GetBlock(void)
{
	return &g_aBlock[MAX_BLOCK];
}