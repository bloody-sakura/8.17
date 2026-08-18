#include "myostask.h"

motor DJI={
    .GearRatio=36,
    .MaxCurrent=3,
    .MaxSpeed=416,
};

volatile PID_Typedef v_pid={
    .integral=0,
    .kd=0,
    .ki=0.01,
    .kp=2,
    .pre_error=0,
    .pre_feedback=0,
    .integral_limit=1000.0,
    .output_limit=30000.0,
};//对应速度环

volatile PID_Typedef d_pid={
    .integral=0,
    .kd=0,
    .ki=0.01,
    .kp=1,
    .pre_error=0,
    .pre_feedback=0,
    .integral_limit=1000.0,
    .output_limit=300.0,
};//对应角度环

rotor set_rotor={
    .Angle=0,
    .Speed=500,
    .Torque=0,
};
volatile rotor real_rotor={
    .Angle=0,
    .Speed=0,
    .Torque=0,
};
STATE_MODE state_mode=SPEED;
void Transmit_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(2); // 2ms一循环s
    for(;;)
    {
        get_real_rotor(&real_rotor,&DJI);
        if(state_mode==DEGREE)
        {
            set_rotor.Speed=PID_Process(&d_pid,set_rotor.Angle,real_rotor.Angle);
        }


        uint16_t temp=PID_Process(&v_pid,set_rotor.Speed,real_rotor.Speed);



        can_transmit(temp);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

