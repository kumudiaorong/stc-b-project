#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "def.h"

#define __SEG_CNT 8

extern XDATA uint8_t display_num_decoding[16];
extern XDATA uint8_t display_num_index[__SEG_CNT];
/**
 * @brief initialize led
 *
 * @param none
 */
void display_init(void);
void display_en(uint8_t en);
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
void display_seg7(
  uint8_t seg1, uint8_t seg2, uint8_t seg3, uint8_t seg4, uint8_t seg5, uint8_t seg6, uint8_t seg7, uint8_t seg8);
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
void display_num(uint32_t num)REENTRANT;

#endif