#include "beep.h"



void beep_on()
{
    HAL_GPIO_WritePin(beep_GPIO_Port,beep_Pin,GPIO_PIN_SET);
}

void beep_off()
{
    HAL_GPIO_WritePin(beep_GPIO_Port,beep_Pin,GPIO_PIN_RESET);
}

void beep_init()
{
    beep_off();
}

