#ifndef __ADC_H__
#define __ADC_H__
#include "def.h"
#define ADC_VECTOR 5
#define ADC_INT_PRIORITY 1
#define CFGNAV(key, event) (key << 1 | event)
#define __ADC_USE_INTERRUPT  //

#ifdef __ADC_USE_INTERRUPT
SBIT(EADC, 0xA8, 5);
#endif

/**
 * @brief adc control register
 * @note
 * |bit|name|description|
 * |---|----|-----------|
 * |7|ADC_POWER|A/D转换电源控制位,0:关闭,1:打开|
 * |6|SPEED1|A/D转换速度控制位|
 * |5|SPEED0|A/D转换速度控制位|
 * |4|ADC_FLAG|A/D转换完成标志位,0:未完成,1:完成|
 * |3|ADC_START|A/D转换启动控制位,0:停止,1:启动|
 * |2|CHS2|A/D转换通道选择位|
 * |1|CHS1|A/D转换通道选择位|
 * |0|CHS0|A/D转换通道选择位|
 */

SFR(ADC_CONTR, 0xBC);  // 0000,0000 A/D转换控制寄存器

SFR(ADC_RES, 0xBD);
SFR(ADC_RESL, 0xBE);
#ifdef __ADC_USE_INTERRUPT
#define __ADC_INIT() __DO_WHILE0(EADC = 1; P1ASF = 0x98)
#else
#define __ADC_INIT() __DO_WHILE0(P1ASF = 0x98)
#endif
#define __ADC_RT 0x3
#define __ADC_ROP 0x4
#define __ADC_NAV 0x7
#define __ADC_START(channel) __DO_WHILE0(ADC_RES = 0; ADC_RESL = 0; ADC_CONTR = (0x88 | (channel)))
// #define __ADC_CLEAR() __DO_WHILE0(ADC_CONTR = ADC_CONTR & ~0x10; ADC_RES = 0; ADC_RESL = 0)
#define __ADC_GET() ((ADC_RES << 2) + (ADC_RESL >> 6))
#define __ADC_GET_HIGH() (ADC_RES)
enum AdcChannel {
  ADCRT = 0x3,
  ADCROP = 0x4,
  ADCNAV = 0x7,
};
enum NavEvent {
  NAVPRESS,
  NAVRELEASE,
  NAVNONE = 7,
};
enum NavNum {
  NAVKEY3,
  NAVRIGHT,
  NAVDOWN,
  NAVCENTER,
  NAVLEFT,
  NAVUP,
};

typedef struct {
  uint8_t rt;
  uint8_t rop;
  uint8_t nav;
} adc_t;
extern XDATA adc_t adcs;
void adc_init(void);
extern uint8_t ADC;
#endif