#include "sys.h"
xdata sys_t sys = {0};
void sys_init() {
  EA = 1;
  TMOD &= 0x0f;  // T0��T1���ǹ�����ģʽ0��,��16λ�Զ���װ��ģʽ
  AUXR |= 0x55;  // T1��2������1Tģʽ����T2��ʼ��ʱ
  PT1 = 1;       // T1Ϊ�����ȼ�
  TH1 = (65535 - TIMER1_RELOAD_VALUE) >> 8;
  TL1 = (65535 - TIMER1_RELOAD_VALUE) & 0xff;
  ET1 = 1;  // T1�ж�����
  TR1 = 1;  // T1��ʼ��ʱ
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