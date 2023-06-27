#include "display.h"

#include "detail/sys.h"
uint8_t display_num_decoding[16] = {
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};
uint8_t display_num_index[__SEG_CNT] = {0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
static enum __display_base __db = DISPLAY_BASE_DEC;
static uint8_t __display_seg[__SEG_CNT] = {0};
static uint8_t __display_led = 0;
static void delay() reentrant {
  int i = 600;
  while(i--)
    ;
}
static void display_schedule() reentrant {
  int i = 0;
  __SEG_EN();
  for(; i < __SEG_CNT; ++i) {
    __SEG = 0;
    __SEG_SEL = __SEG_SEL & 0xF8 | display_num_index[i];
    __SEG = __display_seg[i];
    delay();
  }
  __LED = 0;
  __LED_EN();
  __LED = __display_led;
  delay();
}
void display_init() {
  P0M0 = 0xff;
  P0M1 = 0x00;
  sys.display_schedule = display_schedule;
}

#ifdef LED_CONTINUOUS
void display_led(uint8_t num) {
  __display_led = num;
}
#else
#endif

#ifdef SEG_SEL_CONTINUOUS
void __display_sel(uint8_t pos) {
  __SEG_SEL = __SEG_SEL & 0xF8 | (pos & 0x07);
}
#else
#endif

#ifdef SEG_CONTINUOUS
void display_seg(uint8_t seg[8]) {
  int i = 0;
  for(; i < __SEG_CNT; ++i) {
    __display_seg[i] = seg[i];
  }
}
#else
#endif
void display_base(enum __display_base db) {
  __db = db;
}
void display_num(uint32_t num) {
  int i = 0;
  for(; i < __SEG_CNT; i++) {
    __display_seg[i] = display_num_decoding[num % __db];
    num /= __db;
  }
}
