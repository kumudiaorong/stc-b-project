#include "adc.h"
#include "display.h"
#include "hall.h"
#include "key.h"
#include "rtc.h"
#include "sys.h"
#include "uart.h"
#include "vib.h"

static uint8_t mode = 0;
static uint8_t m_time = 0;
static bit m_t_update = 0;
static bit m_t_d_idx = 0;
static uint8_t m_t_e_idx = 0;
static uint8_t m_t_e_cnt = 0;
#define m_t_e_c_max 10
static uint8_t m_s_cnt = 0;
#define m_s_c_max 30
static uint8_t test_cnt = 0;
rtc_t editrtc;
rtc_t *prtc;
void _key1(void) {
  mode = (mode + 1) % 2;
  if(mode == 1) {
    m_s_cnt = 0;
    display_seg[7] = 0x00;
  }
}
void _key2(void) {
  if(mode == 0) {
    if(m_time) {
    } else {
      m_t_e_idx = 0;
      m_t_e_cnt = 0;
    }
    m_time = (m_time + 1) % 2;
  } else {
  }
}
void _key3(void) {
  if(mode == 0) {
    m_t_d_idx = !m_t_d_idx;
  } else {
  }
}
void _nav_up(void) {
  if(mode == 0) {
    if(m_time) {
      if(m_t_d_idx) {
        switch(m_t_e_idx) {
          case 0 :
            rtc.year = (rtc.year + 10) % 100;
            break;
          case 1 :
            rtc.year = (rtc.year + 1) % 10;
            break;
          case 3 :
            rtc.month = (rtc.month + 10) % 100;
            break;
          case 4 :
            rtc.month = (rtc.month + 1) % 10;
            break;
          case 6 :
            rtc.date = (rtc.date + 10) % 100;
            break;
          case 7 :
            rtc.date = (rtc.date + 1) % 10;
            break;
          default :
            break;
        }
      } else {
        switch(m_t_e_idx) {
          case 0 :
            rtc.hour = (rtc.hour + 10) % 100;
            break;
          case 1 :
            rtc.hour = (rtc.hour + 1) % 10;
            break;
          case 3 :
            rtc.minute = (rtc.minute + 10) % 100;
            break;
          case 4 :
            rtc.minute = (rtc.minute + 1) % 10;
            break;
          case 6 :
            rtc.second = (rtc.second + 10) % 100;
            break;
          case 7 :
            rtc.second = (rtc.second + 1) % 10;
            break;
          default :
            break;
        }
      }
    } else {
    }
  }
}
void _nav_down(void) {
  if(mode == 0) {
    if(m_time) {
      if(m_t_d_idx) {
        switch(m_t_e_idx) {
          case 0 :
            rtc.year = (rtc.year + 90) % 100;
            break;
          case 1 :
            rtc.year = (rtc.year + 9) % 10;
            break;
          case 3 :
            rtc.month = (rtc.month + 90) % 100;
            break;
          case 4 :
            rtc.month = (rtc.month + 9) % 10;
            break;
          case 6 :
            rtc.date = (rtc.date + 90) % 100;
            break;
          case 7 :
            rtc.date = (rtc.date + 9) % 10;
            break;
          default :
            break;
        }
      } else {
        switch(m_t_e_idx) {
          case 0 :
            rtc.hour = (rtc.hour + 90) % 100;
            break;
          case 1 :
            rtc.hour = (rtc.hour + 9) % 10;
            break;
          case 3 :
            rtc.minute = (rtc.minute + 90) % 100;
            break;
          case 4 :
            rtc.minute = (rtc.minute + 9) % 10;
            break;
          case 6 :
            rtc.second = (rtc.second + 90) % 100;
            break;
          case 7 :
            rtc.second = (rtc.second + 9) % 10;
            break;
          default :
            break;
        }
      }
    } else {
    }
  } else {
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
  if(mode == 0) {
    if(m_time) {
      rtc_write();
    }
  } else {
  }
}
void _hall(void) {
  if(mode == 0) {
  } else if(mode == 1) {
    display_seg[7] |= 0x06;
    m_s_cnt = 0;
  } else {
  }
}
void _vib(void) {
  if(mode == 0) {
  } else if(mode == 1) {
    display_seg[7] |= 0x30;
    m_s_cnt = 0;
  } else {
  }
}
void _100ms(void) {
  if(mode == 0) {
    if((!m_time) & m_t_update) {
      rtc_read();
    } else {
      if(m_t_d_idx) {
        display_seg[0] = display_num_decoding[rtc.year / 10];
        display_seg[1] = display_num_decoding[rtc.year % 10];
        display_seg[2] = 0x40;
        display_seg[3] = display_num_decoding[rtc.month / 10];
        display_seg[4] = display_num_decoding[rtc.month % 10];
        display_seg[5] = 0x40;
        display_seg[6] = display_num_decoding[rtc.date / 10];
        display_seg[7] = display_num_decoding[rtc.date % 10];
      } else {
        display_seg[0] = display_num_decoding[rtc.hour / 10];
        display_seg[1] = display_num_decoding[rtc.hour % 10];
        display_seg[2] = 0x40;
        display_seg[3] = display_num_decoding[rtc.minute / 10];
        display_seg[4] = display_num_decoding[rtc.minute % 10];
        display_seg[5] = 0x40;
        display_seg[6] = display_num_decoding[rtc.second / 10];
        display_seg[7] = display_num_decoding[rtc.second % 10];
      }
      if(m_time) {
        if(m_t_e_cnt < (m_t_e_c_max >> 1)) {
          display_seg[m_t_e_idx] = 0;
        } else if(m_t_e_cnt == m_t_e_c_max) {
          m_t_e_cnt = 0;
        }
        ++m_t_e_cnt;
      }
      display_seg[0] = display_num_decoding[test_cnt];
    }
    m_t_update = !m_t_update;
  } else if(mode == 1) {
    display_seg[0] = display_num_decoding[adc.rop / 100];
    display_seg[1] = display_num_decoding[(adc.rop / 10) % 10];
    display_seg[2] = display_num_decoding[adc.rop % 10];
    display_seg[3] = 0x40;
    display_seg[4] = display_num_decoding[adc.rt / 10];
    display_seg[5] = display_num_decoding[adc.rt % 10];
    display_seg[6] = 0x40;
    if(m_s_cnt == m_s_c_max) {
      display_seg[7] = 0;
    } else {
      ++m_s_cnt;
    }
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
  rtc_read();
  rtc_charge();
  display_area(0, 8);
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
  sys_exec(0);
}