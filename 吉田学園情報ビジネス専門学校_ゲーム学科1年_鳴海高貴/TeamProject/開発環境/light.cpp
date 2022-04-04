//=============================================================================
//
// ���� []
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------

#include "light.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------

#define	LIGHT_MAX	(3)	//���C�g�̍ő吔

//-----------------------------------------------------------------------------
//�O���[�o���ϐ��錾
//-----------------------------------------------------------------------------

D3DLIGHT9 g_alight[LIGHT_MAX];//���C�g�̏��

//*****************************************************************************
//���C�g�̏�����
//*****************************************************************************

void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

									//�f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXVECTOR3 vecDir[LIGHT_MAX];//���C�g�̕����x�N�g��

	//���C�g���N���A����
	ZeroMemory(&g_alight[0], sizeof(g_alight));

	//���C�g�̎�ނ�ݒ�
	g_alight[0].Type = D3DLIGHT_DIRECTIONAL;
	g_alight[1].Type = D3DLIGHT_DIRECTIONAL;
	g_alight[2].Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U����ݒ�
	g_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_alight[1].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	g_alight[2].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	//���C�g�̈ʒu
	g_alight[0].Position = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	g_alight[0].Position = D3DXVECTOR3(100.0f, 50.0f, 0.0f);
	g_alight[0].Position = D3DXVECTOR3(-100.0f, 50.0f, 0.0f);

	//���C�g�̕�����ݒ�
	vecDir[0] = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	vecDir[1] = D3DXVECTOR3(-0.4f, 0.2f, -0.8f);
	vecDir[2] = D3DXVECTOR3(-0.8f, -0.4f, 0.2f);

	//���K������i�傫���P�̃x�N�g���ɂ���j�G
	D3DXVec3Normalize(&vecDir[0], &vecDir[0]);
	D3DXVec3Normalize(&vecDir[1], &vecDir[1]);
	D3DXVec3Normalize(&vecDir[2], &vecDir[2]);
	g_alight[0].Direction = vecDir[0];
	g_alight[1].Direction = vecDir[1];
	g_alight[2].Direction = vecDir[2];

	//���C�g��ݒ肷��
	pDevice->SetLight(0, &g_alight[0]);
	pDevice->SetLight(1, &g_alight[1]);
	pDevice->SetLight(2, &g_alight[2]);

	//���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);

}

//*****************************************************************************
//���C�g�̏I������
//*****************************************************************************

void UninitLight(void)
{

}

//*****************************************************************************
//���C�g�̍X�V����
//*****************************************************************************

void UpdateLight(void)
{

}

//*****************************************************************************
//���C�g�̐ݒ菈��
//*****************************************************************************

void SetLight(void)
{

}