#include "motor_config.h"

DJIMotor DJI = {
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

void motor_set_init(DJIMotor* motor,MotorMode motor_mode,float set_motor_angle,float set_motor_speed,float set_motor_torque)
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

