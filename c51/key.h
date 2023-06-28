#ifndef __KEY_H__
#define __KEY_H__
#include "def.h"
#define __KEY_1 P3_2
#define __KEY_2 P3_3
#define __KEY_3 P1_7
#define __KEY_CNT 3
#define __KEY_PRESS(x) (x == 0)
enum KeyEvent { KEY_PRESS, KEY_RELEASE };
void key_init(void);
uint8_t key_get(void);
#endif