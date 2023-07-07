#include "vib.h"

#include "detail/sys.h"
#include "string.h"
#include "sys.h"

static XDATA sys_callback_t vib_callback_table[2];//!< vib callback table
static XDATA uint8_t vib_state = 0;//!< vib state
static void vib_register(uint8_t event, sys_callback_t callback);//!< vib register function
static uint8_t vib_scan(void);//!< vib scan function
static void vib_callback(uint8_t msg);//!< vib callback function
/**
 * @fn vib_init
 * @brief vib init
 * @return none
 */
void vib_init(void) {
  __VIB_INIT();
  memset(vib_callback_table, 0, sizeof(vib_callback_table));
  __sys_sensor_add(VIB,  vib_register,vib_scan, vib_callback);
}
/**
 * @fn vib_register
 * @brief vib register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void vib_register(uint8_t event, sys_callback_t callback) {
  vib_callback_table[event - 1] = callback;
}
/**
 * @fn vib_scan
 * @brief vib scan
 * @return msg 1 for vib start, 2 for vib stop
 */
static uint8_t vib_scan(void) {
  uint8_t ret = 0;
  if(GET_VIB()) {
    if(vib_state == 0 && vib_callback_table[0]) {
      ret |= VIBSTART;
    }
    vib_state = 20;
  } else if(vib_state != 0) {
    --vib_state;
    if(vib_state == 0 && vib_callback_table[1]) {
      ret |= VIBSTOP;
    }
  }
  return ret;
}
/**
 * @fn vib_callback
 * @brief vib callback
 * @param msg msg type
 * @return none
 */
static void vib_callback(uint8_t msg) {
  vib_callback_table[msg - 1]();
}