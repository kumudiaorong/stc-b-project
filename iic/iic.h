#ifndef XSL_IIC_H
#define XSL_IIC_H
#define __XSL_IIC_GEN_BASE(SCL, SDA, DelayFn) \
  static void __iic_start(void) {             \
    SDA = 1;                                  \
    DelayFn();                                \
    SCL = 1;                                  \
    DelayFn();                                \
    SDA = 0;                                  \
    DelayFn();                                \
    SCL = 0;                                  \
    DelayFn();                                \
  }                                           \
  static void __iic_stop(void) {              \
    SDA = 0;                                  \
    DelayFn();                                \
    SCL = 1;                                  \
    DelayFn();                                \
    SDA = 1;                                  \
    DelayFn();                                \
  }                                           \
  static uint8_t __iic_recv_ack(void) {       \
    uint8_t ret = 0;                          \
    SDA = 1;                                  \
    DelayFn();                                \
    SCL = 1;                                  \
    DelayFn();                                \
    DelayFn();                                \
    ret |= SDA;                               \
    SCL = 0;                                  \
    DelayFn();                                \
    if(ret)                                   \
      __iic_stop();                           \
    return ret;                               \
  }
#define __XSL_IIC_GEN_WRITE_BYTE(SCL, SDA, DelayFn) \
  static void __iic_write_byte(uint8_t dat) {       \
    uint8_t i = 0;                                  \
    for(; i < 8; i++) {                             \
      SDA = (dat & 0x80) >> 7;                      \
      DelayFn();                                    \
      SCL = 1;                                      \
      DelayFn();                                    \
      dat <<= 1;                                    \
      SCL = 0;                                      \
      DelayFn();                                    \
    }                                               \
  }
#define __XSL_IIC_GEN_READ_BYTE(SCL, SDA, DelayFn) \
  static uint8_t __iic_read_byte(void) {           \
    uint8_t i = 0, dat = 0;                        \
    for(; i < 8; i++) {                            \
      DelayFn();                                   \
      SCL = 1;                                     \
      DelayFn();                                   \
      dat <<= 1;                                   \
      dat |= SDA;                                  \
      SCL = 0;                                     \
    }                                              \
    return dat;                                    \
  }
#define __XSL_IIC_GEN_WRITE_ADDR_BYTE(name, DevAddr) \
  void name(uint8_t addr, uint8_t dat) {             \
    uint8_t i = 0;                                   \
    __iic_start();                                   \
    __iic_write_byte(DevAddr << 1);                  \
    if(__iic_recv_ack())                             \
      return;                                        \
    __iic_write_byte(addr);                          \
    if(__iic_recv_ack())                             \
      return;                                        \
    __iic_write_byte(dat);                           \
    __iic_recv_ack();                                \
    __iic_stop();                                    \
  }
#define __XSL_IIC_GEN_READ_ADDR_BYTE(name, DevAddr) \
  uint8_t name(uint8_t addr) {                      \
    uint8_t i = 0, dat = 0;                         \
    __iic_start();                                  \
    __iic_write_byte(DevAddr << 1);                 \
    if(__iic_recv_ack())                            \
      return 0;                                     \
    __iic_write_byte(addr);                         \
    if(__iic_recv_ack())                            \
      return 0;                                     \
    __iic_start();                                  \
    __iic_write_byte((DevAddr << 1) | 0x01);        \
    if(__iic_recv_ack())                            \
      return 0;                                     \
    dat = __iic_read_byte();                        \
    __iic_stop();                                   \
    return dat;                                     \
  }
#endif