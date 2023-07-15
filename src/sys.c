#include "sys.h"

#include "detail/sys.h"


static uint8_t sys_schedule_idx = 0;  //!< system schedule index
XDATA __sys_t __sys = {{0}, {0}};     //!< system
/**
 * @fn __sys_schedule_add
 * @brief add schedule
 * @param schedule
 * @return none
 */
void __sys_schedule_add(__sys_schedule schedule) {
  __sys.schedule[sys_schedule_idx++] = schedule;
}
static uint8_t sys_sensor_idx = 0;  //!< system sensor index
/**
 * @fn __sys_sensor_add
 * @brief add sensor
 * @param _register
 * @param scan
 * @param callback
 * @return sensor index
 */
uint8_t __sys_sensor_add(__sys_sensor_register _register, __sys_sensor_scan scan, __sys_sensor_callback callback) {
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
void sys_register(uint8_t idx, sys_callback_t callback, uint32_t cfg) {
  __sys.sensor[idx]._register(cfg, callback);
}

uint32_t __sysclk = 0;  //!< system clock
/**
 * @fn sys_init
 * @brief system init
 * @param clk
 * @return none
 */
void sys_init(uint32_t clk) {
  __sysclk = clk;
  TMOD = 0x00;  // T0和T1都是工作在模式0下,即16位自动重装载模式
  TH0 = (65535 - __sysclk / 1000) >> 8;
  TL0 = (65535 - __sysclk / 1000) & 0xff;
  IE |= 0x02;
  // EA = 1;
  // ET0 = 1;  // T0中断允许
  IP &= ~0x2;
  // PT0 = 0;
}
uint32_t __sys_timer_cnt = 0;  //!< system timer count
/**
 * @fn sys_timer
 * @brief system timer
 * @return none
 */
INTERRUPT(__sys_use_timer, TF0_VECTOR) {
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
void sys_exec(sys_callback_t callback) {
  AUXR |= 0xD4;  // T0，2工作在1T模式，且T2开始计时
  IE |= 0x80;
  // CMOD |= 0x1;
  TCON |= 0x50;
  CR = 1;
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
    for(i = 0; i < SCHDULE_CNT; ++i)
      if(__sys.schedule[i]) {
        __sys.schedule[i]();
      }
    if(callback) {
      callback();
    }
  }
}