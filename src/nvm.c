#include "nvm.h"

#include "def.h"
#include "iic.h"
__XSL_IIC_GEN_BASE(__NVM_SCL, __NVM_SDA, NOP)
__XSL_IIC_GEN_WRITE_BYTE(__NVM_SCL, __NVM_SDA, NOP)
__XSL_IIC_GEN_READ_BYTE(__NVM_SCL, __NVM_SDA, NOP)
__XSL_IIC_GEN_WRITE_ADDR_BYTE(nvm_write,__NVM_DEVICE_ADDR)
__XSL_IIC_GEN_READ_ADDR_BYTE(nvm_read,__NVM_DEVICE_ADDR)
