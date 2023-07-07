#include "timer.h"


#include "def.h"
#include "detail/sys.h"
static uint8_t __timer_idx = 0;

static XDATA sys_callback_t timer_callback_table[4]={0};                 //!< timer callback table
static void timer_register(uint8_t event, sys_callback_t callback);  //!< timer register function
static uint8_t timer_scan(void) REENTRANT;                           //!< timer scan function
static void timer_callback(uint8_t msg) REENTRANT;                   //!< timer callback function
uint8_t timer_idx(void) {
  return __timer_idx;
}
/**
 * @fn timer_init
 * @brief timer init
 * @return none
 */
void timer_init(void) {
  __timer_idx = __sys_sensor_add(timer_register, timer_scan, timer_callback);
}
/**
 * @fn timer_register
 * @brief timer register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void timer_register(uint8_t event, sys_callback_t callback) {
  timer_callback_table[event] = callback;
}
/**
 * @fn timer_scan
 * @brief timer scan
 * @return msg bit 0-3 for 1ms, 10ms, 100ms, 1000ms
 */
static uint8_t timer_scan(void) REENTRANT {
  uint8_t ret = 0;
  if(timer_callback_table[0])
    ret |= 1;
  if(__sys_timer_cnt % 10 == 0) {
    if(timer_callback_table[1]) {
      ret |= 2;
    }
    if(__sys_timer_cnt % 100 == 0) {
      if(timer_callback_table[2]) {
        ret |= 4;
      }
      if(__sys_timer_cnt % 1000 == 0) {
        if(timer_callback_table[3]) {
          ret |= 8;
        }
      }
    }
  }
  return ret;
}
/**
 * @fn timer_callback
 * @brief timer callback
 * @param msg msg type
 * @return none
 */
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
