#ifndef __UART_H__
#define __UART_H__
#include "def.h"
#include "detail/sys.h"
#define __UART1_VECTOR SI0_VECTOR
#define __UART2_VECTOR SI1_VECTOR
#define __UART_DEFAULT_BAUDRATE 9600
#define __UART_RX_BUF_SIZE 8

extern uint8_t UART;
enum UartInit { InitUART1 = 1, InitUART2_Ext = 2, InitUART2_485 = 6 };
enum UartDev { DevUART1 = 0, DevUART2 = 1 };
enum UartEvent { EventUartRecvOver,EventUartSendOver };
#define CONUART(dev, event) (dev << 1 | event)
void uart_init(uint8_t ini);
void uart_send(enum UartDev dev, void *buf, uint8_t len);
void uart_cfg_recv(enum UartDev cfg, void *buf, uint16_t cnt);
#endif