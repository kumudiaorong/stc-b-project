#ifndef __HALL_H__
#define __HALL_H__
#include "def.h"
#define __HALL P1_2

enum HallEvent { EventHallGetClose, EventHallGetAway };

void hall_init(void);

#endif