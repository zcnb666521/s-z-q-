#include "pwm.h"
#include "stm32f10x.h"

/*
 * 频率计算：计数频率 = 72MHz / 72 = 1MHz
 * 周期 ARR = (1000000 / SERVO_FREQ) - 1
 * 例如：50 → ARR=19999 (20ms), 100 → ARR=9999 (10ms)
 */
#define PWM_PERIOD  (1000000 / SERVO_FREQ - 1)

void PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = PWM_PERIOD;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);   // PA0
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);   // PA1
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);   // PA6
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);   // PA7
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

void PWM_SetCompare(uint8_t ch, uint16_t Compare)
{
    if(Compare > PWM_PERIOD) Compare = PWM_PERIOD - 1;   // 防止占空比 100%
    switch(ch)
    {
        case 1: TIM_SetCompare1(TIM2, Compare); break;
        case 2: TIM_SetCompare2(TIM2, Compare); break;
        case 3: TIM_SetCompare1(TIM3, Compare); break;
        case 4: TIM_SetCompare2(TIM3, Compare); break;
        default: break;
    }
}
