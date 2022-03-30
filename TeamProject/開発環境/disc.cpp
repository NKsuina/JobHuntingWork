//-------------------------------------
//
//ディスクの処理[disc.cpp]
//Author:梶田大夢
//
//-------------------------------------
#include <time.h>
#include "camera.h"
#include "Input.h"
#include "disc.h"
#include "goal.h"
#include "score1.h"
#include "score2.h"
#include "wallmodel.h"
#include "2dpolyron.h"
#include "sound.h"

//グローバル変数
LPD3DXMESH g_pMeshDisc = NULL;					//メッシュへのポインタ
LPD3DXBUFFER g_pBuffMatDisc = NULL;				//頂点バッファへのポインタ
DWORD g_nNumMatDisc= 0;
static Disc g_Disc[MAX_DISC];
static PlayerHaveDisc player;
static int nTime;

static int Count;

static int s_nPtsNum;
static bool s_bHaving;

//ディスクの初期化処理
void InitDisc(void)
{
	s_bHaving = false;
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\ディスク.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatDisc,
		NULL,
		&g_nNumMatDisc,
		&g_pMeshDisc);

	Count = 0;
	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマットのサイズ
	BYTE * pVtxBuff;		//頂点バッファへのポインタ

	//頂点数の取得
	nNumVtx = g_pMeshDisc->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshDisc->GetFVF());

	//頂点バッファをロック
	g_pMeshDisc->LockVertexBuffer(D3DLOCK_READONLY, (void **)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		for (int nCntDisc = 0; nCntDisc < MAX_DISC; nCntDisc++)
		{
			//x
			if (vtx.x < g_Disc[nCntDisc].vtxMinDisc.x)
			{
				g_Disc[nCntDisc].vtxMinDisc.x = vtx.x;
			}

			if (vtx.x > g_Disc[nCntDisc].vtxMaxDisc.x)
			{
				g_Disc[nCntDisc].vtxMaxDisc.x = vtx.x;
			}

			//y
			if (vtx.y < g_Disc[nCntDisc].vtxMinDisc.y)
			{
				g_Disc[nCntDisc].vtxMinDisc.y = vtx.y;
			}

			if (vtx.y > g_Disc[nCntDisc].vtxMaxDisc.y)
			{
				g_Disc[nCntDisc].vtxMaxDisc.y = vtx.y;
			}

			//z
			if (vtx.z < g_Disc[nCntDisc].vtxMinDisc.z)
			{
				g_Disc[nCntDisc].vtxMinDisc.z = vtx.z;
			}

			if (vtx.z > g_Disc[nCntDisc].vtxMaxDisc.z)
			{
				g_Disc[nCntDisc].vtxMaxDisc.z = vtx.z;
			}

			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}
	}

	//頂点バッファのアンロック
	g_pMeshDisc->UnlockVertexBuffer();

	for (int nCntDisc = 0; nCntDisc < MAX_DISC; nCntDisc++)
	{
		g_Disc[nCntDisc].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置の初期化
		g_Disc[nCntDisc].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動の初期化
		g_Disc[nCntDisc].size = D3DXVECTOR3(g_Disc[nCntDisc].vtxMaxDisc.x - g_Disc[nCntDisc].vtxMinDisc.x, 0.0f, g_Disc[nCntDisc].vtxMaxDisc.z - g_Disc[nCntDisc].vtxMinDisc.z);
		g_Disc[nCntDisc].rotDisc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期化
		g_Disc[nCntDisc].rotDestDisc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Disc[nCntDisc].vtxMinDisc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Disc[nCntDisc].vtxMaxDisc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Disc[nCntDisc].bUse = false;									//使われているかどうか
		g_Disc[nCntDisc].bGoal = false;									//ゴールしたかどうか
		g_Disc[nCntDisc].nGoal = 0;										//どっちにゴールしたか
	}
}

//ディスクの終了処理
void UninitDisc(void)
{
	for (int nCntDisc = 0; nCntDisc < MAX_DISC; nCntDisc++)
	{
		//メッシュの破棄
		if (g_pMeshDisc != NULL)
		{
			g_pMeshDisc->Release();
			g_pMeshDisc = NULL;
		}
	}
	//マテリアルの破棄
	if (g_pBuffMatDisc != NULL)
	{
		g_pBuffMatDisc->Release();
		g_pBuffMatDisc = NULL;
	}
}

//ディスクの更新処理
void UpdateDisc(void)
{
	Camera * pCamera = GetCamera();

	for (int nCntDisc = 0; nCntDisc < MAX_DISC; nCntDisc++)
	{

		//角度の正規化
		if (g_Disc[nCntDisc].rotDestDisc.y - g_Disc[nCntDisc].rotDisc.y > D3DX_PI)
		{
			g_Disc[nCntDisc].rotDestDisc.y = g_Disc[nCntDisc].rotDestDisc.y - D3DX_PI * 2;
		}
		else if (g_Disc[nCntDisc].rotDestDisc.y - g_Disc[nCntDisc].rotDisc.y < -D3DX_PI)
		{
			g_Disc[nCntDisc].rotDestDisc.y = g_Disc[nCntDisc].rotDestDisc.y + D3DX_PI * 2;
		}

		g_Disc[nCntDisc].rotDisc += (g_Disc[nCntDisc].rotDestDisc - g_Disc[nCntDisc].rotDisc) * 0.08f;

		//角度の正規化
		if (g_Disc[nCntDisc].rotDisc.y > D3DX_PI)
		{
			g_Disc[nCntDisc].rotDisc.y = g_Disc[nCntDisc].rotDisc.y - D3DX_PI * 2;
		}
		else if (g_Disc[nCntDisc].rotDisc.y < -D3DX_PI)
		{
			g_Disc[nCntDisc].rotDisc.y = g_Disc[nCntDisc].rotDisc.y + D3DX_PI * 2;
		}

		//前回の位置を保存
		g_Disc[nCntDisc].posOld = g_Disc[nCntDisc].pos;

		//位置を更新
		g_Disc[nCntDisc].pos.x += g_Disc[nCntDisc].move.x;
		g_Disc[nCntDisc].pos.y += g_Disc[nCntDisc].move.y;
		g_Disc[nCntDisc].pos.z += g_Disc[nCntDisc].move.z;

		//ゴールのあたり判定
		if ((g_Disc[nCntDisc].pos.x >= 600.0f && g_Disc[nCntDisc].pos.z >= 180.0f) || (g_Disc[nCntDisc].pos.x >= 600.0f &&g_Disc[nCntDisc].pos.z <= -80.0f) && !s_bHaving)
		{
			g_Disc[nCntDisc].bUse = false;
			g_Disc[nCntDisc].bGoal = true;
			g_Disc[nCntDisc].nGoal = 2;

			if (nTime == 1)
			{
				Polygon_2D Polygon;

				Polygon.fX = 200.0f;
				Polygon.fY = 100.0f;
				Polygon.nPat = 1;
				Polygon.pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f,0.0f);

				s_nPtsNum = Set2DPolygon(Polygon);

				PlaySound(SOUND_LABEL_SE_KANSEI);
				AddScore(3);
			}
		}
		else if (g_Disc[nCntDisc].pos.x >= 600.0f && !s_bHaving)
		{
			g_Disc[nCntDisc].bUse = false;
			g_Disc[nCntDisc].bGoal = true;
			g_Disc[nCntDisc].nGoal = 2;

			if (nTime == 1)
			{
				Polygon_2D Polygon;

				Polygon.fX = 200.0f;
				Polygon.fY = 100.0f;
				Polygon.nPat = 3;
				Polygon.pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);

				s_nPtsNum = Set2DPolygon(Polygon);
				PlaySound(SOUND_LABEL_SE_KANSEI);
				AddScore(5);

			}
		}

		if ((g_Disc[nCntDisc].pos.x <= -600.0f && g_Disc[nCntDisc].pos.z >= 180.0f) || (g_Disc[nCntDisc].pos.x <= -600.0f &&g_Disc[nCntDisc].pos.z <= -80.0f) && !s_bHaving)
		{
			g_Disc[nCntDisc].bUse = false;
			g_Disc[nCntDisc].bGoal = true;
			g_Disc[nCntDisc].nGoal = 1;

			if (nTime == 1)
			{
				Polygon_2D Polygon;

				Polygon.fX = 200.0f;
				Polygon.fY = 100.0f;
				Polygon.nPat = 0;
				Polygon.pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);

				s_nPtsNum = Set2DPolygon(Polygon);
				PlaySound(SOUND_LABEL_SE_KANSEI);
				AddScore2(3);

			}
		}
		else if (g_Disc[nCntDisc].pos.x <= -600.0f && !s_bHaving)
		{
			g_Disc[nCntDisc].bUse = false;
			g_Disc[nCntDisc].bGoal = true;
			g_Disc[nCntDisc].nGoal = 1;

			if (nTime == 1)
			{
			
				Polygon_2D Polygon;

				Polygon.fX = 200.0f;
				Polygon.fY = 100.0f;
				Polygon.nPat = 2;
				Polygon.pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
				PlaySound(SOUND_LABEL_SE_KANSEI);
				s_nPtsNum = Set2DPolygon(Polygon);
				AddScore2(5);

			}
		}

		//2Pがゴールしたら1P側にディスク復活
		if (g_Disc[nCntDisc].bGoal == true && g_Disc[nCntDisc].nGoal == 1)
		{
			if (nTime >= 60)
			{
				g_Disc[nCntDisc].pos = D3DXVECTOR3(-200.0f, 0.0f, 0.0f);
				g_Disc[nCntDisc].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_Disc[nCntDisc].bUse = true;
				g_Disc[nCntDisc].bGoal = false;
				g_Disc[nCntDisc].nGoal = 0;
				nTime = 0;

				Break2DPolygon(s_nPtsNum);
			}
			nTime++;
		}

		//1Pがゴールしたら2P側にディスク復活
		if (g_Disc[nCntDisc].bGoal == true && g_Disc[nCntDisc].nGoal == 2)
		{
			if (nTime >= 60)
			{
				g_Disc[nCntDisc].pos = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
				g_Disc[nCntDisc].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_Disc[nCntDisc].bUse = true;
				g_Disc[nCntDisc].bGoal = false;
				g_Disc[nCntDisc].nGoal = 0;
				nTime = 0;

				Break2DPolygon(s_nPtsNum);
			}
			nTime++;
		}

		//壁の当たり判定
		if (g_Disc[nCntDisc].pos.z >= 320.0f && g_Disc[nCntDisc].bUse)
		{
			g_Disc[nCntDisc].pos.z = 320.0f;
			g_Disc[nCntDisc].move.z *= -1;
			SetVibration(0);
			PlaySound(SOUND_LABEL_SE_FENCE);
		}
		else if (g_Disc[nCntDisc].pos.z <= -320.0f && g_Disc[nCntDisc].bUse)
		{
			g_Disc[nCntDisc].pos.z = -320.0f;
			g_Disc[nCntDisc].move.z *= -1;
			SetVibration(1);
			PlaySound(SOUND_LABEL_SE_FENCE);
		}
	}
}

//ディスクの描画処理
void DrawDisc(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9 matDef;				//現在のマテリアル保存用
	D3DXMATERIAL * pMat;				//マテリアルデータへのポインタ

	for (int nCntDisc = 0; nCntDisc < MAX_DISC; nCntDisc++)
	{
		if (g_Disc[nCntDisc].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Disc[nCntDisc].mtxWorldDisc);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Disc[nCntDisc].rotDisc.y, g_Disc[nCntDisc].rotDisc.x, g_Disc[nCntDisc].rotDisc.z);
			D3DXMatrixMultiply(&g_Disc[nCntDisc].mtxWorldDisc, &g_Disc[nCntDisc].mtxWorldDisc, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Disc[nCntDisc].pos.x, g_Disc[nCntDisc].pos.y, g_Disc[nCntDisc].pos.z);
			D3DXMatrixMultiply(&g_Disc[nCntDisc].mtxWorldDisc, &g_Disc[nCntDisc].mtxWorldDisc, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Disc[nCntDisc].mtxWorldDisc);

			//現在のマテリアルを保持
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL *)g_pBuffMatDisc->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatDisc; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//ディスクパーツの描画
				g_pMeshDisc->DrawSubset(nCntMat);
			}
			//保持していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//ディスクの設定処理
void SetDisc(PlayerHaveDisc player)
{
	for (int nCnt = 0; nCnt < MAX_DISC; nCnt++)
	{
		switch (player)
		{
		case E_PLAYER_1:
			if (g_Disc[nCnt].bUse == false)
			{
				g_Disc[nCnt].pos = D3DXVECTOR3(-300.0f, 0.0f, 0.0f);
				g_Disc[nCnt].bUse = true;
				g_Disc[nCnt].bGoal = false;
				g_Disc[nCnt].nGoal = 0;
				g_Disc[nCnt].nDiscNumber = 0;
			}

			break;

		case E_PLAYER_2:
			if (g_Disc[nCnt].bUse == false)
			{
				g_Disc[nCnt].pos = D3DXVECTOR3(300.0f, 0.0f, 0.0f);
				g_Disc[nCnt].bUse = true;
				g_Disc[nCnt].bGoal = false;
				g_Disc[nCnt].nGoal = 0;
				g_Disc[nCnt].nDiscNumber = 1;
			}
			break;
		}
	}
}

//ディスクの位置を変える処理
void SetDiscPos(int nDiscNumber, D3DXVECTOR3 pos)
{
	g_Disc[nDiscNumber].pos = pos;
	s_bHaving = true;
}

Disc * GetDisc(void)
{
	return &g_Disc[0];	//一番最初からやってあげますよ敵の情報をあげますの意味
}

//------------------------
//角度変更
void TherowingDisc(int nRot, int nSpeed, int nNum)
{
	PlaySound(SOUND_LABEL_SE_SHOT);
	g_Disc[nNum].move.x = sinf(D3DXToRadian(nRot))* nSpeed;		//Z　奥行き
	g_Disc[nNum].move.z = cosf(D3DXToRadian(nRot))* nSpeed;		//X　
	g_Disc[nNum].pos.y = 0.0f;
	s_bHaving = false;
}

void BreakDisc(void)
{
	for (int nCnt = 0; nCnt < MAX_DISC; nCnt++)
	{
		if (g_Disc[nCnt].bUse)
		{
			g_Disc[nCnt].bUse = false;
			g_Disc[nCnt].bGoal = false;
		}
	}
}