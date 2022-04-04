//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "game.h"
#include "input.h"
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "pause.h"
#include "result.h"
#include "player.h"
#include "disc.h"
#include "load.h"
#include "goal.h"

#include "wallmodel.h"
#include <time.h>
#include "billboard.h"
#include "particle.h"
#include "score2.h"
#include "score1.h"

#include "countdown.h"
#include "2dpolyron.h"
#include "sound.h"



//=============================================================================
// �X�^�e�B�b�N�ϐ�
//=============================================================================
static bool g_bPause = false;		//�|�[�Y�����ǂ���

//=============================================================================
// ����������
//=============================================================================
void InitGame(void)
{
	PlaySound(SOUND_LABEL_BGM001);
	//�����̏�����
	srand((unsigned int)time(0));

	Init2DPolygon();
	Initcountdown();

	//�v���C���[������
	InitPlayer();

	//���[�h�����i�v���C���[�j
	LoadSetPlayer();

	//�|���S���̏���������
	InitPolygon();

	//�r���{�[�h�̏���������
	InitBillboard();

	//�X�R�A1�̏�����
	InitScore();

	//�X�R�A2�̏�����
	InitScore2();

	//�S�[���̏���������
	InitGoal();

	//�ǃ��f���̏���������
	InitWallModel();

	//�f�B�X�N�̏���������
	InitDisc();

	//�f�B�X�N�̈ʒu�������I�ɕς���
	//SetDiscPos(0, D3DXVECTOR3(500.0f, 0.0f, 0.0f));

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�J�����̏���������
	InitCamera();

	//�r���{�[�h�t�@�C���̓ǂݍ��ݏ���
	InputBillboard();

	//���C�g�̏���������
	InitLight();

	//���U���g�̏���������
	InitResult();

	//�|�[�Y��ʂ̏���������
	InitPause();		

	//�|�[�Y����
	g_bPause = false;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	Uninitcountdown();

	//�|���S���̏I������
	UninitPolygon();

	Uninit2DPolygon();
	//�X�R�A1�̏I������
	UninitScore();

	//�X�R�A2�̏I������
	UninitScore2();

	//�r���{�[�h�̏I������
	UninitBillboard();

	//�f�B�X�N�̏I������
	UninitDisc();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�ǃ��f���̏I������
	UninitWallModel();

	//�S�[���̏I������
	UninitGoal();

	//�v���C���[�I������
	UninitPlayer();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//���U���g�̏I������
	UninitResult();

	//�|�[�Y��ʂ̏I������
	UninitPause();		
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	//�|�[�Y���̎擾
	PAUSE *pPause = GetPause();

	if ((GetKeyboardTrigger(DIK_P) || GetJoypadAllTrigger(JOYKEY_START)) && g_bPause == false)
	{//�|�[�Y�L�[(P�L�[)�������ꂽ
		g_bPause = g_bPause ? false : true;		//true�Ȃ�false,false�Ȃ�true �O�����Z�q
	}

	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y�̍X�V����
		UpdatePause();
	}

	if (g_bPause == false /*&& nTimer != 0*/)
	{//�|�[�Y���łȂ����

		Updatecountdown();
		Update2DPolygon();
		//�|���S���̍X�V����
		UpdatePolygon();

		//�X�R�A1�̍X�V����
		UpdateScore();

		//�X�R�A�Q�̍X�V����
		UpdateScore2();

		//�f�B�X�N�̍X�V����
		UpdateDisc();

		//�p�[�e�B�N���̍X�V����
		UpdateParticle();

		//�ǃ��f���̍X�V����
		UpdateWallModel();

		//�S�[���̍X�V����
		UpdateGoal();

		//�v���C���[�̍X�V����
		UpdatePlayer();

		//�r���{�[�h�̍X�V����
		UpdatePlayer();

		//�J�����̍X�V����
		UpdateCamera();

		//���C�g�̍X�V����
		UpdateLight();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{

	//�J�����̐ݒ菈��
	SetCamera();

	//�|���S���̕`�揈��
	DrawPolygon();

	//�f�B�X�N�̕`�揈��
	DrawDisc();

	//�r���{�[�h�̕`�揈��
	DrawBillboard();

	//�X�R�A1�̕`�揈��
	DrawScore();

	//�X�R�A2�̕`�揈��
	DrawScore2();

	//�ǃ��f���̕`�揈��
	DrawWallModel();

	Drawcountdown();

	//�S�[���̕`�揈��
	DrawGoal();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	Draw2DPolygon();

	//�p�[�e�B�N���̕`�揈��
	DrawParticle();

	//���U���g�̕`�揈��
	DrawResult();

	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y�̕`�揈��
		DrawPause();
	}
}

//=============================================================================
// �|�[�Y�̗L�������ݒ�
//=============================================================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}