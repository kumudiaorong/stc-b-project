#ifndef __HALL_H__
#define __HALL_H__
#include "def.h"
#define __HALL P1_2


enum HallEvent { HALLGETCLOSE, HALLGETAWAY, HALLNONE };

void hall_init(void);
extern uint8_t HALL;

#endif