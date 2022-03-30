//-----------------------------------------------------------------------------------------------------------
//
//壁モデル	
//Author; Takano Minto
//
//-----------------------------------------------------------------------------------------------------------
#include<stdio.h>
#include"wallmodel.h"
#include"camera.h"

//--------------------------
//マクロ定義
//--------------------------
#define MAX_MODEL				(128)								//配置出来るモデルの最大数
#define SCAN_SPELL				(1200)								//読み込みできる最大文字数
#define LOAD_FALE				"Data/Text/wallModel.txt"			//読み込むテキストファイル

//--------------------------
//スタティック変数
//--------------------------
static WALLMODEL s_aWallModel[MAX_MODEL];							//壁モデルの構造体情報
static int s_nModelCount;											//モデル用のカウント
static int s_nPartsCount;											//モデル情報（POSなど）用のカウント

//----------------------
//初期化
//----------------------
void InitWallModel(void)
{
	//各モデルカウント変数の初期化
	s_nModelCount = 0;
	s_nPartsCount = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスのポインタ

													// ファイルポインタの宣言
	FILE * pFile;

	pFile = fopen(LOAD_FALE, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合

		char aMoji[SCAN_SPELL];

		fscanf(pFile, "%s", &aMoji[0]);		//開いたファイル内を読み込む

		while (strncmp(&aMoji[0], "SCRIPT", 6) != 0)
		{
			fscanf(pFile, "%s", &aMoji[0]);
		}

		while (strncmp(&aMoji[0], "END_SCRIPT", 10) != 0)
		{// 文字列の初期化と読み込み
			fscanf(pFile, "%s", &aMoji[0]);

			if (strcmp(&aMoji[0], "MODEL_FILENAME") == 0)
			{// 文字列が一致した場合
				fscanf(pFile, "%s", &aMoji[0]);		//”＝"を読み飛ばす
				fscanf(pFile, "%s", &aMoji[0]);		//＝から先を読み込む

				//Xファイルの読み込み
				D3DXLoadMeshFromX(&aMoji[0],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&s_aWallModel[s_nModelCount].pBuffMat,
					NULL,
					&s_aWallModel[s_nModelCount].nNumMat,
					&s_aWallModel[s_nModelCount].pMesh);

				s_nModelCount++;
			}
			else if (strcmp(&aMoji[0], "MODELSET") == 0)
			{// 文字列が一致した場合

				while (strcmp(&aMoji[0], "END_MODELSET") != 0)
				{
					fscanf(pFile, "%s", &aMoji[0]);

					if (strcmp(&aMoji[0], "POS") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &aMoji[0]);
						fscanf(pFile, "%f", &s_aWallModel[s_nPartsCount].pos.x);
						fscanf(pFile, "%f", &s_aWallModel[s_nPartsCount].pos.y);
						fscanf(pFile, "%f", &s_aWallModel[s_nPartsCount].pos.z);
					}

					if (strcmp(&aMoji[0], "ROT") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &aMoji[0]);
						fscanf(pFile, "%f", &s_aWallModel[s_nPartsCount].rot.x);
						fscanf(pFile, "%f", &s_aWallModel[s_nPartsCount].rot.y);
						fscanf(pFile, "%f", &s_aWallModel[s_nPartsCount].rot.z);
					}

					if (strcmp(&aMoji[0], "COLLITION") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &aMoji[0]);
						fscanf(pFile, "%f", &s_aWallModel[s_nPartsCount].size.x);
						fscanf(pFile, "%f", &s_aWallModel[s_nPartsCount].size.y);
						fscanf(pFile, "%f", &s_aWallModel[s_nPartsCount].size.z);
					}
				}

				s_nPartsCount++;
			}
			else if (strcmp(&aMoji[0], "END_SCRIPT") == 0)
			{// 文字列が一致した場合
				break;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けない場合
		printf("\n * * * ファイルが...開けませんでしたッッッ!!!!!!!!! * * * \n");
	}

	int nNumVtx;				//頂点の種類 (数)
	DWORD SizeFVF;				//頂点に使用するフォーマット（書式）
	BYTE *pVtxBuff;				//一時的に情報を書き留めておく頂点のポインタ

	for (int nUse = 0; nUse < MAX_MODEL; nUse++)
	{//MIN・MAX等の初期化
		s_aWallModel[nUse].Min = D3DXVECTOR3(5000.0f, 5000.0f, 5000.0f);
		s_aWallModel[nUse].Max = D3DXVECTOR3(-5000.0f, -5000.0f, -5000.0f);
		s_aWallModel[nUse].bUse = false;
		s_aWallModel[nUse].bVibration = false;
		s_aWallModel[nUse].nCntWall = 0;
	}

	//Xファイル呼び込み時にカウントしたs_nModelCount分でforを回す
	for (int nCnt = 0; nCnt < s_nModelCount; nCnt++)
	{
		SetWallModel(s_aWallModel[nCnt].pos, s_aWallModel[nCnt].rot);
	}

	//Xファイル呼び込み時にカウントしたs_nModelCount分でforを回す
	for (int nCnt = 0; nCnt < s_nModelCount; nCnt++)
	{
		//頂点数の取得する
		nNumVtx = s_aWallModel[nCnt].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		SizeFVF = D3DXGetFVFVertexSize(s_aWallModel[nCnt].pMesh->GetFVF());

		//頂点情報をロックする
		s_aWallModel[nCnt].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCollition = 0; nCollition < nNumVtx; nCollition++)
		{
			//頂点情報の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//頂点座標の比較
			if (vtx.x < s_aWallModel[nCnt].Min.x)
			{
				s_aWallModel[nCnt].Min.x = vtx.x;	//値をそれと同じになるように設定する
			}

			if (vtx.x > s_aWallModel[nCnt].Max.x)
			{
				s_aWallModel[nCnt].Max.x = vtx.x;	//値をそれと同じになるように設定する
			}

			if (vtx.z < s_aWallModel[nCnt].Min.z)
			{
				s_aWallModel[nCnt].Min.z = vtx.z;	//値をそれと同じになるように設定する
			}

			if (vtx.z > s_aWallModel[nCnt].Max.z)
			{
				s_aWallModel[nCnt].Max.z = vtx.z;	//値をそれと同じになるように設定する
			}

			//頂点フォーマットのサイズ分のポインタを求める(？)
			pVtxBuff += SizeFVF;
		}

		//頂点バッファをアンロックする
		s_aWallModel[nCnt].pMesh->UnlockVertexBuffer();

		// バッファの先頭ポインタをD3DXMATERIALに変換して取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_aWallModel[nCnt].pBuffMat->GetBufferPointer();

		//各メッシュのマテリアル情報を取得する
		for (int nCntTex = 0; nCntTex < (int)s_aWallModel[nCnt].nNumMat; nCntTex++)
		{//整数変換されたマテリアルの種類分forを回す
			s_aWallModel[nCnt].pTexture[nCntTex] = NULL;

			if (pMat[nCntTex].pTextureFilename != NULL)
			{//マテリアルで設定されているテクスチャが読み込まれている
				D3DXCreateTextureFromFileA(pDevice,
					pMat[nCntTex].pTextureFilename,
					&s_aWallModel[nCnt].pTexture[nCntTex]);
			}
			else
			{//マテリアルで設定されているテクスチャが読み込まれていない
				s_aWallModel[nCnt].pTexture[nCntTex] = NULL;
			}
		}

	}

}

//----------------------
//終了処理
//----------------------
void UninitWallModel(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		for (int nCnt = 0; nCnt < (int)s_aWallModel[nCntModel].nNumMat; nCnt++)
		{//整数変換されたマテリアルの種類分forを回す
			if (s_aWallModel[nCntModel].pTexture[nCnt] != NULL)
			{// テクスチャの解放
				s_aWallModel[nCntModel].pTexture[nCnt]->Release();
				s_aWallModel[nCntModel].pTexture[nCnt] = NULL;
			}
		}

		//メッシュの破壊
		if (s_aWallModel[nCntModel].pMesh != NULL)
		{
			s_aWallModel[nCntModel].pMesh->Release();
			s_aWallModel[nCntModel].pMesh = NULL;
		}

		//マテリアルの破棄
		if (s_aWallModel[nCntModel].pBuffMat != NULL)
		{
			s_aWallModel[nCntModel].pBuffMat->Release();
			s_aWallModel[nCntModel].pBuffMat = NULL;
		}

	}
}

//----------------
//更新
//----------------
void UpdateWallModel(void)
{
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (s_aWallModel[nCnt].bUse && s_aWallModel[nCnt].bVibration)
		{
			s_aWallModel[nCnt].nCntWall++;

			if (s_aWallModel[nCnt].nCntWall % 8 == 0)
			{
				s_aWallModel[nCnt].pos.z += 5.0f;
			}
			else if (s_aWallModel[nCnt].nCntWall % 4 == 0)
			{
				s_aWallModel[nCnt].pos.z -= 5.0f;
			}

			if (s_aWallModel[nCnt].nCntWall > 30)
			{
				s_aWallModel[nCnt].nCntWall = 0;
				s_aWallModel[nCnt].bVibration = false;

				if (nCnt == 0)
				{
					s_aWallModel[nCnt].pos.z = 360.0f;
				}
				else
				{
					s_aWallModel[nCnt].pos.z = -360.0f;
				}
				

			}
		}
	}
}

//----------------
//描画
//----------------
void DrawWallModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスのポインタ

	D3DXMATRIX mtxRot, mtxTrans;					//計算用マトリックス
	D3DMATERIAL9 matDef;							//現在のマトリックス保存用
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ

	for (int nCnt = 0; nCnt < s_nModelCount; nCnt++)
	{
		if (s_aWallModel[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&s_aWallModel[nCnt].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, s_aWallModel[nCnt].rot.y, s_aWallModel[nCnt].rot.x, s_aWallModel[nCnt].rot.z);

			D3DXMatrixMultiply(&s_aWallModel[nCnt].mtxWorld, &s_aWallModel[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, s_aWallModel[nCnt].pos.x, s_aWallModel[nCnt].pos.y, s_aWallModel[nCnt].pos.z);

			D3DXMatrixMultiply(&s_aWallModel[nCnt].mtxWorld, &s_aWallModel[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスに設定
			pDevice->SetTransform(D3DTS_WORLD, &s_aWallModel[nCnt].mtxWorld);

			//現在のマテリアル保持
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)s_aWallModel[nCnt].pBuffMat->GetBufferPointer();

			for (int nCntModel = 0; nCntModel < (int)s_aWallModel[nCnt].nNumMat; nCntModel++)
			{//整数変換されたマテリアルの種類分forを回す
			 //マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntModel].MatD3D);

				// アンビエントライトの反映(色変更可能)
				//pMat[nCntModel].MatD3D.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };

				//テクスチャの設定
				pDevice->SetTexture(0, s_aWallModel[nCnt].pTexture[nCntModel]);

				//モデルの描画
				s_aWallModel[nCnt].pMesh->DrawSubset(nCntModel);
			}
		}
	}

	//保持していたマテリアルを返却
	pDevice->SetMaterial(&matDef);

}

//---------------------------
//壁モデルの設置
//---------------------------
void SetWallModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (s_aWallModel[nCnt].bUse == false)
		{//アイテムが使用されていない時
			s_aWallModel[nCnt].bUse = true;
			int nNumVtx;
			DWORD SizeFVF;
			BYTE *pVtxBuff;
			s_aWallModel[nCnt].Min = D3DXVECTOR3(5000.0f, 5000.0f, 5000.0f);
			s_aWallModel[nCnt].Max = D3DXVECTOR3(-5000.0f, -5000.0f, -5000.0f);
			s_aWallModel[nCnt].pos = pos;
			s_aWallModel[nCnt].rot = rot;

			//頂点数の取得する
			nNumVtx = s_aWallModel[0].pMesh->GetNumVertices();

			//頂点フォーマットのサイズを取得
			SizeFVF = D3DXGetFVFVertexSize(s_aWallModel[0].pMesh->GetFVF());

			s_aWallModel[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCollition = 0; nCollition < s_nModelCount; nCollition++)
			{
				//頂点情報の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//頂点座標の比較
				if (vtx.x < s_aWallModel[nCollition].Min.x)
				{
					s_aWallModel[0].Min.x = vtx.x;	//値をそれと同じになるように設定する
				}

				if (vtx.x > s_aWallModel[nCollition].Max.x)
				{
					s_aWallModel[nCollition].Max.x = vtx.x;	//値をそれと同じになるように設定する
				}

				if (vtx.z < s_aWallModel[nCollition].Min.z)
				{
					s_aWallModel[nCollition].Min.z = vtx.z;	//値をそれと同じになるように設定する
				}

				if (vtx.z > s_aWallModel[nCollition].Max.z)
				{
					s_aWallModel[nCollition].Max.z = vtx.z;	//値をそれと同じになるように設定する
				}

				//頂点フォーマットのサイズ分のポインタを求める(？)
				pVtxBuff += SizeFVF;
			}

			//頂点バッファをアンロックする
			s_aWallModel[0].pMesh->UnlockVertexBuffer();

			break;
		}
	}

	//頂点バッファをアンロックする
	s_aWallModel[0].pMesh->UnlockVertexBuffer();

}

//--------------------------
//当たり判定
//--------------------------
void CollitionWallModel(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size)
{
	for (int i = 0; i < MAX_MODEL; i++)
	{
		if (s_aWallModel[i].bUse == true)
		{
			float fRight = s_aWallModel[i].pos.x + s_aWallModel[i].Min.x;
			float fLeft = s_aWallModel[i].pos.x + s_aWallModel[i].Max.x;
			float fBack = s_aWallModel[i].pos.z + s_aWallModel[i].Max.z;
			float fFlont = s_aWallModel[i].pos.z + s_aWallModel[i].Min.z;

			size = s_aWallModel[i].size;

			if (pPos->x + size.x > fRight &&
				pPos->x - size.x < fLeft)
			{
				if (pPosOld->z + size.z <= fFlont &&
					pPos->z + size.z > fFlont)
				{//前から後ろの判定
					pPos->z = fFlont - size.z - 0.0001f;
				}

				if (pPosOld->z - size.z >= fBack &&
					pPos->z - size.z < fBack)
				{
					pPos->z = fBack + size.z + 0.0001f;
				}
			}

			if (pPos->z + size.z > fFlont &&
				pPos->z - size.z < fBack)
			{
				if (pPosOld->x + size.x <= fRight &&
					pPos->x + size.x > fRight)
				{//前から後ろの判定
					pPos->x = fRight - size.x - 0.0001f;
				}

				if (pPosOld->x - size.x >= fLeft &&
					pPos->x - size.x < fLeft)
				{
					pPos->x = fLeft + size.x + 0.0001f;
				}
			}
		}
	}
}

//--------------------------
//壁モデル情報の取得
//--------------------------
WALLMODEL *GetWallModel(void)
{
	return s_aWallModel;
}

void SetVibration(int nNum)
{
	s_aWallModel[nNum].bVibration = true;
}