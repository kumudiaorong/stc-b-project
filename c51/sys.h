#ifndef __SYS_H__
#define __SYS_H__
#include "def.h"
typedef void (*sys_callback_t)(void);

#define CONEVENT(sys, dri) ((sys << 4) | dri)
void sys_init(uint32_t sysclk);
void sys_register(uint8_t event, sys_callback_t callback);
void sys_exec(sys_callback_t callback);
void sys_test(uint32_t msg) REENTRANT;
void sys_set(uint32_t msg);
void led_test(uint8_t msg);
#endif
