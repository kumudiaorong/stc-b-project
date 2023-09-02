#define TEST 0
#if TEST == 0
#include "adc.h"
#include "beep.h"
#include "display.h"
#include "hall.h"
#include "key.h"
#include "sys.h"
#include "timer.h"
#include "uart.h"
#include "vib.h"

unsigned char buf1[9];
unsigned char buf2[9];
uint16_t freq = 300;
#define BUF_LEN 2
void uart1_recv_test(void) {
  // uart_send(DevUART1, buf1, BUF_LEN);
  // display_seg(buf1);
  ++display_led;
  uart_send(DevUART2, buf1, BUF_LEN);
}
void uart1_send_test(void) {
  // display_seg(buf1);
  // uart_send(DevUART1, buf1, BUF_LEN);
}
void uart2_recv_test(void) {
  // display_seg7(buf2[0], buf2[1], buf2[2], buf2[3], buf2[4], buf2[5], buf2[6], buf2[7]);
  // uart_send(DevUART1, buf2, BUF_LEN);
  // display_seg(buf1);
  uart_send(DevUART1, buf2, BUF_LEN);
  ++display_led;
}
void uart2_send_test(void) {
  uart_send(DevUART1, buf1, BUF_LEN);
}
static uint32_t i = 0;

void _10ms_test(void) {
  static uint8_t i = 0;
  display_seg[0] = display_num_decoding[i];
  if(++i == 10) {
    i = 0;
  }
}
void _100ms_test(void) {
  // static uint8_t i = 0;
  // display_seg[1] = display_num_decoding[i];
  // if(++i == 10) {
  //   i = 0;
  // }
  display_num(i);
}
void _1s_test(void) {
  static uint8_t i = 0;
  display_seg[2] = display_num_decoding[i];
  if(++i == 10) {
    i = 0;
  }
}
// const char CODE key1p[] = "key1 press";
void key1_press_test(void) {
  //uart_send(DevUART1, "key1 press", 10);
  ++i;
  // beep_on();
}
// const char CODE key1r[] = "key1 release";
void key1_release_test(void) {
  //uart_send(DevUART1, "key1 release", 12);
  ++i;
}
// const char CODE key2p[] = "key2 press";
void key2_press_test(void) {
  //uart_send(DevUART1, "key2 press", 10);
  ++i;
  // beep_off();
}
// const char CODE key2r[] = "key2 release";
void key2_release_test(void) {
  //uart_send(DevUART1, "key2 release", 12);
  ++i;
}
// const char CODE vib_start[] = "vib start";
void vib_start_test(void) {
  //uart_send(DevUART1, "vib start", 9);
  ++i;
}
void hall_get_close_test(void) {
  //uart_send(DevUART1, "hall get close", 14);
  ++i;
}
void hall_get_away_test(void) {
  //uart_send(DevUART1, "hall get away", 13);
  ++i;
}
void nav_up_press_test(void) {
  //uart_send(DevUART1, "nav up press", 12);
  ++i;
  // freq += 10;
  // beep_freq(freq);
}
void nav_up_release_test(void) {
  //uart_send(DevUART1, "nav up release", 14);
  ++i;
}
void nav_down_press_test(void) {
  //uart_send(DevUART1, "nav down press", 14);
  ++i;
  // freq -= 10;
  // beep_freq(freq);
}
void nav_down_release_test(void) {
  //uart_send(DevUART1, "nav down release", 16);
  ++i;
}
void nav_left_press_test(void) {
  //uart_send(DevUART1, "nav left press", 14);
  ++i;
}
void nav_left_release_test(void) {
  //uart_send(DevUART1, "nav left release", 16);
  ++i;
}
void nav_right_press_test(void) {
  //uart_send(DevUART1, "nav right press", 15);
  ++i;
}
void nav_right_release_test(void) {
  //uart_send(DevUART1, "nav right release", 17);
  ++i;
}
void nav_center_press_test(void) {
  //uart_send(DevUART1, "nav center press", 16);
  ++i;
}
void nav_center_release_test(void) {
  ++i;
  //uart_send(DevUART1, "nav center release", 18);
}
void nav_key3_press_test(void) {
  ++i;
  //uart_send(DevUART1, "nav key3 press", 14);
}
void nav_key3_release_test(void) {
  ++i;
  //uart_send(DevUART1, "nav key3 release", 16);
}
void loop(void) {
}
void main(void) {
  sys_init(27000000);
  key_init();
  vib_init();
  adc_init();
  hall_init();
  // beep_init();
  display_init();
  display_area(0, 8);
  // beep_freq(300);
  uart_init(InitUART1 | InitUART2_485);
  // uart_cfg_recv(DevUART1, buf1, BUF_LEN);
  // uart_cfg_recv(DevUART2, buf2, BUF_LEN);
  // sys_register(RegUart, uart1_recv_test, CONUART(DevUART1, EventUartRecvOver));
  // sys_register(RegUart, uart1_send_test, CONUART(DevUART1, EventUartSendOver));
  // sys_register(RegUart, uart2_send_test, CONUART(DevUART2, EventUartSendOver));
  // sys_register(RegUart, uart2_recv_test, CONUART(DevUART2, EventUartRecvOver));
  // sys_register(RegTimer, _10ms_test, EventTimer10ms);
  sys_register(RegTimer, _100ms_test, EventTimer100ms);
  // sys_register(RegTimer, _1s_test, EventTimer1S);
  sys_register(RegKey, key1_press_test, CONKEY(DevKey1, EventKeyPress));
  sys_register(RegKey, key1_release_test, CONKEY(DevKey1, EventKeyReleas));
  sys_register(RegKey, key2_press_test, CONKEY(DevKey2, EventKeyPress));
  sys_register(RegKey, key2_release_test, CONKEY(DevKey2, EventKeyReleas));
  sys_register(RegVib, vib_start_test, EventVibStart);
  sys_register(RegHall, hall_get_close_test, EventHallGetClose);
  sys_register(RegHall, hall_get_away_test, EventHallGetAway);
  sys_register(RegNav, nav_up_press_test, CONNAV(DevNavUp, EventNavPress));
  sys_register(RegNav, nav_up_release_test, CONNAV(DevNavUp, EventNavRelease));
  sys_register(RegNav, nav_down_press_test, CONNAV(DevNavDown, EventNavPress));
  sys_register(RegNav, nav_down_release_test, CONNAV(DevNavDown, EventNavRelease));
  sys_register(RegNav, nav_left_press_test, CONNAV(DevNavLeft, EventNavPress));
  sys_register(RegNav, nav_left_release_test, CONNAV(DevNavLeft, EventNavRelease));
  sys_register(RegNav, nav_right_press_test, CONNAV(DevNavRight, EventNavPress));
  sys_register(RegNav, nav_right_release_test, CONNAV(DevNavRight, EventNavRelease));
  sys_register(RegNav, nav_center_press_test, CONNAV(DevNavCenter, EventNavPress));
  sys_register(RegNav, nav_center_release_test, CONNAV(DevNavCenter, EventNavRelease));
  sys_register(RegNav, nav_key3_press_test, CONNAV(DevNavKey3, EventNavPress));
  sys_register(RegNav, nav_key3_release_test, CONNAV(DevNavKey3, EventNavRelease));
  sys_exec(loop);
}
#elif TEST == 1
#include "sys.h"
#include "uart.h"
char buf[11];
void uart1_recv_test(void) {
  uint8_t i = 0;
  for(; i < 5; ++i) {
    buf[i] ^= buf[9 - i];
    buf[9 - i] ^= buf[i];
    buf[i] ^= buf[9 - i];
  }
  uart_send(DevUART1, buf, 10);
}
void main(void) {
  sys_init(27000000);
  uart_init(InitUART1);
  uart_cfg_recv(DevUART1, buf, 10);
  sys_register(RegUart, uart1_recv_test, CONUART(DevUART1, EventUartRecvOver));
  sys_exec(0);
}
#elif TEST == 2
#include "display.h"
#include "sys.h"
#include "uart.h"

char buf1[6];
char buf2[6];
void uart1_recv_test(void) {
  uart_send(DevUART2, buf1, 5);
}
void uart2_recv_test(void) {
  display_seg[0] = display_num_decoding[buf2[0]];
  display_seg[1] = display_num_decoding[buf2[1]];
  display_seg[2] = display_num_decoding[buf2[2]];
  display_seg[3] = display_num_decoding[buf2[3]];
  display_seg[4] = display_num_decoding[buf2[4]];
  display_led = buf2[0] + buf2[1] + buf2[2] + buf2[3] + buf2[4];
}
void main(void) {
  sys_init(27000000);
  uart_init(InitUART1 | InitUART2_Ext);
  display_init();
  display_area(0, 8);
  uart_cfg_recv(DevUART1, buf1, 5);
  uart_cfg_recv(DevUART2, buf2, 5);
  sys_register(RegUart, uart1_recv_test, CONUART(DevUART1, EventUartRecvOver));
  sys_register(RegUart, uart2_recv_test, CONUART(DevUART2, EventUartRecvOver));
  sys_exec(0);
}
#elif TEST == 3
#include "display.h"
#include "key.h"
#include "rtc.h"
#include "sys.h"

void key1_press_test(void) {
  rtc.hour = 0;
  rtc.minute = 0;
  rtc.second = 0;
  rtc_write();
}
void key2_press_test(void) {
  rtc_read();
  display_seg[0] = display_num_decoding[rtc.second % 10];
  display_seg[1] = display_num_decoding[rtc.second / 10];
  display_seg[2] = 0x40;
  display_seg[3] = display_num_decoding[rtc.minute % 10];
  display_seg[4] = display_num_decoding[rtc.minute / 10];
  display_seg[5] = 0x40;
  display_seg[6] = display_num_decoding[rtc.year % 10];
  display_seg[7] = display_num_decoding[rtc.year / 10];
}
void main(void) {
  sys_init(27000000);
  key_init();
  rtc_init();
  display_init();
  sys_register(RegKey, key1_press_test, CONKEY(DevKey1, EventKeyPress));
  sys_register(RegKey, key2_press_test, CONKEY(DevKey2, EventKeyPress));
  sys_exec(0);
}
#elif TEST == 4
#include "display.h"
#include "nvm.h"
#include "sys.h"

uint8_t v;
void main(void) {
  sys_init(27000000);
  display_init();
  v = nvm_read(0x00);
  display_led = v;
  nvm_write(0x00, v + 1);
  sys_exec(0);
}
#elif TEST == 8
#include "adc.h"
#include "display.h"
#include "sys.h"
#include "timer.h"

void _100ms_test(void) {
  display_seg[0] = display_num_decoding[adcs.rt % 10];
  display_seg[1] = display_num_decoding[adcs.rt / 10];
}
void main(void) {
  sys_init(27000000);
  display_init();
  adc_init();
  sys_register(RegTimer, _100ms_test, EventTimer100ms);
  sys_exec(0);
}
#endif
