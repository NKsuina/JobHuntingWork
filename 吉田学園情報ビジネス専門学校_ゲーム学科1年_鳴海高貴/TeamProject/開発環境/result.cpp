//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "result.h"
#include "particle.h"
#include "score1.h"
#include "score2.h"
#include "fade.h"
#include "sound.h"
#include "disc.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TEXTURE	(14)		//使用するテクスチャの枚数

//=============================================================================
// スタティック変数
//=============================================================================
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_TEXTURE] = {NULL};		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;				//頂点バッファへのポインタ
static bool bUseResult;											//リザルトを使用してるか
static bool bEnterSwitch;										//画面遷移するためのエンタースイッチ
static int NonEnter;											//何も触らなかったら勝手に画面遷移を開始する用の変数
static RESULT s_Result[MAX_TEXTURE];							//構造体
static bool bResult;

//=============================================================================
// 初期化処理
//=============================================================================
void InitResult(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点情報へのポインタ
	VERTEX_2D * pVtx;

	//変数の初期化
	bUseResult = false;
	bEnterSwitch = false;
	NonEnter = 0;
	bResult = false;

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/勝者.png",
		&s_pTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/敗者.png",
		&s_pTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/win.png",
		&s_pTexture[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/lose.png",
		&s_pTexture[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ZeroPoint.png",
		&s_pTexture[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/OnePoint.png",
		&s_pTexture[5]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/TwoPoint.png",
		&s_pTexture[6]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SETCOUNT.png",
		&s_pTexture[7]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ThreePoint.png",
		&s_pTexture[8]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/勝者1P.png",
		&s_pTexture[9]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/勝者2P.png",
		&s_pTexture[10]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/敗者1P.png",
		&s_pTexture[11]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/敗者2P.png",
		&s_pTexture[12]);

	//構造体の初期化
	s_Result[0].pos = D3DXVECTOR3(-200.0f, 350.0f, 0.0f);
	s_Result[0].move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	s_Result[1].pos = D3DXVECTOR3(1480.0f, 350.0f, 0.0f);
	s_Result[1].move = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
	s_Result[2].pos = D3DXVECTOR3(-200.0f, 150.0f, 0.0f);
	s_Result[2].move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	s_Result[3].pos = D3DXVECTOR3(1480.0f, 150.0f, 0.0f);
	s_Result[3].move = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ情報の初期化
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

		//頂点データを4つ進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		//テクスチャの破棄
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}

		//頂点バッファの破棄
		if (s_pVtxBuff != NULL)
		{
			s_pVtxBuff->Release();
			s_pVtxBuff = NULL;
		}
	}
	//サウンド停止
	StopSound();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	//1Pのスコアの取得
	int OneScore = GetScore();

	//2Pのスコアの取得
	int TwoScore = GetScore2();

	//リザルトのセット処理
	SetResult(OneScore, TwoScore);
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[i]);

		//ポリゴンの描画処理
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//ポリゴンの形
			4 * i,						//頂点の開始場所
			2);							//プリミティブの数
	}
}

//=============================================================================
// リザルトセット処理
//=============================================================================
void SetResult(int nSetCount1, int nSetCount2)
{
	
	if (!bResult)
	{
		//サウンド停止
		StopSound();
		PlaySound(SOUND_LABEL_BGM002);
		PlaySound(SOUND_LABEL_SE_KANSEI2);
		bResult = true;
	}

	BreakDisc();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点情報へのポインタ
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (bUseResult == false)
	{//使用していなければ実行
		if (nSetCount1 > nSetCount2)
		{//1Pが勝利した場合
			//頂点座標の設定
			pVtx[36].pos = D3DXVECTOR3(s_Result[0].pos.x - 150.0f, s_Result[0].pos.y - 150.0f, 0.0f);
			pVtx[37].pos = D3DXVECTOR3(s_Result[0].pos.x + 150.0f, s_Result[0].pos.y - 150.0f, 0.0f);
			pVtx[38].pos = D3DXVECTOR3(s_Result[0].pos.x - 150.0f, s_Result[0].pos.y + 150.0f, 0.0f);
			pVtx[39].pos = D3DXVECTOR3(s_Result[0].pos.x + 150.0f, s_Result[0].pos.y + 150.0f, 0.0f);

			pVtx[48].pos = D3DXVECTOR3(s_Result[1].pos.x - 150.0f, s_Result[1].pos.y - 150.0f, 0.0f);
			pVtx[49].pos = D3DXVECTOR3(s_Result[1].pos.x + 150.0f, s_Result[1].pos.y - 150.0f, 0.0f);
			pVtx[50].pos = D3DXVECTOR3(s_Result[1].pos.x - 150.0f, s_Result[1].pos.y + 150.0f, 0.0f);
			pVtx[51].pos = D3DXVECTOR3(s_Result[1].pos.x + 150.0f, s_Result[1].pos.y + 150.0f, 0.0f);

			pVtx[8].pos = D3DXVECTOR3(s_Result[2].pos.x - 150.0f, s_Result[2].pos.y - 50.0f, 0.0f);
			pVtx[9].pos = D3DXVECTOR3(s_Result[2].pos.x + 150.0f, s_Result[2].pos.y - 50.0f, 0.0f);
			pVtx[10].pos = D3DXVECTOR3(s_Result[2].pos.x - 150.0f, s_Result[2].pos.y + 50.0f, 0.0f);
			pVtx[11].pos = D3DXVECTOR3(s_Result[2].pos.x + 150.0f, s_Result[2].pos.y + 50.0f, 0.0f);

			pVtx[12].pos = D3DXVECTOR3(s_Result[3].pos.x - 150.0f, s_Result[3].pos.y - 50.0f, 0.0f);
			pVtx[13].pos = D3DXVECTOR3(s_Result[3].pos.x + 150.0f, s_Result[3].pos.y - 50.0f, 0.0f);
			pVtx[14].pos = D3DXVECTOR3(s_Result[3].pos.x - 150.0f, s_Result[3].pos.y + 50.0f, 0.0f);
			pVtx[15].pos = D3DXVECTOR3(s_Result[3].pos.x + 150.0f, s_Result[3].pos.y + 50.0f, 0.0f);

			//リザルトの位置更新
			s_Result[0].pos += s_Result[0].move;
			s_Result[1].pos += s_Result[1].move;
			s_Result[2].pos += s_Result[2].move;
			s_Result[3].pos += s_Result[3].move;

			//移動量を0にする
			if (s_Result[0].pos.x >= 375.0f)
			{
				s_Result[0].move.x = 0;
				s_Result[1].move.x = 0;
				s_Result[2].move.x = 0;
				s_Result[3].move.x = 0;
				bEnterSwitch = true;
				ForcedFade();
			}

			//パーティクルのセット処理
			SetParticle(D3DXVECTOR3(370.0f, 750.0f, 0.0f), 1);
		}

		else if (nSetCount1 < nSetCount2)
		{//2Pが勝利した場合
			//頂点座標の設定
			pVtx[40].pos = D3DXVECTOR3(s_Result[1].pos.x - 150.0f, s_Result[1].pos.y - 150.0f, 0.0f);
			pVtx[41].pos = D3DXVECTOR3(s_Result[1].pos.x + 150.0f, s_Result[1].pos.y - 150.0f, 0.0f);
			pVtx[42].pos = D3DXVECTOR3(s_Result[1].pos.x - 150.0f, s_Result[1].pos.y + 150.0f, 0.0f);
			pVtx[43].pos = D3DXVECTOR3(s_Result[1].pos.x + 150.0f, s_Result[1].pos.y + 150.0f, 0.0f);

			pVtx[44].pos = D3DXVECTOR3(s_Result[0].pos.x - 150.0f, s_Result[0].pos.y - 150.0f, 0.0f);
			pVtx[45].pos = D3DXVECTOR3(s_Result[0].pos.x + 150.0f, s_Result[0].pos.y - 150.0f, 0.0f);
			pVtx[46].pos = D3DXVECTOR3(s_Result[0].pos.x - 150.0f, s_Result[0].pos.y + 150.0f, 0.0f);
			pVtx[47].pos = D3DXVECTOR3(s_Result[0].pos.x + 150.0f, s_Result[0].pos.y + 150.0f, 0.0f);

			pVtx[8].pos = D3DXVECTOR3(s_Result[3].pos.x - 150.0f, s_Result[3].pos.y - 50.0f, 0.0f);
			pVtx[9].pos = D3DXVECTOR3(s_Result[3].pos.x + 150.0f, s_Result[3].pos.y - 50.0f, 0.0f);
			pVtx[10].pos = D3DXVECTOR3(s_Result[3].pos.x - 150.0f, s_Result[3].pos.y + 50.0f, 0.0f);
			pVtx[11].pos = D3DXVECTOR3(s_Result[3].pos.x + 150.0f, s_Result[3].pos.y + 50.0f, 0.0f);

			pVtx[12].pos = D3DXVECTOR3(s_Result[2].pos.x - 150.0f, s_Result[2].pos.y - 50.0f, 0.0f);
			pVtx[13].pos = D3DXVECTOR3(s_Result[2].pos.x + 150.0f, s_Result[2].pos.y - 50.0f, 0.0f);
			pVtx[14].pos = D3DXVECTOR3(s_Result[2].pos.x - 150.0f, s_Result[2].pos.y + 50.0f, 0.0f);
			pVtx[15].pos = D3DXVECTOR3(s_Result[2].pos.x + 150.0f, s_Result[2].pos.y + 50.0f, 0.0f);

			//リザルトの位置更新
			s_Result[0].pos += s_Result[0].move;
			s_Result[1].pos += s_Result[1].move;
			s_Result[2].pos += s_Result[2].move;
			s_Result[3].pos += s_Result[3].move;

			//移動量を0にする
			if (s_Result[0].pos.x >= 375.0f)
			{
				s_Result[0].move.x = 0;
				s_Result[1].move.x = 0;
				s_Result[2].move.x = 0;
				s_Result[3].move.x = 0;
				bEnterSwitch = true;
				ForcedFade();
			}

			//パーティクルのセット処理
			SetParticle(D3DXVECTOR3(900.0f, 750.0f, 0.0f), 1);
		}
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	//ENTER押して画面遷移
	if (bEnterSwitch)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A))
		{
			SetFade(MODE_TITLE);
			PlaySound(SOUND_LABEL_SE_CLICK);
		}
	}

	//セットカウントのセット処理
	//SetCount(nSetCount1, D3DXVECTOR3(275.0f, 600.0f, 0.0f));
	//SetCount(nSetCount2, D3DXVECTOR3(975.0f, 600.0f, 0.0f));
}

//=============================================================================
// セットカウントセット処理
//=============================================================================
void SetCount(int nSetCount, D3DXVECTOR3 pos)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点情報へのポインタ
	VERTEX_2D * pVtx;

	//変数宣言
	D3DXVECTOR3 nCountPos = pos;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (nSetCount == 0)
	{//セットカウントが0だった場合実行
		//頂点座標の設定
		pVtx[16].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[17].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[18].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y + 70.0f, 0.0f);
		pVtx[19].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y + 70.0f, 0.0f);
	}

	else if (nSetCount == 1)
	{//セットカウントが1だった場合実行
		//頂点座標の設定
		pVtx[20].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[21].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[22].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y + 70.0f, 0.0f);
		pVtx[23].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y + 70.0f, 0.0f);
	}

	else if (nSetCount == 2)
	{//セットカウントが2だった場合実行
	  //頂点座標の設定
		pVtx[24].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[25].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[26].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y + 70.0f, 0.0f);
		pVtx[27].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y + 70.0f, 0.0f);
	}

	else if (nSetCount == 3)
	{//セットカウントが3だった場合実行
	  //頂点座標の設定
		pVtx[32].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[33].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[34].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y + 70.0f, 0.0f);
		pVtx[35].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y + 70.0f, 0.0f);
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=============================================================================
// 強制画面遷移処理
//=============================================================================
void ForcedFade(void)
{
	//Enterを推さない限りインクリメントする
	NonEnter++;

	//強制画面遷移
	if (NonEnter >= 640)
	{
		SetFade(MODE_TITLE);
	}
}