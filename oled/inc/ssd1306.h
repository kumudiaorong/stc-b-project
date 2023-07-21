#ifndef XSL_SSD1306_H
#define XSL_SSD1306_H
#include "def.h"
#define SSD1306_SDL
extern uint8_t CODE asc2_0806[][6];
void ssd1306_init(void);
void ssd1306_clear(void) REENTRANT;
void ssd1306_draw_bmp(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t bmp[]) REENTRANT;
void ssd1306_update(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey) REENTRANT;
void ssd1306_set_pos(uint8_t x, uint8_t y) REENTRANT;
#endif