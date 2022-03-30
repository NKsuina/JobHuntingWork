#include "input.h"
#include "score1.h"
#include "score2.h"
#include "result.h"
#include "fade.h"
#include "sound.h"

//マクロ定義
#define NUM_SCORE (2)		//桁数

#define SCORE_SIZE 20

//グローバル変数
static LPDIRECT3DTEXTURE9 g_pTexture = NULL;					//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff = NULL;				//頂点バッファへのポインタ

static D3DXMATRIX mtxWorld;				//ワールドマトリックス
static D3DXVECTOR3 rot;					//向き
static D3DXCOLOR col;						//カラー

static D3DXVECTOR3 g_posScore;									//スコアの位置
static int g_nScore;											//スコアの値

//=========================
//スコア2の初期化処理
//=========================
void InitScore2(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number001.png",
		&g_pTexture);

	rot = D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f);
	g_posScore = D3DXVECTOR3(80.0f, 130.0f, 500.0f);			//位置を初期化する
	
	g_nScore = 0;											//値を初期化する
	int nCntScore;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		(sizeof(VERTEX_3D) * 4 * NUM_SCORE),		//4つで画像一個分
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuff,
		NULL);


	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//スコアの情報の初期化
	for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(37.5f* nCntScore-SCORE_SIZE, -SCORE_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(37.5f* nCntScore+SCORE_SIZE, -SCORE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(37.5f* nCntScore-SCORE_SIZE, +SCORE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(37.5f* nCntScore+SCORE_SIZE, + SCORE_SIZE, 0.0f);

		//g_posScore += D3DXVECTOR3(37.5f*nCntScore, 0.0f, 0.0f);

		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//頂点カラーの設定
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4; //頂点ポイントを四つ進む
	}

	//頂点バッファをアンロックする
	g_pVtxBuff->Unlock();

	
}

//==========================
//スコア2の終了処理
//==========================
void UninitScore2(void)
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

//==========================
//スコア2の更新処理
//==========================
void UpdateScore2(void)
{
	//フェードの情報を取得
	FADE pFade = GetFade();
	int nScore = GetScore();


	if (g_nScore >= 21 && pFade == FADE_NONE)
	{
		SetResult(nScore, g_nScore);
	}
}

//==========================
//スコア2の描画処理
//==========================
void DrawScore2(void)
{

	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);


	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, g_posScore.x, g_posScore.y, g_posScore.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	
		for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexture);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCntScore * 4,				//描画する最初の頂点インデックス
				2);							//プリミティブ（ポリゴン）の数
		}
}

//==========================
//スコア2のセット処理
//==========================
void SetScore2(int nScore)
{
	int aPosTexU[2];	//各桁の数字を格納

	g_nScore = nScore;

	/*aPosTexU[0] = (g_nScore % 1000000) / 100000;
	aPosTexU[1] = (g_nScore % 100000) / 10000;
	aPosTexU[2] = (g_nScore % 10000) / 1000;
	aPosTexU[3] = (g_nScore % 1000) / 100;*/
	aPosTexU[0] = (g_nScore % 100) / 10;
	aPosTexU[1] = (g_nScore % 10) / 1;

	VERTEX_3D*pVtx; //頂点へのポインタ	

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore2 = 0; nCntScore2 < NUM_SCORE; nCntScore2++)
	{

		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore2], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore2] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore2], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore2] + 0.1f, 1.0f);

		pVtx += 4; //頂点ポイントを四つ進む

	}

	//頂点バッファをアンロック
	g_pVtxBuff->Unlock();
}

//======================
//スコア2の加算処理
//======================
void AddScore2(int nValse)
{
	int aPosTexU[2]; //各桁の数字を格納

	g_nScore += nValse;

	//aPosTexU[0] = (g_nScore % 1000000) / 100000;
	//aPosTexU[1] = (g_nScore % 100000) / 10000;
	//aPosTexU[2] = (g_nScore % 10000) / 1000;
	//aPosTexU[3] = (g_nScore % 1000) / 100;
	aPosTexU[0] = (g_nScore % 100) / 10;
	aPosTexU[1] = (g_nScore % 10) / 1;

	VERTEX_3D*pVtx; //頂点へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 1.0f);

		pVtx += 4; //頂点ポイントを四つ進む
	}

	//頂点バッファをアンロック
	g_pVtxBuff->Unlock();

}

//======================
//スコア2の減算処理
//======================
void SubScore2(int nValse)
{
	int aPosTexU[6]; //各桁の数字を格納

	g_nScore -= nValse;

	//これしないと0以下のスコアが表記されてしまう！！
	if (g_nScore <= 0)
	{
		g_nScore = 0;
	}

	aPosTexU[0] = (g_nScore % 1000000) / 100000;
	aPosTexU[1] = (g_nScore % 100000) / 10000;
	aPosTexU[2] = (g_nScore % 10000) / 1000;
	aPosTexU[3] = (g_nScore % 1000) / 100;
	aPosTexU[4] = (g_nScore % 100) / 10;
	aPosTexU[5] = (g_nScore % 10) / 1;

	VERTEX_3D*pVtx; //頂点へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//テクスチャの座標設定
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 1.0f);

		pVtx += 4; //頂点ポイントを四つ進む
	}

	//頂点バッファをアンロック
	g_pVtxBuff->Unlock();

}

//=================
//スコア2の取得
//=================
int GetScore2(void)
{
	return g_nScore;
}