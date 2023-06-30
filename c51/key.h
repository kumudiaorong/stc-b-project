#ifndef __KEY_H__
#define __KEY_H__
#include "def.h"
#define __KEY_1 P3_2
#define __KEY_2 P3_3
#define __KEY_3 P1_7
#define __KEY_CNT 3
#define __KEY_PRESS(x) (x == 0)
/**
 * @brief use interrupt to detect key
 * 
 */
#define __KEY_USE_INTERRUPT//
// #define __KEY_USE_POLLING//
enum KeyEvent { KEYNONE = 0, KEYPRESS = 1, KEYRELEASE = 2 };
void key_init(void);
uint8_t key_get(void);
#endif