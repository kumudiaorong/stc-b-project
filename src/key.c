#include "key.h"

#include "def.h"
#include "detail/sys.h"
#include "string.h"
#include "sys.h"

static sys_callback_t key_callback_table[__KEY_CNT][2];
static uint8_t key_states;
static uint8_t key_state[__KEY_CNT] = {0};

#ifdef __KEY_USE_INTERRUPT
#define __KEY_BY_INT(idx, vec)                                                  \
  INTERRUPT(__key##idx, vec) {                                                  \
    static uint32_t last = 0;                                                   \
    if(!last || __sys_timer_cnt - last > 20) {                                    \
      if(key_callback_table[idx][(key_states >> idx) & 1])                      \
       __sys.sensor[KEY].msg |= ((((key_states >> idx) & 1) + 1) << (idx << 1)); \
      key_states ^= 1;                                                          \
    }                                                                           \
    last = __sys_timer_cnt;                                                       \
  }
__KEY_BY_INT(0, IE0_VECTOR)
__KEY_BY_INT(1, IE1_VECTOR)
#undef __KEY_BY_INT
#endif
static void key_callback(uint8_t msg) REENTRANT {
  uint8_t i = 0;
  for(; i < __KEY_CNT; ++i) {
    uint8_t event = (msg >> (i << 1)) & 0x3;
    if(event) {
      key_callback_table[i][event - 1]();
    }
  }
}
static void key_register(uint8_t event, void (*callback)(void)) {
  key_callback_table[event & 0x3][(event >> 2) - 1] = callback;
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
void key_init(void) {
#ifdef __KEY_USE_INTERRUPT
  IE |= 0x5;
  // EX1 = 1;
  // EX0 = 1;
  TCON &= ~0x5;
  // IT1 = 0;
  // IT0 = 0;
  IP |= 0x5;
  // PX1 = 1;
  // PX0 = 1;
#endif
  memset(key_callback_table, 0, sizeof(key_callback_table));
  __sys_add_sensor(KEY,
#ifdef __KEY_USE_INTERRUPT
    0
#endif
#ifdef __KEY_USE_POLLING
    key_scan
#endif
    ,
    key_register, key_callback);
  // __sys_add_sensor(KEY, key_scan, key_register, key_callback);
}
uint8_t key_get(void) {
  return key_states;
}