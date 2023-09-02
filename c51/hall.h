#ifndef __HALL_H__
#define __HALL_H__
#include "def.h"

enum HallEvent { EventHallGetClose, EventHallGetAway };

void hall_init(void);

#endif