#ifndef XSL_IIC_H
#define XSL_IIC_H
#include "def.h"
#define __XSL_IIC_GEN_BASE(SCL, SDA, DelayFn) \
  static void __iic_start(void) {             \
    SDA = 1;                                  \
    SCL = 1;                                  \
    DelayFn();                                \
    SDA = 0;                                  \
    DelayFn();                                \
    SCL = 0;                                  \
  }                                           \
  static void __iic_stop(void) {              \
    SDA = 0;                                  \
    SCL = 1;                                  \
    DelayFn();                                \
    SDA = 1;                                  \
  }
#define __XSL_IIC_GEN_WRITE_BYTE(SCL, SDA, DelayFn)        \
  static uint8_t __iic_write_byte(uint8_t dat) REENTRANT { \
    uint8_t i = 0;                                         \
    for(; i < 8; i++) {                                    \
      SDA = (dat & 0x80) >> 7;                             \
      DelayFn();                                           \
      SCL = 1;                                             \
      DelayFn();                                           \
      dat <<= 1;                                           \
      SCL = 0;                                             \
      DelayFn();                                           \
    }                                                      \
    i = 0;                                                 \
    SDA = 1;                                               \
    SCL = 1;                                               \
    DelayFn();                                             \
    i |= SDA;                                              \
    SCL = 0;                                               \
    return i;                                              \
  }
#define __XSL_IIC_GEN_READ_BYTE(SCL, SDA, DelayFn)        \
  static uint8_t __iic_read_byte(void *pdat) {            \
    uint8_t i = 0;                                        \
    for(; i < 8; i++) {                                   \
      SCL = 1;                                            \
      DelayFn();                                          \
      *(uint8_t *)pdat = ((*(uint8_t *)pdat) << 1) | SDA; \
      SCL = 0;                                            \
      DelayFn();                                          \
    }                                                     \
    SDA = 0;                                              \
    SCL = 1;                                              \
    DelayFn();                                            \
    SCL = 0;                                              \
    return 0;                                             \
  }
#define __XSL_IIC_GEN_WRITE_ADDR_BYTE(sta, name, DevAddr) \
  sta void name(uint8_t addr, uint8_t dat) {              \
    uint8_t i = 0;                                        \
    __iic_start();                                        \
    if(__iic_write_byte(DevAddr << 1))                    \
      return;                                             \
    if(__iic_write_byte(addr))                            \
      return;                                             \
    if(__iic_write_byte(dat))                             \
      return;                                             \
    __iic_stop();                                         \
  }
#define __XSL_IIC_GEN_READ_ADDR_BYTE(sta, name, DevAddr) \
  sta uint8_t name(uint8_t addr) {                       \
    uint8_t i = 0, dat = 0;                              \
    __iic_start();                                       \
    if(__iic_write_byte(DevAddr << 1))                   \
      return 0;                                          \
    if(__iic_write_byte(addr))                           \
      return 0;                                          \
    __iic_start();                                       \
    if(__iic_write_byte((DevAddr << 1) | 0x01))          \
      return 0;                                          \
    __iic_read_byte(&dat);                               \
    __iic_stop();                                        \
    return dat;                                          \
  }
#endif