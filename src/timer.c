#include "detail/sys.h"
#include "timer.h"
static uint32_t timer_count = 0;
static void (*timer_callback_table[4])() = { 0 };
static void timer_callback(uint8_t event, void (*callback)()) reentrant {
  if (event < 4) {
    timer_callback_table[event] = callback;
  }
}
static void timer_call() {
  // if (timer_count % 100 == 0 && timer_callbac) {
  //   timer_callbac();
  // }
  int i = 0, j = 1;
  for (; i < 4; ++i, j *= 10) {
    if (timer_count % j == 0 && timer_callback_table[i]) {
      // timer_callback_table[i]();
    }
  }
}
INTERRUPT(timer, TF0_VECTOR) {
  // timer_call();
  int i = 0, j = 1;
  for (; i < 4; ++i, j *= 10) {
    if (timer_count % j == 0 && timer_callback_table[i]) {
      timer_callback_table[i]();
    }
  }
  ++timer_count;
}
void timer_init() {
  TMOD &= 0xf0;  // T0和T1都是工作在模式0下,即16位自动重装载模式
  TH0 = (65535 - sysclk / 12 / 1000) >> 8;
  TL0 = (65535 - sysclk / 12 / 1000) & 0xff;
  ET0 = 1;  // T0中断允许
  TR0 = 1;  // T0开始计时
  sys.timer_callback = timer_callback;
}