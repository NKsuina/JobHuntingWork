//=============================================================================
//
// モデル処理 [model.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "Effect.h"
#include "main.h"
#include "input.h"
#include "model.h"
#include "score.h"
#include "camera.h"
#include "player.h"
#include "loadfile.h"
#include "sound.h"
#include "shadow.h"
#include <stdio.h>
#include <math.h>

//マクロ変数
#define NUM_MODEL (8)				//モデルの種類
#define MAX_MODEL (200)				//モデルの最大数
#define VTX	(10000)

//グローバル変数宣言
typedef struct
{
	char aFileName[128];
}ModelSub;

Model g_model[MAX_MODEL];	
ModelSub g_modelsub[NUM_MODEL];
D3DXMATRIX g_mtxWorldModel;						//ワールドマトリックス

//==============================================
//モデルの初期化処理
//==============================================
void InitModel(void)
{
	int nCntModel;		//モデルの種類用の変数

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	memset(&g_modelsub[0], 0, sizeof(g_modelsub)); 
	strcpy(&g_modelsub[0].aFileName[0], "data/MODEL/house.x");
	strcpy(&g_modelsub[1].aFileName[0], "data/MODEL/biru.x");
	strcpy(&g_modelsub[2].aFileName[0], "data/MODEL/dentyu.x");
	strcpy(&g_modelsub[3].aFileName[0], "data/MODEL/colorcone.x");
	strcpy(&g_modelsub[4].aFileName[0], "data/MODEL/kouzi_mokuzai.x");
	strcpy(&g_modelsub[5].aFileName[0], "data/MODEL/buranko.x");
	strcpy(&g_modelsub[6].aFileName[0], "data/MODEL/tree.x");

	for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_model[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_model[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_model[nCntModel].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_model[nCntModel].nType = 0;
		g_model[nCntModel].bUse = false;							//使ってるかどうか
	}
}

//====================================
//モデルの終了処理
//====================================
void UninitModel(void)
{
	int nCntNullModel;		//終了処理に使う変数
	
	for (nCntNullModel = 0; nCntNullModel < NUM_MODEL; nCntNullModel++)
	{
		//メッシュの破棄
		if (g_model[nCntNullModel].pBuffMatModel != NULL)
		{
			g_model[nCntNullModel].pMeshModel->Release();
			g_model[nCntNullModel].pMeshModel = NULL;
		}

		//マテリアルの破棄
		if (g_model[nCntNullModel].pBuffMatModel != NULL)
		{
			g_model[nCntNullModel].pBuffMatModel->Release();
			g_model[nCntNullModel].pBuffMatModel = NULL;
		}
	}
}

//================================
//モデルの更新処理
//================================
void UpdateModel(void)
{
	
}

//================================
//モデルの描画処理
//================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;						//計算用のマトリックス
	D3DMATERIAL9 matDef;								//現在のマテリアル保存用
	D3DXMATERIAL *pMat;									//マテリアルデータへのポインタ

	for (int nCntModelMtx = 0; nCntModelMtx < MAX_MODEL; nCntModelMtx++)
	{
		if (g_model[nCntModelMtx].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldModel);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_model[nCntModelMtx].rot.y, g_model[nCntModelMtx].rot.x, g_model[nCntModelMtx].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_model[nCntModelMtx].pos.x, g_model[nCntModelMtx].pos.y, g_model[nCntModelMtx].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

			//現在のマテリアルを保持
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_model[nCntModelMtx].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_model->nNumMatModel; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//モデルパーツの描画
				g_model[nCntModelMtx].pMeshModel->DrawSubset(nCntMat);
			}
			//保持していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}

	}
}

//====================================
//モデルの配置処理
//====================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, char *FileName)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BYTE *pVtxBuff;								//頂点バッファへのポインタ
	DWORD sizeFVF;								//頂点フォーマットのサイズ

	int nNumVtx;								//頂点数

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_model[nCntModel].bUse == false)
		{
			g_model[nCntModel].pos = pos;
			g_model[nCntModel].rot = rot;
			g_model[nCntModel].nType = nType;
			g_model[nCntModel].bUse = true;

			//xファイルの読み込み
			D3DXLoadMeshFromX(&FileName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_model[nCntModel].pBuffMatModel,
				NULL,
				&g_model[nCntModel].nNumMatModel,
				&g_model[nCntModel].pMeshModel);

			g_model[nCntModel].vtxMaxModel = D3DXVECTOR3(-100000, -100000, -100000);
			g_model[nCntModel].vtxMinModel = D3DXVECTOR3(100000, 100000, 100000);

			//頂点数の取得
			nNumVtx = g_model[nCntModel].pMeshModel->GetNumVertices();

			//頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(g_model[nCntModel].pMeshModel->GetFVF());

			//頂点バッファのロック
			g_model[nCntModel].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//頂点座標を比較してモデルの最小値、最大値を取得
				//x座標
				if (vtx.x < g_model[nCntModel].vtxMinModel.x)
				{
					g_model[nCntModel].vtxMinModel.x = floorf(vtx.x);
				}
				if (vtx.x > g_model[nCntModel].vtxMaxModel.x)
				{
					g_model[nCntModel].vtxMaxModel.x = floorf(vtx.x);
				}
				//y座標
				if (vtx.y < g_model[nCntModel].vtxMinModel.y)
				{
					g_model[nCntModel].vtxMinModel.y = floorf(vtx.y);
				}
				if (vtx.y > g_model[nCntModel].vtxMaxModel.y)
				{
					g_model[nCntModel].vtxMaxModel.y = floorf(vtx.y);
				}
				//z座標
				if (vtx.z < g_model[nCntModel].vtxMinModel.z)
				{
					g_model[nCntModel].vtxMinModel.z = floorf(vtx.z);
				}
				if (vtx.z > g_model[nCntModel].vtxMaxModel.z)
				{
					g_model[nCntModel].vtxMaxModel.z = floorf(vtx.z);
				}
				pVtxBuff += sizeFVF;

				g_model[nCntModel].size.x = g_model[nCntModel].vtxMaxModel.x - g_model[nCntModel].vtxMinModel.x;
				g_model[nCntModel].size.y = g_model[nCntModel].vtxMaxModel.y - g_model[nCntModel].vtxMinModel.y;
				g_model[nCntModel].size.z = g_model[nCntModel].vtxMaxModel.z - g_model[nCntModel].vtxMinModel.z;
			}
			//頂点バッファのロック
			g_model[nCntModel].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			//頂点バッファのアンロック
			g_model[nCntModel].pMeshModel->UnlockVertexBuffer();
			break;
		}
	}
}
//====================================
//モデルの当たり判定処理
//====================================
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *size)
{
	bool bIsLanding = false;		//着地しているかどうか

	//Player *pPlayer;

	//プレイヤーの当たり判定処理
	//pPlayer = GetPlayer();
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_model[nCntModel].bUse == true)
		{
			//モデルの左側当たり判定
			if (pPos->z - size->z / 2 < g_model[nCntModel].pos.z + g_model[nCntModel].vtxMaxModel.z
				&& pPos->z + size->z / 2 > g_model[nCntModel].pos.z + g_model[nCntModel].vtxMinModel.z
				&& pPosOld->x + size->x / 2 <= g_model[nCntModel].pos.x + g_model[nCntModel].vtxMinModel.x
				&& pPos->x + size->x / 2 > g_model[nCntModel].pos.x + g_model[nCntModel].vtxMinModel.x)
			{
				pPos->x = g_model[nCntModel].pos.x - size->x / 2.0f + g_model[nCntModel].vtxMinModel.x;
				SetEffect(g_model[nCntModel].pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 25.0f), 40, 50);

				if (g_model[nCntModel].nType == 0)		//家を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(300000);
				}
				if (g_model[nCntModel].nType == 1)		//ビルを壊した時
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(1000000);
				}
				if (g_model[nCntModel].nType == 2)		//電柱を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 3)		//三角コーンを壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(30000);
				}
				if (g_model[nCntModel].nType == 4)		//工事現場の木材を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 5)		//ブランコを壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(100000);
				}
				if (g_model[nCntModel].nType == 6)		//木を破壊した時
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(20000);
				}
				g_model[nCntModel].bUse = false;
			}

			//モデルの右側当たり判定
			else if(pPos->z - size->z / 2 < g_model[nCntModel].pos.z + g_model[nCntModel].vtxMaxModel.z
				&& pPos->z + size->z / 2 > g_model[nCntModel].pos.z + g_model[nCntModel].vtxMinModel.z
				&& pPosOld->x - size->x / 2 >= g_model[nCntModel].pos.x - g_model[nCntModel].vtxMinModel.x
				&& pPos->x - size->x / 2 < g_model[nCntModel].pos.x - g_model[nCntModel].vtxMinModel.x)
			{
				pPos->x = g_model[nCntModel].pos.x + size->x / 2.0f - g_model[nCntModel].vtxMinModel.x;
				SetEffect(g_model[nCntModel].pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 25.0f), 40, 50);

				if (g_model[nCntModel].nType == 0)		//家を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(300000);
				}
				if (g_model[nCntModel].nType == 1)		//ビルを壊した時
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(1000000);
				}
				if (g_model[nCntModel].nType == 2)		//電柱を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 3)		//三角コーンを壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(30000);
				}
				if (g_model[nCntModel].nType == 4)		//工事現場の木材を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 5)		//ブランコを壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(100000);
				}
				if (g_model[nCntModel].nType == 6)		//木を破壊した時
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(20000);
				}

				g_model[nCntModel].bUse = false;
			}

			//モデルの奥側当たり判定
			else if (pPos->x - size->x / 2 < g_model[nCntModel].pos.x + g_model[nCntModel].vtxMaxModel.x
				&& pPos->x + size->x / 2 > g_model[nCntModel].pos.x + g_model[nCntModel].vtxMinModel.x
				&& pPosOld->z - size->z / 2 >= g_model[nCntModel].pos.z - g_model[nCntModel].vtxMinModel.z
				&& pPos->z - size->z / 2 < g_model[nCntModel].pos.z - g_model[nCntModel].vtxMinModel.z)
			{
				pPos->z = g_model[nCntModel].pos.z + size->z / 2.0f - g_model[nCntModel].vtxMinModel.z;
				SetEffect(g_model[nCntModel].pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 25.0f), 40, 50);

				if (g_model[nCntModel].nType == 0)		//家を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(300000);
				}
				if (g_model[nCntModel].nType == 1)		//ビルを壊した時
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(1000000);
				}
				if (g_model[nCntModel].nType == 2)		//電柱を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 3)		//三角コーンを壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(30000);
				}
				if (g_model[nCntModel].nType == 4)		//工事現場の木材を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 5)		//ブランコを壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(100000);
				}
				if (g_model[nCntModel].nType == 6)		//木を破壊した時
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(20000);
				}
				g_model[nCntModel].bUse = false;
			}

			//モデルの手前側当たり判定
			else if (pPos->x - size->x / 2 < g_model[nCntModel].pos.x + g_model[nCntModel].vtxMaxModel.x
				&& pPos->x + size->x / 2 > g_model[nCntModel].pos.x + g_model[nCntModel].vtxMinModel.x
				&& pPosOld->z + size->z / 2 <= g_model[nCntModel].pos.z + g_model[nCntModel].vtxMinModel.z
				&& pPos->z + size->z / 2 > g_model[nCntModel].pos.z + g_model[nCntModel].vtxMinModel.z)
			{
				pPos->z = g_model[nCntModel].pos.z - size->z / 2.0f + g_model[nCntModel].vtxMinModel.z;
				SetEffect(g_model[nCntModel].pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 25.0f), 40, 50);

if (g_model[nCntModel].nType == 0)		//家を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(300000);
				}
				if (g_model[nCntModel].nType == 1)		//ビルを壊した時
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(1000000);
				}
				if (g_model[nCntModel].nType == 2)		//電柱を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 3)		//三角コーンを壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(30000);
				}
				if (g_model[nCntModel].nType == 4)		//工事現場の木材を壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(50000);
				}
				if (g_model[nCntModel].nType == 5)		//ブランコを壊したとき
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(100000);
				}
				if (g_model[nCntModel].nType == 6)		//木を破壊した時
				{
					PlaySound(SOUND_LABEL_SE_BREAK);
					AddScore
					(20000);
				}
				g_model[nCntModel].bUse = false;
			}
		}
	}
	return bIsLanding;
}