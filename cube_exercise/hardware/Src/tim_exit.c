#include "tim_exit.h"





void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint32_t time_count=0;
    static uint8_t trend=0;
    static uint32_t rate;
    if (htim->Instance == TIM3)
    {
       
    
    
    __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,rate);
    __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,rate);
    }
    
}

void pwm()
{
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}
