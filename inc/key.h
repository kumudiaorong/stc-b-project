#ifndef __KEY_H__
#define __KEY_H__
#include "def.h"
#define CONKEY(dev, event) (dev << 1 | event)
enum KeyEvent { EventKeyPress, EventKeyReleas };
enum KeyDev { DevKey1, DevKey2 };
void key_init(void);
#endif