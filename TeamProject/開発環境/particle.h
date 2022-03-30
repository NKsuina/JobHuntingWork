//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : KADO TAKUMA
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include <time.h>

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PARTICLE	(10000)			//�p�[�e�B�N���̍ő吔

//=============================================================================
// �\����
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 move;			//�ړ���
	D3DXVECTOR3 rot;			//�p�x
	D3DXCOLOR col;				//���_�J���[
	float fRadius;				//���a
	float fRot;					//���ˊp�x
	float fAngle;				//�p�x
	float fLength;				//�ړ���
	float fSubtraction��;		//���l���Z��
	float fSubtractionRadius;	//���a���Z��
	int nLife;					//����
	int nMaxLife;				//�ő����
	int nTipe;					//�^�C�v
	bool bUse;					//�g�p���Ă��邩�ǂ���
}Particle;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos , int nTipe);
Particle *GetParticle(void);


#endif