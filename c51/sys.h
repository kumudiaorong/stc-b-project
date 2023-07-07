#ifndef __SYS_H__
#define __SYS_H__
#include "def.h"
typedef void (*sys_callback_t)(void);

enum SysEvent { TIMER, KEY, HALL, VIB, NAV };

void sys_init(uint32_t sysclk);
#define CONEVENT(sys, dri) ((sys << 4) | dri)
void sys_register(uint8_t event, sys_callback_t callback);
void sys_exec(void (*callback)(void) REENTRANT);
void sys_test(uint32_t msg) REENTRANT;
void sys_set(uint32_t msg);
void led_test(uint8_t msg);
#endif
