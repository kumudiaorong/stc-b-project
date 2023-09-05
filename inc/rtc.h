#ifndef __RTC_H__
#define __RTC_H__
#include "def.h"
// sbit RTC_sclk=P1^5;//时钟控制引脚，控制数据的输入输出
// sbit RTC_rst=P1^6;//CE引脚，读写时必须置高电平
// sbit RTC_io=P5^4;//数据引脚
typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t date;
  uint8_t hour;//bit7: 0-24h, 1-12h
  uint8_t minute;
  uint8_t second;
} rtc_t;
extern rtc_t rtc;
SBIT(RTC_SCLK, 0x90, 5);
SBIT(RTC_RST, 0x90, 6);
SBIT(RTC_IO, 0xC8, 4);
void rtc_init(void);
void rtc_read(void);
void rtc_write(void);
void rtc_charge(void);
void rtc_nvm_write(uint8_t addr, uint8_t dat);
uint8_t rtc_nvm_read(uint8_t addr);
#endif