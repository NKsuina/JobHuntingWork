#include"Time.h"
#include"game.h"
#include"fade.h"
#include"countdown.h"
#include"Player.h"
#include "polygon.h"
#include "disc.h"

#define MAX				(256)

static LPDIRECT3DTEXTURE9 s_pTexturecountdown3 = NULL; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffcountdown3 = NULL; //���_�o�b�t�@�̐ݒ�
static D3DXVECTOR3 s_poscountdown3; //�X�R�A�̈ʒu
static LPDIRECT3DTEXTURE9 s_pTexturecountdownBack = NULL; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffcountdownBack = NULL; //���_�o�b�t�@�̐ݒ�

static D3DXVECTOR3 s_poscountdownBack; //�X�R�A�̈ʒu



static int g_nPatternanim;
static int g_nCounterAnim;
static int nCut;
static Player *pPlayer = GetPlayer();
bool bFlag;
//===================
//����������
//===================
void Initcountdown(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	

	bFlag = true;
	g_nPatternanim = 0;
	g_nCounterAnim = 0;
	int s_nTimer = 0;
	nCut = 0;

	s_poscountdown3 = D3DXVECTOR3(650.0f, 250.0f, 0.0f);			//�ʒu�ݒ�		�h�b�g�u�����_�v
	//s_poscountdownBack = D3DXVECTOR3(230.0f, 130.0f, 0.0f);			//�ʒu�ݒ�

	//�f�o�C�X�̎擾
	pDevice = GetDevice();


	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/countdown.png",
		&s_pTexturecountdown3);
	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//�������_�t�H�[�}�b�g
		&s_pVtxBuffcountdown3,
		NULL);


	////�e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice,
	//	"data/image/back 1.png",
	//	&s_pTexturecountdownBack);
	////���_�o�b�t�@
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_2D,
	//	D3DPOOL_MANAGED,//�������_�t�H�[�}�b�g
	//	&s_pVtxBuffcountdownBack,
	//	NULL);

	//-------------------------------------------------------------------------------------------
	VERTEX_2D*pVtx; //���_�ւ̃|�C���^

		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		s_pVtxBuffcountdown3->Lock(0, 0, (void**)&pVtx, 0);
		//���_���W
		SetNorotpos(pVtx,
			s_poscountdown3.x - 100,
			s_poscountdown3.x + 100,
			s_poscountdown3.y - 100,
			s_poscountdown3.y + 100);
		//RHW�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//�e�N�X�`���̍��W�ݒ�
		Settex(pVtx, 0.0f, 1.0f/3.0f, 0.0f, 1.0f);
		pVtx += 4; //���_�|�C���g���l�i��
	//���_�o�b�t�@���A�����b�N
		s_pVtxBuffcountdown3->Unlock();

		////���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		//s_pVtxBuffcountdownBack->Lock(0, 0, (void**)&pVtx, 0);
		////���_���W
		//SetNorotpos(pVtx,
		//	s_poscountdownBack.x - 80,
		//	s_poscountdownBack.x + 80,
		//	s_poscountdownBack.y - 80,
		//	s_poscountdownBack.y + 80);
		////RHW�̐ݒ�
		//pVtx[0].rhw = 1.0f;
		//pVtx[1].rhw = 1.0f;
		//pVtx[2].rhw = 1.0f;
		//pVtx[3].rhw = 1.0f;
		////���_�J���[�̐ݒ�
		//pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		//pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		//pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		//pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		////�e�N�X�`���̍��W�ݒ�
		//Settex(pVtx, 0.0f, 1.0f / 3.0f, 0.0f, 1.0f);
		//pVtx += 4; //���_�|�C���g���l�i��
		//		   //���_�o�b�t�@���A�����b�N
		//s_pVtxBuffcountdownBack->Unlock();
			

}

//===================
//�j������
//===================
void Uninitcountdown(void)
{
	
	////�e�N�X�`���̔j��
	//if (s_pTexturecountdownBack != NULL)
	//{
	//	s_pTexturecountdownBack->Release();
	//	s_pTexturecountdownBack = NULL;
	//}	
	//	//���_�o�b�t�@�̔j��
	//	if (s_pVtxBuffcountdownBack != NULL)
	//	{
	//		s_pVtxBuffcountdownBack->Release();
	//		s_pVtxBuffcountdownBack = NULL;
	//	}

	//�e�N�X�`���̔j��
	if (s_pTexturecountdown3 != NULL)
	{
		s_pTexturecountdown3->Release();
		s_pTexturecountdown3 = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffcountdown3 != NULL)
	{
		s_pVtxBuffcountdown3->Release();
		s_pVtxBuffcountdown3 = NULL;
	}
}
//===================
//�X�V����
//===================
void Updatecountdown(void)
{

	//if (nCut == 10)
	//{
	//	AddTimer(-20);
	//	nCut = 0;
	//}

	nCut++;

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	s_pVtxBuffcountdown3->Lock(0, 0, (void**)&pVtx, 0);

	g_nCounterAnim++;								//�A�j���[�V����
	if (g_nCounterAnim >= 60)
	{
		g_nCounterAnim = 0;
		g_nPatternanim++;

		if (g_nPatternanim >= 25)
		{
			g_nPatternanim = 0;
		}

		pVtx[0].tex = D3DXVECTOR2((1.0f / 3.0f) * g_nPatternanim, 0.0f);									//--------------------------�G���������Ƃ����₷
		pVtx[1].tex = D3DXVECTOR2((1.0f / 3.0f) * (g_nPatternanim + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / 3.0f) * g_nPatternanim, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / 3.0f) * (g_nPatternanim + 1), 1.0f);

	}
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffcountdown3->Unlock();
	
	for (int nCnt = 0; nCnt < MAX; nCnt++)
	{
		if (nCut > 1)
		{
			TrueSet();
		}
	}
	if (nCut > 179)		//�J�E���g
	{
		if (bFlag)
		{
			//�f�B�X�N�̐ݒ�
			SetDisc(E_PLAYER_1);
		}

		bFlag = false;
		FalseSet();
	}	
	
}

//===================
//�`�揈��
//===================
void Drawcountdown(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�̃|�C���^
	 
	if (bFlag)
	{
		pDevice = GetDevice();		//���

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuffcountdown3, 0, sizeof(VERTEX_2D));
		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexturecountdown3);
		//
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			2);


		////���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		//pDevice->SetStreamSource(0, s_pVtxBuffcountdownBack, 0, sizeof(VERTEX_2D));
		////���_�t�H�[�}�b�g�̐ݒ�
		//pDevice->SetFVF(FVF_VERTEX_2D);
		////�e�N�X�`���̐ݒ�
		//pDevice->SetTexture(0, s_pTexturecountdownBack);
		////
		//pDevice->DrawPrimitive(
		//	D3DPT_TRIANGLESTRIP,
		//	0,
		//	2);
	}
}
void SetNorotpos(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	pVtx[0].pos = D3DXVECTOR3(left, top, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(right, top, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(left, down, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(right, down, 0.0f);
}
void Settex(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);
}