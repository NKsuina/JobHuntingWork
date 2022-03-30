//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "fade.h"

//=============================================================================
// �X�^�e�B�b�N�ϐ�
//=============================================================================
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffFade = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static FADE s_fade;											//�t�F�[�h�̏��
static MODE s_modeNext;										//���̉��(���[�h)
static D3DXCOLOR s_colorFade;								//�|���S��(�t�F�[�h)�̐F

//=============================================================================
// ����������
//=============================================================================
void InitFade(MODE modeNext)
{
	s_fade = FADE_IN;									//�t�F�[�h�C����Ԃ�
	s_modeNext = modeNext;								//���̉��(���[�h)��ݒ�
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//�����|���S��(�s����)�ɂ��Ă���

	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffFade,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	s_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(255, 255, 255, 255);
	pVtx[1].col = D3DXCOLOR(255, 255, 255, 255);
	pVtx[2].col = D3DXCOLOR(255, 255, 255, 255);
	pVtx[3].col = D3DXCOLOR(255, 255, 255, 255);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffFade->Unlock();

	//���[�h�̐ݒ�
	SetMode(s_modeNext);
}

//=============================================================================
// �I������
//=============================================================================
void UninitFade(void)
{
	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffFade != NULL)
	{
		s_pVtxBuffFade->Release();
		s_pVtxBuffFade = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFade(void)
{
	VERTEX_2D*pVtx;					//���_���ւ̃|�C���^
	s_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	if (s_fade != FADE_NONE)
	{
		if (s_fade == FADE_IN)
		{ //�t�F�[�h�C�����
			s_colorFade.a -= 0.03f;	//�|���S���𓧖��ɂ��Ă���
			if (s_colorFade.a <= 0.0f)
			{
				s_colorFade.a = 0.0f;
				s_fade = FADE_NONE;		//�������Ă��Ȃ���Ԃ�
			}
		}

		else if (s_fade == FADE_OUT)
		{ //�t�F�[�h�A�E�g���
			s_colorFade.a += 0.03f;	//�|���S����s�����ɂ��Ă���
			if (s_colorFade.a >= 1.0f)
			{
				s_colorFade.a = 1.0f;
				s_fade = FADE_IN;		//�t�F�[�h�C����Ԃ�

				//���[�h�ݒ�(���̉�ʂɈڍs)
				SetMode(s_modeNext);
			}
		}

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[�̐ݒ�
		pVtx[0].col = s_colorFade;
		pVtx[1].col = s_colorFade;
		pVtx[2].col = s_colorFade;
		pVtx[3].col = s_colorFade;

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuffFade->Unlock();

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��       
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�|���S���̌`
		0,						//���_�̊J�n�ꏊ
		2);						//�v���~�e�B�u�̐�
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void SetFade(MODE modeNext)
{
	s_fade = FADE_OUT;										//�t�F�[�h�A�E�g��Ԃ�
	s_modeNext = modeNext;									//���̉��(���[�h)��ݒ�
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//�����|���S��(����)�ɂ��Ă���
}

//=============================================================================
// ���̎擾
//=============================================================================
FADE GetFade(void)
{
	return s_fade;
}