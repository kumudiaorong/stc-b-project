#ifndef __SYS_H__
#define __SYS_H__
#include "def.h"
/**
 * @brief timer0 reload value
 *
 */
#ifdef SYSCLK_11059200
#define TIMER1_RELOAD_VALUE 1106
#else  // SYSCLK_11059200
#define TIMER1_RELOAD_VALUE 1106
#endif
typedef struct {
  void (*key_scan)() reentrant;
  void (*key_callback)(uint8_t event, void (*callback)()) reentrant;
  void (*display_schedule)() reentrant;
  void (*hall_scan)() reentrant;
  void (*hall_callback)(uint8_t event, void (*callback)()) reentrant;
} sys_t;

extern xdata sys_t sys;
enum SysEvent { KEY, HALL };
void sys_init();
#define CONEVENT(sys, dri) ((sys << 8) | dri)
void sys_callback(uint32_t event, void (*callback)());
void sys_schedule();
#endif
