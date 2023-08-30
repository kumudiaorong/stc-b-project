// #include "uart.h"

// #include "detail/sys.h"
// #include "detail/uart.h"
// // static bit uart_tx_busy = 0;                           //!< uart tx busy flag
// static XDATA uint8_t rx_buf[__UART_RX_BUF_SIZE];       //!< uart rx buffer
// static XDATA uint16_t rx_msg_idx = 0, rx_buf_idx = 0;  //!< uart rx buffer index
// static XDATA uint16_t rx_buf_len = 0;                  //!< uart rx buffer length
// // uint8_t UART;
// XDATA uart_recv_cfg_t u1recv_cfg = {0};
// XDATA uart_send_cfg_t u2send_cfg = {0};
// INTERRUPT_USING(__uart1, __UART1_VECTOR, 1) {
//   while(RI) {
//     RI = 0;
//     if(u1recv_cfg.len) {
//       if(u1recv_cfg.idx < u1recv_cfg.len) {
//         u1recv_cfg.buf[u1recv_cfg.idx++] = SBUF;
//         if(u1recv_cfg.idx == u1recv_cfg.len) {
//           __sys_sensor_set_msg(UART,CONUART(UART1,UARTRECVOVER + 1));
//         }
//       }
//     } else if(rx_buf_idx < __UART_RX_BUF_SIZE) {
//       rx_buf[rx_buf_idx++] = SBUF;
//       rx_buf_len++;
//     }
//   }
//   while(TI) {
//     TI = 0;
//     if(u2send_cfg.idx < u2send_cfg.len) {
//       SBUF = u2send_cfg.buf[u2send_cfg.idx++];
//     } else if(u2send_cfg.callback) {
//       __sys_sensor_set_msg(UART, CONUART(UART1,UARTSENDOVER + 1));
//     }
//   }
// }
