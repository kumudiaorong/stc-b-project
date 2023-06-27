#include "hall.h"

#include "detail/sys.h"

static void (*hall_callback_table[2])() = {0};
static void hall_scan() reentrant {
  static uint8_t hall_state = __HALL_INI;
  if(hall_state != __HALL) {
    hall_state = __HALL;
    if(hall_callback_table[hall_state]) {
      hall_callback_table[hall_state]();
    }
  }
}
static void hall_callback(uint8_t event, void (*callback)()) reentrant {
  switch(event) {
    case HALLGETCLOSE :
      hall_callback_table[0] = callback;
      break;
    case HALLGETAWAY :
      hall_callback_table[1] = callback;
      break;
    default :
      break;
  }
}

void hall_init(void) {
  P1M1 &= ~(1 << 2);
  P1M0 |= 1 << 2;
  sys.hall_scan = hall_scan;
  sys.hall_callback = hall_callback;
}