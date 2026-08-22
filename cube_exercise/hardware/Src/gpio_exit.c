#include "gpio_exit.h"
volatile uint32_t key_press_tick;
volatile uint8_t key_press_flag;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == INPUT_1_Pin) 
    {
    
        key_press_tick = HAL_GetTick();  
        key_press_flag = 1;              
    }
}