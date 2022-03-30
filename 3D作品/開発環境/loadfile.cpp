//================================================
//
//		ファイル入出力処理[loadfile.cpp]
//		Author	: NARUMI KOUKI
//
//================================================

#include "main.h"
#include "model.h"
#include "loadfile.h"
#include <stdio.h>
#include <string.h>

//===================================
//ファイル入出力の初期化処理
//===================================
void InitLoadFile(void)
{
	FILE *pFile;
	char a[128];					// = を保存する
	char savefile[128][256];		// =から先の部分を保存する
	int	 Cnt = 0;

	//ファイルを開く
	pFile = fopen("data/loadfile.txt", "r");

	if (pFile != NULL)
	{
		while (fscanf(pFile, "%s", &a[0]) != EOF)	//EOF = EndOfFile
		{//END_MODELSETが来るまで回す
			if (strcmp(&a[0], "MODEL_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &a[0]);
				fscanf(pFile, "%s", &savefile[Cnt][0]);
				Cnt++;
			}
			if (strcmp(&a[0], "MODELSET") == 0)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				int nType;

				while (1)		
				{
					fscanf(pFile, "%s", &a[0]);
					if ((strcmp(&a[0], "TYPE")) == 0)
					{
						fscanf(pFile, "%s", &a[0]);
						fscanf(pFile, "%d", &nType);		//モデルの種類
					}
					if ((strcmp(&a[0], "POS")) == 0)
					{
						fscanf(pFile, "%s", &a[0]);		
						fscanf(pFile, "%f", &pos.x);		//x座標の保存
						fscanf(pFile, "%f", &pos.y);		//y座標の保存
						fscanf(pFile, "%f", &pos.z);		//z座標の保存
					}
					if ((strcmp(&a[0], "ROT")) == 0)
					{
						fscanf(pFile, "%s", &a[0]);
						fscanf(pFile, "%f", &rot.x);		//角度xの保存
						fscanf(pFile, "%f", &rot.y);		//角度yの保存
						fscanf(pFile, "%f", &rot.z);		//角度zの保存
					}
					if ((strcmp(&a[0], "END_MODELSET")) == 0)
					{//この文字が来たら保存した情報をSetModelに入れてゲームにセットする
						SetModel(pos, rot, nType,&savefile[nType][0]);		
						break;
					}
				}
			}
		}
	}
}