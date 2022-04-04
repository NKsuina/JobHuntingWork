#include "score1.h"
#include "score2.h"
#include "input.h"
#include "result.h"
#include "fade.h"
#include "sound.h"

//�}�N����`
#define NUM_SCORE (2)		//����
#define SCORE_SIZE	20

//�O���[�o���ϐ�
static LPDIRECT3DTEXTURE9 g_pTexture = NULL;					//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^
static D3DXVECTOR3 g_posScore;									//�X�R�A�̈ʒu
static D3DXMATRIX mtxWorld;				//���[���h�}�g���b�N�X
static D3DXVECTOR3 rot;					//����
static D3DXCOLOR col;						//�J���[


static int g_nScore;											//�X�R�A�̒l

//=========================
//�X�R�A�̏���������
//=========================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number001.png",
		&g_pTexture);

	rot = D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f);
	g_posScore = D3DXVECTOR3(-60.0f, 130.0f, 500.0f);			//�ʒu������������


	g_nScore = 0;											//�l������������
	int nCntScore;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		(sizeof(VERTEX_3D) * 4 * NUM_SCORE),		//4�ŉ摜���
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuff,
		NULL);

	VERTEX_3D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�X�R�A�̏��̏�����
	for (nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//���_���W

		pVtx[0].pos = D3DXVECTOR3(37.5f* nCntScore - SCORE_SIZE, -SCORE_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(37.5f* nCntScore + SCORE_SIZE, -SCORE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(37.5f* nCntScore - SCORE_SIZE, +SCORE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(37.5f* nCntScore +SCORE_SIZE,+SCORE_SIZE, 0.0f);

		//g_posScore += D3DXVECTOR3(37.5f*nCntScore, 0.0f, 0.0f);

		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		//���_�J���[�̐ݒ�
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		col = D3DXCOLOR(0.0f, 0.3f, 1.0f, 1.0f);
		//�������ăr���{�[�h	��ʂɓ��_�\��

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.8f, 0.0f, 0.6f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.8f, 0.0f, 0.6f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.8f, 0.0f, 0.6f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.8f, 0.0f, 0.6f, 1.0f);
		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4; //���_�|�C���g���l�i��
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuff->Unlock();

}

//===================
//�X�R�A�̏I������
//===================
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexture != NULL)
	{
		g_pTexture->Release();
		g_pTexture = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuff != NULL)
	{
		g_pVtxBuff->Release();
		g_pVtxBuff = NULL;
	}
}

//====================
//�X�R�A�̍X�V����
//====================
void UpdateScore(void)
{
	//�t�F�[�h�̏����擾
	FADE pFade = GetFade();
	int nScore2 = GetScore2();


	if (g_nScore >= 21 && pFade == FADE_NONE)
	{
		SetResult(g_nScore,nScore2);
	}

}

//====================
//�X�R�A�̕`�揈��
//====================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
//	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
	rot.y, rot.x, rot.z);


	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, g_posScore.x, g_posScore.y, g_posScore.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexture);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCntScore * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);							//�v���~�e�B�u�i�|���S���j�̐�
	}
}

//========================
//�X�R�A�̃Z�b�g����
//========================
void SetScore(int nScore)
{
	int aPosTexU[2];	//�e���̐������i�[

	g_nScore = nScore;

	/*aPosTexU[0] = (g_nScore % 1000000) / 100000;
	aPosTexU[1] = (g_nScore % 100000) / 10000;
	aPosTexU[2] = (g_nScore % 10000) / 1000;
	aPosTexU[3] = (g_nScore % 1000) / 100;*/
	aPosTexU[0] = (g_nScore % 100) / 10;
	aPosTexU[1] = (g_nScore % 10) / 1;

	VERTEX_3D*pVtx; //���_�ւ̃|�C���^	

					//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore1 = 0; nCntScore1 < NUM_SCORE; nCntScore1++)
	{

		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore1], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore1] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore1], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore1] + 0.1f, 1.0f);

		pVtx += 4; //���_�|�C���g���l�i��

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuff->Unlock();

}

//======================
//�X�R�A�̉��Z����
//======================
void AddScore(int nValse)
{
	int aPosTexU[2]; //�e���̐������i�[

	g_nScore += nValse;

	//aPosTexU[0] = (g_nScore % 1000000) / 100000;
	//aPosTexU[1] = (g_nScore % 100000) / 10000;
	//aPosTexU[2] = (g_nScore % 10000) / 1000;
	//aPosTexU[3] = (g_nScore % 1000) / 100;
	aPosTexU[0] = (g_nScore % 100) / 10;
	aPosTexU[1] = (g_nScore % 10) / 1;

	VERTEX_3D*pVtx; //���_�ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 1.0f);

		pVtx += 4; //���_�|�C���g���l�i��
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuff->Unlock();
}

//======================
//�X�R�A�̌��Z����
//======================
void SubScore(int nValse)
{
	int aPosTexU[6]; //�e���̐������i�[

	g_nScore -= nValse;

	//���ꂵ�Ȃ���0�ȉ��̃X�R�A���\�L����Ă��܂��I�I
	if (g_nScore <= 0)
	{
		g_nScore = 0;
	}

	aPosTexU[0] = (g_nScore % 1000000) / 100000;
	aPosTexU[1] = (g_nScore % 100000) / 10000;
	aPosTexU[2] = (g_nScore % 10000) / 1000;
	aPosTexU[3] = (g_nScore % 1000) / 100;
	aPosTexU[4] = (g_nScore % 100) / 10;
	aPosTexU[5] = (g_nScore % 10) / 1;

	VERTEX_3D*pVtx; //���_�ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++)
	{
		//�e�N�X�`���̍��W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntScore] + 0.1f, 1.0f);

		pVtx += 4; //���_�|�C���g���l�i��
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuff->Unlock();
}

//==================
//�X�R�A�̎擾
//==================
int GetScore(void)
{
	return g_nScore;
}