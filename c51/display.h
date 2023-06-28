#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "def.h"
#define SEG_CONTINUOUS
#define SEG_SEL_CONTINUOUS
#define LED_CONTINUOUS

#define __LED_0 P0_0
#define __LED_1 P0_1
#define __LED_2 P0_2
#define __LED_3 P0_3
#define __LED_4 P0_4
#define __LED_5 P0_5
#define __LED_6 P0_6
#define __LED_7 P0_7

#define __LED_EN() \
  do {             \
    P2_3 = 1;      \
  } while(0)
#define __LED_DIS() \
  do {              \
    P2_3 = 0;       \
  } while(0)

#ifdef LED_CONTINUOUS
#define __LED P0
#else
#endif

#define __SEG_A P0_0
#define __SEG_B P0_1
#define __SEG_C P0_2
#define __SEG_D P0_3
#define __SEG_E P0_4
#define __SEG_F P0_5
#define __SEG_G P0_6
#define __SEG_H P0_7

#define __SEG_CNT 8
#define __SEG_SEL_0 P2_0
#define __SEG_SEL_1 P2_1
#define __SEG_SEL_2 P2_2

#define __SEG_EN() \
  do {             \
    P2_3 = 0;      \
  } while(0)
#define __SEG_DIS() \
  do {              \
    P2_3 = 1;       \
  } while(0)

#ifdef SEG_CONTINUOUS
#define __SEG P0
#else
#endif

#ifdef SEG_SEL_CONTINUOUS
#define __SEG_SEL P2
#else
#endif

extern XDATA uint8_t display_num_decoding[16];
extern XDATA uint8_t display_num_index[__SEG_CNT];
/**
 * @brief initialize led
 *
 * @param none
 */
void display_init(void);
/**
 * @brief display a number on led
 *
 * @param num number to display
 */
void display_led(uint8_t num);
/**
 * @brief display a number on led segment
 *
 * @param num number to display
 */
void display_seg(uint8_t seg[8]);
enum __display_base {
  DISPLAY_BASE_BIN = 2,
  DISPLAY_BASE_OCT = 8,
  DISPLAY_BASE_DEC = 10,
  DISPLAY_BASE_HEX = 16,
};
/**
 * @brief set display base
 *
 * @param db display base
 */
void display_base(enum __display_base db);
/**
 * @brief display a number on led segment
 *
 * @param num number to display
 * @param base number base
 */
void display_num(uint32_t num);
/**
 * @brief display a number on led
 *
 * @param num number to display
 */
#endif