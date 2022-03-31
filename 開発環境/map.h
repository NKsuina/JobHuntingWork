//=============================================================================
//
// Excelでマップ配置 [map.h]
// Author : NARUMI KOUKI
//
//=============================================================================
#ifndef _MAP_H_
#define	_MAP_H_

#include "main.h"

#define	MAP_WIDTH	(62)
#define	MAP_HEIGHT	(36)
#define MAP_PATH ("data/txt/map.txt")

//プロトタイプ宣言
void InitMap(void);
void DrawMap(void);

#endif 

