#ifndef __VIB_H__
#define __VIB_H__
#include "def.h"
#define GET_VIB() (uint8_t)(P2_4 == 0)
#define __VIB_INIT() __DO_WHILE0(P2M0 &= ~(1 << 4); P2M1 |= 1 << 4; P2_4 = 1)
enum VibEvent { VIBNONE = 0, VIBSTART = 1, VIBSTOP = 2 };
void vib_init(void);
#endif