#include "timer.h"


#include "def.h"
#include "detail/sys.h"
uint8_t TIMER = 0;

static XDATA sys_callback_t timer_callback_table[4]={0};                 //!< timer callback table
static void timer_register(uint32_t cfg, sys_callback_t callback);  //!< timer register function
static __sys_msg_t timer_scan(void) REENTRANT;                           //!< timer scan function
static void timer_callback(__sys_msg_t msg) REENTRANT;                   //!< timer callback function
/**
 * @fn timer_init
 * @brief timer init
 * @return none
 */
void timer_init(void) {
  TIMER = __sys_sensor_add(timer_register, timer_scan, timer_callback);
}
/**
 * @fn timer_register
 * @brief timer register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void timer_register(uint32_t cfg, sys_callback_t callback) {
  timer_callback_table[cfg] = callback;
}
/**
 * @fn timer_scan
 * @brief timer scan
 * @return msg bit 0-3 for 1ms, 10ms, 100ms, 1000ms
 */
static __sys_msg_t timer_scan(void) REENTRANT {
  __sys_msg_t ret = 0;
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
static void timer_callback(__sys_msg_t msg) REENTRANT {
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
