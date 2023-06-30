#include "display.h"

#include "detail/sys.h"
XDATA uint8_t display_num_decoding[16] = {
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};
XDATA uint8_t display_num_index[__SEG_CNT] = {0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
static XDATA enum __display_base __db = DISPLAY_BASE_DEC;
static XDATA uint8_t __display_seg[__SEG_CNT] = {0};
static XDATA uint8_t __display_led = 0;
static XDATA uint8_t __display_en = 0;
static void delay(void) {
  uint16_t i = 600;
  while(i--)
    ;
}
static void display_schedule(void) {
  uint8_t i = 0;
  __SEG_EN();
  for(; i < __SEG_CNT; ++i) {
    __SEG = 0;
    __SEG_SEL = __SEG_SEL & 0xF8 | display_num_index[i];
    if(__display_en & (1 << i))
      __SEG = __display_seg[i];
    delay();
  }
  __LED = 0;
  __LED_EN();
  __LED = __display_led;
  delay();
}
void display_init(void) {
  P0M0 = 0xff;
  P0M1 = 0x00;
  P2M0 |= 0x08;
  P2M1 &= ~0x08;
 __sys.display_schedule = display_schedule;
}
void display_en(uint8_t en) {
  __display_en = en;
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
  uint8_t i = 0;
  for(; i < __SEG_CNT; ++i) {
    __display_seg[i] = seg[i];
  }
}
void display_seg7(
  uint8_t seg1, uint8_t seg2, uint8_t seg3, uint8_t seg4, uint8_t seg5, uint8_t seg6, uint8_t seg7, uint8_t seg8) {
  __display_seg[0] = seg1;
  __display_seg[1] = seg2;
  __display_seg[2] = seg3;
  __display_seg[3] = seg4;
  __display_seg[4] = seg5;
  __display_seg[5] = seg6;
  __display_seg[6] = seg7;
  __display_seg[7] = seg8;
}
#else
#endif
void display_base(enum __display_base db) {
  __db = db;
}
void display_num(uint32_t num) {
  uint8_t i = 0;
  for(; i < __SEG_CNT; i++) {
    __display_seg[i] = display_num_decoding[num % __db];
    num /= __db;
  }
}
