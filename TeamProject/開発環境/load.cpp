//=============================================================================
//
// �O���t�@�C���ǂݍ��ݏ��� [losd.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "load.h"
#include "player.h"
#include "calculation.h"
#include "camera.h"
#include "polygon.h"

#include <assert.h>

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------

#define LINE_MAX_READING_LENGTH (256)			//��s�̍ő�ǂݎ�蕶����

int g_LoadMode = 0;

//-----------------------------------------------------------------------------
//�v���C���[�̃Z�b�g���[�h
//-----------------------------------------------------------------------------

HRESULT LoadSetPlayer()
{
	FILE *pFile = NULL;			//�t�@�C���|�C���^�[�錾

	char cBff[LINE_MAX_READING_LENGTH];		//��s���ǂݎ�邽�߂̕ϐ�
	char cBffHead[LINE_MAX_READING_LENGTH];	//���̕�����ǂݎ�邽�߂̕ϐ�
	Player aPlayer;		//���f���\���̂̕ۑ��p�ϐ��錾
	int nMotionset = 0;//���[�V�����̔ԍ��J�E���g

	//�v���C���[�̍\���̂̏�����
	ZeroMemory(&aPlayer, sizeof(Player));

	//�t�@�C�����J��
	pFile = fopen("data//TET//motion.txt", "r");

	if (pFile == NULL)
	{//�J���Ȃ��������p
		assert(false);
//		MessageBox(hWnd, "�t�@�C�����J���܂���ł���", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	//������̓ǂݎ�胋�[�v����
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
	{
		//������̕���
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//X�t�@�C���̑��΃p�X�p

			char sPath[LINE_MAX_READING_LENGTH];		//���΃p�X�ۑ��p

			//��s�̕����񂩂瑊�΃p�X�̓ǂݎ��
			sscanf(cBff, "%s = %s", &cBffHead, &sPath[0]);

			//X�t�@�C���̓ǂݍ���
			LoadPlayerPattern(&sPath[0]);
		}
		else if (strcmp(&cBffHead[0], "CHARACTERSET") == 0)
		{//�v���C���[�̔z�u�p

			//���f���Z�b�g�ɕK�v�ȏ��ǂݎ�胋�[�v����
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//������̕���
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "NUM_PARTS") == 0)
				{//�p�[�c��
					//������̕���
					sscanf(cBff, "%s = %d", &cBffHead, &aPlayer.nNumParts);
				}
				else if (strcmp(&cBffHead[0], "MOVE") == 0)
				{//�ړ���
					//������̕���
					sscanf(cBff, "%s = %f", &cBffHead, &aPlayer.fMove);
				}
				else if (strcmp(&cBffHead[0], "JUMP") == 0)
				{//�W�����v��
				 //������̕���
					sscanf(cBff, "%s = %f", &cBffHead, &aPlayer.fJump);
				}
				else if (strcmp(&cBffHead[0], "PARTSSET") == 0)
				{//���f���ЂƂ̃Z�b�g�p�̃f�[�^�ǂݎ��
					//�C���f�b�N�X�ۑ��p
					int nIndex = 0;
					//���f���Z�b�g�ɕK�v�ȏ��ǂݎ�胋�[�v����
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//������̕���
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "INDEX") == 0)
						{//�g�p���郂�f���̔ԍ�
							//������̕���
							sscanf(cBff, "%s = %d", &cBffHead, &nIndex);
							aPlayer.Parts[nIndex].nIndex = nIndex;
							aPlayer.Parts[nIndex].bUse = true;
						}
						else if (strcmp(&cBffHead[0], "PARENT") == 0)
						{//�e�̔ԍ�
							//������̕���
							sscanf(cBff, "%s = %d", &cBffHead, &aPlayer.Parts[nIndex].nParent);
						}
						else if (strcmp(&cBffHead[0], "POS") == 0)
						{//���f���̈ʒu
							//������̕���
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &aPlayer.Parts[nIndex].pos.x, &aPlayer.Parts[nIndex].pos.y, &aPlayer.Parts[nIndex].pos.z);
						}
						else if (strcmp(&cBffHead[0], "ROT") == 0)
						{//���f���̌���
							//������̕���
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &aPlayer.Parts[nIndex].rot.x, &aPlayer.Parts[nIndex].rot.y, &aPlayer.Parts[nIndex].rot.z);
						}
						else if (strcmp(&cBffHead[0], "END_PARTSSET") == 0)
						{//���f���P�p�[�c�̃Z�b�g�I��
							break;
						}

						//�ۑ����̕�����̏�����
						ZeroMemory(&cBff, sizeof(cBff));
						ZeroMemory(&cBffHead, sizeof(cBffHead));

					}
				}
				else if (strcmp(&cBffHead[0], "END_CHARACTERSET") == 0)
				{//�v���C���[�̃f�[�^�ǂݎ��I��
					break;
				}

				//�ۑ����̕�����̏�����
				ZeroMemory(&cBff, sizeof(cBff));
				ZeroMemory(&cBffHead, sizeof(cBffHead));

			}
		}
		else if (strcmp(&cBffHead[0], "MOTIONSET") == 0)
		{//���[�V�����p�̃f�[�^�ǂݎ��
			int nSetMotionEnd = 0;//�g�����z��̐��̃J�E���g
			int nMotionKey = 0;//�g�����L�[�̐��̃J�E���g
			//���f���Z�b�g�ɕK�v�ȏ��ǂݎ�胋�[�v����
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//������̕���
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "LOOP") == 0)
				{//���̃��[�V�����̓��[�v�Đ����邩�ǂ���
				 //������̕���
					sscanf(cBff, "%s = %d", &cBffHead, &aPlayer.Motion[nMotionset].nLoop);
				}
				else if (strcmp(&cBffHead[0], "NUM_KEY") == 0)
				{//���[�V�����̃L�[��
				 //������̕���
					sscanf(cBff, "%s = %d", &cBffHead, &aPlayer.Motion[nMotionset].nNumKey);
				}
				else if (strcmp(&cBffHead[0], "KEYSET") == 0)
				{//���f���p�[�c�̃��[�V�����ݒ�
					int nModelPasCnt = 0;		//���f���p�[�c�̃Z�b�g�ԍ�
					//���f���Z�b�g�ɕK�v�ȏ��ǂݎ�胋�[�v����
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//������̕���
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "FRAME") == 0)
						{//���̃��[�V�����͉��t���[�����ōĐ����邩�`�F�b�N
							//������̕���
							sscanf(cBff, "%s = %d", &cBffHead, &aPlayer.Motion[nMotionset].nFrame[nMotionKey]);
							if (aPlayer.Motion[nMotionset].nFrame[nMotionKey] == 0)
							{//�Đ��t���[������0�̏ꍇ1�ɂ���
								aPlayer.Motion[nMotionset].nFrame[nMotionKey] = 1;
							}
						}
						else if (strcmp(&cBffHead[0], "KEY") == 0)
						{//���[�V�����̂P�p�[�c�̃Z�b�g
							//���f���Z�b�g�ɕK�v�ȏ��ǂݎ�胋�[�v����
							while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
							{
								//������̕���
								sscanf(cBff, "%s", &cBffHead);

								if (strcmp(&cBffHead[0], "POS") == 0)
								{//���[�V�������̂P�p�[�c�̈ʒu
									//������̕���
									sscanf(cBff, "%s = %f%f%f", &cBffHead, 
										&aPlayer.Parts[nModelPasCnt].posMotionset[nMotionset][nMotionKey].x,
										&aPlayer.Parts[nModelPasCnt].posMotionset[nMotionset][nMotionKey].y,
										&aPlayer.Parts[nModelPasCnt].posMotionset[nMotionset][nMotionKey].z);
								}
								else if (strcmp(&cBffHead[0], "ROT") == 0)
								{//���[�V�������̂P�p�[�c�̌���
								 //������̕���
									sscanf(cBff, "%s = %f%f%f", &cBffHead,
										&aPlayer.Parts[nModelPasCnt].rotMotionset[nMotionset][nMotionKey].x,
										&aPlayer.Parts[nModelPasCnt].rotMotionset[nMotionset][nMotionKey].y,
										&aPlayer.Parts[nModelPasCnt].rotMotionset[nMotionset][nMotionKey].z);

									if (nMotionset == 3 && nModelPasCnt == 2)
									{
										int nData = 0;
									}
								}
								else if (strcmp(&cBffHead[0], "END_KEY") == 0)
								{//���[�V�������̂P�p�[�c�̃Z�b�g�I��
									nModelPasCnt++;		//�ݒ芮�������p�[�c�̔ԍ���i�߂�
									break;
								}

								//�ۑ����̕�����̏�����
								ZeroMemory(&cBff, sizeof(cBff));
								ZeroMemory(&cBffHead, sizeof(cBffHead));

							}
						}
						else if (strcmp(&cBffHead[0], "END_KEYSET") == 0)
						{
							nSetMotionEnd += aPlayer.Motion[nMotionset].nNumKey;
							nMotionKey++;
							break;
						}

						//�ۑ����̕�����̏�����
						ZeroMemory(&cBff, sizeof(cBff));
						ZeroMemory(&cBffHead, sizeof(cBffHead));

					}
				}
				else if (strcmp(&cBffHead[0], "END_MOTIONSET") == 0)
				{//���f���p�[�c�̃��[�V�����ݒ�I��
					nMotionset++;//�Z�b�g�������[�V�����̔ԍ���i�߂�
					break;
				}

				//�ۑ����̕�����̏�����
				ZeroMemory(&cBff, sizeof(cBff));
				ZeroMemory(&cBffHead, sizeof(cBffHead));

			}
		}
		else if (strcmp(&cBffHead[0], "END_SCRIPT") == 0)
		{//�e�L�X�g�t�@�C����ǂݐ؂�����
			break;
		}

		//�ۑ����̕�����̏�����
		ZeroMemory(&cBff, sizeof(cBff));
		ZeroMemory(&cBffHead, sizeof(cBffHead));

	}



	//�Z�b�g�v���C���[
	SetPlayer(aPlayer);

	//�t�@�C�������
	fclose(pFile);

	return S_OK;
}