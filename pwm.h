#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

#define SERVO_FREQ  100          // 舵机PWM频率(Hz)，推荐50，最大建议333
// 50Hz → 周期20ms，最安全
// 100Hz → 周期10ms，部分舵机能用，可能抖动
// 200Hz → 周期5ms，只有极少数高速舵机支持

void PWM_Init(void);
void PWM_SetCompare(uint8_t ch, uint16_t Compare);
#endif
