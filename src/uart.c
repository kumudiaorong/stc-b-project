#include "uart.h"
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
