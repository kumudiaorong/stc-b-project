#ifndef __KEY_H__
#define __KEY_H__
#include "def.h"
#define __KEY_1 P3_2
#define __KEY_2 P3_3
#define __KEY_3 P1_7
#define __KEY_CNT 3
#define __KEY_PRESS(x) (x == 0)
#define CONKEY(event, key) ((event << 2) | key)
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
enum KeyEvent { KEYNONE = 0, KEYPRESS = 1, KEYRELEASE = 2 };
enum KeyNum { KEY1 = 0, KEY2 = 1, KEY3 = 2 };
void key_init(void);
uint8_t key_get(void);
extern uint8_t KEY;
#endif