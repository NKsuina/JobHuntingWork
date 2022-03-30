//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルード
//-----------------------------------------------------------------------------

#include "camera.h"
#include "input.h"
#include "calculation.h"

//-----------------------------------------------------------------------------
//グローバル変数宣言
//-----------------------------------------------------------------------------

Camera g_camera; //カメラ情報

//*****************************************************************************
//カメラの初期化
//*****************************************************************************

void InitCamera(void)
{
	//視点・注視点・上方向を設定をする
	g_camera.posV = D3DXVECTOR3(0.0f, 700.0f, -700.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fData = g_camera.posV.x - g_camera.posR.x;
	float fData1 = g_camera.posV.z - g_camera.posR.z;

	g_camera.fDistance = sqrtf(ThreeSquareTheorem(fData, fData1));
}

//*****************************************************************************
//カメラの終了処理
//*****************************************************************************

void UninitCamera(void)
{

}

//*****************************************************************************
//カメラの更新処理
//*****************************************************************************

void UpdateCamera(void)
{
	//float fDistance = sqrtf(g_camera.rot.x * g_camera.rot.x + g_camera.rot.y * g_camera.rot.y);
	////旋回
	//if (GetKeyboardPress(DIK_T))
	//{//Aキーが押された
	//	g_camera.rot.y += -0.01f;
	//	g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
	//	g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	//}
	//else if (GetKeyboardPress(DIK_G))
	//{//Dキーが押された
	//	g_camera.rot.y += 0.01f;
	//	g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
	//	g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	//}
	//else if (GetKeyboardPress(DIK_R))
	//{//注視点を上に
	//	g_camera.posR.y += 5.0f;
	//}
	//else if (GetKeyboardPress(DIK_F))
	//{//注視点を下に
	//	g_camera.posR.y += -5.0f;
	//}
	//else if (GetKeyboardPress(DIK_Y))
	//{//視点を上に
	//	g_camera.posV.y += 5.0f;
	//}
	//else if (GetKeyboardPress(DIK_H))
	//{//視点を下に
	//	g_camera.posV.y += -5.0f;
	//}
	
	//視点移動
	//if (GetKeyboardPress(DIK_W))
	//{//上キーが押された
	//	g_camera.posV.x += sinf(g_camera.rot.y) * 2.0f;
	//	g_camera.posV.z += cosf(g_camera.rot.y) * 2.0f;
	//	g_camera.posR.x += sinf(g_camera.rot.y) * 2.0f;
	//	g_camera.posR.z += cosf(g_camera.rot.y) * 2.0f;
	//}
	//else if (GetKeyboardPress(DIK_A))
	//{//左キーが押された

	//	g_camera.posV.x += sinf(g_camera.rot.y + D3DX_PI * -0.5f) * 2.0f;
	//	g_camera.posV.z += cosf(g_camera.rot.y + D3DX_PI * -0.5f) * 2.0f;
	//	g_camera.posR.x += sinf(g_camera.rot.y + D3DX_PI * -0.5f) * 2.0f;
	//	g_camera.posR.z += cosf(g_camera.rot.y + D3DX_PI * -0.5f) * 2.0f;
	//}
	//else if (GetKeyboardPress(DIK_D))
	//{//右キーが押された
	//	
	//	g_camera.posV.x += sinf(g_camera.rot.y + D3DX_PI * 0.5f) * 2.0f;
	//	g_camera.posV.z += cosf(g_camera.rot.y + D3DX_PI * 0.5f) * 2.0f;
	//	g_camera.posR.x += sinf(g_camera.rot.y + D3DX_PI * 0.5f) * 2.0f;
	//	g_camera.posR.z += cosf(g_camera.rot.y + D3DX_PI * 0.5f) * 2.0f;
	//}
	//else if (GetKeyboardPress(DIK_S))
	//{//下キーが押された
	//	
	//	g_camera.posV.x += sinf(g_camera.rot.y + D3DX_PI) * 2.0f;
	//	g_camera.posV.z += cosf(g_camera.rot.y + D3DX_PI) * 2.0f;
	//	g_camera.posR.x += sinf(g_camera.rot.y + D3DX_PI) * 2.0f;
	//	g_camera.posR.z += cosf(g_camera.rot.y + D3DX_PI) * 2.0f;
	//}

	//////視点旋回
	//if (GetKeyboardPress(DIK_Q))
	//{
	//	g_camera.rot.y += -0.01f;
	//	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
	//	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	//}
	//else if (GetKeyboardPress(DIK_E))
	//{
	//	g_camera.rot.y += 0.01f;
	//	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
	//	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	//}

	//補正
	g_camera.rot.y = Normalization(g_camera.rot.y);
}

//*****************************************************************************
//セットカメラ処理
//*****************************************************************************

void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	D3DXMatrixLookAtLH(&g_camera.mtxView,&g_camera.posV,&g_camera.posR,&g_camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),//視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,//アスペクト比
		10.0f,//どこからどこまでカメラで表示するか
		4000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}

//-----------------------------------------------------------------------------
//カメラの情報を渡す
//-----------------------------------------------------------------------------

Camera *GetCamera(void)
{
	return &g_camera;
}