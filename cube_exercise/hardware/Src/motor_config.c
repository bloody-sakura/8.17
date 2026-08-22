#include "motor_config.h"

void Get_MotorVal_Feedback(volatile MotorVal* Rotor ,MotorParam* Motor)
{
    static int16_t count=0;
    float raw_angle=((ESC_Data[0]<<8)|(ESC_Data[1]))*360.0/8191.0;
    if(((Rotor->Angle-360*count)-raw_angle)>180)
    count++;
    if(((Rotor->Angle-360*count)-raw_angle)<-180)
    count--;
    Rotor->Angle=count*360+raw_angle;
    Rotor->Speed=((int16_t)((ESC_Data[2]<<8)|(ESC_Data[3])));
    Rotor->Torque=(int16_t)(ESC_Data[4]<<8)|(ESC_Data[5]);
}

void motor_set_init(Motor* motor,MotorMode motor_mode,float set_motor_angle,float set_motor_speed,float set_motor_torque)
{
    motor->motor_mode=motor_mode;
    motor->set_motor_val.Angle=set_motor_angle;
    motor->set_motor_val.Speed=set_motor_speed;
    motor->set_motor_val.Torque=set_motor_torque;
}

void motor_pid_init(PID_Typedef *pid,float kp,float ki,float kd)
{
    pid->kp=kp;
    pid->ki=ki;
    pid->kd=kd;
} 

