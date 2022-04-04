//=============================================================================
//
// ���C������ [mein.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _MAIN_H_			//���̃}�N����`������Ȃ�������
#define _MAIN_H_			//��d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------

#include<windows.h>
#include"d3dx9.h"											//�`��ɕK�v
#define	 DIRECTINPUT_VERSION (0x0800)						//�r���h���̌x���΍��p�}�N��(���̈ʒu���瓮�����Ȃ�)
#include "dinput.h"											//���͏����ɕK�v
#include "xaudio2.h"										//�T�E���h�����ɕK�v
#include "Xinput.h"											//�W���C�p�b�g����
#include <time.h>

//-----------------------------------------------------------------------------
//���C�u�����̃����N
//-----------------------------------------------------------------------------

#pragma comment(lib,"d3d9.lib")			//�`��ɕK�v
#pragma comment(lib,"d3dx9.lib")		//d3d9.lib�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")		//DirectX�R���|�[�l���g�Ɏg�p
#pragma comment(lib,"winmm.lib")		//�V�X�e�������擾�ɕK�v


//-----------------------------------------------------------------------------
//�}�N����`x1280y720
//-----------------------------------------------------------------------------

#define SCREEN_WIDTH			(1280)												//�E�C���h�E�̕�
#define SCREEN_HEIGHT			(720)												//�E�C���h�E�̍���
#define SCREEN_STAGE_WIDTH		(640)												//�Q�[���X�e�[�W�̕�5
#define SCREEN_STAGE_HEIGHT		(500)												//�Q�[���X�e�[�W�̍���
#define SCREEN_STAGE_DEPTH		(360)												//�Q�[���X�e�[�W�̉��s��
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				//���W�A�J���[ �A�e�N�X�`��
#define FVF_VERTEX_LINE	(D3DFVF_XYZ | D3DFVF_DIFFUSE)								//���W�A�J���[
#define CLASS_NAME			"WindowClass"											//�E�C���h�E�N���X��
#define WINDOW_NAWE			"3DA"													//�E�C���h�E�̖��O�i�L���v�V�����\���j
#define SELECT_GAME_MODE (6) //�Q�[�����[�h�̐�
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE  | D3DFVF_TEX1)	//���W�A�@���A�J���[ �A�e�N�X�`��

//-----------------------------------------------------------------------------
//�\���̂̒�`
//-----------------------------------------------------------------------------

// ���_���[���C��]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	D3DCOLOR col;			//���_�J���[
}VERTEX_LINE;

// ���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	float rhw;				//���W�ϊ��p�W���i1.0f�ŌŒ�j
	D3DCOLOR col;			//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W

}VERTEX_2D;

// ���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 nor;		//�@���x�N�g��
	D3DCOLOR col;			//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
}VERTEX_3D;

//���(���[�h)�̎��
typedef enum
{
	MODE_TITLE = 0,		//�^�C�g�����
	MODE_TUTORIAL,		//�`���[�g���A�����
	MODE_GAME,			//�Q�[�����
	MODE_RESULT,		//���U���g���
	MODE_RANKING,		//�����L���O���
	MODE_MAX
}MODE;

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------

LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif
