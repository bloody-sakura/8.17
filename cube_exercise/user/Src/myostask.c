#include "myostask.h"

Motor DJI = {
    .motor_param.GearRatio = 36,
    .motor_param.MaxCurrent = 3,
    .motor_param.MaxSpeed = 416,
    
    .vel_pid.integral_limit=1000.0,
    .vel_pid.output_limit=30000.0,
    .vel_pid.pre_error=0,
    .vel_pid.pre_feedback=0,
    .vel_pid.integral=0,

    .pos_pid.integral_limit=1000.0,
    .pos_pid.output_limit=1000.0,
    .pos_pid.pre_error=0,
    .pos_pid.pre_feedback=0,
    .pos_pid.integral=0,

    .motor_mode=IDLE,
    .cur_motor_mode=IDLE,

};



volatile MotorVal motor_feedback = {
    .Angle = 0,
    .Speed = 0,
    .Torque = 0,
};

int initial_signal=1;
void Transmit_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(2); // 2ms一循环s
    for (;;)
    {
        if(initial_signal==1)
        {
            motor_pid_init(&DJI.vel_pid, 2, 0.01, 0);
            motor_pid_init(&DJI.pos_pid, 0.017, 0, 0);
            motor_set_init(&DJI, SPEED, 960, 500, 0);
            initial_signal=0;
        }
        if(DJI.cur_motor_mode!=DJI.motor_mode)
        {
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

        can_transmit(temp);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
