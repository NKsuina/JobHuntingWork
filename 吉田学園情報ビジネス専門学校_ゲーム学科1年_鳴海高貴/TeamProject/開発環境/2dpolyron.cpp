#include "2dpolyron.h"

#define POLYGON_2D_MAX			(120)

LPDIRECT3DTEXTURE9 g_pTexture2DPolygon[20] = { NULL };//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff2DPolygon = { NULL };//頂点バッファへのポインタ
Polygon_2D g_Polygon_2D[POLYGON_2D_MAX];

void Init2DPolygon(void)
{
	ZeroMemory(&g_Polygon_2D, sizeof(g_Polygon_2D));

	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

									//デバイスの取得
	pDevice = GetDevice();


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/3PTS_L.jpg",
		&g_pTexture2DPolygon[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/R_3PTS.jpg",
		&g_pTexture2DPolygon[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/5PTS_L.png",
		&g_pTexture2DPolygon[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/5PTS_R.png",
		&g_pTexture2DPolygon[3]);
	


	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * POLYGON_2D_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuff2DPolygon,
		NULL);
}

void Uninit2DPolygon(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < 20; nCnt++)
	{
		if (g_pTexture2DPolygon[nCnt] != NULL)
		{
			g_pTexture2DPolygon[nCnt]->Release();
			g_pTexture2DPolygon[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuff2DPolygon != NULL)
	{
		g_pVtxBuff2DPolygon->Release();
		g_pVtxBuff2DPolygon = NULL;
	}
}

void Update2DPolygon(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff2DPolygon->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCnt = 0; nCnt < POLYGON_2D_MAX; nCnt++)
	{
		if (g_Polygon_2D[nCnt].bUse)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_Polygon_2D[nCnt].pos.x - g_Polygon_2D[nCnt].fX, g_Polygon_2D[nCnt].pos.y - g_Polygon_2D[nCnt].fY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Polygon_2D[nCnt].pos.x + g_Polygon_2D[nCnt].fX, g_Polygon_2D[nCnt].pos.y - g_Polygon_2D[nCnt].fY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Polygon_2D[nCnt].pos.x - g_Polygon_2D[nCnt].fX, g_Polygon_2D[nCnt].pos.y + g_Polygon_2D[nCnt].fY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Polygon_2D[nCnt].pos.x + g_Polygon_2D[nCnt].fX, g_Polygon_2D[nCnt].pos.y + g_Polygon_2D[nCnt].fY, 0.0f);

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
		}
		pVtx += 4; //データを４つ分進める
	}

	//頂点バッファをアンロック
	g_pVtxBuff2DPolygon->Unlock();
}
void Draw2DPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuff2DPolygon, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < POLYGON_2D_MAX; nCnt++)
	{
		if (g_Polygon_2D[nCnt].bUse)
		{

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexture2DPolygon[g_Polygon_2D[nCnt].nPat]);


			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				4 * nCnt,						//描画する最初の頂点インデックス
				2);		//プリミティブ（ポリゴン）数
		}
	}
}

int Set2DPolygon(Polygon_2D Polygon_2D)
{
	for (int nCnt = 0; nCnt < POLYGON_2D_MAX; nCnt++)
	{
		if (!g_Polygon_2D[nCnt].bUse)
		{
			g_Polygon_2D[nCnt] = Polygon_2D;

			g_Polygon_2D[nCnt].bUse = true;

			return nCnt;
		}
	}
	return 0;
}

void Break2DPolygon(int nNum)
{
	g_Polygon_2D[nNum].bUse = false;
}