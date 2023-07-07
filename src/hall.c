#include "hall.h"

#include "detail/sys.h"
#include "string.h"
#include "sys.h"

static XDATA sys_callback_t hall_callback_table[2];

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
static void hall_register(uint8_t event, sys_callback_t callback) {
  hall_callback_table[event - 1] = callback;
}
static void hall_callback(uint8_t msg) {
  hall_callback_table[msg - 1]();
}
void hall_init(void) {
  __HALL_INIT();
  memset(hall_callback_table, 0, sizeof(hall_callback_table));
  __sys_add_sensor(HALL,  hall_register,hall_scan, hall_callback);
}