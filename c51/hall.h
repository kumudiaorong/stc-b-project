#ifndef __HALL_H__
#define __HALL_H__
#include "def.h"
#define __HALL P1_2
#define __HALL_INI 1
void hall_init(void);
enum HallEvent { HALLNONE = 0, HALLGETCLOSE = 1, HALLGETAWAY = 2 };
#endif