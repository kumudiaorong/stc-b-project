#ifndef __TIMER_H__
#define __TIMER_H__
#include "def.h"
#include "sys.h"
enum TimerEvent { T1MS, T10MS, T100MS, T1S };
void timer_init(void);
#endif