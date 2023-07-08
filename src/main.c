// /**
//  * *********************************************
//  *
//  * 8051 blink demo
//  *
//  * PIN: P11
//  *
//  * *********************************************
// */

// #include <mcs51/8051.h>
// #include <mcs51/stc12.h>
// #include<STC/STC12C5A60S2.H>
#include "def.h"
// // #include<mcs51/8052.h>
// // #include<mcs51/compiler.h>
#include "adc.h"
#include "detail/sys.h"
#include "display.h"
#include "hall.h"
#include "key.h"
#include "sys.h"
#include "timer.h"
#include "vib.h"

#define TEST 0

#if TEST == 0
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
uint8_t led = 1;
void ok(void) {
  // ++i;
  display_led(led);
  if(led == 0x80) {
    led = 1;
  } else {
    led <<= 1;
  }
}
#endif
void loop(void) {
  // display_num(i);
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
  display_init();
  display_en(0xff);
  // timer_handler_set(handler10ms);
  // display_base(DISPLAY_BASE_HEX);
  sys_register(CONEVENT(key_idx(), CONKEY(KEYPRESS, 0)), addhandler);
  sys_register(CONEVENT(key_idx(), CONKEY(KEYRELEASE, 0)), addhandler);
  sys_register(CONEVENT(key_idx(), CONKEY(KEYPRESS, 1)), addhandler);
  sys_register(CONEVENT(key_idx(), CONKEY(KEYRELEASE, 1)), addhandler);
  sys_register(CONEVENT(timer_idx(), TIMER10MS), update);
  sys_register(CONEVENT(timer_idx(), TIMER1S), ok);
  sys_register(CONEVENT(hall_idx(), HALLGETCLOSE), addhandler);
  sys_register(CONEVENT(hall_idx(), HALLGETAWAY), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVPRESS, NAVUP)), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVRELEASE, NAVUP)), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVPRESS, NAVDOWN)), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVRELEASE, NAVDOWN)), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVPRESS, NAVLEFT)), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVRELEASE, NAVLEFT)), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVPRESS, NAVRIGHT)), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVRELEASE, NAVRIGHT)), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVPRESS, NAVCENTER)), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVRELEASE, NAVCENTER)), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVPRESS, NAVKEY3)), addhandler);
  sys_register(CONEVENT(adc_idx(), CONNAV(NAVRELEASE, NAVKEY3)), addhandler);
  sys_register(CONEVENT(vib_idx(), VIBSTART), addhandler);
  sys_register(CONEVENT(vib_idx(), VIBSTOP), addhandler);
#elif TEST == 1
  display_init();
  display_num_decoding[3] = 0x76;
  display_num_decoding[4] = 0x38;
  display_num_decoding[5] = 0x40;
  display_en(0xfe);
  display_seg7(0, display_num_decoding[1], display_num_decoding[2], 0x40, 0x38, 0x76, display_num_decoding[2],
    display_num_decoding[1]);
#elif TEST == 2
  display_init();
  display_num_decoding[3] = 0x76;
  display_num_decoding[4] = 0x38;
  display_num_decoding[5] = 0x40;
  display_en(0xfe);
  display_seg7(0, display_num_decoding[1], display_num_decoding[2], 0x40, 0x38, 0x76, display_num_decoding[2],
    display_num_decoding[1]);
#endif
  sys_exec(loop);
}
