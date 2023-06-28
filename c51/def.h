// #include <mcs51/compiler.h>
#ifndef __DEF_H__
#define __DEF_H__

#ifdef __C51__
#define SBIT(name, addr, bit) sbit name = addr ^ bit
#define SFR(name, addr) sfr name = addr
#define INTERRUPT(name, vector) void name(void) interrupt vector
#define XDATA xdata
#define REENTRANT reentrant
#elif defined(__SDCC) || defined(SDCC)
#define SBIT(name, addr, bit) __sbit __at(addr + bit) name
#define SFR(name, addr) __sfr __at(addr) name
#define INTERRUPT(name, vector) void name(void) __interrupt(vector)
#define XDATA __xdata
#define REENTRANT __reentrant
#endif
// #define xdata __xdata
// #define reentrant __reentrant
// #include"mcs51/compiler.h"

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
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

SFR(P3, 0xB0);
SBIT(P3_0, 0xB0, 0);
SBIT(P3_1, 0xB0, 1);
SBIT(P3_2, 0xB0, 2);
SBIT(P3_3, 0xB0, 3);

SFR(TL0, 0x8A);
SFR(TL1, 0x8B);
SFR(TH0, 0x8C);
SFR(TH1, 0x8D);
SFR(T2H, 0xD6);  // 0000,0000 T2���ֽ�
SFR(T2L, 0xD7);  // 0000,0000 T2���ֽ�

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
SBIT(ET0, 0xA8, 1);

SFR(TMOD, 0x89);

SFR(TCON, 0x88);
SBIT(TR1, 0x88, 6);
SBIT(TR0, 0x88, 4);

// sfr IP          =   0xB8;   //0000,0000 �ж����ȼ��Ĵ���
// sbit PPCA       =   IP^7;
// sbit PLVD       =   IP^6;
// sbit PADC       =   IP^5;
// sbit PS         =   IP^4;
// sbit PT1        =   IP^3;
// sbit PX1        =   IP^2;
// sbit PT0        =   IP^1;
// sbit PX0        =   IP^0;

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
 * |7|T0x12|��ʱ��0��Ƶ����λ,0:��Ƶ,1:����Ƶ|
 * |6|T1x12|��ʱ��1��Ƶ����λ,0:��Ƶ,1:����Ƶ|
 * |5|UART_M0x6|����1�����ʷ���������λ|
 * |4|T2R|��ʱ��2���п���λ,0:ֹͣ,1:����|
 * |3|T2_CT|��ʱ��2�������Ϳ���λ,0:ģʽ0,1:ģʽ1|
 * |2|T2x12|��ʱ��2��Ƶ����λ,0:��Ƶ,1:����Ƶ|
 * |1|EXTRAM|�ⲿRAM����λ|
 * |0|S1ST2|UART1�жϲ����ʷ���������λ,0:��ʱ��1,1:��ʱ��2|
 */
SFR(AUXR, 0x8E);  // 0000,0001 �����Ĵ���

#define IE0_VECTOR 0 /* 0x03 external interrupt 0 */
#define TF0_VECTOR 1 /* 0x0b timer 0 */
#define IE1_VECTOR 2 /* 0x13 external interrupt 1 */
#define TF1_VECTOR 3 /* 0x1b timer 1 */
#define SI0_VECTOR 4 /* 0x23 serial port 0 */
#endif               // !__DEF_H__