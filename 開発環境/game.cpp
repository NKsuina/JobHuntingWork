//=============================================================================
//
// �Q�[�������܂Ƃ� [game.cpp]
// Author : NARUMI KOUKI
//
//=============================================================================
#include "Player.h"
#include "main.h"
#include "input.h"
#include "bg.h"
#include "item.h"
#include "block.h"
#include "Enemy.h"
#include "score.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "map.h"
#include "goal.h"

void InitGame(void)
{
	//�T�E���h�̎擾
	PlaySound(SOUND_LABEL_BGM001);

	//�w�i�̏���������
	InitBG();

	//�X�R�A�̏���������
	InitScore();

	//�A�C�e���̏���������
	InitItem();

	//�G�̏���������
	InitEnemy();

	//�|���S���̏���������
	InitPlayer();

	//�S�[���̏���������
	InitGoal();

	//�u���b�N�̏���������
	InitBlock();

	InitMap();

}

void UninitGame(void)
{

	//�T�E���h�̒�~
	StopSound();

	//�w�i�̏I������
	UninitBG();

	//�X�R�A�̏I������
	UninitScore();
	
	//�A�C�e���̏I������
	UninitItem();

	//�G�̏I������
	UninitEnemy();

	//�|���S���̏I������
	UninitPlayer();

	//�S�[���̏I������
	UninitGoal();

	//�u���b�N�̏���������
	UninitBlock();
}

void UpdateGame(void)
{

	//�w�i�̍X�V����
	UpdateBG();

	//�X�R�A�̍X�V����
	UpdateScore();

	//�A�C�e���̍X�V����
	UpdateItem();
	
	//�G�̍X�V����
	UpdateEnemy();

	//�|���S���̍X�V����
	UpdatePlayer();

	//�S�[���̍X�V����
	UpdateGoal();
	
	//�u���b�N�̍X�V����
	UpdateBlock();
}

void DrawGame(void)
{

	//�w�i�̕`�揈��
	DrawBG();

	//�X�R�A�̕`�揈��
	DrawScore();

	//�A�C�e���̕`�揈��
	DrawItem();

	//�G�̕`�揈��
	DrawEnemy();

	//�|���S���̕`�揈��
	DrawPlayer();

	//�S�[���̕`�揈��
	DrawGoal();

	//�u���b�N�̕`�揈��
	DrawBlock();
}
