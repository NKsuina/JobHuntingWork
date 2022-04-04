//-------------------------------------
//
//�f�B�X�N�w�b�_�[[disc.h]
//Author:���c�喲
//
//-------------------------------------
#ifndef  _DISC_H_						//���̃}�N����`������ĂȂ�������
#define  _DISC_H_						//��d�C���N���[�h�h�~�̃}�N����`

//�C���N���[�h�t�@�C��
#include "main.h"

//�}�N����`
#define MAX_DISC		(1)			//�f�B�X�N�̍ő吔
#define MAX_SPEED		(1.0f)		//�ړ����x

typedef enum
{
	E_PLAYER_1 = 0,					//1P�̃f�B�X�N
	E_PLAYER_2,						//2P�̃f�B�X�N
	E_PLAYER_MAX
}PlayerHaveDisc;

typedef struct
{
	D3DXVECTOR3 pos;								//�ʒu
	D3DXVECTOR3 posOld;								//�O�̈ʒu
	D3DXVECTOR3 move;								//�ړ���
	D3DXVECTOR3 size;								//�T�C�Y
	D3DXVECTOR3 rotDisc;							//����
	D3DXVECTOR3 rotDestDisc;
	D3DXVECTOR3 vtxMinDisc;							//�ő咸�_
	D3DXVECTOR3 vtxMaxDisc;							//�ŏ����_
	bool bUse;										//�g���Ă��邩
	bool bGoal;										//�S�[���������ǂ���
	int nGoal;										//�ǂ��炪�S�[��������
	int nDiscNumber;								//�f�B�X�N�̔ԍ�
	D3DXMATRIX mtxWorldDisc;						//���[���h�}�g���b�N�X
}Disc;

//�v���g�^�C�v�錾
void InitDisc(void);					//�f�B�X�N�̏���������
void UninitDisc(void);					//�f�B�X�N�̏I������
void UpdateDisc(void);					//�f�B�X�N�̍X�V����
void DrawDisc(void);					//�f�B�X�N�̕`�揈��
void SetDisc(PlayerHaveDisc player);	//�f�B�X�N�̐ݒ菈��
void SetDiscPos(int nDiscNumber, D3DXVECTOR3 pos);//�f�B�X�N�̈ʒu��ς��鏈��

void TherowingDisc(int nRot, int nSpeed, int nNum);	//�p�x�ύX


Disc * GetDisc(void);
void BreakDisc(void);
#endif
