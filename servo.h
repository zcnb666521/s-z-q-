#ifndef __SERVO_H
#define __SERVO_H
#include "sys.h"

void Servo_Init(void);
void Servo_SetAngle(uint8_t ch, float Angle);
void Servo_SetAngle_Soft(uint8_t ch, float target, float step, u16 step_delay);
float Servo_GetAngle(uint8_t ch);
#endif
