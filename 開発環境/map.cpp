//=============================================================================
//
// マップ制作をExcelでする処理 [map.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "main.h"
#include "map.h"
#include "block.h"
#include "enemy.h"
#include "item.h"
#include "goal.h"
#include <stdio.h>

int mapDataBox[MAP_HEIGHT][MAP_WIDTH]= {};
LPDIRECT3DTEXTURE9 g_apTextureMap = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMap = NULL;

void InitMap(void)
{
	FILE *pFile;

	//ファイルを開く
	pFile = fopen(MAP_PATH, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		for (int nCntY = 0; nCntY < MAP_HEIGHT; nCntY++)
		{
			for (int nCntX = 0; nCntX < MAP_WIDTH; nCntX++)
			{//MAPの読み込み
				int temp;
				fscanf(pFile, "%d", &temp);
				mapDataBox[nCntY][nCntX] = temp;
			}
		}
	}
	else
	{
		printf("***ファイルが開けませんでした\n");
	}

	//ファイルを閉じる
	fclose(pFile);

	//ブロック設定
	for (int nCntY = 0; nCntY < MAP_HEIGHT; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAP_WIDTH; nCntX++)
		{//Mapの書き込み
			switch (mapDataBox[nCntY][nCntX])
			{
			case 0: //何もない
				break;

			case 1: //ブロック
				SetBlock(D3DXVECTOR3((BLOCK_SIZE *nCntX), BLOCK_SIZE * nCntY, 0.0f), BLOCK_SIZE, BLOCK_SIZE);
				break;

			case 2:	//敵配置
				SetEnemy(D3DXVECTOR3(ENEMY_WIDTH *nCntX, ENEMY_HEIGHT * nCntY, 0.0f),1);
				break;

			case 3: //コイン配置
				SetItem(D3DXVECTOR3 (ITEM_SIZE * nCntX, ITEM_SIZE * nCntY, 0.0f));
				break;

			case 4: //敵2配置
				SetEnemy(D3DXVECTOR3(ENEMY_WIDTH * nCntX, ENEMY_HEIGHT * nCntY, 0.0f),0);
				break;

			default:
				break;
			}
		}
	}
}
void DrawMap(void)
{
}