//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "Effect.h"
#include "main.h"
#include "input.h"
#include "model.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "wall.h"

//グローバル変数宣言
Player g_Player;	
//ModelParts g_ModelParts;
//LPD3DXMESH g_pMeshPlayer = NULL;		//メッシュ情報へのポインタ
//LPD3DXBUFFER g_pBuffMatPlayer = NULL;	//マテリアル情報へのポインタ
//DWORD g_nNumMatPlayer = 0;				//マテリアル情報の数
//==============================================
//プレイヤーの初期化処理
//==============================================
void InitPlayer(void)
{
	int nNumVtx;		//頂点数

	g_Player.vtxMaxPlayer = D3DXVECTOR3(-10000, -10000, -10000);		//プレイヤーの最大値を用意

	g_Player.vtxMinPlayer = D3DXVECTOR3(10000, 10000, 10000);			//プレイヤーの最小値を用意

	//プレイヤーの情報を初期化
	g_Player.pos = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		
	g_Player.nTime = 0;									//プレイヤーが加速できるまでの時間
	g_Player.nTimeCoolDown = 0;							//加速してからの計測用変数

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/player1.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[0].pBuffMat,		//プレイヤーのマテリアル
		NULL,
		&g_Player.aModel[0].nNumMat,
		&g_Player.aModel[0].pMesh);

	//xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/player2.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[1].pBuffMat,		//プレイヤーのマテリアル
		NULL,
		&g_Player.aModel[1].nNumMat,
		&g_Player.aModel[1].pMesh);


	g_Player.aModel[0].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);		//位置の初期設定
	g_Player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期設定
	g_Player.aModel[0].IdxNumber = 0;							//インデックス番号
	g_Player.aModel[0].nldxModelParent = -1;					//親のインデックス

	g_Player.aModel[1].pos = D3DXVECTOR3(0.0f, -3.0f, -17.0f);	//位置の初期設定
	g_Player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期設定
	g_Player.aModel[1].IdxNumber = 1;							//インデックス番号
	g_Player.aModel[1].nldxModelParent = 0;						//親のインデックス

	////xファイルの読み込み
	//D3DXLoadMeshFromX("data/MODEL/airplane000.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatPlayer,
	//	NULL,
	//	&g_nNumMatPlayer,
	//	&g_pMeshPlayer);

	DWORD sizeFVF;		//頂点フォーマットのサイズ

	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	for (int nCntPlayer = 0; nCntPlayer < NUM_MODELPARTS; nCntPlayer++)
	{
		//頂点数の取得
		nNumVtx = g_Player.aModel[nCntPlayer].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_Player.aModel[nCntPlayer].pMesh->GetFVF());

		//頂点バッファのロック
		g_Player.aModel[nCntPlayer].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//頂点座標を比較してモデルの最小値、最大値を取得
			//x座標
			if (vtx.x < g_Player.vtxMinPlayer.x)
			{
				g_Player.vtxMinPlayer.x = floorf(vtx.x);
			}
			if (vtx.x > g_Player.vtxMaxPlayer.x)
			{
				g_Player.vtxMaxPlayer.x = floorf(vtx.x);
			}
			//y座標
			if (vtx.y < g_Player.vtxMinPlayer.y)
			{
				g_Player.vtxMinPlayer.y = floorf(vtx.y);
			}
			if (vtx.y > g_Player.vtxMaxPlayer.y)
			{
				g_Player.vtxMaxPlayer.y = floorf(vtx.y);
			}
			//z座標
			if (vtx.z < g_Player.vtxMinPlayer.z)
			{
				g_Player.vtxMinPlayer.z = floorf(vtx.z);
			}
			if (vtx.z > g_Player.vtxMaxPlayer.z)
			{
				g_Player.vtxMaxPlayer.z = floorf(vtx.z);
			}
			pVtxBuff += sizeFVF;
		}
		//頂点バッファのアンロック
		g_Player.aModel[nCntPlayer].pMesh->UnlockVertexBuffer();
	}
		g_Player.size.x = g_Player.vtxMaxPlayer.x - g_Player.vtxMinPlayer.x;
		g_Player.size.z = g_Player.vtxMaxPlayer.z - g_Player.vtxMinPlayer.z;

	//最初にプレイヤーが出現する座標
	g_Player.pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//影をプレイヤーにセット
	g_Player.IdxShadow = SetShadow(D3DXVECTOR3(g_Player.pos.x, 0.1f, g_Player.pos.z), g_Player.rot);
}

//====================================
//プレイヤーの終了処理
//====================================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < NUM_MODELPARTS; nCntPlayer++)
	{
		//メッシュの破棄
		if (g_Player.aModel[nCntPlayer].pMesh != NULL)
		{
			g_Player.aModel[nCntPlayer].pMesh->Release();
			g_Player.aModel[nCntPlayer].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_Player.aModel[nCntPlayer].pBuffMat != NULL)
		{
			g_Player.aModel[nCntPlayer].pBuffMat->Release();
			g_Player.aModel[nCntPlayer].pBuffMat = NULL;
		}
	}
}

//================================
//プレイヤーの更新処理
//================================
void UpdatePlayer(void)
{
	Wall *Wall = GetWall();			//壁のポインタ
	g_Player.nTime++;				
	Camera *pCamera = GetCamera();	//カメラのポインタ

	//前回の位置を保存
	g_Player.posOld = g_Player.pos;

	//↑移動
	if (GetKeyboardPress(DIK_W) == true)
	{//↑移動中に→移動を入れたら
		if (GetKeyboardPress(DIK_D) == true)
		{
			g_Player.pos.x -= sinf(pCamera->rot.y + (D3DX_PI / 4 * -4.0f)) * 0.7f;
			g_Player.pos.z -= cosf(pCamera->rot.y + (D3DX_PI / 4 * -4.0f)) * 0.7f;
			g_Player.rotDest.y = (-D3DX_PI / 1.5f) + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//↑移動中に←移動を入れたら
			g_Player.pos.x -= sinf(pCamera->rot.y + (D3DX_PI / 4 * -4.0f)) * 0.7f;
			g_Player.pos.z -= cosf(pCamera->rot.y + (D3DX_PI / 4 * -4.0f)) * 0.7f;
			g_Player.rotDest.y = (D3DX_PI / 1.5f) + pCamera->rot.y;
		}
		else
		{//↑移動だけ
			g_Player.pos.x += sinf(pCamera->rot.y) * 0.7f;
			g_Player.pos.z += cosf(pCamera->rot.y) * 0.7f;
			g_Player.rotDest.y = D3DX_PI + pCamera->rot.y;
		}
	}
	//↓移動
	if (GetKeyboardPress(DIK_S) == true)
	{//↓移動中に→移動を入れたら
		if (GetKeyboardPress(DIK_D) == true)
		{
			g_Player.pos.x += sinf(pCamera->rot.y - (D3DX_PI / 4 * 4.0f)) * 0.7f;
			g_Player.pos.z += cosf(pCamera->rot.y - (D3DX_PI / 4 * 4.0f)) * 0.7f;
			g_Player.rotDest.y = (-D3DX_PI / 4) + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//↓移動中に←移動を入れたら
			g_Player.pos.x += sinf(pCamera->rot.y + (D3DX_PI / 4 * 4.0f)) * 0.7f;
			g_Player.pos.z += cosf(pCamera->rot.y + (D3DX_PI / 4 * 4.0f)) * 0.7f;
			g_Player.rotDest.y = (D3DX_PI / 4) + pCamera->rot.y;
		}
		else
		{//↓移動のみ
			g_Player.pos.x += sinf(pCamera->rot.y + D3DX_PI) * 0.7f;
			g_Player.pos.z += cosf(pCamera->rot.y + D3DX_PI) * 0.7f;
			g_Player.rotDest.y = 0.0f + pCamera->rot.y;
		}
	}
	//←移動
	if (GetKeyboardPress(DIK_A) == true)
	{
		g_Player.pos.x += sinf(pCamera->rot.y - (D3DX_PI / 2)) * 0.7f;
		g_Player.pos.z += cosf(pCamera->rot.y - (D3DX_PI / 2)) * 0.7f;
		g_Player.rotDest.y = (D3DX_PI / 2) + pCamera->rot.y;
	}
	//→移動
	if (GetKeyboardPress(DIK_D) == true)
	{
		g_Player.pos.x += sinf(pCamera->rot.y + (D3DX_PI / 2)) * 0.7f;
		g_Player.pos.z += cosf(pCamera->rot.y + (D3DX_PI / 2)) * 0.7f;
		g_Player.rotDest.y = (-D3DX_PI / 2) + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		g_Player.pos.x += sinf(pCamera->rot.y) * 5.0f;
		g_Player.pos.z += cosf(pCamera->rot.y) * 5.0f;
		g_Player.rot.y = pCamera->rot.y + D3DX_PI;
	}

	for (int nCntWall = 0; nCntWall < 4; nCntWall++)
	{
		//壁の奥に行くと止まる
		if (g_Player.pos.z >= Wall->pos.z + 500)
		{
			g_Player.pos.z = Wall->pos.z + 500;
		}
		//壁の手前に来ると止まる
		if (g_Player.pos.z <= Wall->pos.z - 500)
		{
			g_Player.pos.z = Wall->pos.z - 500;
		}
		//壁の右側に行くと止まる
		if (g_Player.pos.x >= Wall->pos.x + 500)
		{
			g_Player.pos.x = Wall->pos.x + 500;
		}
		//壁の左側に行くと止まる
		if (g_Player.pos.x <= Wall->pos.x - 500)
		{
			g_Player.pos.x = Wall->pos.x - 500;
		}
	}

	//角度の更新
	if (g_Player.rotDest.y - g_Player.rot.y > D3DX_PI)
	{
		g_Player.rotDest.y -= D3DX_PI * 2;
	}
	if (g_Player.rotDest.y - g_Player.rot.y < -D3DX_PI)
	{
		g_Player.rotDest.y += D3DX_PI * 2;
	}
	g_Player.rot.y += (g_Player.rotDest.y - g_Player.rot.y) * 0.1f;

	//角度の正規化
	if (g_Player.rot.y > D3DX_PI)
	{
		g_Player.rot.y -= D3DX_PI * 2;
	}
	if (g_Player.rot.y < -D3DX_PI)
	{
		g_Player.rot.y += D3DX_PI * 2;
	}
	//プレイヤーのパーツ座標
	D3DXVECTOR3 partsPos = D3DXVECTOR3(g_Player.aModel[1].mtxWorld._41, g_Player.aModel[1].mtxWorld._42, g_Player.aModel[1].mtxWorld._43);
	
	//プレイヤーのパーツサイズ[41]はx座標[42]はy座標[43]はz座標
	D3DXVECTOR3 partsSize = D3DXVECTOR3(g_Player.aModel[1].mtxWorld._41 = 20, g_Player.aModel[1].mtxWorld._42 = 1, g_Player.aModel[1].mtxWorld._43 = 20);
	
	CollisionModel(&partsPos, &g_Player.posOld, &partsSize);

	//プレイヤーに影をつける処理
	SetPositionShadow(g_Player.IdxShadow, D3DXVECTOR3(g_Player.pos.x, 0.1f, g_Player.pos.z));
}

//================================
//プレイヤーの描画処理
//================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;						//計算用のマトリックス
	D3DMATERIAL9 matDef;								//現在のマテリアル保存用
	D3DXMATERIAL *pMat;									//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);

	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);

	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

	for (int nCntPlayerModel = 0; nCntPlayerModel < NUM_MODELPARTS; nCntPlayerModel++)
	{
		//モデルパーツの構造体のワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Player.aModel[nCntPlayerModel].mtxWorld);

		//モデルパーツの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.aModel[nCntPlayerModel].rot.y, g_Player.aModel[nCntPlayerModel].rot.x, g_Player.aModel[nCntPlayerModel].rot.z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayerModel].mtxWorld, &g_Player.aModel[nCntPlayerModel].mtxWorld, &mtxRot);

		//モデルパーツの位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Player.aModel[nCntPlayerModel].pos.x, g_Player.aModel[nCntPlayerModel].pos.y, g_Player.aModel[nCntPlayerModel].pos.z);

		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayerModel].mtxWorld, &g_Player.aModel[nCntPlayerModel].mtxWorld, &mtxTrans);

		//自分の親のモデルのマトリックスとの掛け算
		D3DXMATRIX mtxParent;
		if (g_Player.aModel[nCntPlayerModel].nldxModelParent == -1)
		{
			mtxParent = g_Player.mtxWorld;
		}
		else
		{
			mtxParent = g_Player.aModel[g_Player.aModel[nCntPlayerModel].nldxModelParent].mtxWorld;
		}

		D3DXMatrixMultiply(&g_Player.aModel[nCntPlayerModel].mtxWorld, &g_Player.aModel[nCntPlayerModel].mtxWorld, &mtxParent);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntPlayerModel].mtxWorld);

		//現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		//現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Player.aModel[nCntPlayerModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntPlayerModel].nNumMat; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//モデルパーツの描画
			g_Player.aModel[nCntPlayerModel].pMesh->DrawSubset(nCntMat);
		}
		//保持していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}
//=================================
//プレイヤーの取得
//=================================
Player* GetPlayer(void)
{
	return &g_Player;
}