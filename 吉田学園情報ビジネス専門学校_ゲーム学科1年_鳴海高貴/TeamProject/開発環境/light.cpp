//=============================================================================
//
// 処理 []
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------

#include "light.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------

#define	LIGHT_MAX	(3)	//ライトの最大数

//-----------------------------------------------------------------------------
//グローバル変数宣言
//-----------------------------------------------------------------------------

D3DLIGHT9 g_alight[LIGHT_MAX];//ライトの情報

//*****************************************************************************
//ライトの初期化
//*****************************************************************************

void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	D3DXVECTOR3 vecDir[LIGHT_MAX];//ライトの方向ベクトル

	//ライトをクリアする
	ZeroMemory(&g_alight[0], sizeof(g_alight));

	//ライトの種類を設定
	g_alight[0].Type = D3DLIGHT_DIRECTIONAL;
	g_alight[1].Type = D3DLIGHT_DIRECTIONAL;
	g_alight[2].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	g_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_alight[1].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	g_alight[2].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	//ライトの位置
	g_alight[0].Position = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	g_alight[0].Position = D3DXVECTOR3(100.0f, 50.0f, 0.0f);
	g_alight[0].Position = D3DXVECTOR3(-100.0f, 50.0f, 0.0f);

	//ライトの方向を設定
	vecDir[0] = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	vecDir[1] = D3DXVECTOR3(-0.4f, 0.2f, -0.8f);
	vecDir[2] = D3DXVECTOR3(-0.8f, -0.4f, 0.2f);

	//正規化する（大きさ１のベクトルにする）；
	D3DXVec3Normalize(&vecDir[0], &vecDir[0]);
	D3DXVec3Normalize(&vecDir[1], &vecDir[1]);
	D3DXVec3Normalize(&vecDir[2], &vecDir[2]);
	g_alight[0].Direction = vecDir[0];
	g_alight[1].Direction = vecDir[1];
	g_alight[2].Direction = vecDir[2];

	//ライトを設定する
	pDevice->SetLight(0, &g_alight[0]);
	pDevice->SetLight(1, &g_alight[1]);
	pDevice->SetLight(2, &g_alight[2]);

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);

}

//*****************************************************************************
//ライトの終了処理
//*****************************************************************************

void UninitLight(void)
{

}

//*****************************************************************************
//ライトの更新処理
//*****************************************************************************

void UpdateLight(void)
{

}

//*****************************************************************************
//ライトの設定処理
//*****************************************************************************

void SetLight(void)
{

}