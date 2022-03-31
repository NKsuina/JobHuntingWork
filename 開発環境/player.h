#ifndef _POLY_H_			
#define _POLY_H_			

#include "main.h"

#define PLAYER_WIDTH (30.0f)	//�v���C���[��Y�T�C�Y
#define PLAYER_HEIGHT (30.0f)	//�v���C���[��X�T�C�Y

typedef enum
{
	PLAYER_STAY = 0,		//�~�܂��Ă��鎞
	PLAYER_RUN,				//�����Ă���Ƃ�
	PLAYER_JUMP,			//��񂾎�
	PLAYER_ATTACK,			//�U�������Ƃ�
	PLAYER_DETH				//���񂾂Ƃ�
}E_PLAYER;

typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�ߋ��̃v���C���[�ʒu
	D3DXVECTOR3 move;		//�ړ���
	int nLife;				//�̗�
	int nCounterAnim;		//�J�E���^�[
	int nPatternAnim;		//�p�^�[���ԍ�
	int nDirectionMove;		//����
	int nCounterState;
	bool bUse;
	bool bDisp;				//�\�����邩���Ȃ���
	bool bIsJumping;		//�W�����v��
	E_PLAYER state;
}Player;		

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif
