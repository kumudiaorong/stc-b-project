// #include "uart.h"

// #include "detail/sys.h"
// #include "detail/uart.h"

// static bit uart_tx_busy = 0;                           //!< uart tx busy flag
// static XDATA uint8_t rx_buf[__UART_RX_BUF_SIZE];       //!< uart rx buffer
// static XDATA uint16_t rx_msg_idx = 0, rx_buf_idx = 0;  //!< uart rx buffer index
// static XDATA uint16_t rx_buf_len = 0;                  //!< uart rx buffer length
// uint8_t UART;
// typedef struct {
//   uint8_t *buf;
//   uint16_t len;
//   uint16_t idx;

//   uint8_t rx_buf[__UART_RX_BUF_SIZE];  //!< uart rx buffer
//   uint16_t rx_buf_idx;                 //!< uart rx buffer index

//   sys_callback_t callback;
// } uart_recv_cfg_t;
// typedef struct {
//   uint8_t *buf;
//   uint16_t len;
//   uint16_t idx;
//   sys_callback_t callback;
// } uart_send_cfg_t;
// static XDATA uart_recv_cfg_t recv_cfg = {0};
// static XDATA uart_send_cfg_t send_cfg = {0};
// uint8_t uart_cfg_recv(void *buf, uint16_t cnt) {
//   recv_cfg.buf = (uint8_t *)buf;
//   recv_cfg.len = cnt;
//   recv_cfg.idx = 0;
//   recv_cfg.rx_buf_idx = 0;
//   return 0;
// }
// static void uart_register(uint32_t cfg, sys_callback_t callback) {
//   switch(cfg & 0x3) {
//     case UARTRECVOVER :
//       recv_cfg.callback = callback;
//       break;
//     case UARTSENDOVER :
//       send_cfg.callback = callback;
//       break;
//     default :
//       break;
//   }
// }
// static void uart_callback(__sys_msg_t msg) REENTRANT {
//   switch((msg - 1) & 0x3) {
//     case UARTRECVOVER :
//       recv_cfg.callback();
//       if(recv_cfg.rx_buf_idx != 0) {
//         uint8_t i = 0;
//         if(recv_cfg.len < recv_cfg.rx_buf_idx) {
//           for(; i < recv_cfg.len; i++) {
//             recv_cfg.buf[i] = recv_cfg.rx_buf[i];
//           }
//           for(; i < recv_cfg.rx_buf_idx; i++) {
//             recv_cfg.rx_buf[i - recv_cfg.len] = recv_cfg.rx_buf[i];
//           }
//           recv_cfg.rx_buf_idx -= recv_cfg.len;
//           recv_cfg.idx = recv_cfg.len;
//         } else {
//           for(; i < recv_cfg.rx_buf_idx; i++) {
//             recv_cfg.buf[i] = recv_cfg.rx_buf[i];
//           }
//           recv_cfg.idx = recv_cfg.rx_buf_idx;
//           recv_cfg.rx_buf_idx = 0;
//         }
//       } else {
//         recv_cfg.idx = 0;
//       }
//       break;
//     case UARTSENDOVER :
//       send_cfg.callback();
//       break;
//     default :
//       break;
//   }
// }
// void uart2_init(void) {
//   AUXR |= 0x4;//timer1 divide by 12,timer2 divide by 12
//   PS = 1;//interrupt priority 1
//   SCON |= 0x50;//SM0 = 1, SM1 = 0, REN = 1 ,8-bit UART, variable baudrate, receive enable
//   TH1 = (65536 - (__sysclk / __UART_DEFAULT_BAUDRATE / 4)) >> 8;
//   TL1 = (65536 - (__sysclk / __UART_DEFAULT_BAUDRATE / 4)) & 0xFF;//set baudrate
//   RI = 0;//clear receive interrupt flag
//   TI = 0;//clear transmit interrupt flag
//   ES = 1;//enable serial interrupt
//   TCON |= 0x40;//timer1 run
//   AUXR |= 0x10;//timer2 run
//   UART = __sys_sensor_add(uart_register, 0, uart_callback);
// }
  
// INTERRUPT_USING(__uart, __UART_VECTOR, 1) {
//   while(RI) {
//     RI = 0;
//     if(recv_cfg.len) {
//       if(recv_cfg.idx < recv_cfg.len) {
//         recv_cfg.buf[recv_cfg.idx++] = SBUF;
//         if(recv_cfg.idx == recv_cfg.len) {
//           __sys_sensor_set_msg(UART, UARTRECVOVER + 1);
//         }
//       }
//     } else if(rx_buf_idx < __UART_RX_BUF_SIZE) {
//       rx_buf[rx_buf_idx++] = SBUF;
//       rx_buf_len++;
//     }
//   }
//   while(TI) {
//     TI = 0;
//     if(send_cfg.idx < send_cfg.len) {
//       SBUF = send_cfg.buf[send_cfg.idx++];
//     } else if(send_cfg.callback) {
//       __sys_sensor_set_msg(UART, UARTSENDOVER + 1);
//     }
//   }
// }
// void uart_send(void *buf, uint8_t len) {
//   if(len) {
//     send_cfg.buf = buf;
//     send_cfg.len = len;
//     send_cfg.idx = 1;
//     SBUF = send_cfg.buf[0];
//   }
// }
