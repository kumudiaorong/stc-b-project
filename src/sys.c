#include "sys.h"
xdata sys_t sys = {0};
void sys_init() {
  EA = 1;
  TMOD &= 0x0f;  // T0和T1都是工作在模式0下,即16位自动重装载模式
  AUXR |= 0x55;  // T1，2工作在1T模式，且T2开始计时
  PT1 = 1;       // T1为高优先级
  TH1 = (65535 - TIMER1_RELOAD_VALUE) >> 8;
  TL1 = (65535 - TIMER1_RELOAD_VALUE) & 0xff;
  ET1 = 1;  // T1中断允许
  TR1 = 1;  // T1开始计时
}
INTERRUPT(sys_timer, TF1_VECTOR) {
  static uint32_t sys_timer_cnt = 0;
  if(sys.key_scan) {
    sys.key_scan();
  }
  if(sys.hall_scan) {
    sys.hall_scan();
  }
  ++sys_timer_cnt;
}
void sys_callback(uint32_t event, void (*callback)()) {
  switch(event >> 8) {
    case KEY :
      sys.key_callback(event & 0xff, callback);
      break;
    case HALL :
      sys.hall_callback(event & 0xff, callback);
      break;
    default :
      break;
  }
}
void sys_schedule() {
  if(sys.display_schedule) {
    sys.display_schedule();
  }
}