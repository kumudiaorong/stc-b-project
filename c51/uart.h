#ifndef __UART_H__
#define __UART_H__
#include "def.h"
#include "detail/sys.h"
#define __UART1_VECTOR SI0_VECTOR
#define __UART2_VECTOR SI1_VECTOR
#define __UART_DEFAULT_BAUDRATE 2400
#define __UART_RX_BUF_SIZE 8

extern uint8_t UART;
enum UartInit { UART1 = 1, UART2_Ext = 2, UART2_485 = 6 };
enum UartEvent { UARTSENDOVER, UARTRECVOVER, UARTNONE };
#define CONUART(cfg, event) (cfg << 2 | event)
void uart_init(uint8_t cfg);
void uart_send(enum UartInit cfg, void *buf, uint8_t len) REENTRANT;
uint8_t uart_cfg_recv(enum UartInit cfg, void *buf, uint16_t cnt);
#endif