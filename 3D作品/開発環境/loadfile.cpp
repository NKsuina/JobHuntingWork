//================================================
//
//		�t�@�C�����o�͏���[loadfile.cpp]
//		Author	: NARUMI KOUKI
//
//================================================

#include "main.h"
#include "model.h"
#include "loadfile.h"
#include <stdio.h>
#include <string.h>

//===================================
//�t�@�C�����o�͂̏���������
//===================================
void InitLoadFile(void)
{
	FILE *pFile;
	char a[128];					// = ��ۑ�����
	char savefile[128][256];		// =�����̕�����ۑ�����
	int	 Cnt = 0;

	//�t�@�C�����J��
	pFile = fopen("data/loadfile.txt", "r");

	if (pFile != NULL)
	{
		while (fscanf(pFile, "%s", &a[0]) != EOF)	//EOF = EndOfFile
		{//END_MODELSET������܂ŉ�
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
						fscanf(pFile, "%d", &nType);		//���f���̎��
					}
					if ((strcmp(&a[0], "POS")) == 0)
					{
						fscanf(pFile, "%s", &a[0]);		
						fscanf(pFile, "%f", &pos.x);		//x���W�̕ۑ�
						fscanf(pFile, "%f", &pos.y);		//y���W�̕ۑ�
						fscanf(pFile, "%f", &pos.z);		//z���W�̕ۑ�
					}
					if ((strcmp(&a[0], "ROT")) == 0)
					{
						fscanf(pFile, "%s", &a[0]);
						fscanf(pFile, "%f", &rot.x);		//�p�xx�̕ۑ�
						fscanf(pFile, "%f", &rot.y);		//�p�xy�̕ۑ�
						fscanf(pFile, "%f", &rot.z);		//�p�xz�̕ۑ�
					}
					if ((strcmp(&a[0], "END_MODELSET")) == 0)
					{//���̕�����������ۑ���������SetModel�ɓ���ăQ�[���ɃZ�b�g����
						SetModel(pos, rot, nType,&savefile[nType][0]);		
						break;
					}
				}
			}
		}
	}
}