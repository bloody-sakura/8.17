#include "myostask.h"



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





void DJIProcess_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    motor_set_init(&DJI, SPEED, 960, 500, 0);
    const TickType_t xFrequency = pdMS_TO_TICKS(2); // 2ms一循环s
    for (;;)
    {
        if(DJI.cur_motor_mode!=DJI.motor_mode)
        {
            motor_pid_init(&DJI.vel_pid, 2, 0.01, 0);
            motor_pid_init(&DJI.pos_pid, 0.017, 0, 0);
            DJI.vel_pid.integral=0;
            DJI.pos_pid.integral=0;
            DJI.cur_motor_mode=DJI.motor_mode;
        }
        Get_MotorVal_Feedback(&motor_feedback, &DJI.motor_param);
        if (DJI.motor_mode== DEGREE)
        {
            DJI.set_motor_val.Speed = PID_Process(&DJI.pos_pid, DJI.set_motor_val.Angle, motor_feedback.Angle);
        }

        uint16_t temp = PID_Process(&DJI.vel_pid, DJI.set_motor_val.Speed, motor_feedback.Speed);

        DJIcan_transmit(temp);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
