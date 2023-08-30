#ifndef __BEEP_H__
#define __BEEP_H__
#include "def.h"
#define __BEEP_VECTOR 7
#define __BEEP_DEFAULT_FREQ 1000

void beep_init(void);
void beep_on(void);
void beep_off(void);
void beep_freq(uint16_t freq)REENTRANT;
#endif