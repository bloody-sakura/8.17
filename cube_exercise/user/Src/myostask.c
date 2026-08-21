#include "myostask.h"

StackType_t transmit_stack;

void Transmit_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(2); 
    for (;;)
    {
    can_transmit();
    can_transmit();
    }
}


void Process_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(6); 
    for (;;)
    {
        
        ZdriveFunc();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
