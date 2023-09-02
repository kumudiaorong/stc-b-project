#ifndef __ADC_H__
#define __ADC_H__
#include "def.h"

SBIT(EADC, 0xA8, 5);

/**
 * @brief adc control register
 * @note
 * |bit|name|description|
 * |---|----|-----------|
 * |7|ADC_POWER|A/Dת����Դ����λ,0:�ر�,1:��|
 * |6|SPEED1|A/Dת���ٶȿ���λ|
 * |5|SPEED0|A/Dת���ٶȿ���λ|
 * |4|ADC_FLAG|A/Dת����ɱ�־λ,0:δ���,1:���|
 * |3|ADC_START|A/Dת����������λ,0:ֹͣ,1:����|
 * |2|CHS2|A/Dת��ͨ��ѡ��λ|
 * |1|CHS1|A/Dת��ͨ��ѡ��λ|
 * |0|CHS0|A/Dת��ͨ��ѡ��λ|
 */

SFR(ADC_CONTR, 0xBC);  // 0000,0000 A/Dת�����ƼĴ���

SFR(ADC_RES, 0xBD);
SFR(ADC_RESL, 0xBE);

enum NavEvent {
  EventNavPress,
  EventNavRelease,
  NAVNONE = 7,
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
  uint8_t rt;
  uint8_t rop;
  uint8_t nav;
} adc_t;
extern XDATA adc_t adcs;
void adc_init(void);
#endif