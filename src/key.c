#include "key.h"

#include "def.h"
#include "detail/sys.h"
#include "sys.h"
static uint8_t __key_idx = 0;
static XDATA sys_callback_t key_callback_table[__KEY_CNT][2] = {{0}, {0}, {0}};  //!< key callback table
static XDATA uint8_t key_states = 0;  //!< key states,bit 0-2 for key1-3, 1 for press, 0 for release
static void key_register(uint8_t event, sys_callback_t callback);  //!< key register function

static void key_callback(uint8_t msg) REENTRANT;  //!< key callback function
uint8_t key_idx(void) {
  return __key_idx;
}
/**
 * @fn key_init
 * @brief key init
 * @return none
 */
void key_init(void) {
  __KEY_INIT();
  __key_idx = __sys_sensor_add(key_register,
#ifdef __KEY_USE_INTERRUPT
    0
#endif
#ifdef __KEY_USE_POLLING
    key_scan
#endif
    ,
    key_callback);
  // __sys_sensor_add(KEY, key_scan, key_register, key_callback);
}
/**
 * @fn key_get
 * @brief get key state
 * @return key state
 */
uint8_t key_get(void) {
  return key_states;
}
static XDATA uint8_t key_state[__KEY_CNT] = {0};  //!< key state
/**
 * @fn __key##idx
 * @brief key interrupt function
 * @return none
 */
#ifdef __KEY_USE_INTERRUPT
#define __KEY_BY_INT(idx, vec)                                                            \
  INTERRUPT(__key##idx, vec) {                                                            \
    static uint32_t lastT = 0;                                                            \
    if(!lastT || __sys_timer_cnt - lastT > 20) {                                          \
      if(key_callback_table[idx][(key_states >> idx) & 1]) {                              \
        __sys_sensor_set_msg(__key_idx, ((((key_states >> idx) & 1) + 1) << (idx << 1))); \
      }                                                                                   \
      key_states ^= 1;                                                                    \
    }                                                                                     \
    lastT = __sys_timer_cnt;                                                              \
  }
__KEY_BY_INT(0, IE0_VECTOR)
__KEY_BY_INT(1, IE1_VECTOR)
#undef __KEY_BY_INT
#endif
/**
 * @fn key_register
 * @brief key register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void key_register(uint8_t event, sys_callback_t callback) {
  key_callback_table[event & 0x3][(event >> 2) - 1] = callback;
}
/**
 * @fn key_callback
 * @brief key callback
 * @param msg msg type
 * @return none
 */
static void key_callback(uint8_t msg) REENTRANT {
  uint8_t i = 0;
  for(; i < __KEY_CNT; ++i) {
    uint8_t event = (msg >> (i << 1)) & 0x3;
    if(event) {
      key_callback_table[i][event - 1]();
    }
  }
}
#ifdef __KEY_USE_POLLING
static uint8_t key_scan_single(uint8_t info) {
  uint8_t msg = 0, idx = info >> 1;
  if(info & 0x1) {
    if(key_state[idx] == 0 && key_callback_table[idx][0]) {
      key_states |= 1 << idx;
      msg = KEYPRESS << (idx << 1);
    }
    key_state[idx] = 20;
  } else if(key_state[idx] != 0) {
    --key_state[idx];
    if(key_state[idx] == 0 && key_callback_table[idx][1]) {
      key_states &= ~(1 << idx);
      msg = KEYRELEASE << (idx << 1);
    }
  }
  return msg;
}

static uint8_t key_scan(void) {
  uint8_t ret = 0;
  ret |= key_scan_single(__KEY_PRESS(__KEY_1));
  ret |= key_scan_single(__KEY_PRESS(__KEY_2) | 0x2);
  ret |= key_scan_single(__KEY_PRESS(__KEY_3) | 0x4);
  return ret;
}
#endif
