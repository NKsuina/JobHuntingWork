//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------

#include "polygon.h"
#include "disc.h"
#include "input.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------

#define POLYGON_MAX				(256)	//ポリゴンの最大数
#define POLYGON_TEXTURE_MAX		(16)	//テクスチャの最大数

//-----------------------------------------------------------------------------
//構造体の定義
//-----------------------------------------------------------------------------

//ポリゴン構造体
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 rot;					//向き
	D3DXCOLOR col;						//カラー
	float fRadiusX; float fRadiusZ;		//各横縦幅
	D3DXMATRIX mtxWorld;				//ワールドマトリックス
	TexturetPolygon Texturetstate;		//テクスチャの状態
	bool bUse;							//使用しているかどうか

}PolygonStructure;

//-----------------------------------------------------------------------------
//グローバル変数宣言
//-----------------------------------------------------------------------------
static int count;

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePolygon[TEXTURE_MAX] = { NULL };//テクスチャへのポインタ
PolygonStructure g_aPolygon[POLYGON_MAX];//ポリゴン構造体


//*****************************************************************************
//ポリゴン初期化
//*****************************************************************************

void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//ポリゴンのテクスチャの初期化
	ZeroMemory(&g_pTexturePolygon[0], sizeof(g_pTexturePolygon));

	//テクスチャの読み込み
	//注意　０番目のテクスチャはNULLで！！
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/yuka.png",
		&g_pTexturePolygon[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/タイトル背景.png",
		&g_pTexturePolygon[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/kannkyakuyuka.png",
		&g_pTexturePolygon[3]);

	//---------------------------------
	//3ポイント
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/R_3PTS.jpg",
		&g_pTexturePolygon[TEXTURE_3PT_R]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/3PTS_L.jpg",
		&g_pTexturePolygon[TEXTURE_3PT_L]);
	//---------------------------------
	//5ポイント
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/5PTS_R.png",
		&g_pTexturePolygon[TEXTURE_5PT_R]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/5PTS_L.png",
		&g_pTexturePolygon[TEXTURE_5PT_L]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * POLYGON_MAX,//確保する頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	//ポリゴン構造体の初期化
	ZeroMemory(&g_aPolygon[0], sizeof(g_aPolygon));

	count = 0;

	//初期ポリゴンの配置
	SetPolygon(D3DXVECTOR3(SCREEN_STAGE_WIDTH * 2, 0.0f, SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(-SCREEN_STAGE_WIDTH * 2, 0.0f, SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(SCREEN_STAGE_WIDTH * 2, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(-SCREEN_STAGE_WIDTH * 2, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(0.0f, 0.0f, SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(0.0f, 0.0f, -SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(SCREEN_STAGE_WIDTH * 2, 0.0f, -SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);
	SetPolygon(D3DXVECTOR3(-SCREEN_STAGE_WIDTH * 2, 0.0f, -SCREEN_STAGE_DEPTH * 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_3);

	SetPolygon(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_STAGE_WIDTH, SCREEN_STAGE_DEPTH, TEXTURE_1);
}
//*****************************************************************************
//ポリゴンの終了処理
//*****************************************************************************

void UninitPolygon(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTexturePolygon[nCnt] != NULL)
		{
			g_pTexturePolygon[nCnt]->Release();
			g_pTexturePolygon[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}

}

//*****************************************************************************
//ポリゴンの更新処理
//*****************************************************************************
void UpdatePolygon(void)	
{

}
//*****************************************************************************
//ポリゴンの描画処理
//*****************************************************************************

void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		if (g_aPolygon[nCnt].bUse)
		{
			D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPolygon[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aPolygon[nCnt].rot.y, g_aPolygon[nCnt].rot.x, g_aPolygon[nCnt].rot.z);

			D3DXMatrixMultiply(&g_aPolygon[nCnt].mtxWorld, &g_aPolygon[nCnt].mtxWorld, &mtxRot);

			//位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_aPolygon[nCnt].pos.x, g_aPolygon[nCnt].pos.y, g_aPolygon[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aPolygon[nCnt].mtxWorld, &g_aPolygon[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPolygon[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePolygon[g_aPolygon[nCnt].Texturetstate]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCnt * 4,						//描画する最初の頂点インデックス
				2);		//プリミティブ（ポリゴン）数
		}
	}
}

//-----------------------------------------------------------------------------
//セットポリゴン処理
//-----------------------------------------------------------------------------

void SetPolygon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadiusX, float fRadiusZ, TexturetPolygon Texturetstate)
{
	VERTEX_3D *pVtx = NULL;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{

		if (!g_aPolygon[nCnt].bUse)
		{//条件が偽だったら

			//構造体を使用状態にする
			g_aPolygon[nCnt].bUse = true;
			g_aPolygon[nCnt].pos = pos;
			g_aPolygon[nCnt].rot = rot;
			g_aPolygon[nCnt].col = col;
			g_aPolygon[nCnt].fRadiusX = fRadiusX;
			g_aPolygon[nCnt].fRadiusZ = fRadiusZ;
			g_aPolygon[nCnt].Texturetstate = Texturetstate;

			//頂点座標の設定//ローカル座標で
			pVtx[0].pos = D3DXVECTOR3(-fRadiusX, 0.0f, fRadiusZ);
			pVtx[1].pos = D3DXVECTOR3(fRadiusX, 0.0f, fRadiusZ);
			pVtx[2].pos = D3DXVECTOR3(-fRadiusX, 0.0f, -fRadiusZ);
			pVtx[3].pos = D3DXVECTOR3(fRadiusX, 0.0f, -fRadiusZ);

			//各頂点の法線の設定（ベクトルの大きさは１にする必要がある）
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aPolygon[nCnt].col;
			pVtx[1].col = g_aPolygon[nCnt].col;
			pVtx[2].col = g_aPolygon[nCnt].col;
			pVtx[3].col = g_aPolygon[nCnt].col;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			break;
		}

		pVtx += 4; //データを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffPolygon->Unlock();
}
void FalseSet(void)
{
	for (int Count = 0; Count < POLYGON_MAX; Count++)
	{
		if (g_aPolygon[Count].Texturetstate >= TEXTURE_3PT_R)
		{
			g_aPolygon[Count].bUse = false;
		}
	}
}
void TrueSet(void)
{
	for (int a = 0; a < 2; a++)
	{
		for (int i = 0; i < 2; i++)
		{
			SetPolygon(D3DXVECTOR3(-500.0f + 1000 * a, 50.0f, -250.0f + 500 * i), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100, 100, TEXTURE_3PT_R);
			SetPolygon(D3DXVECTOR3(-500.0f + 1000 * a, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100, 100, TEXTURE_5PT_R);
		}
	}
}