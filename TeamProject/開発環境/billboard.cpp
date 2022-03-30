//=============================================================================
//
// ビルボード処理 [billboard.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "billboard.h"
#include "input.h"

#define NUM_BILLBOARD (256)			//ビルボードの最大数

//グローバル変数宣言
static Billboard s_aBillboard[NUM_BILLBOARD];
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffBillboard = NULL;		//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 s_pTextureBillboard[BILLBOARD_TEXTURE_MAX];		//テクスチャへのポインタ

//==============================================
//ビルボードの初期化処理
//==============================================
void InitBillboard(void)
{
	//グローバル変数
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	s_pTextureBillboard[NULLTEXTURE] = NULL;

	//テクスチャの読み込み1
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ouen_man.png",
		&s_pTextureBillboard[TEXTURE_AudienceMan]);

	//テクスチャの読み込み2
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ElectricBulletin2.jpg",
		&s_pTextureBillboard[TEXTURE_ElectricBulletin]);

	//テクスチャの読み込み3
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ouen_woman.png",
		&s_pTextureBillboard[TEXTURE_AudienceWo]);

	//テクスチャの読み込み4
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/sarari-man_ouen.png",
		&s_pTextureBillboard[TEXTURE_AudienceSara]);

	//テクスチャの読み込み5
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/alien_ouen.png",
		&s_pTextureBillboard[TEXTURE_AudienceAlien]);

	//テクスチャの読み込み6
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/audience.png",
		&s_pTextureBillboard[TEXTURE_AudienceBack]);

	//テクスチャの読み込み7
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bro.png",
		&s_pTextureBillboard[TEXTURE_AudienceBro]);

	//テクスチャの読み込み8
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/mukimuki_Dog.png",
		&s_pTextureBillboard[TEXTURE_AudienceDog]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBillboard,
		NULL);

	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロック
	s_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < NUM_BILLBOARD; nCntBillboard++)
	{

		s_aBillboard[nCntBillboard].bUse = false;

		//頂点座標の設定（ワールド座標ではなくローカル座標を指定する）
		pVtx[0].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x - 0.0f, s_aBillboard[nCntBillboard].pos.y - 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x + 0.0f, s_aBillboard[nCntBillboard].pos.y - 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x - 0.0f, s_aBillboard[nCntBillboard].pos.y + 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x + 0.0f, s_aBillboard[nCntBillboard].pos.y + 0.0f, 0.0f);

		//各頂点の法線の設定（※ベクトルの大きさは1にする必要がある）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		pVtx += 4;
	}
	//頂点バッファのアンロック
	s_pVtxBuffBillboard->Unlock();

	//ビルボードに影を付ける予定だったもの↓
	//SetShadow(D3DXVECTOR3(g_posBillboard.x, 0.1f, g_posBillboard.z), g_rotBillboard);
}

//==============================================
//ビルボードの終了処理
//==============================================
void UninitBillboard(void)
{
	//バッファ破棄
	if (s_pVtxBuffBillboard != NULL)
	{
		s_pVtxBuffBillboard->Release();
		s_pVtxBuffBillboard = NULL;
	}

	for (int nCnt = 0; nCnt < BILLBOARD_TEXTURE_MAX; nCnt++)
	{
		//テクスチャ破棄
		if (s_pTextureBillboard[nCnt] != NULL)
		{
			s_pTextureBillboard[nCnt]->Release();
			s_pTextureBillboard[nCnt] = NULL;
		}
	}
}

//==============================================
//ビルボードの更新処理
//==============================================
void UpdataBillboard(void)
{
}

//==============================================
//ビルボードの描画処理
//==============================================
void DrawBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX  mtxTrans;							//計算用マトリックス

	for (int nCntBillboard = 0; nCntBillboard < NUM_BILLBOARD; nCntBillboard++)
	{
		if (s_aBillboard[nCntBillboard].bUse == true)
		{
			//テクスチャ設定
			pDevice->SetTexture(0, s_pTextureBillboard[s_aBillboard[nCntBillboard].nType]);

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&s_aBillboard[nCntBillboard].mtxWorld);

			D3DXMATRIX  mtxView;

			//ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			s_aBillboard[nCntBillboard].mtxWorld._11 = mtxView._11;
			s_aBillboard[nCntBillboard].mtxWorld._12 = mtxView._21;
			s_aBillboard[nCntBillboard].mtxWorld._13 = mtxView._31;
			s_aBillboard[nCntBillboard].mtxWorld._21 = mtxView._12;
			s_aBillboard[nCntBillboard].mtxWorld._22 = mtxView._22;
			s_aBillboard[nCntBillboard].mtxWorld._23 = mtxView._32;
			s_aBillboard[nCntBillboard].mtxWorld._31 = mtxView._13;
			s_aBillboard[nCntBillboard].mtxWorld._32 = mtxView._23;
			s_aBillboard[nCntBillboard].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, s_aBillboard[nCntBillboard].pos.x, s_aBillboard[nCntBillboard].pos.y, s_aBillboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&s_aBillboard[nCntBillboard].mtxWorld, &s_aBillboard[nCntBillboard].mtxWorld, &mtxTrans);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, s_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &s_aBillboard[nCntBillboard].mtxWorld);

			//αテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);

			//ライトを有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//αテストを無効化
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
	//テクスチャの設定
	pDevice->SetTexture(0, NULL);
}

//===============================
//ビルボードの設定処理
//===============================
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR2 size, E_BILLBOARD nType, char *FileName)
{
	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロック
	s_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < NUM_BILLBOARD; nCntBillboard++)
	{
		if (s_aBillboard[nCntBillboard].bUse == false)
		{
			s_aBillboard[nCntBillboard].pos = pos;
			s_aBillboard[nCntBillboard].size = size;
			s_aBillboard[nCntBillboard].nType = nType;
			s_aBillboard[nCntBillboard].bUse = true;

			//頂点座標の設定（ワールド座標ではなくローカル座標を指定する）
			pVtx[0].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x - s_aBillboard[nCntBillboard].size.x, s_aBillboard[nCntBillboard].pos.y + s_aBillboard[nCntBillboard].size.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x + s_aBillboard[nCntBillboard].size.x, s_aBillboard[nCntBillboard].pos.y + s_aBillboard[nCntBillboard].size.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x - s_aBillboard[nCntBillboard].size.x, s_aBillboard[nCntBillboard].pos.y - s_aBillboard[nCntBillboard].size.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(s_aBillboard[nCntBillboard].pos.x + s_aBillboard[nCntBillboard].size.x, s_aBillboard[nCntBillboard].pos.y - s_aBillboard[nCntBillboard].size.y, 0.0f);

			break;
		}
		pVtx += 4;
	}
	//頂点バッファのアンロック
	s_pVtxBuffBillboard->Unlock();
}
//===================================
//ビルボードのファイル読み込み処理
//===================================
void InputBillboard(void)
{
	VERTEX_3D* pVtx = NULL;
	FILE *pFile;							//ファイルのポインタを読み込む
	static char a[128];						//メモ帳の文字列を保存する
	static char savefile[128][256];			//ファイルの場所を保存する
	static int	sCnt = 0;					//書いてるだけ回したいから使う変数

	//ファイルを開く
	pFile = fopen("data/Text/loadfile.txt", "r");

	if (pFile != NULL)
	{
		while (fscanf(pFile, "%s", &a[0]) != EOF)	//EOF = EndOfFile
		{
			if (strcmp(&a[0], "TEXTURE_NAME") == 0)
			{
				fscanf(pFile, "%s", &a[0]);					//「=」を入れる所
				fscanf(pFile, "%s", &savefile[sCnt][0]);	//テクスチャのソース場所を入れる所
				sCnt++;
			}
			if (strcmp(&a[0], "BILLBOARD_SET") == 0)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f);
				E_BILLBOARD nType;

				while (1)		//END_MODELSETが来るまで回す
				{
					fscanf(pFile, "%s", &a[0]);

					if ((strcmp(&a[0], "POS")) == 0)
					{//ビルボードの場所を指定
						fscanf(pFile, "%s", &a[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}
					if ((strcmp(&a[0], "SIZE")) == 0)
					{//ビルボードのサイズを指定(2DなのでX,Yのみ指定)
						fscanf(pFile, "%s", &a[0]);
						fscanf(pFile, "%f", &size.x);
						fscanf(pFile, "%f", &size.y);
					}
					if ((strcmp(&a[0], "TYPE")) == 0)
					{
						int i;
						fscanf(pFile, "%s", &a[0]);
						fscanf(pFile, "%d", &i);
						nType = (E_BILLBOARD)i;
					}
					if ((strcmp(&a[0], "END_BILLBOARDSET")) == 0)
					{
						SetBillboard(pos, size, nType, &savefile[sCnt][0]);
						break;
					}
				}
			}
		}
	}
}