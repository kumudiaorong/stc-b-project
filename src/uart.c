#include "uart.h"

#include "detail/sys.h"

#define __UART1_VECTOR SI0_VECTOR
#define __UART2_VECTOR S2IO_VECTOR

uint8_t UART;
typedef struct {
  uint8_t *buf;
  uint16_t len;
  uint16_t idx;

  sys_callback_t callback;
} uart_cfg_t;
static XDATA uart_cfg_t uart_cfg[2 * 2] = {0};
uint8_t uart_gen_cfg(uint8_t cfg, uint32_t baudrate, void *buf, uint16_t len) {
  switch(cfg & 0x2) {
    case 0x0 :
      PS = 1;
      SCON |= 0x50;
      TH1 = (65536 - (__sysclk / baudrate / 4)) >> 8;
      TL1 = (65536 - (__sysclk / baudrate / 4)) & 0xFF;
      AUXR |= 0x40;
      ES = 1;
      break;
    case 0x2 :
      IP2 |= 0x1;
      S2CON |= 0x10;
      P_SW2 = P_SW2 & (~0x1) | cfg >> 2;
      T2H = (65536 - (__sysclk / baudrate / 4)) >> 8;
      T2L = (65536 - (__sysclk / baudrate / 4)) & 0xFF;
      AUXR |= 0x04;
      IE2 |= 0x01;
      break;
  }
  if(cfg & 0x2)
    P_SW2 = P_SW2 & (~0x1) | cfg >> 2;
  uart_cfg[cfg & 0x3].buf = (uint8_t *)buf;
  uart_cfg[cfg & 0x3].len = len;
  uart_cfg[cfg & 0x3].idx = 0;
  return cfg & 0x3;
}
static void uart_register(uint32_t cfg, sys_callback_t callback) {
  uart_cfg[cfg & 0x3].callback = callback;
}
static void uart_callback(__sys_msg_t msg) REENTRANT {
  uart_cfg[msg - 1].callback();
  uart_cfg[msg - 1].idx = 0;
}
void uart_init(void) {
  AUXR &= ~0x1;
  UART = __sys_sensor_add(uart_register, 0, uart_callback);
}
INTERRUPT_USING(__uart1, __UART1_VECTOR, 1) {
  while(RI) {
    RI = 0;
    if(uart_cfg[1].len && uart_cfg[1].idx < uart_cfg[1].len) {
      uart_cfg[1].buf[uart_cfg[1].idx++] = SBUF;
      if(uart_cfg[1].idx == uart_cfg[1].len && uart_cfg[1].callback) {
        __sys.sensor[UART].msg |= 2;
      }
    }
  }
  while(TI) {
    TI = 0;
    if(uart_cfg[0].idx < uart_cfg[0].len) {
      SBUF = uart_cfg[0].buf[uart_cfg[0].idx++];
    } else if(uart_cfg[0].callback) {
      __sys.sensor[UART].msg |= 1;
    }
  }
}
INTERRUPT_USING(__uart2, __UART2_VECTOR, 1) {
  while(S2CON & 0x01) {
    S2CON &= ~0x01;
    if(uart_cfg[3].len && uart_cfg[3].idx < uart_cfg[3].len) {
      uart_cfg[3].buf[uart_cfg[3].idx++] = S2BUF;
      if(uart_cfg[3].idx == uart_cfg[3].len && uart_cfg[3].callback) {
        __sys.sensor[UART].msg |= 4;
      }
    }
  }
  while(S2CON & 0x02) {
    S2CON &= ~0x02;
    if(uart_cfg[2].idx < uart_cfg[2].len) {
      S2BUF = uart_cfg[2].buf[uart_cfg[2].idx++];
    } else if(uart_cfg[2].callback) {
      __sys.sensor[UART].msg |= 3;
    }
  }
}
void uart_send(enum UartPort port) {
  uart_cfg[port & 0x2].idx = 1;
  if(port & 0x2)
    S2BUF = uart_cfg[port & 0x2].buf[0];
  else
    SBUF = uart_cfg[port & 0x2].buf[0];
}
