#ifndef __HALL_H__
#define __HALL_H__
#include "def.h"

enum HallEvent { HALLGETCLOSE, HALLGETAWAY, HALLNONE };

void hall_init(void);
extern uint8_t HALL;

#endif