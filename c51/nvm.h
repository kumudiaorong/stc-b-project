#ifndef __NVM_H__
#define __NVM_H__
#include "def.h"
#define __NVM_SCL P5_5
#define __NVM_SDA P4_0
#define __NVM_DEVICE_ADDR 0x50
void nvm_init(void);
void nvm_write(uint8_t addr, uint8_t dat);
uint8_t nvm_read(uint8_t addr);
#endif