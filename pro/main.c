#include "adc.h"
#include "display.h"
#include "hall.h"
#include "key.h"
#include "rtc.h"
#include "sys.h"
#include "uart.h"
#include "vib.h"

static bit mode = 0;
void _key1(void) {
  mode = !mode;
}
static bit m_time = 0;
static bit m_t_update = 0;
static bit m_t_d_idx = 0;
static uint8_t m_t_e_idx = 0;
static uint8_t m_t_e_cnt = 0;
#define m_t_e_c_max 6
static uint8_t m_s_h_cnt = 0;
static uint8_t m_s_v_cnt = 0;
#define m_s_c_max 30
static bit m_s_hall = 0;
static bit m_s_vib = 0;
static uint8_t test_cnt = 0;
static uint8_t uart1rbuf[1];
static uint8_t uart2rbuf[1];

void _key2(void) {
  if(mode == 0) {
    if(!m_time) {
      m_t_e_idx = 0;
      m_t_e_cnt = 0;
    }
    m_time = !m_time;
  } else {
    sys_reset();
  }
}
void _key3(void) {
  if(mode == 0) {
    m_t_d_idx = !m_t_d_idx;
  }
}
void _change(uint8_t step) {
  if(m_t_d_idx) {
    switch(m_t_e_idx) {
      case 0 :
        rtc.year = (rtc.year + step * 10) % 100;
        break;
      case 1 :
        rtc.year = (rtc.year + step) % 10;
        break;
      case 3 :
        rtc.month = (rtc.month + step * 10) % 100;
        break;
      case 4 :
        rtc.month = (rtc.month + step) % 10;
        break;
      case 6 :
        rtc.date = (rtc.date + step * 10) % 100;
        break;
      case 7 :
        rtc.date = (rtc.date + step) % 10;
        break;
      default :
        break;
    }
  } else {
    switch(m_t_e_idx) {
      case 0 :
        rtc.hour = (rtc.hour + step * 10) % 100;
        break;
      case 1 :
        rtc.hour = (rtc.hour + step) % 10;
        break;
      case 3 :
        rtc.minute = (rtc.minute + step * 10) % 100;
        break;
      case 4 :
        rtc.minute = (rtc.minute + step) % 10;
        break;
      case 6 :
        rtc.second = (rtc.second + step * 10) % 100;
        break;
      case 7 :
        rtc.second = (rtc.second + step) % 10;
        break;
      default :
        break;
    }
  }
}
void _nav_up(void) {
  if(mode == 0 && m_time && m_t_d_idx) {
    _change(1);
  }
}
void _nav_down(void) {
  if(mode == 0 && m_time && m_t_d_idx) {
    _change(9);
  }
}
void _nav_left(void) {
  if(mode == 0) {
    if(m_time) {
      if(m_t_e_idx == 0)
        m_t_e_idx = 7;
      else if(m_t_e_idx == 3)
        m_t_e_idx -= 2;
      else if(m_t_e_idx == 6)
        m_t_e_idx -= 2;
      else {
        --m_t_e_idx;
      }
    } else {
    }
  }
}
void _nav_right(void) {
  if(mode == 0) {
    if(m_time) {
      if(m_t_e_idx == 7)
        m_t_e_idx = 0;
      else if(m_t_e_idx == 1)
        m_t_e_idx += 2;
      else if(m_t_e_idx == 4)
        m_t_e_idx += 2;
      else {
        ++m_t_e_idx;
      }
    } else {
    }
  } else {
  }
}
void _nav_center(void) {
  if(mode == 0 && m_time) {
    rtc_write();
  }
}
void _hall(void) {
  m_s_hall = 1;
}
void _vib(void) {
  m_s_vib = 1;
}
static uint8_t uart1sbuf[4];
static uint8_t uart2sbuf[4];
void _uart1_recv(void) {
  display_led |= 0x02;
   uart1sbuf[0] = adc.rop;
   uart1sbuf[1] = adc.rop >> 8;
   uart1sbuf[2] = adc.rt;
   uart1sbuf[3] = (m_s_v_cnt > 0 ? 0x02 : 0x00) | (m_s_h_cnt > 0 ? 0x01 : 0x00);

  // _set_sbuf(0);
  uart_send(DevUART1, uart1sbuf, 4);
}
void _uart2_recv(void) {
  display_led |= 0x01;
   uart2sbuf[0] = adc.rop;
   uart2sbuf[1] = adc.rop >> 8;
   uart2sbuf[2] = adc.rt;
   uart2sbuf[3] = (m_s_v_cnt > 0 ? 0x02 : 0x00) | (m_s_h_cnt > 0 ? 0x01 : 0x00);

  // _set_sbuf(1);
  uart_send(DevUART2, uart2sbuf, 4);
}
void _uart1_send(void) {
  display_led &= ~0x02;
}
void _uart2_send(void) {
  display_led &= ~0x01;
}
void _set_display(uint8_t _0, uint8_t _1, uint8_t _2, uint8_t _3, uint8_t _4, uint8_t _5, uint8_t _6, uint8_t _7) {
  display_seg[0] = _0;
  display_seg[1] = _1;
  display_seg[2] = _2;
  display_seg[3] = _3;
  display_seg[4] = _4;
  display_seg[5] = _5;
  display_seg[6] = _6;
  display_seg[7] = _7;
}
void _100ms(void) {
  if(m_s_hall) {
    m_s_h_cnt = m_s_c_max;
    m_s_hall = 0;
  } else {
    if(m_s_h_cnt) {
      --m_s_h_cnt;
    }
  }
  if(m_s_vib) {
    m_s_v_cnt = m_s_c_max;
    m_s_vib = 0;
  } else {
    if(m_s_v_cnt) {
      --m_s_v_cnt;
    }
  }
  if(mode == 0) {
    if((!m_time) & m_t_update) {
      rtc_read();
    } else {
      if(m_t_d_idx) {
        _set_display(display_num_decoding[rtc.year / 10], display_num_decoding[rtc.year % 10], 0x40,
          display_num_decoding[rtc.month / 10], display_num_decoding[rtc.month % 10], 0x40,
          display_num_decoding[rtc.date / 10], display_num_decoding[rtc.date % 10]);
      } else {
        _set_display(display_num_decoding[rtc.hour / 10], display_num_decoding[rtc.hour % 10], 0x40,
          display_num_decoding[rtc.minute / 10], display_num_decoding[rtc.minute % 10], 0x40,
          display_num_decoding[rtc.second / 10], display_num_decoding[rtc.second % 10]);
      }
      if(m_time) {
        if(m_t_e_cnt < (m_t_e_c_max >> 1)) {
          display_seg[m_t_e_idx] = 0;
        } else if(m_t_e_cnt == m_t_e_c_max) {
          m_t_e_cnt = 0;
        }
        ++m_t_e_cnt;
      }
    }
    m_t_update = !m_t_update;
  } else if(mode == 1) {
    _set_display(display_num_decoding[adc.rop / 100], display_num_decoding[(adc.rop / 10) % 10],
      display_num_decoding[adc.rop % 10], 0x40, display_num_decoding[adc.rt / 10], display_num_decoding[adc.rt % 10],
      0x40, (m_s_h_cnt > 0 ? 0x30 : 0x00) | (m_s_v_cnt > 0 ? 0x06 : 0x00));
  }
}
void main(void) {
  sys_init(27000000);
  display_init();
  adc_init();
  key_init();
  rtc_init();
  vib_init();
  hall_init();
  uart_init(InitUART1 | InitUART2_Ext);
  rtc_read();
  rtc_charge();
  display_area(0, 8);
  uart_cfg_recv(DevUART1, uart1rbuf, 1);
  uart_cfg_recv(DevUART2, uart2rbuf, 1);
  sys_register(RegTimer, _100ms, EventTimer100ms);
  sys_register(RegKey, _key1, CONKEY(DevKey1, EventKeyPress));
  sys_register(RegKey, _key2, CONKEY(DevKey2, EventKeyPress));
  sys_register(RegNav, _key3, CONKEY(DevNavKey3, EventKeyPress));
  sys_register(RegNav, _nav_up, CONKEY(DevNavUp, EventKeyPress));
  sys_register(RegNav, _nav_down, CONKEY(DevNavDown, EventKeyPress));
  sys_register(RegNav, _nav_left, CONKEY(DevNavLeft, EventKeyPress));
  sys_register(RegNav, _nav_right, CONKEY(DevNavRight, EventKeyPress));
  sys_register(RegNav, _nav_center, CONKEY(DevNavCenter, EventKeyPress));
  sys_register(RegHall, _hall, EventHallGetClose);
  sys_register(RegVib, _vib, EventVibStart);
  sys_register(RegUart, _uart1_recv, CONUART(DevUART1, EventUartRecvOver));
  sys_register(RegUart, _uart1_recv, CONUART(DevUART1, EventUartSendOver));
  sys_register(RegUart, _uart2_recv, CONUART(DevUART2, EventUartRecvOver));
  sys_register(RegUart, _uart2_send, CONUART(DevUART2, EventUartSendOver));
  sys_exec();
}