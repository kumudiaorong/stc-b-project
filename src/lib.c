#include "def.h"
#include "sys.h"
#include "use.h"

typedef uint8_t __flag_t;
#define __FLAG_MASK (0x1 << ((sizeof(__flag_t) << 3) - 1))
#define __FLAG_SIZE sizeof(__flag_t)
uint32_t __sysclk = 0;  //!< system clock
#if defined(USE_KEY) || defined(USE_TIMER)
uint32_t __sys_timer_cnt = 0;  //!< system timer count
#endif
#ifdef USE_ADC
#include "adc.h"
//index+6
static int8_t CODE __d2t[] = {124, 120, 116, 113, 109, 107, 104, 101, 99, 97, 95, 93, 91, 90, 88, 86, 85, 84, 82, 81,
  80, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 67, 66, 65, 64, 63, 63, 62, 61, 61, 60, 59, 58, 58, 57, 57, 56,
  55, 55, 54, 54, 53, 52, 52, 51, 51, 50, 50, 49, 49, 48, 48, 47, 47, 46, 46, 45, 45, 44, 44, 43, 43, 42, 42, 41, 41,
  41, 40, 40, 39, 39, 38, 38, 38, 37, 37, 36, 36, 36, 35, 35, 34, 34, 34, 33, 33, 32, 32, 32, 31, 31, 31, 30, 30, 29,
  29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 26, 25, 25, 24, 24, 24, 23, 23, 23, 22, 22, 22, 21, 21, 21, 20, 20, 20, 19,
  19, 19, 18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 9, 9, 9,
  8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 54, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0, -1, -1, -1, -2, -2, -3, -3, -4, -4, -5, -5, -6, -6,
  -7, -7, -8, -8, -9, -9, -10, -10, -11, -11, -12, -13, -13, -14, -14, -15, -16, -16, -17, -18, -19, -19, -20, -21, -22,
  -23, -24, -25, -26, -27, -28, -29, -30, -32, -33, -35, -36, -38, -40};  //!< adc temperature table
XDATA adc_t adcs = {0};                                                   //!< adc data
/**
 * @fn adc_init
 * @brief adc init
 * @return none
 */
void adc_init(void) {
  EADC = 1;
  P1ASF = 0x98;
#define __ADC_START(channel) __DO_WHILE0(ADC_RES = 0; ADC_RESL = 0; ADC_CONTR = (0x88 | (channel)))
  __ADC_START(0x3);  // adc start with rt channel
}
/**
 * @fn __adc
 * @brief adc interrupt function
 * @return none
 */
INTERRUPT_USING(__adc, 5, 1) {
  static XDATA uint8_t count = 0, curchannel = 0x3;  // rt channel
  static XDATA uint16_t sum = 0;
  sum += (ADC_RES << 2) + (ADC_RESL >> 6);  //__ADC_GET();
  ++count;
#define __ADC_AVG_CNT 2
  if(count == __ADC_AVG_CNT) {
    switch(curchannel) {
      case 0x3 :  // rt channel
        adcs.rt = __d2t[(((sum + (__ADC_AVG_CNT >> 1)) / __ADC_AVG_CNT) >> 2) - 1];
        curchannel = 0x4;  // rop channel
        break;
      case 0x4 :  // rop channel
        adcs.rop = (sum + (__ADC_AVG_CNT >> 1)) / __ADC_AVG_CNT;
        curchannel = 0x7;  // nav channel
        break;
      case 0x7 :  // nav channel
        adcs.nav = ((sum + (__ADC_AVG_CNT >> 1)) / __ADC_AVG_CNT) >> 7;
#undef __ADC_AVG_CNT
        curchannel = 0x3;  // rt channel
        break;
      default :
        break;
    }
    count = 0;
    sum = 0;
  }
  __ADC_START(curchannel);
#undef __ADC_START
}
static XDATA sys_callback_t nav_callback_table[12] = {0};  //!< adc callback table
static XDATA __flag_t nav_flag = 0;
#endif
#ifdef USE_BEEP
#include "beep.h"
static XDATA uint16_t beep_sum = 0;
uint16_t freq = 1000;
void beep_init(void) {
  // step = __sysclk / 24 / __BEEP_DEFAULT_FREQ;
  beep_sum = __sysclk / 24 / freq;
  CCAPM0 |= 0x48;
  PPCA = 1;

  P3M0 |= 1 << 4;
  P3M1 &= ~(1 << 4);
  P3_4 = 0;  // pull voltage to low to protect buzzer

  P_SW1 |= 0x10;
}
void beep_on(void) {
  CCAP0H = beep_sum >> 8;
  CCAP0L = beep_sum & 0xff;
  beep_sum += __sysclk / 24 / freq;
  CCAPM0 |= 0x01;
}
void beep_off(void) {
  CCAPM0 &= ~0x01;
  P3_4 = 0;  // pull voltage to low to protect buzzer
}
INTERRUPT_USING(__beep, __BEEP_VECTOR, 1) {
  CCF0 = 0;
  CCAP0H = beep_sum >> 8;
  CCAP0L = beep_sum & 0xff;
  beep_sum += __sysclk / 24 / freq;
  P3_4 = !P3_4;  // make buzzer sound
  // tmp = CCAP1L;
}
#endif
#ifdef USE_DISPLAY
#include "display.h"
uint8_t display_led = 0;  //!< display led
uint8_t display_num_decoding[16] = {
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};  //!< decoding table
uint8_t display_seg[8] = {0};                                                                       //!< display segment
static XDATA uint8_t display_begin = 0, display_end = 0;                                            //!< display enable
/**
 * @fn display_init
 * @brief display init
 * @return none
 */
void display_init(void) {
  P0M0 = 0xff;
  P0M1 = 0x00;
  P2M0 |= (1 << 3);
  P2M1 &= ~(1 << 3);
  P2_3 = 1;  //__LED_EN();
}

void display_area(uint8_t begin, uint8_t end) {
  display_begin = begin;
  display_end = end;
}
#endif
#ifdef USE_HALL
#include "hall.h"
/**
 * @fn hall_init
 * @brief hall init
 * @return none
 */
void hall_init(void) {
  P1M1 &= ~(1 << 2);
  P1M0 |= 1 << 2;
}
static XDATA sys_callback_t hall_callback_table[2] = {0};  //!< hall callback table
static XDATA __flag_t hall_flag = 0;                       //!< vib flag
static bit hall_state = 1;
#endif
#ifdef USE_KEY
#include "key.h"

/**
 * @fn key_init
 * @brief key init
 * @return none
 */
void key_init(void) {
  IE |= 0x5;
  TCON &= ~0x5;
  IP |= 0x5;
}
static XDATA sys_callback_t key_callback_table[6] = {0};  //!< key callback table
static XDATA __flag_t key_flag = 0;
static XDATA uint8_t key_states = 0;  //!< key states,bit 0-2 for key1-3, 1 for press, 0 for release
/**
 * @fn __key##idx
 * @brief key interrupt function
 * @return none
 */
#define __KEY_BY_INT(idx, vec)                                     \
INTERRUPT(__key##idx, vec) {                                       \
static uint32_t lastT = 0;                                         \
if(!lastT || __sys_timer_cnt - lastT > 20) {                       \
if(key_callback_table[(idx << 1) + ((key_states >> idx) & 0x1)]) { \
key_flag |= (__FLAG_MASK >> idx) | (key_states & (1 << idx));      \
}                                                                  \
key_states ^= 1 << idx;                                            \
}                                                                  \
lastT = __sys_timer_cnt;                                           \
}
__KEY_BY_INT(0, IE0_VECTOR)
__KEY_BY_INT(1, IE1_VECTOR)
#undef __KEY_BY_INT
#endif
#ifdef USE_NVM
#include "nvm.h"
#define __NVM_SCL P5_5
#define __NVM_SDA P4_0
#define __NVM_DEVICE_ADDR 0x50
bit errno_nvm = 0;
static void nvm_iic_start(void) {
  __NVM_SDA = 1;
  NOP();
  __NVM_SCL = 1;
  NOP();
  __NVM_SDA = 0;
  NOP();
  __NVM_SCL = 0;
  NOP();
}
static void nvm_iic_stop(void) {
  __NVM_SDA = 0;
  NOP();
  __NVM_SCL = 1;
  NOP();
  __NVM_SDA = 1;
  NOP();
}
static void nvm_iic_write_byte(uint8_t dat) {
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
static uint8_t nvm_iic_recv_ack(void) {
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
#define __ASSERT_STOP(expr, ret) \
if(!(expr)) {                    \
nvm_iic_stop();                  \
errno_nvm = 1;                   \
return ret;                      \
}
void nvm_write(uint8_t addr, uint8_t dat) {
  uint8_t i = 0;
  nvm_iic_start();
  nvm_iic_write_byte(__NVM_DEVICE_ADDR << 1);
  __ASSERT_STOP(nvm_iic_recv_ack(), );
  nvm_iic_write_byte(addr);
  __ASSERT_STOP(nvm_iic_recv_ack(), );
  nvm_iic_write_byte(dat);
  nvm_iic_recv_ack();
  nvm_iic_stop();
}
uint8_t nvm_read(uint8_t addr) {
  uint8_t i = 0, dat = 0;
  nvm_iic_start();
  nvm_iic_write_byte(__NVM_DEVICE_ADDR << 1);
  __ASSERT_STOP(nvm_iic_recv_ack(), dat);
  nvm_iic_write_byte(addr);
  __ASSERT_STOP(nvm_iic_recv_ack(), dat);
  nvm_iic_start();
  nvm_iic_write_byte((__NVM_DEVICE_ADDR << 1) | 0x01);
  __ASSERT_STOP(nvm_iic_recv_ack(), dat);
  for(; i < 8; i++) {
    NOP();
    __NVM_SCL = 1;
    NOP();
    dat <<= 1;
    dat |= __NVM_SDA;
    __NVM_SCL = 0;
  }
  nvm_iic_stop();
  return dat;
}
#undef __ASSERT_STOP

#endif
#ifdef USE_RTC
#include "rtc.h"
#define __RTC_SECOND_ADDR 0x80
#define __RTC_MINUTE_ADDR 0x82
#define __RTC_HOUR_ADDR 0x84
#define __RTC_DATE_ADDR 0x86
#define __RTC_MONTH_ADDR 0x88
#define __RTC_DAY_ADDR 0x8A
#define __RTC_YEAR_ADDR 0x8C
#define __RTC_WP_ADDR 0x8E
#define __RTC_WRITE(what) (what)
#define __RTC_READ(what) (what | 0x01)

rtc_t rtc = {0};
static void rtc_write_byte(uint8_t dat) {
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
static uint8_t rtc_read_byte(void) {
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
static void rtc_addr_write(uint8_t addr, uint8_t dat) {
  rtc_write_byte(__RTC_WRITE(addr));
  rtc_write_byte(dat);
}
static uint8_t rtc_addr_read(uint8_t addr) {
  uint8_t i = 0, dat = 0;
  rtc_write_byte(__RTC_READ(addr));
  for(; i < 8; i++) {
    dat >>= 1;
    if(RTC_IO == 1) {
      dat |= 0x80;
    }
    RTC_SCLK = 1;
    NOP();
    NOP();
    NOP();
    RTC_SCLK = 0;
  }
  return dat;
}
void rtc_init(void) {
  RTC_SCLK = 0;
}
void rtc_read(void) {
  uint8_t readval = 0;
  RTC_RST = 1;
  readval = rtc_addr_read(__RTC_SECOND_ADDR);
  rtc.second = ((readval & 0x70) >> 4) * 10 + (readval & 0x0F);
  readval = rtc_addr_read(__RTC_MINUTE_ADDR);
  rtc.minute = ((readval & 0x70) >> 4) * 10 + (readval & 0x0F);
  readval = rtc_addr_read(__RTC_HOUR_ADDR);
  rtc.hour = ((readval & 0x30) >> 4) * 10 + (readval & 0x0F);
  readval = rtc_addr_read(__RTC_DATE_ADDR);
  rtc.date = ((readval & 0x30) >> 4) * 10 + (readval & 0x0F);
  readval = rtc_addr_read(__RTC_DAY_ADDR);
  rtc.day = readval & 0x07;
  readval = rtc_addr_read(__RTC_MONTH_ADDR);
  rtc.month = ((readval & 0x10) >> 4) * 10 + (readval & 0x0F);
  readval = rtc_addr_read(__RTC_YEAR_ADDR);
  rtc.year = ((readval & 0xF0) >> 4) * 10 + (readval & 0x0F);
  RTC_RST = 0;
}
void rtc_write(void) {
  RTC_RST = 1;
  rtc_addr_write(__RTC_WP_ADDR, 0x00);
  rtc_addr_write(__RTC_SECOND_ADDR, 0x80);
  rtc_addr_write(__RTC_SECOND_ADDR, 0x00 | ((rtc.second / 10) << 4) | (rtc.second % 10));
  rtc_addr_write(__RTC_MINUTE_ADDR, ((rtc.minute / 10) << 4) | (rtc.minute % 10));
  rtc_addr_write(__RTC_HOUR_ADDR, ((rtc.hour / 10) << 4) | (rtc.hour % 10));
  rtc_addr_write(__RTC_DATE_ADDR, ((rtc.date / 10) << 4) | (rtc.date % 10));
  rtc_addr_write(__RTC_DAY_ADDR, rtc.day);
  rtc_addr_write(__RTC_MONTH_ADDR, ((rtc.month / 10) << 4) | (rtc.month % 10));
  rtc_addr_write(__RTC_YEAR_ADDR, ((rtc.year / 10) << 4) | (rtc.year % 10));
  rtc_addr_write(__RTC_WP_ADDR, 0x80);
  RTC_RST = 0;
}
void rtc_charge(void) {
  RTC_RST = 1;
  rtc_addr_write(__RTC_WP_ADDR, 0x00);
  rtc_addr_write(0x90, 0xA9);
  rtc_addr_write(__RTC_WP_ADDR, 0x80);
  RTC_RST = 0;
}
void rtc_nvm_write(uint8_t addr, uint8_t dat) {
  RTC_RST = 1;
  rtc_addr_write(__RTC_WP_ADDR, 0x00);
  rtc_addr_write((addr << 1) | 0xC0, dat);
  rtc_addr_write(__RTC_WP_ADDR, 0x80);
  RTC_RST = 0;
}
uint8_t rtc_nvm_read(uint8_t addr) {
  uint8_t dat = 0;
  RTC_RST = 1;
  rtc_addr_write(__RTC_WP_ADDR, 0x00);
  dat = rtc_addr_read((addr << 1) | 0xC1);
  rtc_addr_write(__RTC_WP_ADDR, 0x80);
  RTC_RST = 0;
  return dat;
}
#endif
#ifdef USE_TIMER
static XDATA sys_callback_t sys_timer_callback_table[3] = {0};
static XDATA __flag_t sys_timer_flag = 0;  //!< timer flag
#endif
#ifdef USE_UART
#include "uart.h"
static XDATA uint8_t uart_msg = 0;  //!< uart msg
static bit uart1_recv_flag = 0;
static bit uart1_send_flag = 0;
static bit uart2_recv_flag = 0;
static bit uart2_send_flag = 0;

typedef struct {
  uint8_t *buf;
  uint16_t len;
  uint16_t idx;

  sys_callback_t callback;
} uart_cfg_t;
static XDATA uart_cfg_t uart_cfg[4] = {{0}, {0}, {0}, {0}};
void uart_cfg_recv(enum UartDev dev, void *buf, uint16_t cnt) {
  uart_cfg[dev << 1].buf = (uint8_t *)buf;
  uart_cfg[dev << 1].len = cnt;
  uart_cfg[dev << 1].idx = 0;
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
    P3M0 &= ~0x80;
    P3M1 &= ~0x80;
    P3_7 = 0x0;  // 485 set receive
  }
}

uint8_t uart_send(enum UartDev dev, void *buf, uint16_t len) {
  if(uart_cfg[(dev << 1) + 1].idx == uart_cfg[(dev << 1) + 1].len) {
    return EventUartBusy;
  }
  if(len) {
    uart_cfg[(dev << 1) + 1].buf = (uint8_t *)buf;
    uart_cfg[(dev << 1) + 1].len = len;
    uart_cfg[(dev << 1) + 1].idx = 1;
    if(dev) {
      P3_7 = 0x1;
      S2BUF = uart_cfg[3].buf[0];
    } else {
      SBUF = uart_cfg[1].buf[0];
    }
  }
  return 0;
}
INTERRUPT_USING(__uart1, SI0_VECTOR, 1) {
  if(RI) {
    RI = 0;
    if(uart_cfg[0].len) {
      if(uart_cfg[0].idx < uart_cfg[0].len) {
        uart_cfg[0].buf[uart_cfg[0].idx++] = SBUF;
        if(uart_cfg[0].idx == uart_cfg[0].len) {
          uart1_recv_flag = 1;
        }
      }
    }
  }
  if(TI) {
    TI = 0;
    if(uart_cfg[1].idx < uart_cfg[1].len) {
      SBUF = uart_cfg[1].buf[uart_cfg[1].idx++];
    } else {
      uart1_send_flag = 1;
    }
  }
}
INTERRUPT_USING(__uart2, SI1_VECTOR, 1) {
  while(S2CON & 0x01) {
    S2CON &= ~0x01;
    if(uart_cfg[2].len) {
      if(uart_cfg[2].idx < uart_cfg[2].len) {
        uart_cfg[2].buf[uart_cfg[2].idx++] = S2BUF;
        if(uart_cfg[2].idx == uart_cfg[2].len) {
          uart2_recv_flag = 1;
        }
      }
    }
  }
  while(S2CON & 0x02) {
    S2CON &= ~0x02;
    if(uart_cfg[3].idx < uart_cfg[3].len) {
      S2BUF = uart_cfg[3].buf[uart_cfg[3].idx++];
    } else {
      uart2_send_flag = 1;
      P3_7 = 0x0;
    }
  }
}
#endif
#ifdef USE_VIB
#include "vib.h"
static XDATA sys_callback_t vib_callback_table[1] = {0};  //!< vib callback table
static XDATA uint8_t vib_state = 0;                       //!< vib state
static XDATA __flag_t vib_flag = 0;                       //!< vib flag

/**
 * @fn vib_init
 * @brief vib init
 * @return none
 */
void vib_init(void) {
  P2M0 &= ~(1 << 4);
  P2M1 |= 1 << 4;
  P2_4 = 1;
}
#endif

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
  IP &= ~0x2;
#ifdef USE_BEEP
  CCON = 0;
  CH = 0;
  CL = 0;
  CMOD = 0;
#endif
}

/**
 * @fn sys_timer
 * @brief system timer
 * @return none
 */
INTERRUPT(__sys_use_timer, TF0_VECTOR) {
  static XDATA uint8_t display_index = 0;
#ifdef USE_ADC
  uint8_t cur = adcs.nav;
#define __NAV_NONE 0x7
  static XDATA uint8_t nav_last_state = __NAV_NONE, nav_cnt = 0;
  if(cur != __NAV_NONE && nav_last_state == __NAV_NONE) {
    if(nav_cnt) {
      --nav_cnt;
      if(nav_cnt == 0) {
        nav_flag |= 0x1 | (cur << 1);
        nav_last_state = cur;
      }
    } else {
      nav_cnt = 5;
    }
  } else if(cur == __NAV_NONE && nav_last_state != __NAV_NONE) {
    nav_flag |= (__FLAG_MASK >> 3) | (nav_last_state << (__FLAG_SIZE - 3));
    nav_last_state = __NAV_NONE;
#undef __NAV_NONE
  }
#endif
#ifdef USE_DISPLAY
  if(display_index < display_end) {
    if(display_index < 0x8) {
      P2 = P2 & 0xf8 | display_index;   //__SEG_SEL_SET(display_num_index[display_index]);
      P0 = 0;                           //__SEG_SET(0);
      P2_3 = 0;                         //__SEG_EN();
      P0 = display_seg[display_index];  //__SEG_SET(display_seg[display_index]);
    }
    ++display_index;
  } else {
    P0 = 0;            //__LED_SET(0);
    P2_3 = 1;          //__LED_EN();
    P0 = display_led;  //__LED_SET(display_led);
    display_index = display_begin;
  }
#endif
#ifdef USE_HALL
  if(hall_state != P1_2) {
    hall_state = P1_2;
    if(hall_callback_table[hall_state]) {
      hall_flag |= hall_state | __FLAG_MASK;
    }
  }
#endif
#ifdef USE_TIMER
  if((__sys_timer_cnt % 10) == 0) {
    sys_timer_flag |= 0x1;
    if((__sys_timer_cnt % 100) == 0) {
      sys_timer_flag |= 0x2;
      if((__sys_timer_cnt % 1000) == 0) {
        sys_timer_flag |= 0x4;
      }
    }
  }
#endif
#ifdef USE_VIB
  if(P2_4 == 0) {  //__GET_VIB()) {
    if(vib_state == 0) {
      vib_flag = EventVibStart + 1;
    }
    vib_state = 18;
  } else if(vib_state != 0) {
    --vib_state;
  }
#endif
#if defined(USE_KEY) || defined(USE_TIMER)
  ++__sys_timer_cnt;
#endif
}
/**
 * @fn sys_exec
 * @brief system exec
 * @param callback
 * @return none
 */
void sys_exec(sys_callback_t callback) {
  AUXR |= 0x90;  // T0，2工作在1T模式，且T2开始计时
  IE |= 0x80;
  // CMOD |= 0x1;
  TCON |= 0x50;  // timer1 run
#ifdef USE_BEEP
  CR = 1;
#endif
  // TR1 = 0;  // T1
  // TR0 = 1;  // T0开始计时

  while(1) {
    uint8_t i = 0;
#ifdef USE_ADC
    if(nav_flag & 0x1) {
      if(nav_callback_table[nav_flag & ~0x1]) {
        nav_callback_table[nav_flag & ~0x1]();
      }
      nav_flag &= ~0xf;
    }
    if(nav_flag & (__FLAG_MASK >> 3)) {
      if(nav_callback_table[nav_flag >> (__FLAG_SIZE - 4)]) {
        nav_callback_table[nav_flag >> (__FLAG_SIZE - 4)]();
      }
      nav_flag &= ~0xf0;
    }
#endif
#ifdef USE_HALL
    if(hall_flag) {
      if(hall_callback_table[hall_flag & ~__FLAG_MASK])
        hall_callback_table[hall_flag & ~__FLAG_MASK]();
      hall_flag = 0;
    }
#endif
#ifdef USE_KEY
    if(key_flag) {
      i = 0;
      for(; i < 3; ++i) {
        if((key_flag << i) & __FLAG_MASK) {
          key_callback_table[(i << 1) + ((key_flag >> i) & 0x1)]();
        }
      }
      key_flag = 0;
    }
#endif
#ifdef USE_TIMER
    if(sys_timer_flag) {
      i = 0;
      for(; i < 3; ++i) {
        if(((sys_timer_flag >> i) & 0x1) && sys_timer_callback_table[i]) {
          sys_timer_callback_table[i]();
        }
      }
      sys_timer_flag = 0;
    }
#endif
#ifdef USE_UART
    if(uart1_recv_flag) {
      uart1_recv_flag = 0;
      if(uart_cfg[0].callback)
        uart_cfg[0].callback();
      uart_cfg[0].idx = 0;
    }
    if(uart1_send_flag) {
      uart1_send_flag = 0;
      if(uart_cfg[1].callback)
        uart_cfg[1].callback();
    }
    if(uart2_recv_flag) {
      uart2_recv_flag = 0;
      if(uart_cfg[2].callback)
        uart_cfg[2].callback();
      uart_cfg[2].idx = 0;
    }
    if(uart2_send_flag) {
      uart2_send_flag = 0;
      if(uart_cfg[3].callback)
        uart_cfg[3].callback();
    }
#endif
#ifdef USE_VIB
    if(vib_flag) {
      if(vib_callback_table[vib_flag - 1])
        vib_callback_table[vib_flag - 1]();
      vib_flag = 0;
    }
#endif
    if(callback) {
      callback();
    }
  }
}
/**
 * @fn sys_register
 * @brief register sensor
 * @param event event type
 * @param callback callback function
 * @return none
 */
void sys_register(enum RegisterType reg, sys_callback_t callback, uint32_t cfg) {
  switch(reg) {
#ifdef USE_UART
    case RegUart :
      uart_cfg[cfg].callback = callback;
      break;
#endif
#define REG_CASE(Case, Name)           \
case Case :                            \
Name##_callback_table[cfg] = callback; \
break;
#ifdef USE_TIMER
      REG_CASE(RegTimer, sys_timer)
#endif
#ifdef USE_KEY
      REG_CASE(RegKey, key)
#endif
#ifdef USE_VIB
      REG_CASE(RegVib, vib)
#endif
#ifdef USE_HALL
      REG_CASE(RegHall, hall)
#endif
#ifdef USE_ADC
      REG_CASE(RegNav, nav)
#endif
#undef REG_CASE
    default :
      break;
  }
}
