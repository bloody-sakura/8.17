#include "main.h"

void led_on(uint16_t LedPin)
{
    HAL_GPIO_WritePin(GPIOA,LedPin,GPIO_PIN_SET);
}

void led_off(uint16_t LedPin)
{
    HAL_GPIO_WritePin(GPIOA,LedPin,GPIO_PIN_RESET);
}