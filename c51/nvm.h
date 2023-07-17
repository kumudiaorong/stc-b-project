#ifndef __NVM_H__
#define __NVM_H__
#include "def.h"

void nvm_write(uint8_t addr, uint8_t dat);
uint8_t nvm_read(uint8_t addr);
#endif