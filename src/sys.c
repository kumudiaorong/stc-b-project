#include "sys.h"

#include "detail/sys.h"
#include "string.h"

static uint8_t sys_schedule_idx = 0;  //!< system schedule index
void __sys_schedule_add(__sys_func_schedule schedule) {
  __sys.schedule[sys_schedule_idx++] = schedule;
}
static uint8_t sys_sensor_idx = 0;  //!< system sensor index
/**
 * @fn __sys_sensor_add
 * @brief add sensor
 * @param event
 * @param _register
 * @param scan
 * @param callback
 * @return none
 */
uint8_t __sys_sensor_add(__sys_func_register _register, __sys_func_scan scan, __sys_func_callback callback) {
  __sys.sensor[sys_sensor_idx]._register = _register;
  __sys.sensor[sys_sensor_idx].scan = scan;
  __sys.sensor[sys_sensor_idx].callback = callback;
  __sys.sensor[sys_sensor_idx].msg = 0;
  return sys_sensor_idx++;
}
/**
 * @fn sys_register
 * @brief register sensor
 * @param event event type
 * @param callback callback function
 * @return none
 */
void sys_register(uint8_t event, sys_callback_t callback) {
  __sys.sensor[event >> 4]._register(event & 0xf, callback);
}
uint32_t __sysclk = 0;  //!< system clock
XDATA __sys_t __sys;    //!< system

uint32_t __sys_timer_cnt = 0;  //!< system timer count
/**
 * @fn sys_init
 * @brief system init
 * @param clk
 * @return none
 */
void sys_init(uint32_t clk) {
  memset(&__sys, 0, sizeof(__sys_t));
  __sysclk = clk;
  TMOD = 0x00;  // T0和T1都是工作在模式0下,即16位自动重装载模式
  TH0 = (65535 - __sysclk / 1000) >> 8;
  TL0 = (65535 - __sysclk / 1000) & 0xff;
  // TH1 = (65535 - __sysclk / 12) >> 8;
  // TL1 = (65535 - __sysclk / 12) & 0xff;
  IE |= 0x02;
  // EA = 1;
  // ET0 = 1;  // T0中断允许
  IP &= ~0x2;
  // PT0 = 0;
}
/**
 * @fn sys_timer
 * @brief system timer
 * @return none
 */
INTERRUPT(sys_timer, TF0_VECTOR) {
  uint8_t i = 0;
  for(; i < sys_sensor_idx; ++i) {
    if(__sys.sensor[i].scan && !__sys.sensor[i].msg) {
      __sys.sensor[i].msg |= __sys.sensor[i].scan();
    }
  }
  ++__sys_timer_cnt;
}
/**
 * @fn sys_exec
 * @brief system exec
 * @param callback
 * @return none
 */
#include "display.h"
void sys_exec(void (*callback)(void) REENTRANT) {
  IE |= 0x80;
  AUXR |= 0x95;  // T0，2工作在1T模式，且T2开始计时
  TCON |= 0x10;
  // TR1 = 0;  // T1
  // TR0 = 1;  // T0开始计时
  while(1) {
    uint8_t i = 0;
    for(; i < SENSOR_CNT; ++i) {
      if(__sys.sensor[i].callback && __sys.sensor[i].msg) {
        __sys.sensor[i].callback(__sys.sensor[i].msg);
        __sys.sensor[i].msg = 0;
      }
    }
    if(__sys.schedule[0]) {
      __sys.schedule[0]();
    }
    if(callback) {
      callback();
      // display_num(__sys_timer_cnt);
    }
  }
}
void __sys_sensor_set_msg(uint8_t event, uint8_t msg) {
  __sys.sensor[event].msg |= msg;
}
