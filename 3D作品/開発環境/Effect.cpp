//=============================================================================
//
// エフェクト処理 [camera.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "Effect.h"

//マクロ定義
#define MAX_EFFECT		(10000)
#define EFFECT_WIDTH	(20.0f)
#define EFFECT_HEIGHT	(20.0f)

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
Effect g_aEffect[MAX_EFFECT];

//=================================
//エフェクトの初期化処理
//=================================
void InitEffect(void)
{
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bakuha.png",
		&g_pTextureEffect);

	//弾の情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f ,0.0f);
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].nCounterAnim = 0;									//カウンターを初期化する
		g_aEffect[nCntEffect].nPatternAnim = 0;									//パターンNo.を初期化する
		g_aEffect[nCntEffect].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-30.0f, 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(30.0f, 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(30.0f, 0.0f, 0.0f);

		//各頂点の法線の設定（※ベクトルの大きさは1にする必要がある）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.3f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.3f);
		pVtx[3].tex = D3DXVECTOR2(0.3f, 0.3f);

		pVtx += 4;		//頂点データのポンタを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//==============================
//エフェクトの終了処理
//==============================
void UninitEffect(void)
{
		//テクスチャ破棄
		if (g_pTextureEffect != NULL)
		{
			g_pTextureEffect->Release();
			g_pTextureEffect = NULL;
		}
		//バッファ破棄
		if (g_pVtxBuffEffect != NULL)
		{
			g_pVtxBuffEffect->Release();
			g_pVtxBuffEffect = NULL;
		}
}

//=================================
//エフェクトの更新処理
//=================================
void UpdateEffect(void)
{
	int nCntEffect;
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている

		    //頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//エフェクトの座標更新 
			pVtx[0].pos = D3DXVECTOR3(-30.0f, 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(30.0f, 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(30.0f, 0.0f, 0.0f);

			g_aEffect[nCntEffect].nCounterAnim++;		//カウンターを加算
			if (g_aEffect[nCntEffect].nCounterAnim >= 3)
			{
				pVtx[0].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.3f);
				pVtx[3].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.3f);
			}

			if (g_aEffect[nCntEffect].nCounterAnim >= 6)
			{
				pVtx[0].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.3f);
				pVtx[1].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.3f);
				pVtx[2].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.6f);
				pVtx[3].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.6f);
			}

			if (g_aEffect[nCntEffect].nCounterAnim >= 9)
			{
				pVtx[0].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.6f);
				pVtx[1].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.6f);
				pVtx[2].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim, 0.9f);
				pVtx[3].tex = D3DXVECTOR2(0.3f*g_aEffect[nCntEffect].nPatternAnim + 0.3f, 0.9f);
				g_aEffect[nCntEffect].nCounterAnim = 0;			//カウントを0にする
			}

			g_aEffect[nCntEffect].nLife--;

			//寿命が尽きた
			g_aEffect[nCntEffect].nLife--;

			g_aEffect[nCntEffect].fRadius--;

			g_aEffect[nCntEffect].fRadius -= 0.25f;

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].fRadius = 0;
			}

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
		pVtx += 4;			//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//============================
//エフェクトの描画処理
//============================
void DrawEffect(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DXMATRIX  mtxView;

	//ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEffect = 0; nCntEffect< MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureEffect);

			//αテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			//カメラの逆行列を設定
			g_aEffect[nCntEffect].mtxWorld._11 = mtxView._11;
			g_aEffect[nCntEffect].mtxWorld._12 = mtxView._21;
			g_aEffect[nCntEffect].mtxWorld._13 = mtxView._31;
			g_aEffect[nCntEffect].mtxWorld._21 = mtxView._12;
			g_aEffect[nCntEffect].mtxWorld._22 = mtxView._22;
			g_aEffect[nCntEffect].mtxWorld._23 = mtxView._32;
			g_aEffect[nCntEffect].mtxWorld._31 = mtxView._13;
			g_aEffect[nCntEffect].mtxWorld._32 = mtxView._23;
			g_aEffect[nCntEffect].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			//αテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);

			//ライトを有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//αテストを無効化
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}
	}

	//αブレンディングをもとに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//===================================
//エフェクトの設定処理
//===================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntEffect;
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].bUse = true;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-30.0f,30.0f,0.0f);
			pVtx[1].pos = D3DXVECTOR3(30.0f, 30.0f,0.0f);
			pVtx[2].pos = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(30.0f, 0.0f,0.0f);

			//各頂点の法線の設定（※ベクトルの大きさは1にする必要がある）
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.3f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.3f);
			pVtx[3].tex = D3DXVECTOR2(0.3f, 0.3f);
			break;
		}
		pVtx += 4;			//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}