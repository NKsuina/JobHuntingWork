//=============================================================================
//
// モデル処理 [model.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------

#include "player.h"
#include "camera.h"
#include "input.h"
#include "calculation.h"
#include "polygon.h"
#include "disc.h"
#include "collision.h"

#include <assert.h>
#include "sound.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------

#define ANGLE_DAMPING_COEFFICIENT	(0.07f)					//角度の減衰係数
#define SCALE_DOWN					(1.0f)					//スケールサイズの変更
#define ANCHOR_POINT_MAX			(120)					//カメラの移動点
#define GRAVITY_MAX					(3.0f)					//重力
#define ACCELERATION_MAX			(1.0f)					//加速の限界
#define PLAYER_SPEED				(7.0f)
#define PLAYER_MAX					(2)


//-----------------------------------------------------------------------------
//構造体
//-----------------------------------------------------------------------------

//モーション再生中に必要な情報構造体
typedef struct
{
	int nFrameCnt;					//現在のフレーム数
	int nNowRebirthKeySet;			//再生中のキーセット番号
	int nNowRebirthMotion;			//再生中のモーション番号
}PlayerMotionData;

//モデルパターン構造体
typedef struct
{
	LPD3DXMESH pMesh = NULL;			//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;		//マテリアル情報へのポインタ
	DWORD nNumMat = 0;					//マテリアル情報の数
	bool bUse;							//使用しているかどうか
}PlayerPartsPattern;

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------

D3DXMATRIX g_PlayerPartsmtxWorld[PARENT_MAX];				//ワールドマトリックス(プレイヤーパーツ)
PlayerPartsPattern g_PartsPattern[PARENT_MAX];				//モデルパターン構造体
Player g_Player[PLAYER_MAX];											//プレイヤーの構造体
char g_sPlayerModelFilename[PARENT_MAX][256];				//ファイル名
PlayerMotionData g_MotionPlayer[PLAYER_MAX];							//モーションに必要なデータ構造体
float g_fDeceleration;										//プレイヤー移動量の減速割合（ゲーム中用）
D3DXVECTOR3 g_PlayerPos[PLAYER_MAX];									//ゲーム中の横移動などをやるためのPOS
int g_nLineNmn = 0;

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------

void PlayerMove(int nPlayerNum);
void MotionsetPlayer(int nPlayerNum);
void PlayerAction(int nPlayerNum);
void PlayerDraw(int nPlayerNum);
void ChangeMotion(int nPlayerNum,int nMotion, int nKey);
bool MoveKeyPlayer(int nPlayerNum, JOYKEY_CROSS Key);

//*****************************************************************************
//プレイヤーの初期化
//*****************************************************************************

void InitPlayer(void)
{
	//プレイヤーに必要な全ワールドマトリックスの初期化
	ZeroMemory(&g_PlayerPartsmtxWorld, sizeof(g_PlayerPartsmtxWorld));
	//プレイヤーの構造体の初期化
	ZeroMemory(&g_PartsPattern, sizeof(g_PartsPattern));
	ZeroMemory(&g_Player, sizeof(g_Player));
	//モーションに必要なデータ構造体の初期化
	ZeroMemory(&g_MotionPlayer, sizeof(g_MotionPlayer));
	//POS初期化
	ZeroMemory(&g_PlayerPos, sizeof(g_PlayerPos));

	g_PlayerPos[0] = D3DXVECTOR3(-400.0f, 0.0f, 0.0f);
	g_PlayerPos[1] = D3DXVECTOR3(400.0f, 0.0f, 0.0f);

	g_Player[0].nNumDisc = -1;
	g_Player[1].nNumDisc = -1;
	//プレイヤー移動量の減速割合の初期化
	g_fDeceleration = 0.0f;
}

//*****************************************************************************
//プレイヤーの終了処理
//*****************************************************************************

void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < PARENT_MAX; nCnt++)
	{
		//メッシュの破棄
		if (g_PartsPattern[nCnt].pMesh != NULL)
		{
			g_PartsPattern[nCnt].pMesh->Release();
			g_PartsPattern[nCnt].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_PartsPattern[nCnt].pBuffMat != NULL)
		{
			g_PartsPattern[nCnt].pBuffMat->Release();
			g_PartsPattern[nCnt].pBuffMat = NULL;
		}

	}
}

//*****************************************************************************
//プレイヤーの更新処理
//*****************************************************************************

void UpdatePlayer(void)
{
	////カメラの情報の取得（ポインタ）
	//Camera *pCamera = GetCamera();

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//プレイヤーの移動
		PlayerMove(nCnt);
		//プレイヤーの行動
		PlayerAction(nCnt);
		//モーション
		if (g_MotionPlayer[nCnt].nNowRebirthMotion == g_Player[nCnt].PlayerState)
		{
			MotionsetPlayer(nCnt);
		}
	}
}

//*****************************************************************************
//プレイヤーの描画処理
//*****************************************************************************

void DrawPlayer(void)
{
	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		PlayerDraw(nCnt);
	}
}

//-----------------------------------------------------------------------------
//プレイヤーの描画処理（中身）
//-----------------------------------------------------------------------------
void PlayerDraw(int nPlayerNum)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	D3DXMATRIX mtxRootFirst;			//大元の親の親ワールドマトリックス
	D3DXMATRIX mtxRoot;					//大元の親のワールドマトリックス
	D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

										//ワールドマトリックスの初期化（大元の親の親）
	D3DXMatrixIdentity(&mtxRootFirst);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player[nPlayerNum].rot.y, g_Player[nPlayerNum].rot.x, g_Player[nPlayerNum].rot.z);
	D3DXMatrixMultiply(&mtxRootFirst, &mtxRootFirst, &mtxRot);

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, g_PlayerPos[nPlayerNum].x, g_PlayerPos[nPlayerNum].y, g_PlayerPos[nPlayerNum].z);
	D3DXMatrixMultiply(&mtxRootFirst, &mtxRootFirst, &mtxTrans);

	//ワールドマトリックスの初期化（大元の親）
	D3DXMatrixIdentity(&mtxRoot);

	D3DXMatrixScaling(&mtxRot,
		SCALE_DOWN, SCALE_DOWN, SCALE_DOWN);

	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxRot);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);

	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxRot);

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, g_Player[nPlayerNum].pos.x, g_Player[nPlayerNum].pos.y, g_Player[nPlayerNum].pos.z);

	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxTrans);

	//モデルのマトリックス　＊　親のワールドマトリックス
	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxRootFirst);

	for (int nCnt = 0; nCnt < PARENT_MAX; nCnt++)
	{
		if (g_Player[nPlayerNum].Parts[nCnt].bUse)
		{
			D3DMATERIAL9 matDef;			//現在のマテリアル保存
			D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

											//ワールドマトリックスの初期化（子）
			D3DXMatrixIdentity(&g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex]);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_Player[nPlayerNum].Parts[nCnt].rot.y, g_Player[nPlayerNum].Parts[nCnt].rot.x, g_Player[nPlayerNum].Parts[nCnt].rot.z);

			D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &mtxRot);

			//位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_Player[nPlayerNum].Parts[nCnt].pos.x, g_Player[nPlayerNum].Parts[nCnt].pos.y, g_Player[nPlayerNum].Parts[nCnt].pos.z);
			D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &mtxTrans);

			

			if (g_Player[nPlayerNum].Parts[nCnt].nParent < 0)
			{
				//モデルのマトリックス　＊　親のワールドマトリックス
				D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &mtxRoot);
			}
			else
			{
				//モデルのマトリックス　＊　親のワールドマトリックス
				D3DXMatrixMultiply(&g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex], &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nParent]);
			}

			if (nCnt == 4)
			{
				//必要情報のポインタを取得
				D3DXMATRIX *pMtxWorld = &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex];
  				g_Player[nPlayerNum].posHand = D3DXVECTOR3(pMtxWorld->_41, pMtxWorld->_42, pMtxWorld->_43);
			}

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_PlayerPartsmtxWorld[g_Player[nPlayerNum].Parts[nCnt].nIndex]);

			//現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_PartsPattern[g_Player[nPlayerNum].Parts[nCnt].nIndex].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_PartsPattern[g_Player[nPlayerNum].Parts[nCnt].nIndex].nNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, NULL);

				//モデルパーツの描画
				g_PartsPattern[g_Player[nPlayerNum].Parts[nCnt].nIndex].pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-----------------------------------------------------------------------------
//プレイヤーの移動
//-----------------------------------------------------------------------------
void PlayerMove(int nPlayerNum)
{
	//過去の位置の保存
	g_Player[nPlayerNum].posOld = g_PlayerPos[nPlayerNum] + g_Player[nPlayerNum].pos;

	//カメラの情報の取得（ポインタ）
	Camera *pCamera = GetCamera();

	//視点移動
	g_Player[nPlayerNum].PlayerState = PLAYER_RUN;
	g_MotionPlayer[nPlayerNum].nNowRebirthMotion = PLAYER_RUN;
	if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_UP))
	{//上キーが押された
		if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_LEFT))
		{
			g_Player[nPlayerNum].rotDest.y = D3DX_PI * 0.75f + pCamera->rot.y;
			g_Player[nPlayerNum].move.x -= sinf(pCamera->rot.y + D3DX_PI * 0.75f) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z -= cosf(pCamera->rot.y + D3DX_PI * 0.75f) * g_Player[nPlayerNum].fMove;
		}
		else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_RIGHT))
		{
			g_Player[nPlayerNum].rotDest.y = D3DX_PI * -0.75f + pCamera->rot.y;
			g_Player[nPlayerNum].move.x -= sinf(pCamera->rot.y + D3DX_PI * -0.75f) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z -= cosf(pCamera->rot.y + D3DX_PI * -0.75f) * g_Player[nPlayerNum].fMove;
		}
		else
		{
			g_Player[nPlayerNum].rotDest.y = D3DX_PI + pCamera->rot.y;
			g_Player[nPlayerNum].move.x += sinf(pCamera->rot.y) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z += cosf(pCamera->rot.y) * g_Player[nPlayerNum].fMove;
		}
	}
	else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_DOWN))
	{//下キーが押された
		if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_LEFT))
		{
			g_Player[nPlayerNum].rotDest.y = D3DX_PI * 0.25f + pCamera->rot.y;
			g_Player[nPlayerNum].move.x -= sinf(pCamera->rot.y + D3DX_PI * 0.25f) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z -= cosf(pCamera->rot.y + D3DX_PI * 0.25f) * g_Player[nPlayerNum].fMove;
		}
		else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_RIGHT))
		{
			g_Player[nPlayerNum].rotDest.y = D3DX_PI * -0.25f + pCamera->rot.y;
			g_Player[nPlayerNum].move.x -= sinf(pCamera->rot.y + D3DX_PI * -0.25f) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z -= cosf(pCamera->rot.y + D3DX_PI * -0.25f) * g_Player[nPlayerNum].fMove;
		}
		else
		{
			g_Player[nPlayerNum].rotDest.y = pCamera->rot.y;
			g_Player[nPlayerNum].move.x += sinf(pCamera->rot.y + D3DX_PI) * g_Player[nPlayerNum].fMove;
			g_Player[nPlayerNum].move.z += cosf(pCamera->rot.y + D3DX_PI) * g_Player[nPlayerNum].fMove;
		}
	}
	else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_LEFT))
	{//左キーが押された
		g_Player[nPlayerNum].rotDest.y = D3DX_PI * 0.5f + pCamera->rot.y;
		g_Player[nPlayerNum].move.x += sinf(pCamera->rot.y + D3DX_PI * -0.5f) * g_Player[nPlayerNum].fMove;
		g_Player[nPlayerNum].move.z += cosf(pCamera->rot.y + D3DX_PI * -0.5f) * g_Player[nPlayerNum].fMove;
	}
	else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_RIGHT))
	{//右キーが押された
		g_Player[nPlayerNum].rotDest.y = D3DX_PI * -0.5f + pCamera->rot.y;
		g_Player[nPlayerNum].move.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * g_Player[nPlayerNum].fMove;
		g_Player[nPlayerNum].move.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * g_Player[nPlayerNum].fMove;
	}
	else
	{
		ZeroMemory(&g_Player[nPlayerNum].move, sizeof(g_Player[nPlayerNum].move));
		
			g_Player[nPlayerNum].PlayerState = PLAYER_NEUTRAL;
			g_MotionPlayer[nPlayerNum].nNowRebirthMotion = PLAYER_NEUTRAL;
			//モーションの初期化とキーのセット変更
			//ChangeMotion(g_MotionPlayer.nNowRebirthMotion, 1);
		
	}

	//rotが規定数より超えたときの補正込み
	g_Player[nPlayerNum].rot.y += Normalization(g_Player[nPlayerNum].rotDest.y/*目的の角度*/ - g_Player[nPlayerNum].rot.y/*現在の角度*/)
		* ANGLE_DAMPING_COEFFICIENT;

	//rotが規定数より超えたときの補正
	g_Player[nPlayerNum].rot.y = Normalization(g_Player[nPlayerNum].rot.y);

	//プレイヤーPOSの更新
	g_PlayerPos[nPlayerNum] += g_Player[nPlayerNum].move * 1.0f;
	g_Player[nPlayerNum].move.x += (0.0f - g_Player[nPlayerNum].move.x) * 0.2f;
	g_Player[nPlayerNum].move.z += (0.0f - g_Player[nPlayerNum].move.z) * 0.2f;

	//プレイヤーの胴体のPOS更新(ジャンプ処理)
	g_Player[nPlayerNum].pos.y -= (70.0f - g_Player[nPlayerNum].pos.y) * 0.1f;
	

	//加速処理
	if (g_fDeceleration < 0.9f)
	{
		g_fDeceleration += (ACCELERATION_MAX - g_fDeceleration) * 0.01f;
	}

	//床SCREEN_STAGE_WIDTH
	if (g_Player[nPlayerNum].pos.y <= 0.0f)
	{
		g_Player[nPlayerNum].pos.y = 0.0f;
	}

	if (g_PlayerPos[nPlayerNum].x > -100.0f
		&& nPlayerNum == 0)
	{
		g_PlayerPos[nPlayerNum].x = -100.0f;
	}
	else if (g_PlayerPos[nPlayerNum].x  < -SCREEN_STAGE_WIDTH + 100.0f
		&& nPlayerNum == 0)
	{
		g_PlayerPos[nPlayerNum].x = -SCREEN_STAGE_WIDTH + 100.0f;
	}

	if (g_PlayerPos[nPlayerNum].x > SCREEN_STAGE_WIDTH - 100.0f
		&& nPlayerNum == 1)
	{
		g_PlayerPos[nPlayerNum].x = SCREEN_STAGE_WIDTH - 100.0f;
	}
	else if (g_PlayerPos[nPlayerNum].x  < 100.0f
		&& nPlayerNum == 1)
	{
		g_PlayerPos[nPlayerNum].x = 100.0f;
	}

	if (g_PlayerPos[nPlayerNum].z > SCREEN_STAGE_DEPTH - 20.0f)
	{
		g_PlayerPos[nPlayerNum].z = SCREEN_STAGE_DEPTH - 20.0f;
	}
	else if (g_PlayerPos[nPlayerNum].z < -SCREEN_STAGE_DEPTH + 20.0f)
	{
		g_PlayerPos[nPlayerNum].z = -SCREEN_STAGE_DEPTH + 20.0f;
	}

}

//-----------------------------------------------------------------------------
//プレイヤー行動
//-----------------------------------------------------------------------------
void PlayerAction(int nPlayerNum)
{
	//ディスクの情報の取得
	Disc *pDisc = GetDisc();
	//持つ処理
	for (int nCnt = 0; nCnt < MAX_DISC; nCnt++)
	{
		if (pDisc[nCnt].bUse
			&& CollisionCircle(pDisc[nCnt].pos, 20.0f, g_PlayerPos[nPlayerNum], 20.0f)
			&& !g_Player[nPlayerNum].bDiscHave
			&& MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_MAX))
		{
			PlaySound(SOUND_LABEL_SE_HIT);
			g_Player[nPlayerNum].nNumDisc = nCnt;
			g_Player[nPlayerNum].bDiscHave = true;
			break;
		}
	}

	if (g_Player[nPlayerNum].bDiscHave)
	{// 
		SetDiscPos(g_Player[nPlayerNum].nNumDisc, g_Player[nPlayerNum].posHand);
	}

	if (!MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_MAX)
		&& g_Player[nPlayerNum].bDiscHave)
	{//右キーが押された
		g_Player[nPlayerNum].bDiscHave = false;
		int nRot = 1;

		if (nPlayerNum == 1)
		{
			nRot = -1;
		}

		if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_UP))
		{
			TherowingDisc(45 * nRot, 20, g_Player[nPlayerNum].nNumDisc);
		}
		else if (MoveKeyPlayer(nPlayerNum, JOYKEY_CROSS_DOWN))
		{
			TherowingDisc(135 * nRot, 20, g_Player[nPlayerNum].nNumDisc);
		}
		else
		{
			TherowingDisc(90 * nRot, 20, g_Player[nPlayerNum].nNumDisc);
		}
	}
}

//-----------------------------------------------------------------------------
//プレイヤー情報の取得（ポインタ）
//-----------------------------------------------------------------------------
Player *GetPlayer(void)
{
	return &g_Player[0];
}

//-----------------------------------------------------------------------------
//プレイヤー情報の取得（ポインタではない）
//-----------------------------------------------------------------------------
Player GetPlayerData(void)
{
	return g_Player[0];
}

//-----------------------------------------------------------------------------
//モデルタイプ読み込み処理
//-----------------------------------------------------------------------------

HRESULT LoadPlayerPattern(char *sXFilePath)
{

	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	for (int nCntPatn = 0; nCntPatn < PARENT_MAX; nCntPatn++)
	{
		if (!g_PartsPattern[nCntPatn].bUse)
		{

			//Xファイルの読み込み
			D3DXLoadMeshFromX(sXFilePath,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_PartsPattern[nCntPatn].pBuffMat,
				NULL,
				&g_PartsPattern[nCntPatn].nNumMat,
				&g_PartsPattern[nCntPatn].pMesh);

			g_PartsPattern[nCntPatn].bUse = true;

			return S_OK;
		}
	}

	assert(false);
//	MessageBox(hWnd, "読み込み可能モデル数を超えています", "警告！", MB_ICONWARNING);
	return E_FAIL;
}

//-----------------------------------------------------------------------------
//プレイヤーのセット処理
//-----------------------------------------------------------------------------

void SetPlayer(Player Player)
{
	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		if (!g_Player[nCnt].bUes)
		{
			g_Player[nCnt] = Player;

			g_Player[nCnt].rotDest.y = D3DX_PI;
			g_Player[nCnt].rot.y = D3DX_PI;
			//g_PlayerPos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Player[nCnt].PlayerState = PLAYER_NEUTRAL;
			g_MotionPlayer[nCnt].nNowRebirthMotion = PLAYER_NEUTRAL;
			g_Player[nCnt].bUes = true;
			//モーションの初期化とキーのセット変更
			ChangeMotion(nCnt,0, 1);

			int nNumVix;		//頂点数
			DWORD sizeFVF;		//頂点フォーマットのサイズ
			BYTE *pVtxBuff;		//頂点バッファへのポインタ

								//モデルのサイズの比較用初期値
			g_Player[nCnt].vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
			g_Player[nCnt].vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

			//頂点数の取得
			nNumVix = g_PartsPattern[0].pMesh->GetNumVertices();

			//頂点フォーマット取得
			sizeFVF = D3DXGetFVFVertexSize(g_PartsPattern[0].pMesh->GetFVF());

			//頂点ロック
			g_PartsPattern[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			//すべての頂点POSの取得
			for (int nCntVtx = 0; nCntVtx < nNumVix; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				if (g_Player[nCnt].vtxMax.x < vtx.x)
				{//X
					g_Player[nCnt].vtxMax.x = vtx.x;
				}
				if (g_Player[nCnt].vtxMin.x > vtx.x)
				{
					g_Player[nCnt].vtxMin.x = vtx.x;
				}

				if (g_Player[nCnt].vtxMax.y < vtx.y)
				{//Y
					g_Player[nCnt].vtxMax.y = vtx.y;
				}
				if (g_Player[nCnt].vtxMin.y > vtx.y)
				{
					g_Player[nCnt].vtxMin.y = vtx.y;
				}

				if (g_Player[nCnt].vtxMax.z < vtx.z)
				{//Z
					g_Player[nCnt].vtxMax.z = vtx.z;
				}
				if (g_Player[nCnt].vtxMin.z > vtx.z)
				{
					g_Player[nCnt].vtxMin.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			g_Player[nCnt].vtxMax *= SCALE_DOWN;
			g_Player[nCnt].vtxMin *= SCALE_DOWN;


			//頂点アンロックa
			g_PartsPattern[0].pMesh->UnlockVertexBuffer();
		}
	}
}

//-----------------------------------------------------------------------------
//モーション再生処理
//-----------------------------------------------------------------------------

void MotionsetPlayer(int nPlayerNum)
{
	int nMotion = g_MotionPlayer[nPlayerNum].nNowRebirthMotion;
	int nKey = g_MotionPlayer[nPlayerNum].nNowRebirthKeySet;
	int nFrame = g_Player[nPlayerNum].Motion[nMotion].nFrame[nMotion];
	int nNumKey = g_Player[nPlayerNum].Motion[nMotion].nNumKey;

	if (nFrame <= 0)
	{//テキスト設定フレーム数が０以下だった時
		nFrame = 1;
	}


	//モーション再生（全パーツ）D3DXVec3Normalize
	for (int nCnt = 0; nCnt < PARENT_MAX; nCnt++)
	{
		if (g_Player[nPlayerNum].Parts[nCnt].bUse)
		{
			D3DXVECTOR3 move, rot;


			move = (g_Player[nPlayerNum].Parts[nCnt].posMotionset[nMotion][nKey] - g_Player[nPlayerNum].Parts[nCnt].posMotionset[nMotion][((nKey - 1) + nNumKey) % nNumKey]) / (float)g_Player[nPlayerNum].Motion[nMotion].nFrame[nKey];
			//POSの再生（ローカル座標）
			g_Player[nPlayerNum].Parts[nCnt].pos += move;

			rot = (g_Player[nPlayerNum].Parts[nCnt].rotMotionset[nMotion][nKey] - g_Player[nPlayerNum].Parts[nCnt].rotMotionset[nMotion][((nKey - 1) + nNumKey) % nNumKey]) / (float)g_Player[nPlayerNum].Motion[nMotion].nFrame[nKey];

			//ROTの再生
			g_Player[nPlayerNum].Parts[nCnt].rot += rot;
		}
	}


	//フレームの加算
	g_MotionPlayer[nPlayerNum].nFrameCnt++;

	if (g_Player[nPlayerNum].Motion[nMotion].nFrame[nKey] <= g_MotionPlayer[nPlayerNum].nFrameCnt)
	{//フレーム数が設定の値を超えたら

	 //モーションの初期化とキーのセット変更
		ChangeMotion(nPlayerNum,nMotion, nKey);

		//再生中のキー数の加算
		g_MotionPlayer[nPlayerNum].nNowRebirthKeySet++;
		//フレームの初期化
		g_MotionPlayer[nPlayerNum].nFrameCnt = 0;

		if (g_Player[nPlayerNum].Motion[nMotion].nNumKey <= g_MotionPlayer[nPlayerNum].nNowRebirthKeySet)
		{//再生中のキー数が設定の値を超えたら
			if (g_Player[nPlayerNum].Motion[nMotion].nLoop == 1)
			{
				g_MotionPlayer[nPlayerNum].nNowRebirthKeySet = 0;

				//モーションの初期化とキーのセット変更
				ChangeMotion(nPlayerNum,g_MotionPlayer[nPlayerNum].nNowRebirthMotion, 1);
			}
			else if (nMotion == PLAYER_HAVE)
			{
				g_MotionPlayer[nPlayerNum].nNowRebirthKeySet = 0;
				g_MotionPlayer[nPlayerNum].nNowRebirthMotion = 1;

				//モーションの初期化とキーのセット変更
				//ChangeMotion(g_MotionPlayer.nNowRebirthMotion, g_MotionPlayer.nNowRebirthKeySet);
			}
			else
			{//現在再生中のモーションからランモーションに変更
				g_MotionPlayer[nPlayerNum].nNowRebirthKeySet = 0;
				g_MotionPlayer[nPlayerNum].nNowRebirthMotion = 1;
				//モーションの初期化とキーのセット変更
				ChangeMotion(nPlayerNum,g_MotionPlayer[nPlayerNum].nNowRebirthMotion, 1);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//モーションの初期化とキーのセット変更
//-----------------------------------------------------------------------------
void ChangeMotion(int nPlayerNum,int nMotion, int nKey)
{
	for (int nCnt = 0; nCnt < PARENT_MAX; nCnt++)
	{
		if (g_Player[nPlayerNum].Parts[nCnt].bUse)
		{
			//g_Player.Parts[nCnt].pos = g_Player.Parts[nCnt].posMotionset[nMotion][nKey];
			g_Player[nPlayerNum].Parts[nCnt].rot = g_Player[nPlayerNum].Parts[nCnt].rotMotionset[nMotion][nKey];

		}
	}


}

//-----------------------------------------------------------------------------
//プレイヤーの位置の参照
//-----------------------------------------------------------------------------

D3DXVECTOR3 GetPosPlayer(int nPlayerNum)
{
	return g_PlayerPos[nPlayerNum];
}

//-----------------------------------------------------------------------------
//プレイヤーのキーの読み取り
//-----------------------------------------------------------------------------

bool MoveKeyPlayer(int nPlayerNum, JOYKEY_CROSS Key)
{

	if (Key == JOYKEY_CROSS_UP)
	{
		if (GetJoypadPress(JOYKEY_UP, nPlayerNum)
			|| GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP, nPlayerNum)
			|| (GetKeyboardPress(DIK_W) && nPlayerNum == 0)
			|| (GetKeyboardPress(DIK_UP) && nPlayerNum == 1))
		{
			return true;
		}
	}
	else if (Key == JOYKEY_CROSS_LEFT)
	{
		if (GetJoypadPress(JOYKEY_LEFT, nPlayerNum)
			|| GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_LEFT, nPlayerNum)
			|| (GetKeyboardPress(DIK_A) && nPlayerNum == 0)
			|| (GetKeyboardPress(DIK_LEFT) && nPlayerNum == 1))
		{
			return true;
		}
	}
	else if (Key == JOYKEY_CROSS_RIGHT)
	{
		if (GetJoypadPress(JOYKEY_RIGHT, nPlayerNum)
			|| GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_RIGHT, nPlayerNum)
			|| (GetKeyboardPress(DIK_D) && nPlayerNum == 0)
			|| (GetKeyboardPress(DIK_RIGHT) && nPlayerNum == 1))
		{
			return true;
		}
	}
	else if (Key == JOYKEY_CROSS_DOWN)
	{
		if (GetJoypadPress(JOYKEY_DOWN, nPlayerNum)
			|| GetJoypadStickPress(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN, nPlayerNum)
			|| (GetKeyboardPress(DIK_S) && nPlayerNum == 0)
			|| (GetKeyboardPress(DIK_DOWN) && nPlayerNum == 1))
		{
			return true;
		}
	}
	else if (Key == JOYKEY_CROSS_MAX)
	{
		if (GetJoypadPress(JOYKEY_A, nPlayerNum)
			|| (GetKeyboardPress(DIK_SPACE) && nPlayerNum == 0)
			|| (GetKeyboardPress(DIK_RETURN) && nPlayerNum == 1))
		{
			return true;
		}
	}

	return false;
}