#include"Time.h"
#include"game.h"
#include"fade.h"
#include"countdown.h"
#include"Player.h"
#include "polygon.h"
#include "disc.h"

#define MAX				(256)

static LPDIRECT3DTEXTURE9 s_pTexturecountdown3 = NULL; //テクスチャのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffcountdown3 = NULL; //頂点バッファの設定
static D3DXVECTOR3 s_poscountdown3; //スコアの位置
static LPDIRECT3DTEXTURE9 s_pTexturecountdownBack = NULL; //テクスチャのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffcountdownBack = NULL; //頂点バッファの設定

static D3DXVECTOR3 s_poscountdownBack; //スコアの位置



static int g_nPatternanim;
static int g_nCounterAnim;
static int nCut;
static Player *pPlayer = GetPlayer();
bool bFlag;
//===================
//初期化処理
//===================
void Initcountdown(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	

	bFlag = true;
	g_nPatternanim = 0;
	g_nCounterAnim = 0;
	int s_nTimer = 0;
	nCut = 0;

	s_poscountdown3 = D3DXVECTOR3(650.0f, 250.0f, 0.0f);			//位置設定		ドット「小数点」
	//s_poscountdownBack = D3DXVECTOR3(230.0f, 130.0f, 0.0f);			//位置設定

	//デバイスの取得
	pDevice = GetDevice();


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/countdown.png",
		&s_pTexturecountdown3);
	//頂点バッファ
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//ここ頂点フォーマット
		&s_pVtxBuffcountdown3,
		NULL);


	////テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/image/back 1.png",
	//	&s_pTexturecountdownBack);
	////頂点バッファ
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_2D,
	//	D3DPOOL_MANAGED,//ここ頂点フォーマット
	//	&s_pVtxBuffcountdownBack,
	//	NULL);

	//-------------------------------------------------------------------------------------------
	VERTEX_2D*pVtx; //頂点へのポインタ

		//頂点バッファをロックし頂点情報へのポインタを取得
		s_pVtxBuffcountdown3->Lock(0, 0, (void**)&pVtx, 0);
		//頂点座標
		SetNorotpos(pVtx,
			s_poscountdown3.x - 100,
			s_poscountdown3.x + 100,
			s_poscountdown3.y - 100,
			s_poscountdown3.y + 100);
		//RHWの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//テクスチャの座標設定
		Settex(pVtx, 0.0f, 1.0f/3.0f, 0.0f, 1.0f);
		pVtx += 4; //頂点ポイントを四つ進む
	//頂点バッファをアンロック
		s_pVtxBuffcountdown3->Unlock();

		////頂点バッファをロックし頂点情報へのポインタを取得
		//s_pVtxBuffcountdownBack->Lock(0, 0, (void**)&pVtx, 0);
		////頂点座標
		//SetNorotpos(pVtx,
		//	s_poscountdownBack.x - 80,
		//	s_poscountdownBack.x + 80,
		//	s_poscountdownBack.y - 80,
		//	s_poscountdownBack.y + 80);
		////RHWの設定
		//pVtx[0].rhw = 1.0f;
		//pVtx[1].rhw = 1.0f;
		//pVtx[2].rhw = 1.0f;
		//pVtx[3].rhw = 1.0f;
		////頂点カラーの設定
		//pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		//pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		//pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		//pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		////テクスチャの座標設定
		//Settex(pVtx, 0.0f, 1.0f / 3.0f, 0.0f, 1.0f);
		//pVtx += 4; //頂点ポイントを四つ進む
		//		   //頂点バッファをアンロック
		//s_pVtxBuffcountdownBack->Unlock();
			

}

//===================
//破棄処理
//===================
void Uninitcountdown(void)
{
	
	////テクスチャの破棄
	//if (s_pTexturecountdownBack != NULL)
	//{
	//	s_pTexturecountdownBack->Release();
	//	s_pTexturecountdownBack = NULL;
	//}	
	//	//頂点バッファの破棄
	//	if (s_pVtxBuffcountdownBack != NULL)
	//	{
	//		s_pVtxBuffcountdownBack->Release();
	//		s_pVtxBuffcountdownBack = NULL;
	//	}

	//テクスチャの破棄
	if (s_pTexturecountdown3 != NULL)
	{
		s_pTexturecountdown3->Release();
		s_pTexturecountdown3 = NULL;
	}
	//頂点バッファの破棄
	if (s_pVtxBuffcountdown3 != NULL)
	{
		s_pVtxBuffcountdown3->Release();
		s_pVtxBuffcountdown3 = NULL;
	}
}
//===================
//更新処理
//===================
void Updatecountdown(void)
{

	//if (nCut == 10)
	//{
	//	AddTimer(-20);
	//	nCut = 0;
	//}

	nCut++;

	VERTEX_2D*pVtx; //頂点へのポインタ
	//頂点バッファをロックし頂点情報へのポインタを取得
	s_pVtxBuffcountdown3->Lock(0, 0, (void**)&pVtx, 0);

	g_nCounterAnim++;								//アニメーション
	if (g_nCounterAnim >= 60)
	{
		g_nCounterAnim = 0;
		g_nPatternanim++;

		if (g_nPatternanim >= 25)
		{
			g_nPatternanim = 0;
		}

		pVtx[0].tex = D3DXVECTOR2((1.0f / 3.0f) * g_nPatternanim, 0.0f);									//--------------------------敵が増えたとき増やす
		pVtx[1].tex = D3DXVECTOR2((1.0f / 3.0f) * (g_nPatternanim + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / 3.0f) * g_nPatternanim, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / 3.0f) * (g_nPatternanim + 1), 1.0f);

	}
	//頂点バッファをアンロック
	s_pVtxBuffcountdown3->Unlock();
	
	for (int nCnt = 0; nCnt < MAX; nCnt++)
	{
		if (nCut > 1)
		{
			TrueSet();
		}
	}
	if (nCut > 179)		//カウント
	{
		if (bFlag)
		{
			//ディスクの設定
			SetDisc(E_PLAYER_1);
		}

		bFlag = false;
		FalseSet();
	}	
	
}

//===================
//描画処理
//===================
void Drawcountdown(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ
	 
	if (bFlag)
	{
		pDevice = GetDevice();		//代入

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuffcountdown3, 0, sizeof(VERTEX_2D));
		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexturecountdown3);
		//
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			2);


		////頂点バッファをデータストリームに設定
		//pDevice->SetStreamSource(0, s_pVtxBuffcountdownBack, 0, sizeof(VERTEX_2D));
		////頂点フォーマットの設定
		//pDevice->SetFVF(FVF_VERTEX_2D);
		////テクスチャの設定
		//pDevice->SetTexture(0, s_pTexturecountdownBack);
		////
		//pDevice->DrawPrimitive(
		//	D3DPT_TRIANGLESTRIP,
		//	0,
		//	2);
	}
}
void SetNorotpos(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	pVtx[0].pos = D3DXVECTOR3(left, top, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(right, top, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(left, down, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(right, down, 0.0f);
}
void Settex(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);
}