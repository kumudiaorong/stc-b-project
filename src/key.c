#include "key.h"

#include "def.h"
#include "detail/sys.h"
static void (*key_callback_table[__KEY_CNT][2])() = {0};
static uint8_t key_state[__KEY_CNT] = {0};
static void key_callback(uint8_t event, void (*callback)()) reentrant {
  switch(event >> 4) {
    case KEY_PRESS :
      key_callback_table[event & 0xf][0] = callback;
      break;
    case KEY_RELEASE :
      key_callback_table[event & 0xf][1] = callback;
      break;
    default :
      break;
  }
}

static void key_scan_single(uint8_t press, uint8_t idx) reentrant {
  // key_callback_node[idx] = 0;
  if(press) {
    if(key_state[idx] == 0 && key_callback_table[idx][0]) {
      // key_callback_node[0].callback = key_callback_table[idx][0];
      key_callback_table[idx][0]();
      // key_callback_node[idx] = key_callback_table[idx][0];
    }
    key_state[idx] = 20;
  } else if(key_state[idx] != 0) {
    --key_state[idx];
    if(key_state[idx] == 0 && key_callback_table[idx][1]) {
      key_callback_table[idx][1]();
      // key_callback_node[idx] = key_callback_table[idx][1];
    }
  }
}

static void (*key_callback_node[__KEY_CNT])() = {0};
// static struct sys_callback_t sys_callback = {key_callback_node, __KEY_CNT, 0};
static 
// struct sys_callback_t *
void
key_scan() reentrant {
  key_scan_single(__KEY_PRESS(__KEY_1), 0);
  key_scan_single(__KEY_PRESS(__KEY_2), 1);
  key_scan_single(__KEY_PRESS(__KEY_3), 2);
  // return &key_callback_node;
}
void key_init(void) {
  sys.key_scan = key_scan;
  sys.key_callback = key_callback;
}
uint8_t key_get(void) {
  uint8_t key = 0;
  int i = 0;
  for(; i < __KEY_CNT; ++i) {
    if(key_state[i]) {
      key |= 1 << i;
    }
  }
  return key;
}