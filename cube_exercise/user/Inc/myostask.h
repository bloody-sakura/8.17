#ifndef MYOSTASK
#define MYOSTASK

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "beep.h"
#include "led.h"
#include "PWM.h"
#include "can_transmit.h"
#include "uart_transmit.h"
typedef enum
{
    IDLE = 0,
    Pwm,
    Beep
} STATE_MODE;
void LED_Task(void *argument);
void Beep_Task(void *argument);
extern volatile STATE_MODE last_mode;
extern volatile STATE_MODE state_mode;
#endif