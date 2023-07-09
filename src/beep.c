#include "beep.h"

#include "detail/sys.h"

void beep_init(void) {
  TH1 = (65535 - __sysclk / 12 / 500) >> 8;
  TL1 = (65535 - __sysclk / 12 / 500) & 0xff;
  IE |= 0x8;
  IP |= 0x8;
  __BEEP_INIT();
}
static uint16_t __freq = 1000;
static bit beep_state = 0;
void beep_on(void) {
  beep_state = 1;
}
void beep_off(void) {
  beep_state = 0;
  __BEEP = 0;
}
void beep_freq(uint16_t freq) {
  TH1 = (65535 - __sysclk / 12 / (freq / 2)) >> 8;
  TL1 = (65535 - __sysclk / 12 / (freq / 2)) & 0xff;
}
INTERRUPT_USING(__beep, TF1_VECTOR, 1) {
  if(beep_state) {
    __BEEP = !__BEEP;
  }
}