//=============================================================================
//
// 外部ファイル読み込み処理 [losd.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "load.h"
#include "player.h"
#include "calculation.h"
#include "camera.h"
#include "polygon.h"

#include <assert.h>

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------

#define LINE_MAX_READING_LENGTH (256)			//一行の最大読み取り文字数

int g_LoadMode = 0;

//-----------------------------------------------------------------------------
//プレイヤーのセットロード
//-----------------------------------------------------------------------------

HRESULT LoadSetPlayer()
{
	FILE *pFile = NULL;			//ファイルポインター宣言

	char cBff[LINE_MAX_READING_LENGTH];		//一行分読み取るための変数
	char cBffHead[LINE_MAX_READING_LENGTH];	//頭の文字を読み取るための変数
	Player aPlayer;		//モデル構造体の保存用変数宣言
	int nMotionset = 0;//モーションの番号カウント

	//プレイヤーの構造体の初期化
	ZeroMemory(&aPlayer, sizeof(Player));

	//ファイルを開く
	pFile = fopen("data//TET//motion.txt", "r");

	if (pFile == NULL)
	{//開けなかった時用
		assert(false);
//		MessageBox(hWnd, "ファイルを開けませんでした", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	//文字列の読み取りループ処理
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
	{
		//文字列の分析
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//Xファイルの相対パス用

			char sPath[LINE_MAX_READING_LENGTH];		//相対パス保存用

			//一行の文字列から相対パスの読み取り
			sscanf(cBff, "%s = %s", &cBffHead, &sPath[0]);

			//Xファイルの読み込み
			LoadPlayerPattern(&sPath[0]);
		}
		else if (strcmp(&cBffHead[0], "CHARACTERSET") == 0)
		{//プレイヤーの配置用

			//モデルセットに必要な情報読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "NUM_PARTS") == 0)
				{//パーツ数
					//文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead, &aPlayer.nNumParts);
				}
				else if (strcmp(&cBffHead[0], "MOVE") == 0)
				{//移動量
					//文字列の分析
					sscanf(cBff, "%s = %f", &cBffHead, &aPlayer.fMove);
				}
				else if (strcmp(&cBffHead[0], "JUMP") == 0)
				{//ジャンプ量
				 //文字列の分析
					sscanf(cBff, "%s = %f", &cBffHead, &aPlayer.fJump);
				}
				else if (strcmp(&cBffHead[0], "PARTSSET") == 0)
				{//モデルひとつのセット用のデータ読み取り
					//インデックス保存用
					int nIndex = 0;
					//モデルセットに必要な情報読み取りループ処理
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//文字列の分析
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "INDEX") == 0)
						{//使用するモデルの番号
							//文字列の分析
							sscanf(cBff, "%s = %d", &cBffHead, &nIndex);
							aPlayer.Parts[nIndex].nIndex = nIndex;
							aPlayer.Parts[nIndex].bUse = true;
						}
						else if (strcmp(&cBffHead[0], "PARENT") == 0)
						{//親の番号
							//文字列の分析
							sscanf(cBff, "%s = %d", &cBffHead, &aPlayer.Parts[nIndex].nParent);
						}
						else if (strcmp(&cBffHead[0], "POS") == 0)
						{//モデルの位置
							//文字列の分析
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &aPlayer.Parts[nIndex].pos.x, &aPlayer.Parts[nIndex].pos.y, &aPlayer.Parts[nIndex].pos.z);
						}
						else if (strcmp(&cBffHead[0], "ROT") == 0)
						{//モデルの向き
							//文字列の分析
							sscanf(cBff, "%s = %f%f%f", &cBffHead, &aPlayer.Parts[nIndex].rot.x, &aPlayer.Parts[nIndex].rot.y, &aPlayer.Parts[nIndex].rot.z);
						}
						else if (strcmp(&cBffHead[0], "END_PARTSSET") == 0)
						{//モデル１パーツのセット終了
							break;
						}

						//保存中の文字列の初期化
						ZeroMemory(&cBff, sizeof(cBff));
						ZeroMemory(&cBffHead, sizeof(cBffHead));

					}
				}
				else if (strcmp(&cBffHead[0], "END_CHARACTERSET") == 0)
				{//プレイヤーのデータ読み取り終了
					break;
				}

				//保存中の文字列の初期化
				ZeroMemory(&cBff, sizeof(cBff));
				ZeroMemory(&cBffHead, sizeof(cBffHead));

			}
		}
		else if (strcmp(&cBffHead[0], "MOTIONSET") == 0)
		{//モーション用のデータ読み取り
			int nSetMotionEnd = 0;//使った配列の数のカウント
			int nMotionKey = 0;//使ったキーの数のカウント
			//モデルセットに必要な情報読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
			{
				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);

				if (strcmp(&cBffHead[0], "LOOP") == 0)
				{//このモーションはループ再生するかどうか
				 //文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead, &aPlayer.Motion[nMotionset].nLoop);
				}
				else if (strcmp(&cBffHead[0], "NUM_KEY") == 0)
				{//モーションのキー数
				 //文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead, &aPlayer.Motion[nMotionset].nNumKey);
				}
				else if (strcmp(&cBffHead[0], "KEYSET") == 0)
				{//モデルパーツのモーション設定
					int nModelPasCnt = 0;		//モデルパーツのセット番号
					//モデルセットに必要な情報読み取りループ処理
					while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
					{
						//文字列の分析
						sscanf(cBff, "%s", &cBffHead);

						if (strcmp(&cBffHead[0], "FRAME") == 0)
						{//このモーションは何フレーム数で再生するかチェック
							//文字列の分析
							sscanf(cBff, "%s = %d", &cBffHead, &aPlayer.Motion[nMotionset].nFrame[nMotionKey]);
							if (aPlayer.Motion[nMotionset].nFrame[nMotionKey] == 0)
							{//再生フレーム数が0の場合1にする
								aPlayer.Motion[nMotionset].nFrame[nMotionKey] = 1;
							}
						}
						else if (strcmp(&cBffHead[0], "KEY") == 0)
						{//モーションの１パーツのセット
							//モデルセットに必要な情報読み取りループ処理
							while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != NULL)
							{
								//文字列の分析
								sscanf(cBff, "%s", &cBffHead);

								if (strcmp(&cBffHead[0], "POS") == 0)
								{//モーション中の１パーツの位置
									//文字列の分析
									sscanf(cBff, "%s = %f%f%f", &cBffHead, 
										&aPlayer.Parts[nModelPasCnt].posMotionset[nMotionset][nMotionKey].x,
										&aPlayer.Parts[nModelPasCnt].posMotionset[nMotionset][nMotionKey].y,
										&aPlayer.Parts[nModelPasCnt].posMotionset[nMotionset][nMotionKey].z);
								}
								else if (strcmp(&cBffHead[0], "ROT") == 0)
								{//モーション中の１パーツの向き
								 //文字列の分析
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
								{//モーション中の１パーツのセット終了
									nModelPasCnt++;		//設定完了したパーツの番号を進める
									break;
								}

								//保存中の文字列の初期化
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

						//保存中の文字列の初期化
						ZeroMemory(&cBff, sizeof(cBff));
						ZeroMemory(&cBffHead, sizeof(cBffHead));

					}
				}
				else if (strcmp(&cBffHead[0], "END_MOTIONSET") == 0)
				{//モデルパーツのモーション設定終了
					nMotionset++;//セット完了モーションの番号を進める
					break;
				}

				//保存中の文字列の初期化
				ZeroMemory(&cBff, sizeof(cBff));
				ZeroMemory(&cBffHead, sizeof(cBffHead));

			}
		}
		else if (strcmp(&cBffHead[0], "END_SCRIPT") == 0)
		{//テキストファイルを読み切った時
			break;
		}

		//保存中の文字列の初期化
		ZeroMemory(&cBff, sizeof(cBff));
		ZeroMemory(&cBffHead, sizeof(cBffHead));

	}



	//セットプレイヤー
	SetPlayer(aPlayer);

	//ファイルを閉じる
	fclose(pFile);

	return S_OK;
}