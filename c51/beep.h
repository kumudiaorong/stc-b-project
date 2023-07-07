#include"def.h"
#define __BEEP_INIT() __DO_WHILE0( P3M0 &= ~(1<<4); P3M1 |= (1<<4) )
#define __BEEP P3_4
void beep_init(void);