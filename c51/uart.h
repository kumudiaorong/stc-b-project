#ifndef __UART_H__
#define __UART_H__
#include "def.h"
#include "detail/sys.h"
#define __UART1_VECTOR SI0_VECTOR
#define __UART2_VECTOR S2IO_VECTOR
#define __UART_DEFAULT_BAUDRATE 9600
#define __UART_RX_BUF_SIZE 8
#define __UART_CFG_CNT 4
typedef struct {
  uint8_t *buf;
  uint16_t len;
} uart_send_msg_t;
extern uint8_t UART;
enum UartEvent { UARTSENDOVER, UARTRECVOVER, UARTNONE };
enum UartPort { UARTUSB, UARTEXT = 0x2, UART485 = 0x6 };
#define CFGUART(port, event) ((port) | (event))
void uart_init(void);
void uart_send(enum UartPort port);
uint8_t uart_gen_cfg(uint8_t cfg, uint32_t baudrate, void *buf, uint16_t len);
#endif