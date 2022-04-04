//=============================================================================
//
// �Q�[�������܂Ƃ� [game.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================

#include "Effect.h"
#include "camera.h"
#include "light.h"
#include "main.h"
#include "loadfile.h"
#include "meshfield.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "score.h"
#include "sound.h"
#include "time.h"
#include "wall.h"

//�����������֘A
void InitGame(void)
{
	//�T�E���h�̎擾
	PlaySound(SOUND_LABEL_BGM001);

	//�e�̏�����
	InitShadow();

	//���f���̏�����
	InitModel();

	//���b�V���t�B�[���h�̏�����
	InitMeshfield();

	//�v���C���[�̏�����
	InitPlayer();

	//���C�g�̏�����
	InitLight();

	//�J�����̏�����
	InitCamera();

	//�ǂ̏�����
	InitWall();
	
	//�X�R�A�̏�����
	InitScore();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//�^�C���̏�����
	InitTime();

	//���f���t�@�C���̏�����
	InitLoadFile();
}

//�I�������֘A
void UninitGame(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�e�̏I������
	UninitShadow();

	//���b�V���t�B�[���h�̏I������
	UninitMeshfield();

	//���f���̏I������
	UninitModel();

	//�v���C���[�̏I������
	UninitPlayer();

	//���C�g�̏I������
	UninitLight();

	//�J�����̏I������
	UninitCamera();

	//�ǂ̏I������
	UninitWall();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�X�R�A�̏I������
	UninitScore();

	//�^�C���̏I������
	UninitTime();
}

//�X�V�����֘A
void UpdateGame(void)
{
	//���f���̍X�V����
	UpdateModel();

	//���b�V���t�B�[���h�̍X�V����
	UpdateMeshfield();

	//�ǂ̍X�V����
	UpdataWall();

	//�v���C���[�̍X�V����
	UpdatePlayer();

	//�e�̍X�V����
	UpdataShadow();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�J�����̍X�V����
	UpdateCamera();

	//���C�g�̍X�V����
	UpdateLight();

	//�X�R�A�̍X�V����
	UpdateScore();

	//�^�C���̍X�V����
	UpdateTime();
}

//�`�揈���֘A
void DrawGame(void)
{
	//�J�����̕`�揈��
	SetCamera();

	//���f���̕`�揈��
	DrawModel();

	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshfield();

	//�e�̕`�揈��
	DrawShadow();

	//���f���̕`�揈��
	DrawModel();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�ǂ̕`�揈��
	DrawWall();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�X�R�A�̕`�揈��
	DrawScore();

	//�^�C���̕`�揈��
	DrawTime();
}
