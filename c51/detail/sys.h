#ifndef __DETAIL_SYS_H__
#define __DETAIL_SYS_H__
#include "../def.h"
#include "../sys.h"
#define SENSOR_CNT 5
#define SCHDULE_CNT 1
#define TIMER_CNT 4

typedef uint8_t (*__sys_sensor_scan)(void);
typedef void (*__sys_sensor_register)(uint32_t cfg, sys_callback_t callback);
typedef void (*__sys_sensor_callback)(uint8_t msg);
typedef void (*__sys_schedule)(void);

#define __sys_lock() __DO_WHILE0(EA = 0)
#define __sys_unlock() __DO_WHILE0(EA = 1)
extern uint32_t __sys_timer_cnt;
extern uint32_t __sysclk;
uint8_t __sys_sensor_add(__sys_sensor_register _register, __sys_sensor_scan scan, __sys_sensor_callback callback);
void __sys_sensor_set_msg(uint8_t idx, uint8_t msg);
void __sys_schedule_add(__sys_schedule schedule);
void __sys_fine_grained_timer_add(uint16_t _100us, sys_callback_t callback);
#endif
