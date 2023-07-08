#ifndef __HALL_H__
#define __HALL_H__
#include "def.h"
#define __HALL P1_2
#define __HALL_INI 1

#define __HALL_INIT() __DO_WHILE0(P1M1 &= ~(1 << 2); P1M0 |= 1 << 2)

enum HallEvent { HALLNONE = 0, HALLGETCLOSE = 1, HALLGETAWAY = 2 };

void hall_init(void);
extern uint8_t HALL;

#endif