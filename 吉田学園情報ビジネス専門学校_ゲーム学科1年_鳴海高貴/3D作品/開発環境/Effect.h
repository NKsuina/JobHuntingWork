#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//�G�t�F�N�g�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR col;			//�F
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X			
	float fRadius;			//���a(�傫��)
	int nCounterAnim;		//�J�E���^�[
	int nPatternAnim;		//�p�^�[���ԍ�
	int nLife;				//����(�\������)
	int nType;				//�G�t�F�N�g�̎��
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Effect;

//�v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife);
#endif
