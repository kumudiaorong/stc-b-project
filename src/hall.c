#include "hall.h"

#include "detail/sys.h"
#include "string.h"
#include "sys.h"
static uint8_t __hall_idx = 0;
static XDATA sys_callback_t hall_callback_table[2];                 //!< hall callback table
static void hall_register(uint8_t event, sys_callback_t callback);  //!< hall register function
static uint8_t hall_scan(void);                                     //!< hall scan function
static void hall_callback(uint8_t msg);                             //!< hall callback function
uint8_t hall_idx(void) {
  return __hall_idx;
}
/**
 * @fn hall_init
 * @brief hall init
 * @return none
 */
void hall_init(void) {
  __HALL_INIT();
  memset(hall_callback_table, 0, sizeof(hall_callback_table));
  __hall_idx = __sys_sensor_add(hall_register, hall_scan, hall_callback);
}
/**
 * @fn hall_register
 * @brief hall register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void hall_register(uint8_t event, sys_callback_t callback) {
  hall_callback_table[event - 1] = callback;
}
/**
 * @fn hall_scan
 * @brief hall scan
 * @return msg 1 for hall get close, 2 for hall get away
 */
static uint8_t hall_scan(void) {
  static uint8_t hall_state = __HALL_INI;
  uint8_t ret = 0;
  if(hall_state != __HALL) {
    hall_state = __HALL;
    if(hall_callback_table[hall_state]) {
      ret |= (hall_state + 1);
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
static void hall_callback(uint8_t msg) {
  hall_callback_table[msg - 1]();
}