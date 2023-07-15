#ifndef __DETAIL_SYS_H__
#define __DETAIL_SYS_H__
#include "../def.h"
#include "../sys.h"
#define SENSOR_CNT 10
#define SCHDULE_CNT 1
#define TIMER_CNT 4

typedef uint8_t __sys_msg_t;
typedef __sys_msg_t (*__sys_sensor_scan)(void);
typedef void (*__sys_sensor_register)(uint32_t cfg, sys_callback_t callback);
typedef void (*__sys_sensor_callback)(__sys_msg_t msg);
typedef void (*__sys_schedule)(void);

/**
 * @struct __sensor_t
 * @brief sensor struct
 * @var __sensor_t::_register register callback function
 * @var __sensor_t::scan scan callback function
 * @var __sensor_t::callback callback function
 */
typedef struct {
  __sys_sensor_register _register;
  __sys_sensor_scan scan;
  __sys_sensor_callback callback;
  uint8_t msg;
} __sensor_t;
/**
 * @struct __sys_t
 * @brief system struct
 * @var __sys_t::sensor sensor array
 * @var __sys_t::schedule schedule array
 */
typedef struct {
  __sensor_t sensor[SENSOR_CNT];
  __sys_schedule schedule[SCHDULE_CNT];
} __sys_t;

extern XDATA __sys_t __sys;     //!< system


#define __sys_lock() __DO_WHILE0(EA = 0)
#define __sys_unlock() __DO_WHILE0(EA = 1)
extern uint32_t __sys_timer_cnt;
extern uint32_t __sysclk;
uint8_t __sys_sensor_add(__sys_sensor_register _register, __sys_sensor_scan scan, __sys_sensor_callback callback);
void __sys_schedule_add(__sys_schedule schedule);
void __sys_fine_grained_timer_add(uint16_t _100us, sys_callback_t callback);
#endif
