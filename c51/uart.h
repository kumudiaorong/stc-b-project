#ifndef __UART_H__
#define __UART_H__
#include "def.h"
#include "detail/sys.h"
#define __UART_VECTOR SI0_VECTOR
#define __UART_DEFAULT_BAUDRATE 9600
#define __UART_RX_BUF_SIZE 8
#define __UART_INIT()                                                                                            \
  __DO_WHILE0(AUXR &= ~0x1; PS = 1; SCON |= 0x50; TH1 = (65536 - (__sysclk / __UART_DEFAULT_BAUDRATE / 4)) >> 8; \
              TL1 = (65536 - (__sysclk / __UART_DEFAULT_BAUDRATE / 4)) & 0xFF; RI = 0; TI = 0; ES = 1)
typedef struct {
  uint8_t *buf;
  uint16_t len;
} uart_send_msg_t;
extern uint8_t UART;
enum UartEvent { UARTSENDOVER, UARTRECVOVER, UARTNONE };
#define CONUART(cfg, event) (cfg << 2 | event)
void uart_init(void);
void uart_send(void *buf, uint8_t len);
uint8_t uart_cfg_recv(void *buf, uint16_t cnt);
#endif