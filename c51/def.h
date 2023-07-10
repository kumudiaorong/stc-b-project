// #include <mcs51/compiler.h>
#ifndef __DEF_H__
#define __DEF_H__

#ifdef __C51__
#define XDATA xdata
#define CODE code
#define REENTRANT reentrant

#define SBIT(name, addr, bit) sbit name = addr ^ bit
#define SFR(name, addr) sfr name = addr
#define INTERRUPT(name, vector) void name(void) interrupt vector
#define INTERRUPT_USING(name, vector, regnum) void name(void) interrupt vector using regnum

extern void _nop_(void);
#define NOP() _nop_()

#elif defined(__SDCC) || defined(SDCC)
#define XDATA __xdata
#define REENTRANT __reentrant

#define SBIT(name, addr, bit) __sbit __at(addr + bit) name
#define SFR(name, addr) __sfr __at(addr) name
#define INTERRUPT(name, vector) void name(void) __interrupt(vector)
#define INTERRUPT_USING(name, vector, regnum) void name(void) __interrupt(vector) __using(regnum)

#define NOP() __asm NOP __endasm
#endif

#define __DO_WHILE0(x) \
  do {                \
    x;                \
  } while(0)
// #define xdata __xdata
// #define reentrant __reentrant
// #include"mcs51/compiler.h"

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short int uint16_t;
typedef short int int16_t;
typedef unsigned long int uint32_t;
typedef long int32_t;

SFR(P0, 0x80);
SBIT(P0_0, 0x80, 0);
SBIT(P0_1, 0x80, 1);
SBIT(P0_2, 0x80, 2);
SBIT(P0_3, 0x80, 3);
SBIT(P0_4, 0x80, 4);
SBIT(P0_5, 0x80, 5);
SBIT(P0_6, 0x80, 6);
SBIT(P0_7, 0x80, 7);

SFR(P1, 0x90);
SBIT(P1_0, 0x90, 0);
SBIT(P1_1, 0x90, 1);
SBIT(P1_2, 0x90, 2);
SBIT(P1_3, 0x90, 3);
SBIT(P1_4, 0x90, 4);
SBIT(P1_5, 0x90, 5);
SBIT(P1_6, 0x90, 6);
SBIT(P1_7, 0x90, 7);

SFR(P2, 0xA0);
SBIT(P2_3, 0xA0, 3);
SBIT(P2_4, 0xA0, 4);

SFR(P3, 0xB0);
SBIT(P3_0, 0xB0, 0);
SBIT(P3_1, 0xB0, 1);
SBIT(P3_2, 0xB0, 2);
SBIT(P3_3, 0xB0, 3);
SBIT(P3_4, 0xB0, 4);
SBIT(P3_5, 0xB0, 5);

SFR(P5, 0xC8);

SFR(TL0, 0x8A);
SFR(TL1, 0x8B);
SFR(TH0, 0x8C);
SFR(TH1, 0x8D);
SFR(T2H, 0xD6);  // 0000,0000 T2高字节
SFR(T2L, 0xD7);  // 0000,0000 T2低字节

SFR(P1ASF, 0x9D);  // 0000,0000 端口1模拟功能配置寄存器

SFR(CLK_DIV, 0x97);  // 0000,0000 时钟分频控制寄存器

SFR(P0M0, 0x94);
SFR(P0M1, 0x93);
SFR(P1M0, 0x92);
SFR(P1M1, 0x91);
SFR(P2M0, 0x96);
SFR(P2M1, 0x95);
SFR(P3M0, 0xB2);
SFR(P3M1, 0xB1);

SFR(IE, 0xA8);
SBIT(EA, 0xA8, 7);
SBIT(ET1, 0xA8, 3);
SBIT(EX1, 0xA8, 2);
SBIT(ET0, 0xA8, 1);
SBIT(EX0, 0xA8, 0);

SFR(TMOD, 0x89);

SFR(TCON, 0x88);
SBIT(TR1, 0x88, 6);
SBIT(TR0, 0x88, 4);
SBIT(IT1, 0x88, 2);
SBIT(IT0, 0x88, 0);

SFR(IP, 0xB8);
SBIT(PPCA, 0xB8, 7);
SBIT(PLVD, 0xB8, 6);
SBIT(PADC, 0xB8, 5);
SBIT(PS, 0xB8, 4);
SBIT(PT1, 0xB8, 3);
SBIT(PX1, 0xB8, 2);
SBIT(PT0, 0xB8, 1);
SBIT(PX0, 0xB8, 0);

/**
 * @brief Auxiliary Register
 * @note
 * |bit|name|description|
 * |---|----|-----------|
 * |7|T0x12|定时器0分频控制位,0:分频,1:不分频|
 * |6|T1x12|定时器1分频控制位,0:分频,1:不分频|
 * |5|UART_M0x6|串口1波特率发生器控制位|
 * |4|T2R|定时器2运行控制位,0:停止,1:运行|
 * |3|T2_CT|定时器2计数类型控制位,0:模式0,1:模式1|
 * |2|T2x12|定时器2分频控制位,0:分频,1:不分频|
 * |1|EXTRAM|外部RAM控制位|
 * |0|S1ST2|UART1中断波特率发生器控制位,0:定时器1,1:定时器2|
 */
SFR(AUXR, 0x8E);  // 0000,0001 辅助寄存器

#define IE0_VECTOR 0 /* 0x03 external interrupt 0 */
#define TF0_VECTOR 1 /* 0x0b timer 0 */
#define IE1_VECTOR 2 /* 0x13 external interrupt 1 */
#define TF1_VECTOR 3 /* 0x1b timer 1 */
#define SI0_VECTOR 4 /* 0x23 serial port 0 */
#endif               // !__DEF_H__
