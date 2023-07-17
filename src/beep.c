#include "beep.h"

#include "def.h"
#include "detail/sys.h"

#define __BEEP_VECTOR 7
#define __BEEP P3_4

static uint16_t step;
void beep_init(void) {
  CCAPM1 |= 0x48;
  CMOD |= 0x8;
  PPCA = 1;
  P3M0 |= 1 << 4;
  P3M1 &= ~(1 << 4);
  __BEEP = 0;
  step = (65536 - __sysclk / 6 / __BEEP_DEFAULT_FREQ);
}
void beep_on(void) {
  CCAPM1 |= 0x01;
  CCAP1H = step >> 8;
  CCAP1L = step & 0xff;
}
void beep_off(void) {
  CCAPM1 &= ~0x01;
  __BEEP = 0;
}
void beep_freq(uint16_t freq) REENTRANT {
  step = (65536 - __sysclk / 6 / freq);
}
INTERRUPT_USING(__beep, __BEEP_VECTOR, 1) {
  __BEEP = !__BEEP;
  CCAP1L = (((CCAP1H * 256) + CCAP1L) + step) & 0xff;
  CCAP1H = (((CCAP1H * 256) + CCAP1L) + step) >> 8;
  CCF1 = 0;
}