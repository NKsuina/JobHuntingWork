//=============================================================================
//
// �}�b�v�����Excel�ł��鏈�� [map.cpp]
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

	//�t�@�C�����J��
	pFile = fopen(MAP_PATH, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int nCntY = 0; nCntY < MAP_HEIGHT; nCntY++)
		{
			for (int nCntX = 0; nCntX < MAP_WIDTH; nCntX++)
			{//MAP�̓ǂݍ���
				int temp;
				fscanf(pFile, "%d", &temp);
				mapDataBox[nCntY][nCntX] = temp;
			}
		}
	}
	else
	{
		printf("***�t�@�C�����J���܂���ł���\n");
	}

	//�t�@�C�������
	fclose(pFile);

	//�u���b�N�ݒ�
	for (int nCntY = 0; nCntY < MAP_HEIGHT; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAP_WIDTH; nCntX++)
		{//Map�̏�������
			switch (mapDataBox[nCntY][nCntX])
			{
			case 0: //�����Ȃ�
				break;

			case 1: //�u���b�N
				SetBlock(D3DXVECTOR3((BLOCK_SIZE *nCntX), BLOCK_SIZE * nCntY, 0.0f), BLOCK_SIZE, BLOCK_SIZE);
				break;

			case 2:	//�G�z�u
				SetEnemy(D3DXVECTOR3(ENEMY_WIDTH *nCntX, ENEMY_HEIGHT * nCntY, 0.0f),1);
				break;

			case 3: //�R�C���z�u
				SetItem(D3DXVECTOR3 (ITEM_SIZE * nCntX, ITEM_SIZE * nCntY, 0.0f));
				break;

			case 4: //�G2�z�u
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