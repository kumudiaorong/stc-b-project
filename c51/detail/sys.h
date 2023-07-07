#ifndef __DETAIL_SYS_H__
#define __DETAIL_SYS_H__
#include "../def.h"
#include "../sys.h"
#define SENSOR_CNT 5
#define SCHDULE_CNT 1

typedef uint8_t (*__sys_func_scan)(void);
typedef void (*__sys_func_register)(uint8_t event,sys_callback_t callback);
typedef void (*__sys_func_callback)(uint8_t msg);
typedef void (*__sys_func_schedule)(void);

#define __sys_lock() __DO_WHILE0( EA = 0 )
#define __sys_unlock() __DO_WHILE0( EA = 1 )

typedef struct {
  __sys_func_register _register;
  __sys_func_scan scan;
  __sys_func_callback callback;
  uint8_t msg;
} __sensor_t;
extern uint32_t __sys_timer_cnt;
// extern uint8_t sensor_index;

// typedef sys_callback_t *(*sys_func_scan)() REENTRANT;
// typedef sys_callback_t *(*sys_func_callback)(uint8_t event, void (*callback)())REENTRANT;
// typedef sys_callback_t *(*sys_func_schedule)() REENTRANT;

extern uint32_t __sysclk;
typedef struct {
  __sensor_t sensor[SENSOR_CNT];
  __sys_func_schedule schedule[SCHDULE_CNT];
} __sys_t;
void __sys_sensor_add(
  uint8_t event, __sys_func_register _register, __sys_func_scan scan, __sys_func_callback callback) ;
extern XDATA __sys_t __sys;
void __sys_sensor_set_msg(uint8_t idx, uint8_t msg);
void __sys_schedule_add(__sys_func_schedule schedule);
#endif
