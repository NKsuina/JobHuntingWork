#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include"main.h"

//�v���g�^�C�v�錾
void Initcountdown(void);
void Uninitcountdown(void);
void Updatecountdown(void);
void Drawcountdown(void);


void SetNorotpos(VERTEX_2D *pVtx, float left, float right, float top, float down);
void Settex(VERTEX_2D *pVtx, float left, float right, float top, float down);

#endif
