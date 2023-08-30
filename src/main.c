#define TEST 1
#if TEST == 0
#include "def.h"
#include "adc.h"
#include "beep.h"
#include "detail/sys.h"
#include "display.h"
#include "hall.h"
#include "key.h"
#include "nvm.h"
#include "rtc.h"
#include "sys.h"
#include "uart.h"
#include "timer.h"
#include "vib.h"
uint32_t i = 0;
void update(void) {
  // i = adc.rop;
  display_num(i);
}
void addhandler(void) {
  ++i;
}
void subhandler(void) {
  --i;
}
void sys_test(uint32_t msg) REENTRANT {
  i += msg;
}
void sys_set(uint32_t msg) {
  i = msg;
}
uint8_t buf[8];
void uart_send_test(void) REENTRANT {
  static uint8_t arr[1] = {0};
  uart_send(arr, 1);
  ++arr[0];
}
void uart_recv_test(void) REENTRANT {
  // ++i;
  uart_send(buf, 1);
}
uint8_t led = 1;
void ok(void) {
  display_led(led);
  if(led == 0x80) {
    led = 1;
  } else {
    led <<= 1;
  }
}

void beep_test(void) REENTRANT {
  static uint16_t freq = 10;
  beep_freq(freq);
  freq += 100;
}

#elif TEST == 1
#include "sys.h"
#include "uart.h"
unsigned char buf[9];
void uart_recv_test(void) {
  uart_send(buf, 8);
}
#endif
void loop(void) {
}
void main(void) {
  sys_init(27000000);
#if TEST == 0
  adc_init();
  // adc_statistics(200);
  key_init();
  timer_init();
  hall_init();
  vib_init();
  beep_init();
  display_init();
  rtc_init();
  rtc_charge();
  display_en(0xff);
  uart_init();
  // beep_on();
  // display_base(DISPLAY_BASE_BIN);
  // timer_handler_set(handler10ms);
  // display_base(DISPLAY_BASE_HEX);
  // sys_register(UART, uart_send_test, UARTSENDOVER);
  uart_cfg_recv(buf, 1);
  sys_register(UART, uart_recv_test, UARTRECVOVER);
  sys_register(KEY, beep_test, CONKEY(0, KEYPRESS));
  // sys_register(KEY, addhandler, CONKEY(0, KEYPRESS));
  sys_register(KEY, addhandler, CONKEY(0, KEYRELEASE));
  sys_register(KEY, addhandler, CONKEY(1, KEYPRESS));
  sys_register(KEY, addhandler, CONKEY(1, KEYRELEASE));
  sys_register(TIMER, update, TIMER10MS);
  sys_register(TIMER, ok, TIMER1S);
  sys_register(HALL, addhandler, HALLGETCLOSE);
  sys_register(HALL, addhandler, HALLGETAWAY);
  sys_register(ADC, addhandler, CONNAV(NAVUP, NAVPRESS));
  sys_register(ADC, addhandler, CONNAV(NAVUP, NAVRELEASE));
  sys_register(ADC, addhandler, CONNAV(NAVDOWN, NAVPRESS));
  sys_register(ADC, addhandler, CONNAV(NAVDOWN, NAVRELEASE));
  sys_register(ADC, addhandler, CONNAV(NAVLEFT, NAVPRESS));
  sys_register(ADC, addhandler, CONNAV(NAVLEFT, NAVRELEASE));
  sys_register(ADC, addhandler, CONNAV(NAVRIGHT, NAVPRESS));
  sys_register(ADC, addhandler, CONNAV(NAVRIGHT, NAVRELEASE));
  sys_register(ADC, addhandler, CONNAV(NAVCENTER, NAVPRESS));
  sys_register(ADC, addhandler, CONNAV(NAVCENTER, NAVRELEASE));
  sys_register(ADC, addhandler, CONNAV(NAVKEY3, NAVPRESS));
  sys_register(ADC, addhandler, CONNAV(NAVKEY3, NAVRELEASE));
  sys_register(VIB, addhandler, VIBSTART);
  sys_register(VIB, addhandler, VIBSTOP);
#elif TEST == 1
  uart_init();
  uart_cfg_recv(buf, 8);
  sys_register(UART, uart_recv_test, UARTRECVOVER);
#endif
  sys_exec(loop);
}
