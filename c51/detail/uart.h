#ifndef __DETAIL_UART_H__
#define __DETAIL_UART_H__
#include "../sys.h"
#include "../def.h"
#include "../uart.h"
typedef struct {
  uint8_t *buf;
  uint16_t len;
  uint16_t idx;

  uint8_t rx_buf[__UART_RX_BUF_SIZE];  //!< uart rx buffer
  uint16_t rx_buf_idx;                 //!< uart rx buffer index

  sys_callback_t callback;
} uart_recv_cfg_t;
typedef struct {
  uint8_t *buf;
  uint16_t len;
  uint16_t idx;
  sys_callback_t callback;
} uart_send_cfg_t;
#endif