#ifndef __SYS_H__
#define __SYS_H__
#include "def.h"
enum SysEvent { KEY, HALL, TIMER };
void sys_init(uint32_t sysclk);
#define CONEVENT(sys, dri) ((sys << 8) | dri)
void sys_callback(uint32_t event, void (*callback)());
void sys_schedule();
#endif
