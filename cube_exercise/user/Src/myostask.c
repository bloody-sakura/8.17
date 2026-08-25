#include "myostask.h"
int change_flag = 0;
#ifdef ZDRIVE
void CANTransmit_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(1);
    for (;;)
    {
        Zcan_transmit();
        Zcan_transmit();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void ZdriveProcess_Task(void *argument)
{
    QueueInit(&send_queue);
    ZdriveInit();
    Zmotor[0].Begin = true;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(6);
    for (;;)
    {
        ZdriveFunc();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

#endif

#if DJIDRIVE
void DJIProcess_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    motor_set_init(&DJI, IDLE, 0, 0, 0);
    motor_pid_init(&DJI.vel_pid, 2, 0.01, 0);
    motor_pid_init(&DJI.pos_pid, 0.017, 0, 0);
    motor_pid_init(&DJI.tor_pid, 1, 0, 0);
    const TickType_t xFrequency = pdMS_TO_TICKS(2); // 2ms一循环s
    for (;;)
    {
        if (DJI.cur_motor_mode != DJI.motor_mode)
        {
            DJI.vel_pid.integral = 0;
            DJI.pos_pid.integral = 0;
            DJI.cur_motor_mode = DJI.motor_mode;
            change_flag = 1;
        }
        DJI_func(&DJI, &motor_feedback);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

#endif

void Status_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(500);
    static const uint16_t led_queue[] = {LED1_Pin, LED2_Pin};
    uint8_t i = 0;
    for (;;)
    {

        led_off(LED1_Pin | LED2_Pin);
        beep_off();
        if (change_flag == 1)
        {
            beep_on();
            change_flag = 0;
        }
        led_on(led_queue[i]);
        i = (i + 1) % 2;

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}