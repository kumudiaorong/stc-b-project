#ifndef __RTC_H__
#define __RTC_H__
#include "def.h"
#define RTC_DEFAULT_YEAR 23
#define RTC_DEFAULT_DAY 01
#define RTC_DEFAULT_MONTH 12
#define RTC_DEFAULT_DATE 31
#define RTC_DEFAULT_HOUR 23
#define RTC_DEFAULT_MINUTE 59
#define RTC_DEFAULT_SECOND 50
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