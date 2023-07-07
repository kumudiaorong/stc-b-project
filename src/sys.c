#include "sys.h"

#include "detail/sys.h"

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
void sys_register(uint8_t event, sys_callback_t callback) {
  __sys.sensor[event >> 4]._register(event & 0xf, callback);
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
  TMOD = 0x00;  // T0��T1���ǹ�����ģʽ0��,��16λ�Զ���װ��ģʽ
  TH0 = (65535 - __sysclk / 1000) >> 8;
  TL0 = (65535 - __sysclk / 1000) & 0xff;
  // TH1 = (65535 - __sysclk / 12) >> 8;
  // TL1 = (65535 - __sysclk / 12) & 0xff;
  IE |= 0x02;
  // EA = 1;
  // ET0 = 1;  // T0�ж�����
  IP &= ~0x2;
  // PT0 = 0;
}
uint32_t __sys_timer_cnt = 0;  //!< system timer count
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
void sys_exec(sys_callback_t callback) {
  IE |= 0x80;
  AUXR |= 0x95;  // T0��2������1Tģʽ����T2��ʼ��ʱ
  TCON |= 0x10;
  // TR1 = 0;  // T1
  // TR0 = 1;  // T0��ʼ��ʱ
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
/**
 * @fn __sys_sensor_set_msg
 * @brief set sensor msg
 * @param idx
 * @param msg
 * @return none
 */
void __sys_sensor_set_msg(uint8_t idx, uint8_t msg) {
  __sys.sensor[idx].msg |= msg;
}
