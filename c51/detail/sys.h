#ifndef __DETAIL_SYS_H__
#define __DETAIL_SYS_H__
#include "../def.h"

typedef struct {
  void (**callback)();
  uint8_t cnt;
  struct sys_callback_t *next;
} sys_callback_t;

typedef void(*sys_func_scan)() reentrant;
typedef void(*sys_func_callback)(uint8_t event, void (*callback)())reentrant;
typedef void(*sys_func_schedule)() reentrant;


// typedef sys_callback_t *(*sys_func_scan)() reentrant;
// typedef sys_callback_t *(*sys_func_callback)(uint8_t event, void (*callback)())reentrant;
// typedef sys_callback_t *(*sys_func_schedule)() reentrant;


extern uint32_t sysclk;
typedef struct {
  sys_func_scan key_scan;
  sys_func_callback key_callback;
  sys_func_schedule display_schedule;
  sys_func_scan hall_scan;
  sys_func_callback hall_callback;
  sys_func_callback timer_callback;
} sys_t;

extern xdata sys_t sys;
#endif
