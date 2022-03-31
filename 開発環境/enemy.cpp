//=============================================================================
//
// 敵の処理 [enemy.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "sound.h"
#include "score.h"
#include "block.h"

//マクロ定義
#define NUM_ENEMY		(4)		//敵の種類

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
Enemy g_aEnemy[MAX_ENEMY];							//敵の情報
D3DXVECTOR3 g_posOldEnemy;						
D3DXVECTOR3 g_posEnemy;								//位置
D3DXVECTOR3 g_moveEnemy;							//移動量
D3DXVECTOR3 g_rotEnemy;								//向き

//==============================
//敵の初期化処理
//==============================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy000.png",
		&g_apTextureEnemy[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy001.png",
		&g_apTextureEnemy[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy002.png",
		&g_apTextureEnemy[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy004.png",
		&g_apTextureEnemy[3]);

	//敵の情報の初期化
	for (int nCntEnemy = 0; nCntEnemy <MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期化
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			
		g_aEnemy[nCntEnemy].bIsJumping = false;							
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nTime = 0;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posEnemy.x - ENEMY_WIDTH/2, g_posEnemy.y - ENEMY_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posEnemy.x + ENEMY_WIDTH/2, g_posEnemy.y - ENEMY_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posEnemy.x - ENEMY_WIDTH/2, g_posEnemy.y , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posEnemy.x + ENEMY_WIDTH/2, g_posEnemy.y , 0.0f);

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

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();

}

//============================
//敵の終了処理
//============================
void UninitEnemy(void)
{
	int nCnt;

	for (nCnt = 0; nCnt < NUM_ENEMY; nCnt++)
	{
		//テクスチャの破棄
		if (g_apTextureEnemy[nCnt] != NULL)
		{
			g_apTextureEnemy[nCnt]->Release();
			g_apTextureEnemy[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//====================================
//敵の更新処理
//====================================
void UpdateEnemy(void)
{
	int nCntEnemy;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2, g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2, g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2, g_aEnemy[nCntEnemy].pos.y, 0.0f);

			//前回の位置を保存
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
			g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;

			g_aEnemy[nCntEnemy].bIsJumping = !CollisionBlock(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, ENEMY_WIDTH, ENEMY_HEIGHT);

		}

		if (g_aEnemy[nCntEnemy].nType == 1)
		{
			g_aEnemy[nCntEnemy].move.y = 1;
		}

		//右の壁にぶつかった
		if (g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH)
		{
			g_aEnemy[nCntEnemy].pos.x = SCREEN_WIDTH;
			g_aEnemy[nCntEnemy].move.x = 0.0f;
		}
		//左の壁にぶつかった
		if (g_aEnemy[nCntEnemy].pos.x <= 0)
		{
			g_aEnemy[nCntEnemy].pos.x = 0;
			g_aEnemy[nCntEnemy].move.x = 0.0f;
		}
		pVtx += 4;  //頂点データのポインタを４つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}
//=================================
//敵の描画処理
//=================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている
		 //テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntEnemy * 4,
				2);
		}
	}
}
//==============================
//敵の設定処理
//==============================
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//敵が使用されていない場合
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nType = nType;

			//頂点座標の設定処理
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH/2, g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH/2, g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH/2, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH/2, g_aEnemy[nCntEnemy].pos.y, 0.0f);

			g_aEnemy[nCntEnemy].bUse = true;		//使用している状態にする

			break;		//←for文を抜け出す
		}
		pVtx += 4;			//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	Player *pPlayer;

	//プレイヤーの当たり判定処理
	pPlayer = GetPlayer();

	bool bIsLanding = false;		//着地しているかどうか

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//上の当たり判定処理
		if (g_aEnemy[nCntEnemy].bUse)
		{//敵が使用されている
			if (pPos->x + fWidth / 2.0f > g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2.0f
				&& pPos->x - fWidth / 2.0f < g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2.0f
				&& pPosOld->y <= g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2.0f
				&& pPos->y > g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2.0f)
			{
				pPos->y = g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2.0f;
				if (g_aEnemy[nCntEnemy].nType == 1)
				{
					PlaySound(SOUND_LABEL_SE_STUNP);
					g_aEnemy[nCntEnemy].bUse = false;
					AddScore(200);
				}
				if (g_aEnemy[nCntEnemy].nType == 0)
				{
					PlaySound(SOUND_LABEL_SE_HIT);
					pPlayer->nLife = 0;
				}
			}

			//下の当たり判定処理
			if (pPos->x + fWidth / 2.0f > g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2.0f
				&& pPos->x - fWidth / 2.0f < g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2.0f
				&& pPosOld->y - fHeight >= g_aEnemy[nCntEnemy].pos.y + ENEMY_HEIGHT
				&& pPos->y - fHeight < g_aEnemy[nCntEnemy].pos.y + ENEMY_HEIGHT)

			{
				PlaySound(SOUND_LABEL_SE_HIT);
				pPlayer->nLife = 0;
			}

			//左の当たり判定処理
			if (pPos->x + fWidth / 2.0f > g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2.0f
				&& pPosOld->x - fWidth / 2.0f <= g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2.0f
				&& pPos->y - fHeight < g_aEnemy[nCntEnemy].pos.y + ENEMY_HEIGHT / 2.0f
				&& pPos->y > g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2.0f)
			{
				PlaySound(SOUND_LABEL_SE_HIT);
				pPlayer->nLife = 0;
			}

			//右の当たり判定処理
			if (pPos->x - fWidth / 2.0f < g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2.0f
				&& pPosOld->x + fWidth / 2.0f >= g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2.0f
				&& pPos->y - fHeight < g_aEnemy[nCntEnemy].pos.y + ENEMY_HEIGHT / 2.0f
				&& pPos->y > g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2.0f)
			{
				PlaySound(SOUND_LABEL_SE_HIT);
				pPlayer->nLife = 0;
			}
		}
	}
	return bIsLanding;
}

Enemy *GetEnemy(void)
{
	Enemy *pEnemy = g_aEnemy;

	//返り値
	return pEnemy;
}