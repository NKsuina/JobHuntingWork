//-----------------------------------------------------------------------------------------------------------
//
//�S�[��	
//Author;takano�@minto
//
//-----------------------------------------------------------------------------------------------------------
#ifndef _GOAL_H_													//���̃}�N����`���Ȃ���Ȃ�������
#define _GOAL_H_													//��d�C���N���[�h�h�~�̃}�N����`

#include"main.h"

//-----------------
//�}�N����`
//-----------------
#define MAX_TEXTURE		(5)						//�g�p����e�N�X�`���̐�

//-----------------
//�S�[���\����
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
	bool bUse;										//�g�p���Ă��邩
}GOAL;

//-----------------
//�v���g�^�C�v�錾
//-----------------
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void SetGoal(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void CollitionGoal(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size);
GOAL *GetGoal(void);

#endif // !GOAL_H_

