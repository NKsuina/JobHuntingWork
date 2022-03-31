//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "score.h"
#include "item.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureItem= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;						
D3DXVECTOR3 g_posItem;							//位置
D3DXVECTOR3 g_moveItem;							//移動量		
ITEM g_aItem[MAX_ITEM];
//==============================
//アイテムの初期化処理
//==============================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntItem;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/coin000.png",
		&g_apTextureItem);

	for (int nCntItem = 0; nCntItem <MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posItem.x - ITEM_SIZE/2, g_posItem.y - ITEM_SIZE/2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posItem.x + ITEM_SIZE/2, g_posItem.y - ITEM_SIZE/2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posItem.x - ITEM_SIZE/2, g_posItem.y + ITEM_SIZE/2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posItem.x + ITEM_SIZE/2, g_posItem.y + ITEM_SIZE/2, 0.0f);

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
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}
//============================
//アイテムの終了処理
//============================
void UninitItem(void)
{
	//テクスチャの破棄
	if (g_apTextureItem != NULL)
	{
		g_apTextureItem->Release();
		g_apTextureItem = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}
//====================================
//アイテムの更新処理
//====================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			g_aItem[nCntItem].pos.x += g_aItem[nCntItem].move.x;

			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_SIZE/2, g_aItem[nCntItem].pos.y - ITEM_SIZE/2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_SIZE/2, g_aItem[nCntItem].pos.y - ITEM_SIZE/2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_SIZE/2, g_aItem[nCntItem].pos.y + ITEM_SIZE/2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_SIZE/2, g_aItem[nCntItem].pos.y + ITEM_SIZE/2, 0.0f);
		}
		pVtx += 4;  //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();
}

//=================================
//アイテムの描画処理
//=================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntItem;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{//アイテムが使用されている
		 //テクスチャの設定
			pDevice->SetTexture(0, g_apTextureItem);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntItem * 4,
				2);
		}
	}
}

//===================================
//アイテムの当たり判定処理
//===================================
void CollisionItem(D3DXVECTOR3 *pPos, float fWidth, float fHeight)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//上の当たり判定処理
		if (g_aItem[nCntItem].bUse)
		{//アイテムが使用されている
			if (pPos->x + (fWidth / 2) >= g_aItem[nCntItem].pos.x - (ITEM_SIZE / 2)
				&& pPos->x - (fWidth / 2) <= g_aItem[nCntItem].pos.x + (ITEM_SIZE / 2)
				&& pPos->y >= g_aItem[nCntItem].pos.y - ITEM_SIZE
				&& pPos->y - fHeight <= g_aItem[nCntItem].pos.y)
			{
				g_aItem[nCntItem].bUse = false;
				PlaySound(SOUND_LABEL_SE_GETCOIN);
				AddScore(200);
			}
		}
	}
}
//==============================
//アイテムの設定処理
//==============================
void SetItem(D3DXVECTOR3 pos)
{
	int nCntItem;
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{//アイテムが使用されていない場合
			g_aItem[nCntItem].pos = pos;

			//頂点座標の設定処理
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_SIZE/2, g_aItem[nCntItem].pos.y - ITEM_SIZE/2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_SIZE/2, g_aItem[nCntItem].pos.y - ITEM_SIZE/2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_SIZE/2, g_aItem[nCntItem].pos.y + ITEM_SIZE/2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_SIZE/2, g_aItem[nCntItem].pos.y + ITEM_SIZE/2, 0.0f);

			g_aItem[nCntItem].bUse = true;		//使用している状態にする

			break;		//←for文を抜け出す
		}
		pVtx += 4;			//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}