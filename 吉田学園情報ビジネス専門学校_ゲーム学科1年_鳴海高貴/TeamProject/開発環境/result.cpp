//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "result.h"
#include "particle.h"
#include "score1.h"
#include "score2.h"
#include "fade.h"
#include "sound.h"
#include "disc.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TEXTURE	(14)		//�g�p����e�N�X�`���̖���

//=============================================================================
// �X�^�e�B�b�N�ϐ�
//=============================================================================
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_TEXTURE] = {NULL};		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^
static bool bUseResult;											//���U���g���g�p���Ă邩
static bool bEnterSwitch;										//��ʑJ�ڂ��邽�߂̃G���^�[�X�C�b�`
static int NonEnter;											//�����G��Ȃ������珟��ɉ�ʑJ�ڂ��J�n����p�̕ϐ�
static RESULT s_Result[MAX_TEXTURE];							//�\����
static bool bResult;

//=============================================================================
// ����������
//=============================================================================
void InitResult(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���ւ̃|�C���^
	VERTEX_2D * pVtx;

	//�ϐ��̏�����
	bUseResult = false;
	bEnterSwitch = false;
	NonEnter = 0;
	bResult = false;

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/����.png",
		&s_pTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/�s��.png",
		&s_pTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/win.png",
		&s_pTexture[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/lose.png",
		&s_pTexture[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ZeroPoint.png",
		&s_pTexture[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/OnePoint.png",
		&s_pTexture[5]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/TwoPoint.png",
		&s_pTexture[6]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SETCOUNT.png",
		&s_pTexture[7]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ThreePoint.png",
		&s_pTexture[8]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/����1P.png",
		&s_pTexture[9]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/����2P.png",
		&s_pTexture[10]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/�s��1P.png",
		&s_pTexture[11]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/�s��2P.png",
		&s_pTexture[12]);

	//�\���̂̏�����
	s_Result[0].pos = D3DXVECTOR3(-200.0f, 350.0f, 0.0f);
	s_Result[0].move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	s_Result[1].pos = D3DXVECTOR3(1480.0f, 350.0f, 0.0f);
	s_Result[1].move = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
	s_Result[2].pos = D3DXVECTOR3(-200.0f, 150.0f, 0.0f);
	s_Result[2].move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	s_Result[3].pos = D3DXVECTOR3(1480.0f, 150.0f, 0.0f);
	s_Result[3].move = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����̏�����
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�f�[�^��4�i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		//�e�N�X�`���̔j��
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}

		//���_�o�b�t�@�̔j��
		if (s_pVtxBuff != NULL)
		{
			s_pVtxBuff->Release();
			s_pVtxBuff = NULL;
		}
	}
	//�T�E���h��~
	StopSound();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	//1P�̃X�R�A�̎擾
	int OneScore = GetScore();

	//2P�̃X�R�A�̎擾
	int TwoScore = GetScore2();

	//���U���g�̃Z�b�g����
	SetResult(OneScore, TwoScore);
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture[i]);

		//�|���S���̕`�揈��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�|���S���̌`
			4 * i,						//���_�̊J�n�ꏊ
			2);							//�v���~�e�B�u�̐�
	}
}

//=============================================================================
// ���U���g�Z�b�g����
//=============================================================================
void SetResult(int nSetCount1, int nSetCount2)
{
	
	if (!bResult)
	{
		//�T�E���h��~
		StopSound();
		PlaySound(SOUND_LABEL_BGM002);
		PlaySound(SOUND_LABEL_SE_KANSEI2);
		bResult = true;
	}

	BreakDisc();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���ւ̃|�C���^
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (bUseResult == false)
	{//�g�p���Ă��Ȃ���Ύ��s
		if (nSetCount1 > nSetCount2)
		{//1P�����������ꍇ
			//���_���W�̐ݒ�
			pVtx[36].pos = D3DXVECTOR3(s_Result[0].pos.x - 150.0f, s_Result[0].pos.y - 150.0f, 0.0f);
			pVtx[37].pos = D3DXVECTOR3(s_Result[0].pos.x + 150.0f, s_Result[0].pos.y - 150.0f, 0.0f);
			pVtx[38].pos = D3DXVECTOR3(s_Result[0].pos.x - 150.0f, s_Result[0].pos.y + 150.0f, 0.0f);
			pVtx[39].pos = D3DXVECTOR3(s_Result[0].pos.x + 150.0f, s_Result[0].pos.y + 150.0f, 0.0f);

			pVtx[48].pos = D3DXVECTOR3(s_Result[1].pos.x - 150.0f, s_Result[1].pos.y - 150.0f, 0.0f);
			pVtx[49].pos = D3DXVECTOR3(s_Result[1].pos.x + 150.0f, s_Result[1].pos.y - 150.0f, 0.0f);
			pVtx[50].pos = D3DXVECTOR3(s_Result[1].pos.x - 150.0f, s_Result[1].pos.y + 150.0f, 0.0f);
			pVtx[51].pos = D3DXVECTOR3(s_Result[1].pos.x + 150.0f, s_Result[1].pos.y + 150.0f, 0.0f);

			pVtx[8].pos = D3DXVECTOR3(s_Result[2].pos.x - 150.0f, s_Result[2].pos.y - 50.0f, 0.0f);
			pVtx[9].pos = D3DXVECTOR3(s_Result[2].pos.x + 150.0f, s_Result[2].pos.y - 50.0f, 0.0f);
			pVtx[10].pos = D3DXVECTOR3(s_Result[2].pos.x - 150.0f, s_Result[2].pos.y + 50.0f, 0.0f);
			pVtx[11].pos = D3DXVECTOR3(s_Result[2].pos.x + 150.0f, s_Result[2].pos.y + 50.0f, 0.0f);

			pVtx[12].pos = D3DXVECTOR3(s_Result[3].pos.x - 150.0f, s_Result[3].pos.y - 50.0f, 0.0f);
			pVtx[13].pos = D3DXVECTOR3(s_Result[3].pos.x + 150.0f, s_Result[3].pos.y - 50.0f, 0.0f);
			pVtx[14].pos = D3DXVECTOR3(s_Result[3].pos.x - 150.0f, s_Result[3].pos.y + 50.0f, 0.0f);
			pVtx[15].pos = D3DXVECTOR3(s_Result[3].pos.x + 150.0f, s_Result[3].pos.y + 50.0f, 0.0f);

			//���U���g�̈ʒu�X�V
			s_Result[0].pos += s_Result[0].move;
			s_Result[1].pos += s_Result[1].move;
			s_Result[2].pos += s_Result[2].move;
			s_Result[3].pos += s_Result[3].move;

			//�ړ��ʂ�0�ɂ���
			if (s_Result[0].pos.x >= 375.0f)
			{
				s_Result[0].move.x = 0;
				s_Result[1].move.x = 0;
				s_Result[2].move.x = 0;
				s_Result[3].move.x = 0;
				bEnterSwitch = true;
				ForcedFade();
			}

			//�p�[�e�B�N���̃Z�b�g����
			SetParticle(D3DXVECTOR3(370.0f, 750.0f, 0.0f), 1);
		}

		else if (nSetCount1 < nSetCount2)
		{//2P�����������ꍇ
			//���_���W�̐ݒ�
			pVtx[40].pos = D3DXVECTOR3(s_Result[1].pos.x - 150.0f, s_Result[1].pos.y - 150.0f, 0.0f);
			pVtx[41].pos = D3DXVECTOR3(s_Result[1].pos.x + 150.0f, s_Result[1].pos.y - 150.0f, 0.0f);
			pVtx[42].pos = D3DXVECTOR3(s_Result[1].pos.x - 150.0f, s_Result[1].pos.y + 150.0f, 0.0f);
			pVtx[43].pos = D3DXVECTOR3(s_Result[1].pos.x + 150.0f, s_Result[1].pos.y + 150.0f, 0.0f);

			pVtx[44].pos = D3DXVECTOR3(s_Result[0].pos.x - 150.0f, s_Result[0].pos.y - 150.0f, 0.0f);
			pVtx[45].pos = D3DXVECTOR3(s_Result[0].pos.x + 150.0f, s_Result[0].pos.y - 150.0f, 0.0f);
			pVtx[46].pos = D3DXVECTOR3(s_Result[0].pos.x - 150.0f, s_Result[0].pos.y + 150.0f, 0.0f);
			pVtx[47].pos = D3DXVECTOR3(s_Result[0].pos.x + 150.0f, s_Result[0].pos.y + 150.0f, 0.0f);

			pVtx[8].pos = D3DXVECTOR3(s_Result[3].pos.x - 150.0f, s_Result[3].pos.y - 50.0f, 0.0f);
			pVtx[9].pos = D3DXVECTOR3(s_Result[3].pos.x + 150.0f, s_Result[3].pos.y - 50.0f, 0.0f);
			pVtx[10].pos = D3DXVECTOR3(s_Result[3].pos.x - 150.0f, s_Result[3].pos.y + 50.0f, 0.0f);
			pVtx[11].pos = D3DXVECTOR3(s_Result[3].pos.x + 150.0f, s_Result[3].pos.y + 50.0f, 0.0f);

			pVtx[12].pos = D3DXVECTOR3(s_Result[2].pos.x - 150.0f, s_Result[2].pos.y - 50.0f, 0.0f);
			pVtx[13].pos = D3DXVECTOR3(s_Result[2].pos.x + 150.0f, s_Result[2].pos.y - 50.0f, 0.0f);
			pVtx[14].pos = D3DXVECTOR3(s_Result[2].pos.x - 150.0f, s_Result[2].pos.y + 50.0f, 0.0f);
			pVtx[15].pos = D3DXVECTOR3(s_Result[2].pos.x + 150.0f, s_Result[2].pos.y + 50.0f, 0.0f);

			//���U���g�̈ʒu�X�V
			s_Result[0].pos += s_Result[0].move;
			s_Result[1].pos += s_Result[1].move;
			s_Result[2].pos += s_Result[2].move;
			s_Result[3].pos += s_Result[3].move;

			//�ړ��ʂ�0�ɂ���
			if (s_Result[0].pos.x >= 375.0f)
			{
				s_Result[0].move.x = 0;
				s_Result[1].move.x = 0;
				s_Result[2].move.x = 0;
				s_Result[3].move.x = 0;
				bEnterSwitch = true;
				ForcedFade();
			}

			//�p�[�e�B�N���̃Z�b�g����
			SetParticle(D3DXVECTOR3(900.0f, 750.0f, 0.0f), 1);
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	//ENTER�����ĉ�ʑJ��
	if (bEnterSwitch)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A))
		{
			SetFade(MODE_TITLE);
			PlaySound(SOUND_LABEL_SE_CLICK);
		}
	}

	//�Z�b�g�J�E���g�̃Z�b�g����
	//SetCount(nSetCount1, D3DXVECTOR3(275.0f, 600.0f, 0.0f));
	//SetCount(nSetCount2, D3DXVECTOR3(975.0f, 600.0f, 0.0f));
}

//=============================================================================
// �Z�b�g�J�E���g�Z�b�g����
//=============================================================================
void SetCount(int nSetCount, D3DXVECTOR3 pos)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���ւ̃|�C���^
	VERTEX_2D * pVtx;

	//�ϐ��錾
	D3DXVECTOR3 nCountPos = pos;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (nSetCount == 0)
	{//�Z�b�g�J�E���g��0�������ꍇ���s
		//���_���W�̐ݒ�
		pVtx[16].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[17].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[18].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y + 70.0f, 0.0f);
		pVtx[19].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y + 70.0f, 0.0f);
	}

	else if (nSetCount == 1)
	{//�Z�b�g�J�E���g��1�������ꍇ���s
		//���_���W�̐ݒ�
		pVtx[20].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[21].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[22].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y + 70.0f, 0.0f);
		pVtx[23].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y + 70.0f, 0.0f);
	}

	else if (nSetCount == 2)
	{//�Z�b�g�J�E���g��2�������ꍇ���s
	  //���_���W�̐ݒ�
		pVtx[24].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[25].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[26].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y + 70.0f, 0.0f);
		pVtx[27].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y + 70.0f, 0.0f);
	}

	else if (nSetCount == 3)
	{//�Z�b�g�J�E���g��3�������ꍇ���s
	  //���_���W�̐ݒ�
		pVtx[32].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[33].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y - 70.0f, 0.0f);
		pVtx[34].pos = D3DXVECTOR3(nCountPos.x - 50.0f, nCountPos.y + 70.0f, 0.0f);
		pVtx[35].pos = D3DXVECTOR3(nCountPos.x + 50.0f, nCountPos.y + 70.0f, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=============================================================================
// ������ʑJ�ڏ���
//=============================================================================
void ForcedFade(void)
{
	//Enter�𐄂��Ȃ�����C���N�������g����
	NonEnter++;

	//������ʑJ��
	if (NonEnter >= 640)
	{
		SetFade(MODE_TITLE);
	}
}