#include "timer.h"

#include "detail/sys.h"
#include "string.h"

static void (*timer_callback_table[4])(void);
static uint8_t timer_scan(void) REENTRANT {
  uint8_t ret = 0;
  if(timer_callback_table[0])
    ret |= 1;
  if(sys_timer_cnt % 10 == 0) {
    if(timer_callback_table[1]) {
      ret |= 2;
    }
    if(sys_timer_cnt % 100 == 0) {
      if(timer_callback_table[2]) {
        ret |= 4;
      }
      if(sys_timer_cnt % 1000 == 0) {
        if(timer_callback_table[3]) {
          ret |= 8;
        }
      }
    }
  }
  return ret;
}
static void timer_register(uint8_t event, sys_callback_t callback) REENTRANT {
  timer_callback_table[event] = callback;
}
static void timer_callback(uint8_t msg) REENTRANT {
  // if(msg & 0x1){
  //   if(timer_callback_table[0]) {
  //     timer_callback_table[0]();
  //   }
  //   if (msg & 0x2) {
  //     if(timer_callback_table[1]) {
  //       timer_callback_table[1]();
  //     }
  //     if (msg & 0x4) {
  //       if(timer_callback_table[2]) {
  //         timer_callback_table[2]();
  //       }
  //       if (msg & 0x8) {
  //         if(timer_callback_table[3]) {
  //           timer_callback_table[3]();
  //         }
  //       }
  //     }
  //   }
  // }
  //   if(timer_callback_table[0] && (msg & 0x1)) {
  //     timer_callback_table[0]();
  //   }
  //   if(timer_callback_table[1] && (msg & 0x2)) {
  //     timer_callback_table[1]();
  //   }
  //   if(timer_callback_table[2] && (msg & 0x4)) {
  //     timer_callback_table[2]();
  //   }
  //   if(timer_callback_table[3] && (msg & 0x8)) {
  //     timer_callback_table[3]();
  //   }
  if((msg & 0x1)) {
    timer_callback_table[0]();
  }
  if((msg & 0x2)) {
    timer_callback_table[1]();
  }
  if((msg & 0x4)) {
    timer_callback_table[2]();
  }
  if((msg & 0x8)) {
    timer_callback_table[3]();
  }
}
void timer_init(void) {
  memset(timer_callback_table, 0, sizeof(timer_callback_table));
  __sys_add_sensor(TIMER, timer_scan, timer_register, timer_callback);
}
