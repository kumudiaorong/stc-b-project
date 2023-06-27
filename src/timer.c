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
  TMOD &= 0xf0;  // T0��T1���ǹ�����ģʽ0��,��16λ�Զ���װ��ģʽ
  TH0 = (65535 - sysclk / 12 / 1000) >> 8;
  TL0 = (65535 - sysclk / 12 / 1000) & 0xff;
  ET0 = 1;  // T0�ж�����
  TR0 = 1;  // T0��ʼ��ʱ
  sys.timer_callback = timer_callback;
}