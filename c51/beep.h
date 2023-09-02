#ifndef __BEEP_H__
#define __BEEP_H__
#include "def.h"
#define __BEEP_VECTOR 7
#define __BEEP_DEFAULT_FREQ 1000
SFR(CCAPM0, 0xDB);  // 0000,0000 PCA模块1的PWM寄存器
SFR(CCAP0H, 0xFA);  // 0000,0000 PCA模块1的捕获/比较高8位
SFR(CCAP0L, 0xEA);  // 0000,0000 PCA模块1的捕获/比较低8位
SFR(CH, 0xF9);
SFR(CL, 0xE9);
extern uint16_t freq;
void beep_init(void);
void beep_on(void);
void beep_off(void);
#endif