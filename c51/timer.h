#ifndef __TIMER_H__
#define __TIMER_H__
#include "def.h"
/**
 * @brief timer1 reload value
 * timer0 use 10ms as unit
 * timer1 use 1us as unit
 * timer1 is not recommended to use
 *
 */
void timer_init();
enum TimerEvent { TIMER1MS,TIMER10MS, TIMER100MS, TIMER1S };
#endif