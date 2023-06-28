#include "sys.h"

#include "detail/sys.h"
#include "string.h"
// uint8_t sensor_index = 0;
void __sys_add_sensor(
  uint8_t event, __sys_func_scan scan, __sys_func_register _register, __sys_func_callback callback) REENTRANT {
  sys.sensor[event].scan = scan;
  sys.sensor[event]._register = _register;
  sys.sensor[event].callback = callback;
  sys.sensor[event].msg = 0;
  // ++sensor_index;
}
void sys_register(uint8_t event, sys_callback_t callback) {
  sys.sensor[(event >> 4)]._register(event & 0xf, callback);
}
uint32_t sysclk = 0;
XDATA sys_t sys;

uint32_t sys_timer_cnt = 0;

void sys_init(uint32_t clk) {
  memset(&sys, 0, sizeof(sys_t));
  sysclk = clk;
  TMOD = 0x00;  // T0和T1都是工作在模式0下,即16位自动重装载模式
  PT0 = 1;      // T0为高优先级
  // PT1 = 0;       // T1为低优先级
  TH0 = (65535 - sysclk / 1000) >> 8;
  TL0 = (65535 - sysclk / 1000) & 0xff;
  TH1 = (65535 - sysclk / 12) >> 8;
  TL1 = (65535 - sysclk / 12) & 0xff;
  IE |= 0x8A;
  // EA = 1;
  // ET0 = 1;  // T0中断允许
  // ET1 = 1;  // T1中断允许
}

INTERRUPT(sys_timer, TF0_VECTOR) {
  uint8_t i = 0;
  for(; i < SENSOR_CNT; ++i) {
    if(sys.sensor[i].scan && !sys.sensor[i].msg) {
      sys.sensor[i].msg = sys.sensor[i].scan();
    }
  }

  // if(sys.key_scan) {
  //   sys.key_scan();
  // }
  // if(sys.hall_scan) {
  //   sys.hall_scan();
  // }
  ++sys_timer_cnt;
}
void sys_exec(sys_callback_t callback) {
  AUXR |= 0x95;  // T0，2工作在1T模式，且T2开始计时
  TCON |= 0x50;
  // TR1 = 1;  // T1开始计时
  // TR0 = 1;  // T0开始计时
  while(1) {
    uint8_t i = 0;
    for(; i < SENSOR_CNT; ++i) {
      if(sys.sensor[i].callback && sys.sensor[i].msg) {
        sys.sensor[i].callback(sys.sensor[i].msg);
        sys.sensor[i].msg = 0;
      }
    }
    if(sys.display_schedule) {
      sys.display_schedule();
    }
    if(callback) {
      callback();
    }
  }
}

// INTERRUPT(timer, TF1_VECTOR) {
//   int i = 0, j = 1;
//   for(; i < 4; ++i, j *= 10) {
//     if(timer_count % j == 0 && timer_callback_table[i]) {
//       timer_callback_table[i]();
//     }
//   }
//   ++timer_count;
// }