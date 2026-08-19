#include "tim_exit.h"

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint32_t time_count = 0;
    if (htim->Instance == TIM3)
    {
        time_count++;
        if (time_count >= 2) // 2ms运行一次
        {
            time_count = 0;
        }

        // __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,rate);
        //__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,rate);
    }
}

void pwm()
{
    HAL_TIM_Base_Start_IT(&htim3); // 要开启时调用
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}
