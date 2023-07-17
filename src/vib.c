#include "vib.h"

#include "detail/sys.h"
#include "sys.h"

#define GET_VIB() (uint8_t)(P2_4 == 0)
#define __VIB_MASK (0x1 << ((sizeof(__sys_msg_t) << 3) - 1))
uint8_t VIB = 0;
static XDATA sys_callback_t vib_callback_table[2] = {0};          //!< vib callback table
static XDATA uint8_t vib_state = 0;                               //!< vib state
static void vib_register(uint32_t cfg, sys_callback_t callback);  //!< vib register function
static uint8_t vib_scan(void);                                    //!< vib scan function
static void vib_callback(uint8_t msg);                            //!< vib callback function
/**
 * @fn vib_init
 * @brief vib init
 * @return none
 */
void vib_init(void) {
  P2M0 &= ~(1 << 4);
  P2M1 |= 1 << 4;
  P2_4 = 1;  //__VIB_INIT();
  VIB = __sys_sensor_add(vib_register, vib_scan, vib_callback);
}
/**
 * @fn vib_register
 * @brief vib register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void vib_register(uint32_t cfg, sys_callback_t callback) {
  vib_callback_table[cfg] = callback;
}
/**
 * @fn vib_scan
 * @brief vib scan
 * @return msg 1 for vib start, 2 for vib stop
 */
static __sys_msg_t vib_scan(void) {
  __sys_msg_t ret = 0;
  if(GET_VIB()) {
    if(vib_state == 0 && vib_callback_table[0]) {
      ret |= VIBSTART | __VIB_MASK;
    }
    vib_state = 20;
  } else if(vib_state != 0) {
    --vib_state;
    if(vib_state == 0 && vib_callback_table[1]) {
      ret |= VIBSTOP | __VIB_MASK;
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
static void vib_callback(__sys_msg_t msg) {
  vib_callback_table[msg & ~__VIB_MASK]();
}