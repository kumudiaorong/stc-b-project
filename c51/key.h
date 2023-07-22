#ifndef __KEY_H__
#define __KEY_H__
#include "def.h"
#define __KEY_PRESS(x) (x == 0)
#define CFGKEY(key, event) (key << 2 | event)

enum KeyEvent { KEYPRESS, KEYRELEASE, KEYNONE };
enum KeyNum { KEY1, KEY2, KEY3 };
void key_init(void);
uint8_t key_get(void);
extern uint8_t KEY;
#endif