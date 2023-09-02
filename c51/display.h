#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "def.h"

extern uint8_t display_led;     //!< display led
extern uint8_t display_seg[8];  //!< display segment
extern uint8_t display_num_decoding[16];
/**
 * @brief initialize led
 *
 * @param none
 */
void display_init(void);
void display_area(uint8_t begin, uint8_t end);
enum __display_base {
  DISPLAY_BASE_BIN = 2,
  DISPLAY_BASE_OCT = 8,
  DISPLAY_BASE_DEC = 10,
  DISPLAY_BASE_HEX = 16,
};
extern XDATA enum __display_base display_base;  //!< display base, default 10
/**
 * @brief display a number on led segment
 *
 * @param num number to display
 * @param base number base
 */
void display_num(uint32_t num) REENTRANT;
#endif