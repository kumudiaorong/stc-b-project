#include "rtc.h"

#include "def.h"
#include "sys.h"
#define __RTC_SECOND 0x80
#define __RTC_MINUTE 0x82
#define __RTC_HOUR 0x84
#define __RTC_DATE 0x86
#define __RTC_MONTH 0x88
#define __RTC_DAY 0x8A
#define __RTC_YEAR 0x8C
#define __RTC_WP 0x8E
#define __RTC_WRITE(what) (what)
#define __RTC_READ(what) (what | 0x01)

rtc_t rtc = {0};
static void write_byte(uint8_t dat) {
  uint8_t i = 0;
  RTC_SCLK = 0;
  for(; i < 8; i++) {
    RTC_IO = dat & 0x01;
    RTC_SCLK = 1;
    NOP();
    RTC_SCLK = 0;
    dat >>= 1;
  }
}
static uint8_t read_byte(void) {
  uint8_t i = 0, dat = 0;
  for(; i < 8; i++) {
    dat >>= 1;
    if(RTC_IO == 1) {
      dat |= 0x80;
    }
    RTC_SCLK = 1;
    NOP();
    RTC_SCLK = 0;
  }
  return dat;
}
static void write(uint8_t addr, uint8_t dat) {
  write_byte(__RTC_WRITE(addr));
  write_byte(dat);
}
static uint8_t read(uint8_t addr) {
  uint8_t dat = 0;
  write_byte(__RTC_READ(addr));
  dat = read_byte();
  return dat;
}
void rtc_init(void) {
  RTC_SCLK = 0;
  rtc.year = RTC_DEFAULT_YEAR;
  rtc.month = RTC_DEFAULT_MONTH;
  rtc.day = RTC_DEFAULT_DAY;
  rtc.date = RTC_DEFAULT_DATE;
  rtc.hour = RTC_DEFAULT_HOUR;
  rtc.minute = RTC_DEFAULT_MINUTE;
  rtc.second = RTC_DEFAULT_SECOND;
  rtc_write();
}
void rtc_read(void) {
  uint8_t readval = 0;
  RTC_RST = 1;
  readval = read(__RTC_SECOND);
  rtc.second = ((readval & 0x70) >> 4) * 10 + (readval & 0x0F);
  readval = read(__RTC_MINUTE);
  rtc.minute = ((readval & 0x70) >> 4) * 10 + (readval & 0x0F);
  readval = read(__RTC_HOUR);
  rtc.hour = ((readval & 0x30) >> 4) * 10 + (readval & 0x0F);
  readval = read(__RTC_DATE);
  rtc.date = ((readval & 0x30) >> 4) * 10 + (readval & 0x0F);
  readval = read(__RTC_DAY);
  rtc.day = readval & 0x07;
  readval = read(__RTC_MONTH);
  rtc.month = ((readval & 0x10) >> 4) * 10 + (readval & 0x0F);
  readval = read(__RTC_YEAR);
  rtc.year = ((readval & 0xF0) >> 4) * 10 + (readval & 0x0F);
  RTC_RST = 0;
}
void rtc_write(void) {
  RTC_RST = 1;
  write(__RTC_WP, 0x00);
  write(__RTC_SECOND, 0x80);
  write(__RTC_SECOND, 0x00 | ((rtc.second / 10) << 4) | (rtc.second % 10));
  write(__RTC_MINUTE, ((rtc.minute / 10) << 4) | (rtc.minute % 10));
  write(__RTC_HOUR, ((rtc.hour / 10) << 4) | (rtc.hour % 10));
  write(__RTC_DATE, ((rtc.date / 10) << 4) | (rtc.date % 10));
  write(__RTC_DAY, rtc.day);
  write(__RTC_MONTH, ((rtc.month / 10) << 4) | (rtc.month % 10));
  write(__RTC_YEAR, ((rtc.year / 10) << 4) | (rtc.year % 10));
  write(__RTC_WP, 0x80);
  RTC_RST = 0;
}
void rtc_charge(void) {
  RTC_RST = 1;
  write(__RTC_WP, 0x00);
  write(0x90, 0xA9);
  write(__RTC_WP, 0x80);
  RTC_RST = 0;
}
void rtc_nvm_write(uint8_t addr, uint8_t dat) {
  RTC_RST = 1;
  write(__RTC_WP, 0x00);
  write((addr << 1) | 0xC0, dat);
  write(__RTC_WP, 0x80);
  RTC_RST = 0;
}
uint8_t rtc_nvm_read(uint8_t addr) {
  uint8_t dat = 0;
  RTC_RST = 1;
  write(__RTC_WP, 0x00);
  dat = read((addr << 1) | 0xC1);
  write(__RTC_WP, 0x80);
  RTC_RST = 0;
  return dat;
}