#ifndef PWM_H
#define PWM_H

#include "main.h"

extern volatile uint32_t pwm_cycle;


void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void pwm();

#endif