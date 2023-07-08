#ifndef __TIMER_H__
#define __TIMER_H__
#include "def.h"
#include "sys.h"
enum TimerEvent { TIMER1MS, TIMER10MS, TIMER100MS, TIMER1S };
void timer_init(void);
extern uint8_t TIMER;
#endif