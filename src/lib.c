#include "def.h"
#include "sys.h"
#include "detail/sys.h"
#include "adc.h"
#include "beep.h"
#include "hall.h"
#include "key.h"
#include "nvm.h"
#include "rtc.h"
#include "timer.h"
#include "uart.h"
#include "vib.h"
#include "display.h"

XDATA adc_t adcs = {0};                                                                 //!< adc data
static XDATA sys_callback_t adc_callback_table[6][2] = {{0}, {0}, {0}, {0}, {0}, {0}};  //!< adc callback table
static void adc_register(uint32_t cfg, sys_callback_t callback);                        //!< nav register function
static __sys_msg_t adc_scan(void) REENTRANT;                                            //!< nav scan function
static void adc_callback(__sys_msg_t msg) REENTRANT;                                    //!< nav callback function
uint8_t ADC = 0;                                                                        //!< nav idx
/**
 * @fn adc_init
 * @brief adc init
 * @return none
 */
void adc_init(void) {
  __ADC_INIT();
  ADC = __sys_sensor_add(adc_register, adc_scan, adc_callback);
  __ADC_START(ADCRT);
}
/**
 * @fn adc_register
 * @brief adc register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void adc_register(uint32_t cfg, sys_callback_t callback) {
  adc_callback_table[cfg >> 1][cfg & 1] = callback;
}
static XDATA uint8_t flag = 0;  //!< adc flag, use bit 0-1 for idx in __adc, use bit 2 for response in adc_scan
/**
 * @fn adc_scan
 * @brief adc scan
 * @return msg bit 0-2 for navpress, bit 3-5 for navrelease
 */
static __sys_msg_t adc_scan(void) REENTRANT {
  static uint8_t last = NAVNONE, start = NAVNONE, nav_state = 0;
  uint8_t cur, ret = 0;
  __sys_lock();
  cur = adcs.nav;
  __sys_unlock();
  if(cur != start) {
    start = cur;
    nav_state = 10;
  } else if(nav_state != 0) {
    --nav_state;
    if(nav_state == 0) {
      if(start != last && last != NAVNONE && adc_callback_table[last][NAVRELEASE]) {
        ret |= (last + 1) << 3;
        last = NAVNONE;
      }
      if(start != NAVNONE && adc_callback_table[start][NAVPRESS]) {
        last = start;
        ret |= start + 1;
      }
    }
  }
  return ret;
}
/**
 * @fn adc_callback
 * @brief adc callback
 * @param msg msg type
 * @return none
 */
static void adc_callback(__sys_msg_t msg) REENTRANT {
  if(msg & 0x7) {
    adc_callback_table[(msg & 0x7) - 1][0]();
  }
  if(msg & 0x38) {
    adc_callback_table[((msg & 0x38) >> 3) - 1][1]();
  }
}
static int8_t CODE __d2t[] = {239, 197, 175, 160, 150, 142, 135, 129, 124, 120, 116, 113, 109, 107, 104, 101, 99, 97,
  95, 93, 91, 90, 88, 86, 85, 84, 82, 81, 80, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 67, 66, 65, 64, 63, 63,
  62, 61, 61, 60, 59, 58, 58, 57, 57, 56, 55, 55, 54, 54, 53, 52, 52, 51, 51, 50, 50, 49, 49, 48, 48, 47, 47, 46, 46,
  45, 45, 44, 44, 43, 43, 42, 42, 41, 41, 41, 40, 40, 39, 39, 38, 38, 38, 37, 37, 36, 36, 36, 35, 35, 34, 34, 34, 33,
  33, 32, 32, 32, 31, 31, 31, 30, 30, 29, 29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 26, 25, 25, 24, 24, 24, 23, 23, 23,
  22, 22, 22, 21, 21, 21, 20, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13,
  12, 12, 12, 11, 11, 11, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 54, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0, -1, -1,
  -1, -2, -2, -3, -3, -4, -4, -5, -5, -6, -6, -7, -7, -8, -8, -9, -9, -10, -10, -11, -11, -12, -13, -13, -14, -14, -15,
  -16, -16, -17, -18, -19, -19, -20, -21, -22, -23, -24, -25, -26, -27, -28, -29, -30, -32, -33, -35, -36, -38,
  -40};                                                         //!< adc temperature table
static uint8_t CODE __adc_channel[] = {ADCRT, ADCROP, ADCNAV};  //!< adc channel table
/**
 * @fn __adc
 * @brief adc interrupt function
 * @return none
 */
INTERRUPT_USING(__adc, ADC_VECTOR, ADC_INT_PRIORITY) {
  static XDATA uint32_t sum = 0;
  static XDATA uint16_t count = 0, avgCnt = 50;
  uint8_t idx = flag & 0x3;
  sum += __ADC_GET();
  ++count;
  if(count == avgCnt) {
    if(idx == 0x2) {
      adcs.nav = ((sum + (avgCnt >> 1)) / avgCnt) >> 7;
      flag &= ~0x3;
    } else {
      switch(idx) {
        case 0 :
          adcs.rt = __d2t[(((sum + (avgCnt >> 1)) / avgCnt) >> 2) - 1];
          break;
        case 1 :
          adcs.rop = (sum + (avgCnt >> 1)) / avgCnt;
          break;
        default :
          break;
      }
      ++flag;
    }
    count = 0;
    sum = 0;
  }
  __ADC_START(__adc_channel[flag & 0x3]);
}



#define __BEEP P3_4
#define __BEEP_SET(x) __DO_WHILE0(__BEEP = (x))
#define __BEEP_NEGATE() __DO_WHILE0(__BEEP = !__BEEP)

static uint16_t step;
void beep_init(void) {
  CCAPM1 |= 0x48;
  CMOD |= 0x8;
  PPCA = 1;
  P3M0 |= 1 << 4;
  P3M1 &= ~(1 << 4);
  __BEEP = 0;
  step = (65536 - __sysclk / 6 / __BEEP_DEFAULT_FREQ);
}
void beep_on(void) {
  CCAPM1 |= 0x01;
  CCAP1H = step >> 8;
  CCAP1L = step & 0xff;
}
void beep_off(void) {
  CCAPM1 &= ~0x01;
  __BEEP_SET(0);
}
void beep_freq(uint16_t freq) REENTRANT {
  step = (65536 - __sysclk / 6 / freq);
}
INTERRUPT_USING(__beep, __BEEP_VECTOR, 1) {
  __BEEP_NEGATE();
  CCAP1L = (((CCAP1H * 256) + CCAP1L) + step) & 0xff;
  CCAP1H = (((CCAP1H * 256) + CCAP1L) + step) >> 8;
  CCF1 = 0;
}






#define __DISPLAY_INIT() __DO_WHILE0(P0M0 = 0xff; P0M1 = 0x00; P2M0 |=(1<<3); P2M1 &= ~(1<<3))

#define __LED_EN() __DO_WHILE0(P2_3 = 1)

#define __LED_SET(val) __DO_WHILE0(P0 = (val))

#define __SEG_EN() __DO_WHILE0(P2_3 = 0)

#define __SEG_SET(seg) __DO_WHILE0(P0 = (seg))

#define __SEG_SEL_SET(sel) __DO_WHILE0(P2 = P2 & 0xf8 | (sel))

static void display_schedule(void);//!< display schedule
/**
 * @fn display_init
 * @brief display init
 * @return none
 */
void display_init(void) {
  __DISPLAY_INIT();
  __sys_schedule_add(display_schedule);
}
static XDATA uint8_t __display_en = 0;  //!< display enable
/**
 * @fn display_en
 * @brief display enable
 * @param en 
 * @return none
 */
void display_en(uint8_t en) {
  __display_en = en;
}
static XDATA uint8_t __display_led = 0;  //!< display led
/**
 * @fn display_led
 * @brief display led
 * @param num 
 * @return none
 */
void display_led(uint8_t num) {
  __display_led = num;
}

static XDATA uint8_t __display_seg[__SEG_CNT] = {0};  //!< display segment
/**
 * @fn display_seg
 * @brief display segment
 * @param seg 
 * @return none
 */
void display_seg(uint8_t seg[8]) {
  uint8_t i = 0;
  for(; i < __SEG_CNT; ++i) {
    __display_seg[i] = seg[i];
  }
}
/**
 * @fn display_seg7
 * @brief display segment
 * @param seg1 
 * @param seg2 
 * @param seg3 
 * @param seg4 
 * @param seg5 
 * @param seg6 
 * @param seg7 
 * @param seg8 
 * @return none
 */
void display_seg7(
  uint8_t seg1, uint8_t seg2, uint8_t seg3, uint8_t seg4, uint8_t seg5, uint8_t seg6, uint8_t seg7, uint8_t seg8) {
  __display_seg[0] = seg1;
  __display_seg[1] = seg2;
  __display_seg[2] = seg3;
  __display_seg[3] = seg4;
  __display_seg[4] = seg5;
  __display_seg[5] = seg6;
  __display_seg[6] = seg7;
  __display_seg[7] = seg8;
}
static XDATA enum __display_base __db = DISPLAY_BASE_DEC;  //!< display base
/**
 * @fn display_base
 * @brief display base
 * @param db 
 * @return none
 */
void display_base(enum __display_base db) {
  __db = db;
}
/**
 * @fn display_num
 * @brief display number
 * @param num 
 * @return none
 */
void display_num(uint32_t num)REENTRANT {
  uint8_t i = 0;
  for(; i < __SEG_CNT; i++) {
    __display_seg[i] = display_num_decoding[num % __db];
    num /= __db;
  }
}
/**
 * @fn display_schedule
 * @brief display schedule
 * @return none
 */
static void delay(void) {
  uint16_t i = 600;
  while(i--)
    ;
}
XDATA uint8_t display_num_decoding[16] = {
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};  //!< decoding table
XDATA uint8_t display_num_index[__SEG_CNT] = {0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};      //!< index table
/**
 * @fn display_schedule
 * @brief display schedule
 * @return none
 */
static void display_schedule(void) {
  uint8_t i = 0;
  __SEG_EN();
  for(; i < __SEG_CNT; ++i) {
    __SEG_SET(0);
    __SEG_SEL_SET(display_num_index[i]);
    if(__display_en & (1 << i))
      __SEG_SET(__display_seg[i]);
    delay();
  }
  __LED_SET(0);
  __LED_EN();
  __LED_SET(__display_led);
  delay();
}


#define __HALL_INI 1

#define __HALL_INIT() __DO_WHILE0(P1M1 &= ~(1 << 2); P1M0 |= 1 << 2)

uint8_t HALL = 0;
static XDATA sys_callback_t hall_callback_table[2] = {0};          //!< hall callback table
static void hall_register(uint32_t cfg, sys_callback_t callback);  //!< hall register function
static uint8_t hall_scan(void);                                    //!< hall scan function
static void hall_callback(uint8_t msg);                            //!< hall callback function
/**
 * @fn hall_init
 * @brief hall init
 * @return none
 */
void hall_init(void) {
  __HALL_INIT();
  HALL = __sys_sensor_add(hall_register, hall_scan, hall_callback);
}
/**
 * @fn hall_register
 * @brief hall register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void hall_register(uint32_t cfg, sys_callback_t callback) {
  hall_callback_table[cfg] = callback;
}
/**
 * @fn hall_scan
 * @brief hall scan
 * @return msg 1 for hall get close, 2 for hall get away
 */
static __sys_msg_t hall_scan(void) {
  static bit hall_state = __HALL_INI;
  __sys_msg_t ret = 0;
  if(hall_state != __HALL) {
    hall_state = __HALL;
    if(hall_callback_table[hall_state]) {
      ret |= hall_state | 0x80;
    }
  }
  return ret;
}
/**
 * @fn hall_callback
 * @brief hall callback
 * @param msg msg type
 * @return none
 */
static void hall_callback(__sys_msg_t msg) {
  hall_callback_table[msg & 0x7f]();
}



#define __MSG_MASK (0x1 << ((sizeof(__sys_msg_t) << 3) - 1))
uint8_t KEY = 0;
static XDATA sys_callback_t key_callback_table[__KEY_CNT][2] = {{0}, {0}, {0}};  //!< key callback table
static XDATA uint8_t key_states = 0;  //!< key states,bit 0-2 for key1-3, 1 for press, 0 for release
static void key_register(uint32_t cfg, sys_callback_t callback);  //!< key register function

static void key_callback(uint8_t msg) REENTRANT;  //!< key callback function
/**
 * @fn key_init
 * @brief key init
 * @return none
 */
void key_init(void) {
  __KEY_INIT();
  KEY = __sys_sensor_add(key_register,
#ifdef __KEY_USE_INTERRUPT
    0
#endif
#ifdef __KEY_USE_POLLING
    key_scan
#endif
    ,
    key_callback);
  // __sys_sensor_add(KEY, key_scan, key_register, key_callback);
}
/**
 * @fn key_get
 * @brief get key state
 * @return key state
 */
uint8_t key_get(void) {
  return key_states;
}
static XDATA uint8_t key_state[__KEY_CNT] = {0};  //!< key state
/**
 * @fn __key##idx
 * @brief key interrupt function
 * @return none
 */
#ifdef __KEY_USE_INTERRUPT
#define __KEY_BY_INT(idx, vec)                                                      \
  INTERRUPT(__key##idx, vec) {                                                      \
    static uint32_t lastT = 0;                                                      \
    if(!lastT || __sys_timer_cnt - lastT > 20) {                                    \
      if(key_callback_table[idx][(key_states >> idx) & 1]) {                        \
        __sys_sensor_set_msg(KEY, (__MSG_MASK >> idx) | (key_states & (1 << idx))); \
      }                                                                             \
      key_states ^= 1 << idx;                                                       \
    }                                                                               \
    lastT = __sys_timer_cnt;                                                        \
  }
__KEY_BY_INT(0, IE0_VECTOR)
__KEY_BY_INT(1, IE1_VECTOR)
#undef __KEY_BY_INT
#endif
/**
 * @fn key_register
 * @brief key register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void key_register(uint32_t cfg, sys_callback_t callback) {
  key_callback_table[cfg >> 2][cfg & 0x3] = callback;
}
/**
 * @fn key_callback
 * @brief key callback
 * @param msg msg type
 * @return none
 */
static void key_callback(__sys_msg_t msg) REENTRANT {
  uint8_t i = 0;
  for(; i < __KEY_CNT; ++i) {
    if((msg << i) & __MSG_MASK) {
      key_callback_table[i][(msg >> i) & 1]();
    }
  }
}
#ifdef __KEY_USE_POLLING
static uint8_t key_scan_single(uint8_t info) {
  uint8_t msg = 0, idx = info >> 1;
  if(info & 0x1) {
    if(key_state[idx] == 0 && key_callback_table[idx][0]) {
      key_states |= 1 << idx;
      msg = KEYPRESS << (idx << 1);
    }
    key_state[idx] = 20;
  } else if(key_state[idx] != 0) {
    --key_state[idx];
    if(key_state[idx] == 0 && key_callback_table[idx][1]) {
      key_states &= ~(1 << idx);
      msg = KEYRELEASE << (idx << 1);
    }
  }
  return msg;
}

static uint8_t key_scan(void) {
  uint8_t ret = 0;
  ret |= key_scan_single(__KEY_PRESS(__KEY_1));
  ret |= key_scan_single(__KEY_PRESS(__KEY_2) | 0x2);
  ret |= key_scan_single(__KEY_PRESS(__KEY_3) | 0x4);
  return ret;
}
#endif


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


/**
 * @struct __sensor_t
 * @brief sensor struct
 * @var __sensor_t::_register register callback function
 * @var __sensor_t::scan scan callback function
 * @var __sensor_t::callback callback function
 */
typedef struct {
  __sys_sensor_register _register;
  __sys_sensor_scan scan;
  __sys_sensor_callback callback;
  uint8_t msg;
} __sensor_t;
/**
 * @struct __sys_t
 * @brief system struct
 * @var __sys_t::sensor sensor array
 * @var __sys_t::schedule schedule array
 */
typedef struct {
  __sensor_t sensor[SENSOR_CNT];
  __sys_schedule schedule[SCHDULE_CNT];
} __sys_t;
static uint8_t sys_schedule_idx = 0;  //!< system schedule index
XDATA __sys_t __sys = {{0}, {0}};     //!< system
/**
 * @fn __sys_schedule_add
 * @brief add schedule
 * @param schedule
 * @return none
 */
void __sys_schedule_add(__sys_schedule schedule) {
  __sys.schedule[sys_schedule_idx++] = schedule;
}
static uint8_t sys_sensor_idx = 0;  //!< system sensor index
/**
 * @fn __sys_sensor_add
 * @brief add sensor
 * @param _register
 * @param scan
 * @param callback
 * @return sensor index
 */
uint8_t __sys_sensor_add(__sys_sensor_register _register, __sys_sensor_scan scan, __sys_sensor_callback callback) {
  __sys.sensor[sys_sensor_idx]._register = _register;
  __sys.sensor[sys_sensor_idx].scan = scan;
  __sys.sensor[sys_sensor_idx].callback = callback;
  __sys.sensor[sys_sensor_idx].msg = 0;
  return sys_sensor_idx++;
}
/**
 * @fn sys_register
 * @brief register sensor
 * @param event event type
 * @param callback callback function
 * @return none
 */
void sys_register(uint8_t idx, sys_callback_t callback, uint32_t cfg) {
  __sys.sensor[idx]._register(cfg, callback);
}

uint32_t __sysclk = 0;  //!< system clock
/**
 * @fn sys_init
 * @brief system init
 * @param clk
 * @return none
 */
void sys_init(uint32_t clk) {
  __sysclk = clk;
  TMOD = 0x00;  // T0和T1都是工作在模式0下,即16位自动重装载模式
  TH0 = (65535 - __sysclk / 1000) >> 8;
  TL0 = (65535 - __sysclk / 1000) & 0xff;
  IE |= 0x02;
  // EA = 1;
  // ET0 = 1;  // T0中断允许
  IP &= ~0x2;
  // PT0 = 0;
}
uint32_t __sys_timer_cnt = 0;  //!< system timer count
/**
 * @fn sys_timer
 * @brief system timer
 * @return none
 */
INTERRUPT(__sys_use_timer, TF0_VECTOR) {
  uint8_t i = 0;
  for(; i < sys_sensor_idx; ++i) {
    if(__sys.sensor[i].scan && !__sys.sensor[i].msg) {
      __sys.sensor[i].msg |= __sys.sensor[i].scan();
    }
  }
  ++__sys_timer_cnt;
}
/**
 * @fn sys_exec
 * @brief system exec
 * @param callback
 * @return none
 */
void sys_exec(sys_callback_t callback) {
  AUXR |= 0x80;  // T0，2工作在1T模式，且T2开始计时
  IE |= 0x80;
  // CMOD |= 0x1;
  TCON |= 0x10;
  CR = 1;
  // TR1 = 0;  // T1
  // TR0 = 1;  // T0开始计时
  while(1) {
    uint8_t i = 0;
    for(; i < SENSOR_CNT; ++i) {
      if(__sys.sensor[i].callback && __sys.sensor[i].msg) {
        __sys.sensor[i].callback(__sys.sensor[i].msg);
        __sys.sensor[i].msg = 0;
      }
    }
    for(i = 0; i < SCHDULE_CNT; ++i)
      if(__sys.schedule[i]) {
        __sys.schedule[i]();
      }
    if(callback) {
      callback();
    }
  }
}
/**
 * @fn __sys_sensor_set_msg
 * @brief set sensor msg
 * @param idx
 * @param msg
 * @return none
 */
void __sys_sensor_set_msg(uint8_t idx, uint8_t msg) {
  __sys.sensor[idx].msg |= msg;
}
uint32_t sys_cnt = 0;


uint8_t TIMER = 0;

static XDATA sys_callback_t timer_callback_table[4]={0};                 //!< timer callback table
static void timer_register(uint32_t cfg, sys_callback_t callback);  //!< timer register function
static __sys_msg_t timer_scan(void) REENTRANT;                           //!< timer scan function
static void timer_callback(__sys_msg_t msg) REENTRANT;                   //!< timer callback function
/**
 * @fn timer_init
 * @brief timer init
 * @return none
 */
void timer_init(void) {
  TIMER = __sys_sensor_add(timer_register, timer_scan, timer_callback);
}
/**
 * @fn timer_register
 * @brief timer register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void timer_register(uint32_t cfg, sys_callback_t callback) {
  timer_callback_table[cfg] = callback;
}
/**
 * @fn timer_scan
 * @brief timer scan
 * @return msg bit 0-3 for 1ms, 10ms, 100ms, 1000ms
 */
static __sys_msg_t timer_scan(void) REENTRANT {
  __sys_msg_t ret = 0;
  if(timer_callback_table[0])
    ret |= 1;
  if(__sys_timer_cnt % 10 == 0) {
    if(timer_callback_table[1]) {
      ret |= 2;
    }
    if(__sys_timer_cnt % 100 == 0) {
      if(timer_callback_table[2]) {
        ret |= 4;
      }
      if(__sys_timer_cnt % 1000 == 0) {
        if(timer_callback_table[3]) {
          ret |= 8;
        }
      }
    }
  }
  return ret;
}
/**
 * @fn timer_callback
 * @brief timer callback
 * @param msg msg type
 * @return none
 */
static void timer_callback(__sys_msg_t msg) REENTRANT {
  if(msg & 0x1) {
    timer_callback_table[0]();
  }
  if(msg & 0x2) {
    timer_callback_table[1]();
  }
  if(msg & 0x4) {
    timer_callback_table[2]();
  }
  if(msg & 0x8) {
    timer_callback_table[3]();
  }
}


typedef struct {
  uint8_t *buf;
  uint16_t len;
  uint16_t idx;

  sys_callback_t callback;
} uart_recv_cfg_t;
typedef struct {
  uint8_t *buf;
  uint16_t len;
  uint16_t idx;
  sys_callback_t callback;
} uart_send_cfg_t;
// static XDATA uint8_t rx_buf[__UART_RX_BUF_SIZE];       //!< uart rx buffer
// static XDATA uint16_t rx_msg_idx = 0, rx_buf_idx = 0;  //!< uart rx buffer index
// static XDATA uint16_t rx_buf_len = 0;                  //!< uart rx buffer length
uint8_t UART1, UART2;
static uint8_t tmp = 0;
static XDATA uart_recv_cfg_t recv_cfg[2] = {{0}, {0}};
static XDATA uart_send_cfg_t send_cfg[2] = {{0}, {0}};
uint8_t uart_cfg_recv(enum UartInit cfg, void *buf, uint16_t cnt) {
  tmp = cfg - 1;
  recv_cfg[tmp].buf = (uint8_t *)buf;
  recv_cfg[tmp].len = cnt;
  recv_cfg[tmp].idx = 0;
  return 0;
}
static void uart_register(uint32_t cfg, sys_callback_t callback) {
  if((cfg >> 2) & 0x1) {
    if((cfg & 0x3) == UARTRECVOVER) {
      recv_cfg[0].callback = callback;
    } else if((cfg & 0x3) == UARTSENDOVER) {
      send_cfg[0].callback = callback;
    }
  }
  // if((cfg >> 3) & 0x1) {
  //   if((cfg & 0x3) == UARTRECVOVER) {
  //     // recv_cfg[1].callback = callback;
  //   } else if((cfg & 0x3) == UARTSENDOVER) {
  //     // send_cfg[1].callback = callback;
  //   }
  // }
}
static void __uart_callback(__sys_msg_t msg) REENTRANT {
  tmp = (msg >> 2) - 1;
  switch(msg & 0x3) {
    case UARTRECVOVER :
      recv_cfg[tmp].callback();
      // sys_cnt+=tmp;
      recv_cfg[tmp].idx = 0;
      break;
    case UARTSENDOVER :
      send_cfg[tmp].callback();
      break;
    default :
      break;
  }
}
static void uart_callback(__sys_msg_t msg) REENTRANT {
  if((msg >> 2) & 0x1) {
    __uart_callback(msg);
    sys_cnt += (msg >> 2) - 1;
  }
  if((msg >> 3) & 0x1) {
    __uart_callback(msg & (~0x10));
  }
}
void uart_init(uint8_t cfg) {
  if(cfg & 0x1) {
    AUXR &= ~0x1;  // select timer1 as baudrate generator
    AUXR |= 0x40;  // timer1 divide by 12,timer2 divide by 12
    PS = 1;        // interrupt priority 1
    SCON |= 0x50;  // SM0 = 1, SM1 = 0, REN = 1 ,8-bit UART, variable baudrate, receive enable
    TH1 = (65536 - (__sysclk / __UART_DEFAULT_BAUDRATE / 4)) >> 8;
    TL1 = (65536 - (__sysclk / __UART_DEFAULT_BAUDRATE / 4)) & 0xFF;  // set baudrate
    RI = 0;                                                           // clear receive interrupt flag
    TI = 0;                                                           // clear transmit interrupt flag
    ES = 1;                                                           // enable serial interrupt
    TCON |= 0x40;                                                     // timer1 run
    UART1 = __sys_sensor_add(uart_register, 0, uart_callback);
  }
  if(cfg & 0x2) {
    AUXR |= 0x04;   // timer1 divide by 12,timer2 divide by 12
    IP2 |= 0x1;     // interrupt priority 1
    S2CON |= 0x10;  // SM0 = 0, REN = 1 ,8-bit UART, variable baudrate, receive enable
    T2H = (65536 - (__sysclk / __UART_DEFAULT_BAUDRATE / 4)) >> 8;
    T2L = (65536 - (__sysclk / __UART_DEFAULT_BAUDRATE / 4)) & 0xFF;  // set baudrate
    S2CON &= ~0x03;
    IE2 |= 0x1;  // enable serial interrupt
    P_SW2 |= (cfg >> 2) & 0x1;
    AUXR |= 0x10;  // timer2 run
    UART2 = __sys_sensor_add(uart_register, 0, uart_callback);
  }
}
void uart_send(enum UartInit cfg, void *buf, uint8_t len) REENTRANT {
  if(len) {
    tmp = (cfg & 0x3) - 1;
    if(tmp == 0) {
      send_cfg[tmp].buf = buf;
      send_cfg[tmp].len = len;
      send_cfg[tmp].idx = 1;
      if(cfg & 0x1) {
        SBUF = send_cfg[tmp].buf[0];
      } else if(cfg & 0x2) {
        // S2BUF = send_cfg[tmp].buf[0];
      }
    }
  }
}
INTERRUPT_USING(__uart1, __UART1_VECTOR, 1) {
  while(RI) {
    RI = 0;
    if(recv_cfg[0].len) {
      // sys_cnt += recv_cfg[0].idx;
      if(recv_cfg[0].idx < recv_cfg[0].len) {
        // ++sys_cnt;
        recv_cfg[0].buf[recv_cfg[0].idx++] = SBUF;
        if(recv_cfg[0].idx == recv_cfg[0].len) {
          __sys_sensor_set_msg(UART, CONUART(UART1, UARTRECVOVER));
        }
      }
    }
    // else if(rx_buf_idx < __UART_RX_BUF_SIZE) {
    //   rx_buf[rx_buf_idx++] = SBUF;
    //   rx_buf_len++;
    // }
  }
  while(TI) {
    TI = 0;
    if(send_cfg[0].idx < send_cfg[0].len) {
      SBUF = send_cfg[0].buf[send_cfg[0].idx++];
    } else if(send_cfg[0].callback) {
      __sys_sensor_set_msg(UART, CONUART(UART1, UARTSENDOVER));
    }
  }
}
INTERRUPT_USING(__uart2, __UART2_VECTOR, 1) {
  while(S2CON & 0x01) {
    S2CON &= ~0x01;
    if(recv_cfg[1].len) {
      if(recv_cfg[1].idx < recv_cfg[1].len) {
        recv_cfg[1].buf[recv_cfg[1].idx++] = S2BUF;
        if(recv_cfg[1].idx == recv_cfg[1].len) {
          __sys_sensor_set_msg(UART, CONUART(UART2_Ext, UARTRECVOVER));
        }
      }
    }
    // else if(rx_buf_idx < __UART_RX_BUF_SIZE) {
    //   rx_buf[rx_buf_idx++] = SBUF;
    //   rx_buf_len++;
    // }
  }
  while(S2CON & 0x02) {
    S2CON &= ~0x02;
    if(send_cfg[1].idx < send_cfg[1].len) {
      S2BUF = send_cfg[1].buf[send_cfg[1].idx++];
    } else if(send_cfg[1].callback) {
      __sys_sensor_set_msg(UART, CONUART(UART2_Ext, UARTSENDOVER));
    }
  }
}


#include "vib.h"

#include "detail/sys.h"
#include "sys.h"
#define __VIB_MASK (0x1 << ((sizeof(__sys_msg_t) << 3) - 1))
uint8_t VIB = 0;
static XDATA sys_callback_t vib_callback_table[2] = {0};          //!< vib callback table
static XDATA uint8_t vib_state = 0;                               //!< vib state
static void vib_register(uint32_t cfg, sys_callback_t callback);  //!< vib register function
static uint8_t vib_scan(void);                                    //!< vib scan function
static void vib_callback(uint8_t msg);                            //!< vib callback function
/**
 * @fn vib_init
 * @brief vib init
 * @return none
 */
void vib_init(void) {
  __VIB_INIT();
  VIB = __sys_sensor_add(vib_register, vib_scan, vib_callback);
}
/**
 * @fn vib_register
 * @brief vib register
 * @param event event type
 * @param callback callback function
 * @return none
 */
static void vib_register(uint32_t cfg, sys_callback_t callback) {
  vib_callback_table[cfg] = callback;
}
/**
 * @fn vib_scan
 * @brief vib scan
 * @return msg 1 for vib start, 2 for vib stop
 */
static __sys_msg_t vib_scan(void) {
  __sys_msg_t ret = 0;
  if(GET_VIB()) {
    if(vib_state == 0 && vib_callback_table[0]) {
      ret |= VIBSTART | __VIB_MASK;
    }
    vib_state = 20;
  } else if(vib_state != 0) {
    --vib_state;
    if(vib_state == 0 && vib_callback_table[1]) {
      ret |= VIBSTOP | __VIB_MASK;
    }
  }
  return ret;
}
/**
 * @fn vib_callback
 * @brief vib callback
 * @param msg msg type
 * @return none
 */
static void vib_callback(__sys_msg_t msg) {
  vib_callback_table[msg & ~__VIB_MASK]();
}