#include "display.h"

#include "detail/sys.h"


#define __DISPLAY_INIT() __DO_WHILE0(P0M0 = 0xff; P0M1 = 0x00; P2M0 |=(1<<3); P2M1 &= ~(1<<3))

#define __LED_EN() __DO_WHILE0(P2_3 = 1)

#define __LED_SET(val) __DO_WHILE0(P0 = (val))

#define __SEG_EN() __DO_WHILE0(P2_3 = 0)

#define __SEG_SET(seg) __DO_WHILE0(P0 = (seg))

#define __SEG_SEL_SET(sel) __DO_WHILE0(P2 = P2 & 0xf8 | (sel))

static void display_schedule(void);//!< display schedule
/**
 * @fn display_init
 * @brief display init
 * @return none
 */
void display_init(void) {
  __DISPLAY_INIT();
  __sys_schedule_add(display_schedule);
}
static XDATA uint8_t __display_en = 0;  //!< display enable
/**
 * @fn display_en
 * @brief display enable
 * @param en 
 * @return none
 */
void display_en(uint8_t en) {
  __display_en = en;
}
static XDATA uint8_t __display_led = 0;  //!< display led
/**
 * @fn display_led
 * @brief display led
 * @param num 
 * @return none
 */
void display_led(uint8_t num) {
  __display_led = num;
}

static XDATA uint8_t __display_seg[__SEG_CNT] = {0};  //!< display segment
/**
 * @fn display_seg
 * @brief display segment
 * @param seg 
 * @return none
 */
void display_seg(uint8_t seg[8]) {
  uint8_t i = 0;
  for(; i < __SEG_CNT; ++i) {
    __display_seg[i] = seg[i];
  }
}
/**
 * @fn display_seg7
 * @brief display segment
 * @param seg1 
 * @param seg2 
 * @param seg3 
 * @param seg4 
 * @param seg5 
 * @param seg6 
 * @param seg7 
 * @param seg8 
 * @return none
 */
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
static XDATA enum __display_base __db = DISPLAY_BASE_DEC;  //!< display base
/**
 * @fn display_base
 * @brief display base
 * @param db 
 * @return none
 */
void display_base(enum __display_base db) {
  __db = db;
}
/**
 * @fn display_num
 * @brief display number
 * @param num 
 * @return none
 */
void display_num(uint32_t num)REENTRANT {
  uint8_t i = 0;
  for(; i < __SEG_CNT; i++) {
    __display_seg[i] = display_num_decoding[num % __db];
    num /= __db;
  }
}
/**
 * @fn display_schedule
 * @brief display schedule
 * @return none
 */
static void delay(void) {
  uint16_t i = 600;
  while(i--)
    ;
}
XDATA uint8_t display_num_decoding[16] = {
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};  //!< decoding table
XDATA uint8_t display_num_index[__SEG_CNT] = {0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};      //!< index table
/**
 * @fn display_schedule
 * @brief display schedule
 * @return none
 */
static void display_schedule(void) {
  uint8_t i = 0;
  __SEG_EN();
  for(; i < __SEG_CNT; ++i) {
    __SEG_SET(0);
    __SEG_SEL_SET(display_num_index[i]);
    if(__display_en & (1 << i))
      __SEG_SET(__display_seg[i]);
    delay();
  }
  __LED_SET(0);
  __LED_EN();
  __LED_SET(__display_led);
  delay();
}
