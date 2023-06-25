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
#include "display.h"
#include "key.h"
#include "sys.h"
#include "timer.h"
#include"hall.h"
// #include
int i = 0;
void addhandler() {
  ++i;
}
void subhandler() {
  --i;
}
void main() {
  sys_init();
  timer_init();
  display_init();
  key_init();
  hall_init();
  // timer_handler_set(handler10ms);
  // display_base(DISPLAY_BASE_HEX);
  sys_callback(CONEVENT(KEY, CONKEY(KEY_PRESS, 0)), addhandler);
  sys_callback(CONEVENT(KEY, CONKEY(KEY_PRESS, 1)), subhandler);
  sys_callback(CONEVENT(HALL, HALLGETCLOSE), addhandler);
  sys_callback(CONEVENT(HALL, HALLGETAWAY), subhandler);

  while(1) {
    display_num(i);
    display_led(i);
    sys_schedule();
  }
}