//=============================================================================
//
// �|���S������ [polygon.h]
// Author : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _POLYGON_H_			//���̃}�N����`������Ȃ�������
#define _POLYGON_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//-----------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------

#include "main.h"

//-----------------------------------------------------------------------------
//�񋓌^�̒�`
//-----------------------------------------------------------------------------

//�e�N�X�`���񋓌^
typedef enum
{
	TEXTURE_0 = 0,		//�e�N�X�`�����Ȃ����
	TEXTURE_1,			//���e�N�X�`��
	TEXTURE_2,			//�ǃe�N�X�`��
	TEXTURE_3,			//�ǃe�N�X�`��

	TEXTURE_3PT_R,		//3PT�e�N�X�`��
	TEXTURE_3PT_L,		//3PT�e�N�X�`��
	TEXTURE_5PT_R,		//5PT�e�N�X�`��
	TEXTURE_5PT_L,		//5PT�e�N�X�`��

	TEXTURE_MAX			//�e�N�X�`���̍ő吔
}TexturetPolygon;

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------

void InitPolygon(void);		//������
void UninitPolygon(void);	//�I������
void UpdatePolygon(void);	//�X�V����
void DrawPolygon(void);		//�`�揈��
void SetPolygon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadiusX, float fRadiusZ, TexturetPolygon Texturetstate);

void SetGameSet(int nSet);	//�f�o�b�N

void FalseSet(void);
void TrueSet(void);

#endif
