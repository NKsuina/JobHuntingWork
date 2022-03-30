//-----------------------------------------------------------------------------------------------------------
//
//�S�[��	
//Author;takano�@minto
//
//-----------------------------------------------------------------------------------------------------------
#ifndef _WALLMODEL_H_													//���̃}�N����`���Ȃ���Ȃ�������
#define _WALLMODEL_H_													//��d�C���N���[�h�h�~�̃}�N����`

#include"main.h"

//-----------------
//�}�N����`
//-----------------
#define MAX_TEXTURE		(5)						//�g�p����e�N�X�`���̐�

//-----------------
//�Ǎ\����
//-----------------
typedef struct
{
	D3DXVECTOR3 pos;								//���_
	D3DXVECTOR3 rot;								//����
	D3DXVECTOR3 rotDest;							//�p�x�̈���
	D3DXMATRIX mtxWorld;							//���[���h�}�g���b�N�X
	D3DXVECTOR3 Min;								//���f���̍ŏ��l
	D3DXVECTOR3 Max;								//���f���̍ő�l
	DWORD nNumMat;									//�}�e���A�����̐�
	LPD3DXMESH pMesh;								//���b�V�����
	LPD3DXBUFFER pBuffMat;							//�}�e���A�����
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEXTURE];		//�e�N�X�`�����
	D3DXVECTOR3 size;								//���f���T�C�Y
	int nCntWall;
	bool bVibration;
	bool bUse;										//�g�p���Ă��邩
}WALLMODEL;

//-----------------
//�v���g�^�C�v�錾
//-----------------
void InitWallModel(void);
void UninitWallModel(void);
void UpdateWallModel(void);
void DrawWallModel(void);
void SetWallModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void CollitionWallModel(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size);
WALLMODEL *GetWallModel(void);
void SetVibration(int nNum);

#endif // !WALLMODEL_H_