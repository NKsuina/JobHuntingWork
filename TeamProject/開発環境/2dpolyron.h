#ifndef _2DPOLYGON_H_			//���̃}�N����`������Ȃ�������
#define _2DPOLYGON_H_			//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;
	float fX;
	float fY;
	int nPat;
	bool bUse;
}Polygon_2D;

// �v���g�^�C�v�錾
void Init2DPolygon(void);
void Uninit2DPolygon(void);
void Update2DPolygon(void);
void Draw2DPolygon(void);

int Set2DPolygon(Polygon_2D Polygon_2D);
void Break2DPolygon(int nNum);
#endif