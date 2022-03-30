//=============================================================================
//
// �J�������� [camera.cpp]
// Author : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//�C���N���[�h
//-----------------------------------------------------------------------------

#include "camera.h"
#include "input.h"
#include "calculation.h"

//-----------------------------------------------------------------------------
//�O���[�o���ϐ��錾
//-----------------------------------------------------------------------------

Camera g_camera; //�J�������

//*****************************************************************************
//�J�����̏�����
//*****************************************************************************

void InitCamera(void)
{
	//���_�E�����_�E�������ݒ������
	g_camera.posV = D3DXVECTOR3(0.0f, 700.0f, -700.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fData = g_camera.posV.x - g_camera.posR.x;
	float fData1 = g_camera.posV.z - g_camera.posR.z;

	g_camera.fDistance = sqrtf(ThreeSquareTheorem(fData, fData1));
}

//*****************************************************************************
//�J�����̏I������
//*****************************************************************************

void UninitCamera(void)
{

}

//*****************************************************************************
//�J�����̍X�V����
//*****************************************************************************

void UpdateCamera(void)
{
	//float fDistance = sqrtf(g_camera.rot.x * g_camera.rot.x + g_camera.rot.y * g_camera.rot.y);
	////����
	//if (GetKeyboardPress(DIK_T))
	//{//A�L�[�������ꂽ
	//	g_camera.rot.y += -0.01f;
	//	g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
	//	g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	//}
	//else if (GetKeyboardPress(DIK_G))
	//{//D�L�[�������ꂽ
	//	g_camera.rot.y += 0.01f;
	//	g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistance;
	//	g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistance;
	//}
	//else if (GetKeyboardPress(DIK_R))
	//{//�����_�����
	//	g_camera.posR.y += 5.0f;
	//}
	//else if (GetKeyboardPress(DIK_F))
	//{//�����_������
	//	g_camera.posR.y += -5.0f;
	//}
	//else if (GetKeyboardPress(DIK_Y))
	//{//���_�����
	//	g_camera.posV.y += 5.0f;
	//}
	//else if (GetKeyboardPress(DIK_H))
	//{//���_������
	//	g_camera.posV.y += -5.0f;
	//}
	
	//���_�ړ�
	//if (GetKeyboardPress(DIK_W))
	//{//��L�[�������ꂽ
	//	g_camera.posV.x += sinf(g_camera.rot.y) * 2.0f;
	//	g_camera.posV.z += cosf(g_camera.rot.y) * 2.0f;
	//	g_camera.posR.x += sinf(g_camera.rot.y) * 2.0f;
	//	g_camera.posR.z += cosf(g_camera.rot.y) * 2.0f;
	//}
	//else if (GetKeyboardPress(DIK_A))
	//{//���L�[�������ꂽ

	//	g_camera.posV.x += sinf(g_camera.rot.y + D3DX_PI * -0.5f) * 2.0f;
	//	g_camera.posV.z += cosf(g_camera.rot.y + D3DX_PI * -0.5f) * 2.0f;
	//	g_camera.posR.x += sinf(g_camera.rot.y + D3DX_PI * -0.5f) * 2.0f;
	//	g_camera.posR.z += cosf(g_camera.rot.y + D3DX_PI * -0.5f) * 2.0f;
	//}
	//else if (GetKeyboardPress(DIK_D))
	//{//�E�L�[�������ꂽ
	//	
	//	g_camera.posV.x += sinf(g_camera.rot.y + D3DX_PI * 0.5f) * 2.0f;
	//	g_camera.posV.z += cosf(g_camera.rot.y + D3DX_PI * 0.5f) * 2.0f;
	//	g_camera.posR.x += sinf(g_camera.rot.y + D3DX_PI * 0.5f) * 2.0f;
	//	g_camera.posR.z += cosf(g_camera.rot.y + D3DX_PI * 0.5f) * 2.0f;
	//}
	//else if (GetKeyboardPress(DIK_S))
	//{//���L�[�������ꂽ
	//	
	//	g_camera.posV.x += sinf(g_camera.rot.y + D3DX_PI) * 2.0f;
	//	g_camera.posV.z += cosf(g_camera.rot.y + D3DX_PI) * 2.0f;
	//	g_camera.posR.x += sinf(g_camera.rot.y + D3DX_PI) * 2.0f;
	//	g_camera.posR.z += cosf(g_camera.rot.y + D3DX_PI) * 2.0f;
	//}

	//////���_����
	//if (GetKeyboardPress(DIK_Q))
	//{
	//	g_camera.rot.y += -0.01f;
	//	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
	//	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	//}
	//else if (GetKeyboardPress(DIK_E))
	//{
	//	g_camera.rot.y += 0.01f;
	//	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
	//	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	//}

	//�␳
	g_camera.rot.y = Normalization(g_camera.rot.y);
}

//*****************************************************************************
//�Z�b�g�J��������
//*****************************************************************************

void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	D3DXMatrixLookAtLH(&g_camera.mtxView,&g_camera.posV,&g_camera.posR,&g_camera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),//����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,//�A�X�y�N�g��
		10.0f,//�ǂ�����ǂ��܂ŃJ�����ŕ\�����邩
		4000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}

//-----------------------------------------------------------------------------
//�J�����̏���n��
//-----------------------------------------------------------------------------

Camera *GetCamera(void)
{
	return &g_camera;
}