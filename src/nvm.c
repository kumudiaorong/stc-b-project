#include "nvm.h"

#include "def.h"
#include "iic.h"

#define __NVM_SCL P5_5
#define __NVM_SDA P4_0
#define __NVM_DEVICE_ADDR 0x50

__XSL_IIC_GEN_BASE(__NVM_SCL, __NVM_SDA, NOP)
__XSL_IIC_GEN_WRITE_BYTE(__NVM_SCL, __NVM_SDA, NOP)
__XSL_IIC_GEN_READ_BYTE(__NVM_SCL, __NVM_SDA, NOP)
__XSL_IIC_GEN_WRITE_ADDR_BYTE(nvm_write,__NVM_DEVICE_ADDR)
__XSL_IIC_GEN_READ_ADDR_BYTE(nvm_read,__NVM_DEVICE_ADDR)
