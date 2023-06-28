#include "key.h"

#include "def.h"
#include "detail/sys.h"
#include "string.h"
#include "sys.h"

static sys_callback_t key_callback_table[__KEY_CNT][2];
static uint8_t key_state[__KEY_CNT] = {0};
static void key_callback(uint8_t msg) REENTRANT {
  uint8_t i = 0;
  for(; i < __KEY_CNT; ++i) {
    uint8_t event = msg & (0x3 << (i << 1));
    if(event) {
      key_callback_table[i][event - 1]();
    }
  }
}
static void key_register(uint8_t event, void (*callback)(void)) REENTRANT {
  key_callback_table[event & 0x3][event >> 2] = callback;
}

static uint8_t key_scan_single(uint8_t info) REENTRANT {
  uint8_t msg = 0, idx = info >> 1;
  if(info & 0x1) {
    if(key_state[idx] == 0 && key_callback_table[idx][KEY_PRESS]) {
      msg = (KEY_PRESS + 1) << (idx << 1);
    }
    key_state[idx] = 20;
  } else if(key_state[idx] != 0) {
    --key_state[idx];
    if(key_state[idx] == 0 && key_callback_table[idx][KEY_RELEASE]) {
      msg = (KEY_RELEASE + 1) << (idx << 1);
    }
  }
  return msg;
}

static uint8_t key_scan(void) REENTRANT {
  uint8_t ret = 0;
  ret |= key_scan_single(__KEY_PRESS(__KEY_1));
  ret |= key_scan_single(__KEY_PRESS(__KEY_2) | 0x2);
  ret |= key_scan_single(__KEY_PRESS(__KEY_3) | 0x4);
  return ret;
}
void key_init(void) {
  memset(key_callback_table, 0, sizeof(key_callback_table));
  __sys_add_sensor(KEY, key_scan, key_register, key_callback);
}
// uint8_t key_get(void) {
//   uint8_t key = 0;
//   int i = 0;
//   for(; i < __KEY_CNT; ++i) {
//     if(key_state[i]) {
//       key |= 1 << i;
//     }
//   }
//   return key;
// }