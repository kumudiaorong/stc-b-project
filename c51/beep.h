#ifndef __BEEP_H__
#define __BEEP_H__
#include "def.h"
#define __BEEP_VECTOR 7
#define __BEEP_DEFAULT_FREQ 1000
#define __BEEP P3_4

#define __BEEP_INIT() __DO_WHILE0(CCAPM1 |= 0x48; CMOD |= 0x8; PPCA = 1; P3M0 |= 1 << 4; P3M1 &= ~(1 << 4); __BEEP = 0)

void beep_init(void);
void beep_on(void);
void beep_off(void);
void beep_freq(uint16_t freq)REENTRANT;
#endif