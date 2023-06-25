#include "timer.h"

static uint32_t timer_count = 0;
static void (*timer_handler)();
uint32_t timer_get() {
  return timer_count;
}
void timer_handler_set(void (*handler)()) {
  timer_handler = handler;
}
INTERRUPT(timer, TF0_VECTOR) {
  if(timer_handler) {
    timer_handler();
  }
  ++timer_count;
}
void timer_init() {
  TMOD &= 0xf0;  // T0��T1���ǹ�����ģʽ0��,��16λ�Զ���װ��ģʽ
  TH0 = (65535 - TIMER0_RELOAD_VALUE) >> 8;
  TL0 = (65535 - TIMER0_RELOAD_VALUE) & 0xff;
  timer_handler = 0;
  ET0 = 1;  // T0�ж�����
  TR0 = 1;  // T0��ʼ��ʱ
}
void delay_10ms(uint32_t n) {
  uint32_t t10ms = timer_get();
  while(timer_get() - t10ms < n / 10)
    ;
}