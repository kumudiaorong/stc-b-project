#include "hall.h"

#include "detail/sys.h"
#include "sys.h"
#include"string.h"
static sys_callback_t hall_callback_table[2];

static uint8_t hall_scan(void) REENTRANT {
  static uint8_t hall_state = __HALL_INI;
  uint8_t ret = 0;
  if(hall_state != __HALL) {
    hall_state = __HALL;
    ret |= (hall_state + 1);
  }
  return ret;
}
static void hall_register(uint8_t event, sys_callback_t callback) REENTRANT {
  hall_callback_table[event] = callback;
}
static void hall_callback(uint8_t msg) REENTRANT {
  if(hall_callback_table[msg-1]) {
    hall_callback_table[msg-1]();
  }
}
void hall_init(void) {
  P1M1 &= ~(1 << 2);
  P1M0 |= 1 << 2;
  memset(hall_callback_table, 0, sizeof(hall_callback_table));
  __sys_add_sensor(HALL, hall_scan, hall_register, hall_callback);
}