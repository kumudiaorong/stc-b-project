#ifndef __KEY_H__
#define __KEY_H__
#include "def.h"
#define __KEY_1 P3_2
#define __KEY_2 P3_3
#define __KEY_3 P1_7
#define __KEY_CNT 3
#define __KEY_PRESS(x) (x == 0)
#define CFGKEY(key, event) (key << 2 | event)
/**
 * @brief use interrupt to detect key
 *
 */
#define __KEY_USE_INTERRUPT  //
// #define __KEY_USE_POLLING//

#ifdef __KEY_USE_INTERRUPT
/**
 * @brief
 *
 *  IE |= 0x5;
 *  =EX1 = 1;
 *  =EX0 = 1;
 *  TCON &= ~0x5;
 *  =IT1 = 0;
 *  =IT0 = 0;
 *  IP |= 0x5;
 *  =PX1 = 1;
 *  =PX0 = 1;
 */
#define __KEY_INIT() __DO_WHILE0(IE |= 0x5; TCON &= ~0x5; IP |= 0x5)
#endif
#ifdef __KEY_USE_POLLING
#define __KEY_INIT()
#endif
enum KeyEvent { KEYPRESS, KEYRELEASE, KEYNONE };
enum KeyNum { KEY1, KEY2, KEY3 };
void key_init(void);
uint8_t key_get(void);
extern uint8_t KEY;
#endif