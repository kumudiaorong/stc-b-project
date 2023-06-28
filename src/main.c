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
#include "detail/sys.h"
#include "display.h"
#include "hall.h"
#include "key.h"
#include "sys.h"
#include "timer.h"

// #include
uint32_t i = 0;
void update(void) {
  display_num(i);
}
void addhandler(void) {
  ++i;
}
void subhandler(void) {
  --i;
}
void sys_test(uint8_t msg) {
  i += msg;
}
void sys_set(uint8_t msg) {
  i = msg;
}
uint8_t led = 1;
void ok(void) {
  ++i;
  display_led(led);
  if(led == 0x80) {
    led = 1;
  } else {
    led <<= 1;
  }
}
void main(void) {
  sys_init(27000000);
  display_init();
  key_init();
  timer_init();
  hall_init();
  // timer_handler_set(handler10ms);
  // display_base(DISPLAY_BASE_HEX);
  sys_register(CONEVENT(KEY, KEY_PRESS, 0), addhandler);
  // sys_register(CONEVENT(KEY, KEY_PRESS, 1), addhandler);
  // sys_register(CONEVENT(KEY, KEY_PRESS, 2), addhandler);
  sys_register(CONEVENT(TIMER, 0, T10MS), update);
  sys_register(CONEVENT(TIMER, 0, T1S), ok);
  sys_register(CONEVENT(HALL, 0, HALLGETCLOSE), addhandler);
  // sys_register(CONEVENT(HALL, 0, HALLGETAWAY), addhandler);
  sys_exec(0);
}