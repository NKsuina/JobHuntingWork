//=============================================================================
//
// �G���� [enemy.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _ENEMY_H_
#define	_ENEMY_H_

#include "main.h"

//�}�N����`
#define MAX_ENEMY		(128)		//�G�̍ő吔
#define ENEMY_HEIGHT	(21.0f)		//�G��Y�T�C�Y
#define ENEMY_WIDTH		(21.0f)		//�G��X�T�C�Y

//�G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3	move;	//�ړ���
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posOld;		//�ߋ��̃v���C���[�ʒu
	int nTime;			//�J�E���g�^�C�}�[
	int nType;			//���
	bool bIsCollide;
	bool bIsJumping;		//�W�����v���Ă��邩�ǂ���
	bool bUse;			//�g�p���Ă��邩�ǂ���
}Enemy;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
Enemy *GetEnemy(void);

#endif