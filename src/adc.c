#include "adc.h"

#include "def.h"

static int8_t CODE __d2t[] = {239, 197, 175, 160, 150, 142, 135, 129, 124, 120, 116, 113, 109, 107, 104, 101, 99, 97,
  95, 93, 91, 90, 88, 86, 85, 84, 82, 81, 80, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 67, 66, 65, 64, 63, 63,
  62, 61, 61, 60, 59, 58, 58, 57, 57, 56, 55, 55, 54, 54, 53, 52, 52, 51, 51, 50, 50, 49, 49, 48, 48, 47, 47, 46, 46,
  45, 45, 44, 44, 43, 43, 42, 42, 41, 41, 41, 40, 40, 39, 39, 38, 38, 38, 37, 37, 36, 36, 36, 35, 35, 34, 34, 34, 33,
  33, 32, 32, 32, 31, 31, 31, 30, 30, 29, 29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 26, 25, 25, 24, 24, 24, 23, 23, 23,
  22, 22, 22, 21, 21, 21, 20, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, 13, 13, 13,
  12, 12, 12, 11, 11, 11, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 54, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0, -1, -1,
  -1, -2, -2, -3, -3, -4, -4, -5, -5, -6, -6, -7, -7, -8, -8, -9, -9, -10, -10, -11, -11, -12, -13, -13, -14, -14, -15,
  -16, -16, -17, -18, -19, -19, -20, -21, -22, -23, -24, -25, -26, -27, -28, -29, -30, -32, -33, -35, -36, -38, -40};
static uint16_t count, statistics;
static uint32_t sum;
static enum AdcChannel channel;
void adc_init(void) {
  __ADC_INIT();
  statistics = 1;
}
void adc_start(enum AdcChannel cha) {
  count = 0;
  sum = 0;
  channel = cha;
  __ADC_START(cha);
}
void adc_statistics(uint16_t sta) {
  statistics = sta;
}
int16_t adc_wait(void) {
  while(count != statistics)
    ;
  return sum;
}
INTERRUPT_USING(__adc, ADC_VECTOR, ADC_INT_PRIORITY) {
  __ADC_CLEAR();
  sum += __d2t[__ADC_GET_HIGH() - 1];
  ++count;
  if(count == statistics) {
    sum = (sum + statistics / 2) / statistics;
  } else {
    __ADC_START(channel);
  }
}