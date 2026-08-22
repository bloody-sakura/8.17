#include "myostask.h"

StackType_t transmit_stack;

void CANTransmit_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(1); 
    for (;;)
    {
    can_transmit();
    can_transmit();
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}


void ZdriveProcess_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(6); 
    for (;;)
    {
        ZdriveFunc();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void DJIProcess_Task(void *argument)
{
    
}


