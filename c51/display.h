#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "def.h"
SFR(P0, 0x80);

SFR(P0M0, 0x94);
SFR(P0M1, 0x93);

SBIT(P2_3, 0xA0, 3);
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
#endif