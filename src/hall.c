#include "hall.h"

#include "detail/sys.h"
#include "sys.h"

#define __HALL P1_2
#define __HALL_INI 1

uint8_t HALL = 0;
static XDATA sys_callback_t hall_callback_table[2] = {0};          //!< hall callback table
static void hall_register(uint32_t cfg, sys_callback_t callback);  //!< hall register function
static uint8_t hall_scan(void);                                    //!< hall scan function
static void hall_callback(uint8_t msg);                            //!< hall callback function
/**
 * @fn hall_init
 * @brief hall init
 * @return none
 */
void hall_init(void) {
  P1M0 |= 1 << 2;  //__HALL_INIT()
  P1M1 &= ~(1 << 2);
  HALL = __sys_sensor_add(hall_register, hall_scan, hall_callback);
}
/**
 * @fn hall_register
 * @brief hall register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void hall_register(uint32_t cfg, sys_callback_t callback) {
  hall_callback_table[cfg] = callback;
}
/**
 * @fn hall_scan
 * @brief hall scan
 * @return msg 1 for hall get close, 2 for hall get away
 */
static __sys_msg_t hall_scan(void) {
  static bit hall_state = __HALL_INI;
  __sys_msg_t ret = 0;
  if(hall_state != __HALL) {
    hall_state = __HALL;
    if(hall_callback_table[hall_state]) {
      ret |= hall_state | __MSG_MASK;
    }
  }
  return ret;
}
/**
 * @fn hall_callback
 * @brief hall callback
 * @param msg msg type
 * @return none
 */
static void hall_callback(__sys_msg_t msg) {
  hall_callback_table[msg & (~__MSG_MASK)]();
}