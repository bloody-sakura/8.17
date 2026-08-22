#ifndef GPIO_EXIT_H
#define GPIO_EXIT_H

#include "main.h"


extern volatile uint32_t key_press_tick;
extern volatile uint8_t key_press_flag;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);


#endif