#include "servo.h"
#include "pwm.h"
#include "delay.h"

static float last_angles[4] = {0.0f};

void Servo_Init(void)
{
    uint8_t i;
    PWM_Init();
    for(i = 0; i < 4; i++)
        Servo_SetAngle(i + 1, 0.0f);
}

void Servo_SetAngle(uint8_t ch, float angle)
{
    uint16_t pulse;
    if(angle < 0.0f)   angle = 0.0f;
    if(angle > 180.0f) angle = 180.0f;
    pulse = (uint16_t)(angle / 180.0f * 2000.0f + 500.0f);
    PWM_SetCompare(ch, pulse);
    last_angles[ch - 1] = angle;
}

void Servo_SetAngle_Soft(uint8_t ch, float target, float step, u16 step_delay)
{
    float cur = last_angles[ch - 1];
    if(target > cur)
    {
        for(; cur < target; cur += step)
        {
            Servo_SetAngle(ch, cur);
            delay_ms(step_delay);
        }
    }
    else
    {
        for(; cur > target; cur -= step)
        {
            Servo_SetAngle(ch, cur);
            delay_ms(step_delay);
        }
    }
    Servo_SetAngle(ch, target);
}

float Servo_GetAngle(uint8_t ch)
{
    if(ch < 1 || ch > 4) return 0.0f;
    return last_angles[ch - 1];
}
