#ifndef __SYS_H__
#define __SYS_H__
#include "def.h"
typedef void (*sys_callback_t)(void);

enum RegisterType { RegUart, RegKey, RegHall, RegNav, RegVib, RegBeep, RegRtc, RegNvm, RegTimer, RegSys };
enum TimerEvent { EventTimer10ms, EventTimer100ms, EventTimer1S };
void sys_init(uint32_t sysclk);
void sys_register(uint8_t reg, sys_callback_t callback, uint32_t cfg);
void sys_exec();
void sys_reset();
#endif
