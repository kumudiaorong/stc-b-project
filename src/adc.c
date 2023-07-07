#include "adc.h"

#include <string.h>

#include "def.h"
#include "detail/sys.h"
#include "sys.h"

XDATA adc_t adc;  //!< adc data
static XDATA sys_callback_t adc_callback_table[6][2];
static void adc_register(uint8_t event, sys_callback_t callback);  //!< nav register function
static uint8_t adc_scan(void) REENTRANT;                           //!< nav scan function
static void adc_callback(uint8_t msg) REENTRANT;                   //!< nav callback function
static uint8_t __adc_idx = 0;                                      //!< nav idx
uint8_t adc_idx(void) {
  return __adc_idx;
}
void adc_init(void) {
  __ADC_INIT();
  memset(&adc, 0, sizeof(adc_t));
  memset(adc_callback_table, 0, sizeof(adc_callback_table));
  __adc_idx = __sys_sensor_add(adc_register, adc_scan, adc_callback);
  __ADC_START(ADCRT);
}

static void adc_register(uint8_t event, sys_callback_t callback) {
  adc_callback_table[event & 0x7][event >> 3] = callback;
}
static XDATA uint8_t flag = 0;  //!< adc flag, use bit 0-1 for idx in __adc, use bit 2 for response in adc_scan

static uint8_t adc_scan(void) REENTRANT {
  static uint32_t lastT = 0;
  static uint8_t last = NAVNONE, start = NAVNONE;
  uint8_t cur, ret = 0;
  __sys_lock();
  cur = adc.nav;
  __sys_unlock();
  if(cur != start) {
    start = cur;
    lastT = __sys_timer_cnt;
    flag |= 0x4;
  } else if(flag & 0x4) {
    if(__sys_timer_cnt - lastT > 10) {
      flag &= ~0x4;
      if(start != NAVNONE && adc_callback_table[start][NAVPRESS]) {
        last = start;
        ret = start + 1;
      } else if(start == NAVNONE && last != NAVNONE && adc_callback_table[last][NAVRELEASE]) {
        ret = (last + 1) << 3;
        last = NAVNONE;
      }
    }
  }
  return ret;
}

static void adc_callback(uint8_t msg) REENTRANT {
  if(msg & 0x7) {
    adc_callback_table[(msg & 0x7) - 1][0]();
  }
  if(msg & 0x38) {
    adc_callback_table[((msg & 0x38) >> 3) - 1][1]();
  }
}
static int8_t CODE __d2t[] = {239, 197, 175, 160, 150, 142, 135, 129, 124, 120, 116, 113, 109, 107, 104, 101, 99, 97,
  95, 93, 91, 90, 88, 86, 85, 84, 82, 81, 80, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 67, 66, 65, 64, 63, 63,
  62, 61, 61, 60, 59, 58, 58, 57, 57, 56, 55, 55, 54, 54, 53, 52, 52, 51, 51, 50, 50, 49, 49, 48, 48, 47, 47, 46, 46,
  45, 45, 44, 44, 43, 43, 42, 42, 41, 41, 41, 40, 40, 39, 39, 38, 38, 38, 37, 37, 36, 36, 36, 35, 35, 34, 34, 34, 33,
  33, 32, 32, 32, 31, 31, 31, 30, 30, 29, 29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 26, 25, 25, 24, 24, 24, 23, 23, 23,
  22, 22, 22, 21, 21, 21, 20, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13,
  12, 12, 12, 11, 11, 11, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 54, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0, -1, -1,
  -1, -2, -2, -3, -3, -4, -4, -5, -5, -6, -6, -7, -7, -8, -8, -9, -9, -10, -10, -11, -11, -12, -13, -13, -14, -14, -15,
  -16, -16, -17, -18, -19, -19, -20, -21, -22, -23, -24, -25, -26, -27, -28, -29, -30, -32, -33, -35, -36, -38,
  -40};                                                         //!< adc temperature table
static uint8_t CODE __adc_channel[] = {ADCRT, ADCROP, ADCNAV};  //!< adc channel table

INTERRUPT_USING(__adc, ADC_VECTOR, ADC_INT_PRIORITY) {
  static XDATA uint32_t sum = 0;
  static XDATA uint16_t count = 0, avgCnt = 50;
  uint8_t idx = flag & 0x3;
  sum += __ADC_GET();
  ++count;
  if(count == avgCnt) {
    if(idx == 0x2) {
      adc.nav = ((sum + (avgCnt >> 1)) / avgCnt) >> 7;
      flag &= ~0x3;
    } else {
      switch(idx) {
        case 0 :
          adc.rt = __d2t[(((sum + (avgCnt >> 1)) / avgCnt) >> 2) - 1];
          break;
        case 1 :
          adc.rop = (sum + (avgCnt >> 1)) / avgCnt;
          break;
        default :
          break;
      }
      ++flag;
    }
    count = 0;
    sum = 0;
  }
  __ADC_START(__adc_channel[flag & 0x3]);
}