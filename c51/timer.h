#ifndef __TIMER_H__
#define __TIMER_H__
#include "def.h"
/**
 * @brief timer0 reload value
 *
 */
#ifdef SYSCLK_11059200
#define TIMER0_RELOAD_VALUE 9216
#else  // SYSCLK_11059200
#define TIMER0_RELOAD_VALUE 9216
#endif
/**
 * @brief timer1 reload value
 * timer0 use 10ms as unit
 * timer1 use 1us as unit
 * timer1 is not recommended to use
 *
 */
void timer_init();
/**
 * @brief initialize timer
 *
 * @param none
 */
uint32_t timer_get();
/**
 * @brief get time count
 *
 * @param none
 * @return uint32_t time count
 */
void timer_handler_set(void (*handler)());
/**
 * @brief delay n ms
 *
 * @param n
 */
void delay_10ms(uint32_t n);
#endif