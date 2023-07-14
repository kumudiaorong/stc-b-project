#ifndef __UART_H__
#define __UART_H__
#include "def.h"
#include "detail/sys.h"
#define __UART_VECTOR SI0_VECTOR
#define __UART_DEFAULT_BAUDRATE 9600
#define __UART_RX_BUF_SIZE 8
#define __UART_INIT()                                                                              \
  __DO_WHILE0(PS = 1; SCON |= 0x50; T2H = (65536 - (__sysclk / __UART_DEFAULT_BAUDRATE / 4)) >> 8; \
              T2L = (65536 - (__sysclk / __UART_DEFAULT_BAUDRATE / 4)) & 0xFF; RI = 0; TI = 0; ES = 1)
typedef struct {
  uint8_t *buf;
  uint16_t len;
} uart_send_msg_t;
extern uint8_t UART;
enum UartEvent { UARTSENDOVER, UARTRECVOVER, UARTNONE };
#define CONUART(cfg, event) (cfg << 2 | event)
void uart_init(void);
void uart_send(void *buf, uint8_t len);
uint8_t uart_recv_byte(void);
uint8_t uart_recv_cfg(void *buf, uint16_t cnt, void *matchHead, uint16_t matchHeadSize);
uint8_t uart_recv(uint8_t *buf, uint8_t len);
uint8_t uart_cfg_recv(void *buf, uint16_t cnt);
#endif