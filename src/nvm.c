#include "nvm.h"

#include "def.h"
void nvm_init(void) {
  __NVM_SCL = 1;
  __NVM_SDA = 1;
}
static void start(void) {
  __NVM_SDA = 1;
  NOP();
  __NVM_SCL = 1;
  NOP();
  __NVM_SDA = 0;
  NOP();
  __NVM_SCL = 0;
  NOP();
}
static void stop(void) {
  __NVM_SDA = 0;
  NOP();
  __NVM_SCL = 1;
  NOP();
  __NVM_SDA = 1;
  NOP();
}
static void write_byte(uint8_t dat) {
  uint8_t i = 0;
  for(; i < 8; i++) {
    __NVM_SDA = (dat & 0x80) >> 7;
    NOP();
    __NVM_SCL = 1;
    NOP();
    dat <<= 1;
    __NVM_SCL = 0;
    NOP();
  }
}
static uint8_t rece_ack(void) {
  uint8_t i = 0;
  __NVM_SDA = 1;
  NOP();
  __NVM_SCL = 1;
  while(__NVM_SDA == 1 && i < 250) {
    i++;
  }
  __NVM_SCL = 0;
  NOP();
  return i < 250 ? 1 : 0;
}
static uint8_t read_byte(void) {
  uint8_t i = 0, dat = 0;
  for(; i < 8; i++) {
    NOP();
    __NVM_SCL = 1;
    NOP();
    dat <<= 1;
    dat |= __NVM_SDA;
    __NVM_SCL = 0;
  }
  return dat;
}
#define __ASSERT_STOP(expr, ret) \
  if(!(expr)) {                  \
    stop();                      \
    return ret;                  \
  }
void nvm_write(uint8_t addr, uint8_t dat) {
  uint8_t i = 0;
  start();
  write_byte(__NVM_DEVICE_ADDR << 1);
  __ASSERT_STOP(rece_ack(), );
  write_byte(addr);
  __ASSERT_STOP(rece_ack(), );
  write_byte(dat);
  rece_ack();
  stop();
}
uint8_t nvm_read(uint8_t addr) {
  uint8_t i = 0, dat = 0;
  start();
  write_byte(__NVM_DEVICE_ADDR << 1);
  __ASSERT_STOP(rece_ack(), dat);
  write_byte(addr);
  __ASSERT_STOP(rece_ack(), dat);
  start();
  write_byte((__NVM_DEVICE_ADDR << 1) | 0x01);
  __ASSERT_STOP(rece_ack(), dat);
  dat = read_byte();
  stop();
  return dat;
}
#undef __ASSERT_STOP
