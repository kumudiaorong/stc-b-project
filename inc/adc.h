#ifndef __ADC_H__
#define __ADC_H__
#include "def.h"

SBIT(EADC, 0xA8, 5);
SFR(P1ASF, 0x9D);  // 0000,0000 端口1模拟功能配置寄存器
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

enum NavEvent {
  EventNavPress,
  EventNavRelease,
};
enum NavDev {
  DevNavKey3,
  DevNavRight,
  DevNavDown,
  DevNavCenter,
  DevNavLeft,
  DevNavUp,
};
#define CONNAV(dev, event) (dev << 1 | event)

typedef struct {
  int8_t rt;
  uint16_t rop;
  uint8_t nav;
} adc_t;
extern XDATA adc_t adc;
void adc_init(void);
#endif