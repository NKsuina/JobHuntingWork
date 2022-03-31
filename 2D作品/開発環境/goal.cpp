//=============================================================================
//
// ゴール時の処理 [goal.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "goal.h"
#include "player.h"
#include "fade.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureGoal = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGoal = NULL;		//頂点バッファへのポインタ
Goal g_aGoal;

//========================================
//  ゴールの初期化設定処理
//========================================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/hataa.png",
		&g_pTextureGoal);

	g_aGoal.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGoal.bUse = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGoal,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aGoal.pos.x - GOAL_WIDTH, g_aGoal.pos.y - GOAL_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aGoal.pos.x + GOAL_WIDTH, g_aGoal.pos.y - GOAL_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aGoal.pos.x - GOAL_WIDTH, g_aGoal.pos.y + GOAL_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aGoal.pos.x + GOAL_WIDTH, g_aGoal.pos.y + GOAL_HEIGHT, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	SetGoal(D3DXVECTOR3(80.0f, 50.0f, 0.0f));

	//頂点バッファをアンロックする
	g_pVtxBuffGoal->Unlock();
}
//========================================
//  ゴールの終了処理
//========================================
void UninitGoal(void)
{
	//テクスチャの破棄
	if (g_pTextureGoal != NULL)
	{			  
		g_pTextureGoal->Release();
		g_pTextureGoal = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGoal != NULL)
	{			
		g_pVtxBuffGoal->Release();
		g_pVtxBuffGoal = NULL;
	}
}
//========================================
//  ゴールの更新処理
//========================================
void UpdateGoal(void)
{

}
//========================================
//  ゴールの描画処理
//========================================
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGoal, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aGoal.bUse == true)
	{//ゴールが使用されている
	 //テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGoal);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,2);
	}
}
//===================================
//ゴールの当たり判定処理
//===================================
void CollisionGoal(D3DXVECTOR3 *pPos, float fWidth, float fHeight)
{
	//上の当たり判定処理
	if (g_aGoal.bUse)
	{//ゴールが使用されている
		if (pPos->x + (fWidth / 2) >= g_aGoal.pos.x - (GOAL_WIDTH / 2)
			&& pPos->x - (fWidth / 2) <= g_aGoal.pos.x + (GOAL_WIDTH / 2)
			&& pPos->y >= g_aGoal.pos.y - GOAL_HEIGHT
			&& pPos->y - fHeight <= g_aGoal.pos.y)
		{
			g_aGoal.bUse = false;
			SetFade(MODE_CLEAR);
		}

	}
}
//==============================
//ゴールの設定処理
//==============================
void SetGoal(D3DXVECTOR3 pos)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aGoal.bUse == false)
	{//ゴールが使用されていない場合
		g_aGoal.pos = pos;

		//頂点座標の設定処理
		pVtx[0].pos = D3DXVECTOR3(g_aGoal.pos.x - GOAL_WIDTH / 2, g_aGoal.pos.y - GOAL_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGoal.pos.x + GOAL_WIDTH / 2, g_aGoal.pos.y - GOAL_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGoal.pos.x - GOAL_WIDTH / 2, g_aGoal.pos.y + GOAL_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGoal.pos.x + GOAL_WIDTH / 2, g_aGoal.pos.y + GOAL_HEIGHT / 2, 0.0f);

		g_aGoal.bUse = true;		//使用している状態にする
	}
	//頂点バッファをアンロックする
	g_pVtxBuffGoal->Unlock();
}