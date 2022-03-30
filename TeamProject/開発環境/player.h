//=============================================================================
//
// ���� []
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _PLAYER_H_			//���̃}�N����`������Ȃ�������
#define _PLAYER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------

#include "main.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------

#define PARENT_MAX			(30)		//�v���C���[�̃p�[�c�̍ő��
#define MOTION_KEY_MAX		(10)		//���[�V�����̃L�[�̍ő��

//-----------------------------------------------------------------------------
//�񋓌^�̒�`
//-----------------------------------------------------------------------------

//�v���C���[�̏��
typedef enum
{
	PLAYER_NEUTRAL = 0,
	PLAYER_RUN,
	PLAYER_HAVE,
	PLAYER_SLIDING,
	PLAYER_MAX
}PlayerState;

//-----------------------------------------------------------------------------
//�\����
//-----------------------------------------------------------------------------

//���f���p�[�c�\����
typedef struct
{
	D3DXVECTOR3	pos;					//�ʒu
	D3DXVECTOR3	rot;					//����
	D3DXVECTOR3 posMotionset[MOTION_KEY_MAX][MOTION_KEY_MAX];		//���[�V�������̈ʒu
	D3DXVECTOR3 rotMotionset[MOTION_KEY_MAX][MOTION_KEY_MAX];		//���[�V�������̌���
	int nIndex;							//���̃p�[�c�̔ԍ�
	int nParent;						//�e�p�[�c�̔ԍ�
	bool bUse;							//�g�p���Ă��邩�ǂ���
}PlayerParts;

//���[�V�������\��
typedef struct
{
	int nLoop;
	int nNumKey;
	int nFrame[MOTION_KEY_MAX];
	bool bUse;
}MotionSet;

//�v���C���[�\����
typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 posOld;					//�ߋ��̈ʒu
	D3DXVECTOR3 posHand;				//�ʒu(�E��)
	D3DXVECTOR3 rotHand;				//����(�E��)
	D3DXVECTOR3 move;					//�ړ���(���t���[���ϓ�)
	D3DXVECTOR3 rot;					//����
	D3DXVECTOR3 rotDest;				//�ړI�̊p�x
	PlayerParts Parts[PARENT_MAX];		//���f���p�[�c
	MotionSet Motion[MOTION_KEY_MAX];	//���[�V�������
	int nShadow;						//�e�̔ԍ�
	int nNumParts;						//�p�[�c��
	float fMove;						//�ړ���(�s��)
	float fJump;						//�W�����v��
	D3DXVECTOR3 vtxMax;					//���̂̍ő�
	D3DXVECTOR3 vtxMin;					//���̂̍ŏ�
	PlayerState PlayerState;			//�v���C���[�̏��
	int nNumDisc;						//�������Ă�f�B�X�N�̔ԍ�
	bool bDiscHave;						//�f�B�X�N�������Ă邩�ǂ���
	bool bUes;							//�g�p���Ă��邩�ǂ���
}Player;


//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------

void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
Player GetPlayerData(void);
HRESULT LoadPlayerPattern(char *sXFilePath);
void SetPlayer(Player Player);
D3DXVECTOR3 GetPosPlayer(int nPlayerNum);
#endif