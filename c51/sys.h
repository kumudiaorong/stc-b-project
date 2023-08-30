#ifndef __SYS_H__
#define __SYS_H__
#include "def.h"
typedef void (*sys_callback_t)(void);

#define CONEVENT(sys, dri) ((sys << 4) | dri)
void sys_init(uint32_t sysclk);
void sys_register(uint8_t idx, sys_callback_t callback, uint32_t cfg);
void sys_exec(sys_callback_t callback);
extern uint32_t sys_cnt;
// void sys_set(uint32_t msg);
// void led_test(uint8_t msg);
#endif
