//=============================================================================
//
// メイン処理 [main.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"
#include <stdio.h>
#include "input.h"
#include "light.h"
#include "load.h"
#include "player.h"
#include "camera.h"
#include "polygon.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "pause.h"
#include "fade.h"
#include "disc.h"
#include "goal.h"
#include "billboard.h"
#include "tutorial.h"
#include "sound.h"

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------

LPDIRECT3D9		g_pD3D = NULL;						//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;			//Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;							//フォントへのポインタ
int g_nCountFPS = 0;								//FPSカウンタ
int g_nCheck[8][8] = { { NULL } };
int g_nCntHil[2];
MODE g_mode = MODE_TITLE;							//現在のモード

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//*****************************************************************************
//メイン関数
//*****************************************************************************

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,								//ウインドウのスタイル
		WindowProc,								//ウインドウプロシージャ
		0,										//0にする（通常は使用しない）
		0,										//0にする（通常は使用しない）
		hInstance,								//インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),				//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),				//クライアント領域の背景色
		NULL,									//メニューバー
		CLASS_NAME,								//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)			//ファイルのアイコン
	};
	DWORD dwCurrentTime;		//現在時刻

	DWORD dwExecLastTime;		//最後に処理した時刻

	DWORD dwFrameCount;			//フレームカウント

	DWORD dwFPSLastTime;		//最後にFPSを計測した時刻


	HWND hWnd;		//ウインドハンドル（識別子）
	MSG msg;		//メッセージを格納する変数

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };//画面構造体

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);


	//クライアント領域を指定サイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(
		0,						//拡張ウインドウスタイル
		CLASS_NAME,				//ウインドウクラスの名前
		WINDOW_NAWE,			//ウインドウの名前
		WS_OVERLAPPEDWINDOW,	//ウインドウスタイル
		CW_USEDEFAULT,			//ウインドウの左上X座標
		CW_USEDEFAULT,			//ウインドウの左上Y座標
		(rect.right - rect.left),//ウインドウの幅
		(rect.bottom - rect.top),//ウインドウの高さ
		NULL,					//親ウインドウのハンドル
		NULL,					//メニューハンドルまたは子ウィンドウのID
		hInstance,
		NULL);

	//初期化処理FALSE
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化失敗TRUE
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();
	dwCurrentTime = 0;				//初期化する
	dwExecLastTime = timeGetTime(); //現在時刻を取得（保存）

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);		//ウインドウの表示状態を設定
	UpdateWindow(hWnd);				//クライアント領域を更新

									//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();		//現在時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;			//FPSを計測した時刻を保存
				dwFrameCount = 0;						//フレームカウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime)>=(1000/60))
			{//120分の1秒経過
				dwExecLastTime = dwCurrentTime;		//処理開始の時刻[現在時刻]を保存

				//更新処理
				Update();
				

				//描画処理
				Draw();

				dwFrameCount++;		//フレームカウントを加算
			}
		}
	}

	//終了処理
	Uninit();
	//ウインドウクラスの登録解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;

}

//*****************************************************************************
//ウインドウプロシージャ
//*****************************************************************************

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:		//ウインドウ破壊のメッセージ
							//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			//ウインドウの破壊
			DestroyWindow(hWnd);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//*****************************************************************************
//初期化処理
//*****************************************************************************

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;		//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

									//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));	//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;	//ゲーム画面サイズ（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;	//ゲーム画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;	//バッファの形式
	d3dpp.BackBufferCount = 1;	//バッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//ダブルバッファの切り替え（映像信号に同期）
	d3dpp.EnableAutoDepthStencil = TRUE;	//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//デプスバッファとして１６BITを使う
	d3dpp.Windowed = bWindow;	//ウインドモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

	//Direct3Dデバイスの生成（描画処理と頂点処理はハードウェアで行う）
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行う)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3Dデバイスの生成(描画処理と頂点処理はCPUで行う)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);

	//レンダーステートの設定D3DCULL_CCW
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//メッシュ化
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//入力処理の初期化処理
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//サウンドの初期化
	InitSound(hWnd);

	//フェードの設定
	InitFade(g_mode);

	//モードの設定
	SetMode(g_mode);

	//ランダム種の初期化
	srand((unsigned int)time(0));

	 g_nCntHil[0] = 0;
	 g_nCntHil[1] = 0;

	return S_OK;
}

//*****************************************************************************
//終了処理
//*****************************************************************************

void Uninit(void)
{
	
	//分解能を戻す
	timeEndPeriod(1);

	//入力処理の終了処理
	UninitInput();

	//タイトル画面の終了処理
	UninitTitle();

	////チュートリアル画面の終了処理
	//UninitTutorial();

	//ゲーム画面の終了処理
	UninitGame();

	//リザルト画面の終了処理
	UninitResult();
	//サウンド終了処理
	UninitSound();
	//Direct3Dデバイスの破壊
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	//デバック表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//*****************************************************************************
//更新処理
//*****************************************************************************

void Update(void)
{
	//入力処理の更新処理
	UpdateInput();

	//モードの更新処理
	switch (g_mode)
	{
		case MODE_TITLE:		//タイトル画面
			UpdateTitle();
			break;

		case MODE_TUTORIAL:		//チュートリアル画面
			UpdateTutorial();
			break;

		case MODE_GAME:			//ゲーム画面
			UpdateGame();
			break;

		case MODE_RESULT:		//リザルト画面
			UpdateResult();
			break;

		//case MODE_RANKING:		//ランキング画面
		//	UpdateRanking();
	}

	//フェードの更新処理
	UpdateFade();

	/* if (GetMouseWheel() < 0)
	 {
		 int nData = 0;
	 }*/
}

//*****************************************************************************
//描画処理
//*****************************************************************************

void Draw(void)
{
	//画面クリア（バックバッファ＆Zバッファのクリア）
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功したら

	 //モードの更新処理
		switch (g_mode)
		{
			case MODE_TITLE:		//タイトル画面
				DrawTitle();
				break;

			case MODE_TUTORIAL:		//チュートリアル画面
				DrawTutorial();
				break;

			case MODE_GAME:			//ゲーム画面
				DrawGame();
				break;

			case MODE_RESULT:		//リザルト画面
				DrawResult();
				break;

			//case MODE_RANKING:		//ランキング画面
			//	DrawRanking();
		}

#ifdef _DEBUG
		//FPSの表示
		//DrawFPS();
#endif

		//フェードの描画
		DrawFade();

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

//==========================================================================================================
//モードの設定
//==========================================================================================================
void SetMode(MODE mode)
{
	//現在の画面(モード)の終了処理
	switch (g_mode)
	{
		case MODE_TITLE:		//タイトル画面
			UninitTitle();
			break;

		case MODE_TUTORIAL:		//チュートリアル画面
			UninitTutorial();
			break;

		case MODE_GAME:			//ゲーム画面
			UninitGame();
			break;

		case MODE_RESULT:		//リザルト画面
			UninitResult();
			break;

		//case MODE_RANKING:		//ランキング画面
		//	UninitRanking();
		//	break;
	}

	g_mode = mode;		//現在の画面(モード)を切り替える

	//新しい画面(モード)の初期化処理
	switch (g_mode)
	{
		case MODE_TITLE:		//タイトル画面
			InitTitle();
			break;

		case MODE_TUTORIAL:		//チュートリアル画面
			InitTutorial();
			break;

		case MODE_GAME:			//ゲーム画面
			InitGame();
			break;

		case MODE_RESULT:		//リザルト画面
			InitResult();
			break;

		//case MODE_RANKING:		//ランキング画面
		//	InitRanking();
		//	RoadRanking();
		//	SetRanking(GetScore());
		//	break;
	}
}

//-----------------------------------------------------------------------------
//デバイスの取得
//-----------------------------------------------------------------------------

LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-----------------------------------------------------------------------------
//FPSの表示の描画処理
//-----------------------------------------------------------------------------

void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[20][256];
	
	/*if (g_nCntHil[0] < GetMouseWheel())
	{
		g_nCntHil[0] = GetMouseWheel();
	}
	if (g_nCntHil[1] > GetMouseWheel())
	{
		g_nCntHil[1] = GetMouseWheel();
	}*/
	

	//文字列に代入
	sprintf(&aStr[0][0], "現在:%f\n最大:%f\n最小:%f 　[追加] Oキーでスコア加算", GetJoypadStick(JOYKEY_LEFT_STICK,0).x, GetJoypadStick(JOYKEY_LEFT_STICK, 0).y, GetJoypadStick(JOYKEY_LEFT_STICK, 0).z);
	
	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	
}

//==========================================================================================================
//モードの取得
//==========================================================================================================
MODE GetMode(void)
{
	return g_mode;
}