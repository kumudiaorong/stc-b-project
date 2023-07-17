#ifndef __VIB_H__
#define __VIB_H__
#include "def.h"
enum VibEvent { VIBSTART, VIBSTOP, VIBNONE };
void vib_init(void);
extern uint8_t VIB;
#endif