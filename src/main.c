#define TEST 0
#if TEST == 0
// #define TEST_UART
// #define TEST_DISPLAY
// #define TEST_TIMER
// #define TEST_VIB_HALL_KEY_ADC
// #define TEST_RTC_NVM
// #define TEST_BEEP
#ifdef TEST_UART
#include "sys.h"
#include "uart.h"
char buf1[2];
char buf2[4];
void uart1_recv_test(void) {
  uart_send(DevUART1, buf1, 2);
}
void uart1_send_test(void) {
  static bit flag = 0;
  if(flag) {
    flag = 0;
  } else {
    uart_send(DevUART2, buf1, 2);
    flag = 1;
  }
}
void uart2_recv_test(void) {
  uart_send(DevUART1, buf2, 4);
}
void uart2_send_test(void) {
  static bit flag = 0;
  if(flag) {
    flag = 0;
  } else {
    uart_send(DevUART2, buf1, 2);
    flag = 1;
  }
}
void main(void) {
  sys_init(27000000);
  uart_init(InitUART1 | InitUART2_Ext);
  uart_cfg_recv(DevUART1, buf1, 2);
  uart_cfg_recv(DevUART2, buf2, 4);
  sys_register(RegUart, uart1_recv_test, CONUART(DevUART1, EventUartRecvOver));
  sys_register(RegUart, uart1_send_test, CONUART(DevUART1, EventUartSendOver));
  sys_register(RegUart, uart2_send_test, CONUART(DevUART2, EventUartSendOver));
  sys_register(RegUart, uart2_recv_test, CONUART(DevUART2, EventUartRecvOver));
  sys_exec(0);
}
#elif defined(TEST_DISPLAY)
#include "display.h"
#include "sys.h"
#include "uart.h"

char buf1[2];
void uart1_recv_test(void) {
  display_num(buf1[1] * 256 + buf1[0]);
  display_led = buf1[1];
}
void main(void) {
  sys_init(27000000);
  uart_init(InitUART1);
  display_init();
  display_area(0, 8);
  uart_cfg_recv(DevUART1, buf1, 2);
  sys_register(RegUart, uart1_recv_test, CONUART(DevUART1, EventUartRecvOver));
  sys_exec(0);
}
#elif defined(TEST_TIMER)
#include "display.h"
#include "sys.h"

uint16_t cnt = 0;
void _10ms_test(void) {
  static uint8_t i = 0;
  if(++i == 100) {
    i = 0;
    ++cnt;
  }
}
void _100ms_test(void) {
  static uint8_t i = 0;
  if(++i == 10) {
    i = 0;
    display_num(cnt);
  }
}
void _1s_test(void) {
  ++cnt;
}
void main(void) {
  sys_init(27000000);
  display_init();
  display_area(0, 8);
  sys_register(RegTimer, _10ms_test, EventTimer10ms);
  sys_register(RegTimer, _100ms_test, EventTimer100ms);
  sys_register(RegTimer, _1s_test, EventTimer1S);
  sys_exec(0);
}
#elif defined(TEST_VIB_HALL_KEY_ADC)
#include "adc.h"
#include "display.h"
#include "hall.h"
#include "key.h"
#include "sys.h"
#include "vib.h"

static uint32_t cnt = 0;
void _100ms_test(void) {
  display_seg[0] = display_num_decoding[adcs.rop / 100];
  display_seg[1] = display_num_decoding[(adcs.rop / 10) % 10];
  display_seg[2] = display_num_decoding[adcs.rop % 10];
  display_seg[3] = 0x40;
  display_seg[4] = display_num_decoding[adcs.rt / 10];
  display_seg[5] = display_num_decoding[adcs.rt % 10];
  display_seg[6] = 0x40;
  cnt &= 0xf;
  display_seg[7] = display_num_decoding[cnt];
}
void key1_press_test(void) {
  ++cnt;
}
void key1_release_test(void) {
  ++cnt;
}
void key2_press_test(void) {
  ++cnt;
}
void key2_release_test(void) {
  ++cnt;
}
void vib_start_test(void) {
  ++cnt;
}
void hall_get_close_test(void) {
  ++cnt;
}
void hall_get_away_test(void) {
  ++cnt;
}
void nav_up_press_test(void) {
  ++cnt;
}
void nav_up_release_test(void) {
  ++cnt;
}
void nav_down_press_test(void) {
  ++cnt;
}
void nav_down_release_test(void) {
  ++cnt;
}
void nav_left_press_test(void) {
  ++cnt;
}
void nav_left_release_test(void) {
  ++cnt;
}
void nav_right_press_test(void) {
  ++cnt;
}
void nav_right_release_test(void) {
  ++cnt;
}
void nav_center_press_test(void) {
  ++cnt;
}
void nav_center_release_test(void) {
  ++cnt;
}
void nav_key3_press_test(void) {
  ++cnt;
}
void nav_key3_release_test(void) {
  ++cnt;
}
void main(void) {
  sys_init(27000000);
  key_init();
  vib_init();
  hall_init();
  adc_init();
  display_init();
  display_area(0, 8);
  display_base = DISPLAY_BASE_HEX;
  sys_register(RegTimer, _100ms_test, EventTimer100ms);
  sys_register(RegKey, key1_press_test, CONKEY(DevKey1, EventKeyPress));
  sys_register(RegKey, key1_release_test, CONKEY(DevKey1, EventKeyReleas));
  sys_register(RegKey, key2_press_test, CONKEY(DevKey2, EventKeyPress));
  sys_register(RegKey, key2_release_test, CONKEY(DevKey2, EventKeyReleas));
  sys_register(RegVib, vib_start_test, EventVibStart);
  sys_register(RegHall, hall_get_close_test, EventHallGetClose);
  sys_register(RegHall, hall_get_away_test, EventHallGetAway);
  sys_register(RegNav, nav_up_press_test, CONNAV(DevNavUp, EventNavPress));
  sys_register(RegNav, nav_up_release_test, CONNAV(DevNavUp, EventNavRelease));
  sys_register(RegNav, nav_down_press_test, CONNAV(DevNavDown, EventNavPress));
  sys_register(RegNav, nav_down_release_test, CONNAV(DevNavDown, EventNavRelease));
  sys_register(RegNav, nav_left_press_test, CONNAV(DevNavLeft, EventNavPress));
  sys_register(RegNav, nav_left_release_test, CONNAV(DevNavLeft, EventNavRelease));
  sys_register(RegNav, nav_right_press_test, CONNAV(DevNavRight, EventNavPress));
  sys_register(RegNav, nav_right_release_test, CONNAV(DevNavRight, EventNavRelease));
  sys_register(RegNav, nav_center_press_test, CONNAV(DevNavCenter, EventNavPress));
  sys_register(RegNav, nav_center_release_test, CONNAV(DevNavCenter, EventNavRelease));
  sys_register(RegNav, nav_key3_press_test, CONNAV(DevNavKey3, EventNavPress));
  sys_register(RegNav, nav_key3_release_test, CONNAV(DevNavKey3, EventNavRelease));
  sys_exec(0);
}
#elif defined(TEST_RTC_NVM)
#include "display.h"
#include "key.h"
#include "nvm.h"
#include "rtc.h"
#include "sys.h"

static uint8_t cnt1 = 0, cnt2 = 0;
static uint8_t mode = 0;
void _10ms_test(void) {
  static uint8_t i = 0;
  display_seg[0] = display_num_decoding[i];
  if(++i == 10) {
    i = 0;
  }
}
void _100ms_test(void) {
  rtc_read();
  switch(mode) {
    case 0 :
      display_seg[0] = display_num_decoding[rtc.year / 10];
      display_seg[1] = display_num_decoding[rtc.year % 10];
      display_seg[2] = 0x40;
      display_seg[3] = display_num_decoding[rtc.month / 10];
      display_seg[4] = display_num_decoding[rtc.month % 10];
      display_seg[5] = 0x40;
      display_seg[6] = display_num_decoding[rtc.date / 10];
      display_seg[7] = display_num_decoding[rtc.date % 10];
      break;
    case 1 :
      display_seg[0] = display_num_decoding[rtc.hour / 10];
      display_seg[1] = display_num_decoding[rtc.hour % 10];
      display_seg[2] = 0x40;
      display_seg[3] = display_num_decoding[rtc.minute / 10];
      display_seg[4] = display_num_decoding[rtc.minute % 10];
      display_seg[5] = 0x40;
      display_seg[6] = display_num_decoding[rtc.second / 10];
      display_seg[7] = display_num_decoding[rtc.second % 10];
      break;
    case 2 :
      display_seg[0] = display_num_decoding[rtc.day / 10];
      display_seg[1] = display_num_decoding[rtc.day % 10];
      display_seg[2] = 0x40;
      display_seg[3] = display_num_decoding[cnt1 / 10];
      display_seg[4] = display_num_decoding[cnt1 % 10];
      display_seg[5] = 0x40;
      display_seg[6] = display_num_decoding[cnt2 / 10];
      display_seg[7] = display_num_decoding[cnt2 % 10];
      break;
    default :
      break;
  }
}
void key1_press_test(void) {
  mode = (mode + 1) % 3;
}
void key2_press_test(void) {
  rtc.year = 0;
  rtc.month = 0;
  rtc.date = 0;
  rtc.hour = 0;
  rtc.minute = 0;
  rtc.second = 0;
  rtc_write();
  cnt1 = 0;
  cnt2 = 0;
  nvm_write(0x00, 0);
  rtc_nvm_write(0x00, 0);
}
void main(void) {
  sys_init(27000000);
  key_init();
  rtc_init();
  display_init();
  display_area(0, 8);
  cnt1 = rtc_nvm_read(0x00);
  rtc_nvm_write(0x00, cnt1 + 1);
  cnt2 = nvm_read(0x00);
  nvm_write(0x00, cnt2 + 1);
  sys_register(RegTimer, _100ms_test, EventTimer100ms);
  sys_register(RegKey, key1_press_test, CONKEY(DevKey1, EventKeyPress));
  sys_register(RegKey, key2_press_test, CONKEY(DevKey2, EventKeyPress));
  sys_exec(0);
}
#elif defined(TEST_BEEP)
// #include "adc.h"
#include "beep.h"
// #include "display.h"
#include "key.h"
#include "sys.h"

void key1_press_test(void) {
  static bit flag = 0;
  if(flag) {
    flag = 0;
    beep_on();
    // beep_off();
    // display_led &= 0x7f;
  } else {
    flag = 1;
    // beep_on();
    // display_led |= 0x80;
    CCAPM0 &= ~0x01;
    P3_4 = 0;  // pull voltage to low to protect buzzer
  }
}
// void nav_up_press_test(void) {
//   freq += 10;
//   display_seg[4] = display_num_decoding[freq / 1000];
//   display_seg[5] = display_num_decoding[(freq / 100) % 10];
//   display_seg[6] = display_num_decoding[(freq / 10) % 10];
//   display_seg[7] = display_num_decoding[freq % 10];
// }
// void nav_down_press_test(void) {
//   freq -= 10;
//   display_seg[4] = display_num_decoding[freq / 1000];
//   display_seg[5] = display_num_decoding[(freq / 100) % 10];
//   display_seg[6] = display_num_decoding[(freq / 10) % 10];
//   display_seg[7] = display_num_decoding[freq % 10];
// }
void main(void) {
  sys_init(11059200);
  key_init();
  // adc_init();
  beep_init();
  // display_init();
  // display_area(0, 8);
  sys_register(RegKey, key1_press_test, CONKEY(DevKey1, EventKeyPress));
  // sys_register(RegNav, nav_up_press_test, CONNAV(DevNavUp, EventNavPress));
  // sys_register(RegNav, nav_down_press_test, CONNAV(DevNavDown, EventNavPress));
  sys_exec(0);
}
#else
#include "display.h"
#include "sys.h"

uint8_t U8T_data_H_temp, U8T_data_L_temp, U8RH_data_H_temp, U8RH_data_L_temp, U8checkdata_temp, U8comdata;
uint8_t buf[5];
static bit over = 0;
static uint8_t cnt = 0, U8FLAG = 0;
void _10ms(void) {
  if(cnt == 0) {
    over = 1;
  } else {
    --cnt;
  }
}
void Delay10ms()  //@11.0592MHz
{
  unsigned char data i, j;

  i = 108;
  j = 145;
  do {
    while(--j)
      ;
  } while(--i);
}
void Delay_10us(void) {
  unsigned char data i;
  _nop_();
  i = 25;
  while(--i)
    ;
}
void Delay70us()  //@11.0592MHz
{
  unsigned char data i;

  i = 191;
  while(--i)
    ;
}
void Delay50us()  //@11.0592MHz
{
  unsigned char data i;

  _nop_();
  _nop_();
  _nop_();
  i = 135;
  while(--i)
    ;
}
void COM(void) {
  uint8_t i;

  for(i = 0; i < 8; i++) {
    while(P1_0)
      ;
    while(!P1_0)
      ;
    Delay_10us();
    Delay_10us();
    Delay_10us();
    Delay_10us();
    Delay_10us();
    U8comdata <<= 1;
    if(P1_0) {
      U8comdata |= 1;  // 0
    };
  }  // rof
}
uint8_t modecnt = 0;
void RH(void) {
  // 主机拉低18ms
  P1_0 = 0;
  Delay10ms();
  Delay10ms();
  P1_0 = 1;
  // 总线由上拉电阻拉高 主机延时20us
  Delay_10us();
  Delay_10us();
  Delay_10us();
  // 主机设为输入 判断从机响应信号
  // P1_0 = 1;
  // 判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
  if(!P1_0)  // T !
  {
    // uart_send(DevUART1, "3", 1);
    display_seg[7] = display_num_decoding[modecnt];
    modecnt = (modecnt + 1) % 10;
    // 判断从机是否发出 80us 的低电平响应信号是否结束
    while(!P1_0)
      ;
    Delay50us();
    // 判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
    // 数据接收状态
    COM();
    buf[0] = U8comdata;
    // COM();
    // buf[1] = U8comdata;
    // COM();
    // buf[2] = U8comdata;
    // COM();
    // buf[3] = U8comdata;
    // COM();
    // buf[4] = U8comdata;
    // P1_0 = 1;
    // 数据校验
    // if(buf[0] + buf[1] + buf[2] + buf[3] == buf[4]) {
    display_seg[0] = display_num_decoding[buf[0] / 10];
    display_seg[1] = display_num_decoding[buf[0] % 10];
    //   display_seg[3] = display_num_decoding[buf[2] / 10];
    //   display_seg[4] = display_num_decoding[buf[2] % 10];
    // uart_send(DevUART1, buf, 4);
    // }  //   U8temp = (U8T_data_H_temp + U8T_data_L_temp + U8RH_data_H_temp + U8RH_data_L_temp);
    //   if(U8temp == U8checkdata_temp) {
    //     U8RH_data_H = U8RH_data_H_temp;
    //     U8RH_data_L = U8RH_data_L_temp;
    //     U8T_data_H = U8T_data_H_temp;
    //     U8T_data_L = U8T_data_L_temp;
    //     U8checkdata = U8checkdata_temp;
    // }  // fi
  }  // fi
}
void _1s(void) {
  // uart_send(DevUART1, "Hello World!\r\n", 14);
  static uint8_t mode = 0;
  if(mode == 1) {
    RH();
    mode = 0;
  } else {
    ++mode;
  }
}
void main(void) {
  sys_init(11059200);
  display_init();
  display_area(0, 8);
  for(modecnt = 0; modecnt < 8; modecnt++) {
    display_seg[modecnt] = 0;
  }
  display_seg[2] = 0x40;
  display_seg[5] = 0x40;
  sys_register(RegTimer, _10ms, EventTimer10ms);
  sys_register(RegTimer, _1s, EventTimer1S);
  // RH();

  sys_exec(0);
}

#endif
#elif TEST == 1
#include "sys.h"
#include "uart.h"
char buf[11];
void uart1_recv_test(void) {
  uint8_t i = 0;
  for(; i < 5; ++i) {
    buf[i] ^= buf[9 - i];
    buf[9 - i] ^= buf[i];
    buf[i] ^= buf[9 - i];
  }
  uart_send(DevUART1, buf, 10);
}
void main(void) {
  sys_init(27000000);
  uart_init(InitUART1);
  uart_cfg_recv(DevUART1, buf, 10);
  sys_register(RegUart, uart1_recv_test, CONUART(DevUART1, EventUartRecvOver));
  sys_exec(0);
}
#elif TEST == 2
#include "display.h"
#include "sys.h"
#include "uart.h"

char buf1[6];
char buf2[6];
void uart1_recv_test(void) {
  uart_send(DevUART2, buf1, 5);
}
void uart2_recv_test(void) {
  display_seg[0] = display_num_decoding[buf2[0]];
  display_seg[1] = display_num_decoding[buf2[1]];
  display_seg[2] = display_num_decoding[buf2[2]];
  display_seg[3] = display_num_decoding[buf2[3]];
  display_seg[4] = display_num_decoding[buf2[4]];
  display_led = buf2[0] + buf2[1] + buf2[2] + buf2[3] + buf2[4];
}
void main(void) {
  sys_init(27000000);
  uart_init(InitUART1 | InitUART2_Ext);
  display_init();
  display_area(0, 8);
  uart_cfg_recv(DevUART1, buf1, 5);
  uart_cfg_recv(DevUART2, buf2, 5);
  sys_register(RegUart, uart1_recv_test, CONUART(DevUART1, EventUartRecvOver));
  sys_register(RegUart, uart2_recv_test, CONUART(DevUART2, EventUartRecvOver));
  sys_exec(0);
}
#elif TEST == 3
#include "display.h"
#include "key.h"
#include "rtc.h"
#include "sys.h"

void key1_press_test(void) {
  rtc.hour = 0;
  rtc.minute = 0;
  rtc.second = 0;
  rtc_write();
}
void key2_press_test(void) {
  rtc_read();
  display_seg[0] = display_num_decoding[rtc.second % 10];
  display_seg[1] = display_num_decoding[rtc.second / 10];
  display_seg[2] = 0x40;
  display_seg[3] = display_num_decoding[rtc.minute % 10];
  display_seg[4] = display_num_decoding[rtc.minute / 10];
  display_seg[5] = 0x40;
  display_seg[6] = display_num_decoding[rtc.year % 10];
  display_seg[7] = display_num_decoding[rtc.year / 10];
}
void main(void) {
  sys_init(27000000);
  key_init();
  rtc_init();
  display_init();
  sys_register(RegKey, key1_press_test, CONKEY(DevKey1, EventKeyPress));
  sys_register(RegKey, key2_press_test, CONKEY(DevKey2, EventKeyPress));
  sys_exec(0);
}
#elif TEST == 4
#include "display.h"
#include "nvm.h"
#include "sys.h"

uint8_t v;
void main(void) {
  sys_init(27000000);
  display_init();
  v = nvm_read(0x00);
  display_led = v;
  nvm_write(0x00, v + 1);
  sys_exec(0);
}
#elif TEST == 8
#include "adc.h"
#include "display.h"
#include "sys.h"

void _100ms_test(void) {
  display_seg[0] = display_num_decoding[adcs.rt % 10];
  display_seg[1] = display_num_decoding[adcs.rt / 10];
}
void main(void) {
  sys_init(27000000);
  display_init();
  adc_init();
  sys_register(RegTimer, _100ms_test, EventTimer100ms);
  sys_exec(0);
}
#endif
