#include "vib.h"

#include "detail/sys.h"
#include "string.h"
#include "sys.h"

static XDATA sys_callback_t vib_callback_table[2];
static XDATA uint8_t vib_state = 0;
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
static void vib_register(uint8_t event, sys_callback_t callback) {
  vib_callback_table[event - 1] = callback;
}
static void vib_callback(uint8_t msg) {
  vib_callback_table[msg - 1]();
}
void vib_init(void) {
  __VIB_INIT();
  memset(vib_callback_table, 0, sizeof(vib_callback_table));
  __sys_add_sensor(VIB,  vib_register,vib_scan, vib_callback);
}