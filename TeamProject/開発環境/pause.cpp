//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : KADO TAKUMA
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "pause.h"
#include "sound.h"

//=============================================================================
// �X�^�e�B�b�N�ϐ�
//=============================================================================
static LPDIRECT3DTEXTURE9 g_pTexturePause[MAX_PAUSE] = {};		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			//���_�o�b�t�@�ւ̃|�C���^
static PAUSE_MENU g_PauseMenu;									//�|�[�Y���j���[�񋓌^
static PAUSE g_Pause;											//�|�[�Y���j���[�\����
static int nPauseSelect;										//�|�[�Y�I��p�̕ϐ�

//=============================================================================
// ����������
//=============================================================================
void InitPause(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9  pDevice;		

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�ϐ��̏�����
	g_Pause.bPause = false;
	nPauseSelect = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pause100.png",
		&g_pTexturePause[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pause000.png",
		&g_pTexturePause[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pause001.png",
		&g_pTexturePause[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pause002.png",
		&g_pTexturePause[3]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_PAUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	//���_�ւ̃|�C���^
	VERTEX_2D*pVtx;		

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//�|�[�Y�̏��̏�����
	//���_���W
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - MAX_RADIUS, SCREEN_HEIGHT / 2 - MAX_RADIUS, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + MAX_RADIUS, SCREEN_HEIGHT / 2 - MAX_RADIUS, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - MAX_RADIUS, SCREEN_HEIGHT / 2 + MAX_RADIUS, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + MAX_RADIUS, SCREEN_HEIGHT / 2 + MAX_RADIUS, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 250.0f - MAX_SELECT, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 250.0f - MAX_SELECT, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 250.0f + MAX_SELECT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 250.0f + MAX_SELECT, 0.0f);

	pVtx[8].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 360.0f - MAX_SELECT, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 360.0f - MAX_SELECT, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 360.0f + MAX_SELECT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 360.0f + MAX_SELECT, 0.0f);

	pVtx[12].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 470.0f - MAX_SELECT, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 470.0f - MAX_SELECT, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(440.0f - MAX_SELECT, 470.0f + MAX_SELECT, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(840.0f + MAX_SELECT, 470.0f + MAX_SELECT,0.0f);

	//RHW�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.7f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.7f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.7f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.7f);

	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{
	int nCntPause;		//�������J�E���g����p�̕ϐ�

	for (nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{//�e�N�X�`���̔j��
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}

	if (g_pVtxBuffPause != NULL)
	{//���_�o�b�t�@�̔j��
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{
	//�t�F�[�h�̏����|�C���^�ɓ����
	FADE pFade;
	pFade = GetFade();

	if ((GetKeyboardTrigger(DIK_P) || GetJoypadAllTrigger(JOYKEY_START) )&& g_Pause.bPause == false)
	{//P�����͂��ꂽ��
		////�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_CLICK);
		nPauseSelect = 0;
		g_Pause.bPause = true;
	}

	else if ((GetKeyboardTrigger(DIK_P) || GetJoypadAllTrigger(JOYKEY_START)) && g_Pause.bPause == true)
	{//P�����͂��ꂽ��
		////�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_CLICK);
		g_Pause.bPause = false;
	}

	if (g_Pause.bPause == true)
	{
		if (pFade == FADE_NONE)
		{
			if (GetKeyboardTrigger(DIK_W) || GetJoypadAllTrigger(JOYKEY_UP) || GetJoypadStickAllTrigger(JOYKEY_LEFT_STICK, JOYKEY_CROSS_UP))
			{//W�L�[�����͂��ꂽ�Ƃ�
				nPauseSelect--;
				////�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_SELECTION);
				if (nPauseSelect < 0)
				{
					nPauseSelect = 2;
				}
			}
			else if (GetKeyboardTrigger(DIK_S) || GetJoypadAllTrigger(JOYKEY_DOWN) || GetJoypadStickAllTrigger(JOYKEY_LEFT_STICK, JOYKEY_CROSS_DOWN))
			{//S�L�[�����͂��ꂽ�Ƃ�
				nPauseSelect++;
				////�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_SELECTION);
				if (nPauseSelect >= 3)
				{
					nPauseSelect = 0;
				}
			}
		}
	}

	//�|�[�Y��ʂł̃��[�h�I��
	switch (nPauseSelect)
	{
	case 0:
		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A))
		{//CONTINUE
			////�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_CLICK);
			g_Pause.bPause = false;
		}
		break;
	case 1:
		if ((GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A)) && pFade == FADE_NONE)
		{//RETRY
			////�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_CLICK);
			SetFade(MODE_GAME);
		}
		break;
	case 2:
		if ((GetKeyboardTrigger(DIK_RETURN) || GetJoypadAllTrigger(JOYKEY_A)) && pFade == FADE_NONE)
		{//QUIT
			////�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_CLICK);
			SetFade(MODE_TITLE);
		}
		break;
	}


	//�|�[�Y��ʂł̑I�����̐F�̍X�V
	VERTEX_2D*pVtx;		//���_�ւ̃|�C���^

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Pause.bPause == true)
	{

		if (nPauseSelect == 0)
		{
			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (nPauseSelect == 1)
		{
			pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (nPauseSelect == 2)
		{
			pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (nPauseSelect == 1 || nPauseSelect == 2)
		{
			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
		if (nPauseSelect == 0 || nPauseSelect == 1)
		{
			pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
		if (nPauseSelect == 0 || nPauseSelect == 2)
		{
			pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();

	//�|�[�Y�̗L�������ݒ�
	SetEnablePause(g_Pause.bPause);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^
	int nCntPause;

	pDevice = GetDevice();		//���

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		if (g_Pause.bPause == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

			//�|���S���̕`�揈��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�|���S���̌`
				nCntPause * 4,				//���_�̊J�n�ꏊ
				2);							//�v���~�e�B�u�̐�
		}
	}
}

//=============================================================================
// �|�[�Y�̏����擾
//=============================================================================
PAUSE *GetPause(void)
{
	return &g_Pause;
}