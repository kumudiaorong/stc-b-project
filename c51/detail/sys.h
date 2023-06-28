#ifndef __DETAIL_SYS_H__
#define __DETAIL_SYS_H__
#include "../def.h"
#include "../sys.h"
#define SENSOR_CNT 3


typedef uint8_t (*__sys_func_scan)(void) REENTRANT;
typedef void (*__sys_func_register)(uint8_t event,sys_callback_t callback)REENTRANT;
typedef void (*__sys_func_callback)(uint8_t msg)REENTRANT;
typedef void (*__sys_func_schedule)(void);

typedef struct {
  __sys_func_scan scan;
  __sys_func_register _register;
  __sys_func_callback callback;
  uint8_t msg;
} sensor_t;
extern uint32_t sys_timer_cnt;
// extern uint8_t sensor_index;

// typedef sys_callback_t *(*sys_func_scan)() REENTRANT;
// typedef sys_callback_t *(*sys_func_callback)(uint8_t event, void (*callback)())REENTRANT;
// typedef sys_callback_t *(*sys_func_schedule)() REENTRANT;

extern uint32_t sysclk;
typedef struct {
  sensor_t sensor[SENSOR_CNT];
  __sys_func_schedule display_schedule;
} sys_t;
void __sys_add_sensor(
  uint8_t event, __sys_func_scan scan, __sys_func_register _register, __sys_func_callback callback) REENTRANT;
extern XDATA sys_t sys;
#endif
