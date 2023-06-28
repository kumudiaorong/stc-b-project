#ifndef __SYS_H__
#define __SYS_H__
#include "def.h"
typedef void (*sys_callback_t)(void);

enum SysEvent { TIMER, KEY, HALL };

void sys_init(uint32_t sysclk);
#define CONEVENT(sys, dri, msg) ((sys << 4) | (dri << 2) | msg)
void sys_register(uint8_t event, sys_callback_t callback);
void sys_exec(sys_callback_t callback);
void sys_test(uint8_t msg);
void sys_set(uint8_t msg);
void led_test(uint8_t msg);
#endif
