#include "pid.h"


void get_real_rotor(volatile rotor* Rotor,motor* Motor)
{
    static int16_t count=0;
    float raw_degree=((ESC_Data[0]<<8)|(ESC_Data[1]))*360.0/8191.0;
    if(((Rotor->Angle-360*count)-raw_degree)>180)
    count++;
    if(((Rotor->Angle-360*count)-raw_degree)<-180)
    count--;
    Rotor->Angle=count*360+raw_degree;
    Rotor->Speed=((int16_t)((ESC_Data[2]<<8)|(ESC_Data[3])));
    Rotor->Torque=(int16_t)(ESC_Data[4]<<8)|(ESC_Data[5]);
}


float PID_Process(volatile PID_Typedef *pid,float target,float feedback)
{
    float error=target-feedback;
    
    float P=pid->kp*error; 
    
    if(pid->integral<pid->integral_limit&&pid->integral>-pid->integral_limit)
    pid->integral+=error;
    
    if(pid->integral>pid->integral_limit)
    pid->integral=pid->integral_limit;
    
    if(pid->integral<-pid->integral_limit)
    pid->integral=-pid->integral_limit;
    float I=pid->ki*pid->integral;

    float D=pid->kd*(pid->pre_feedback-feedback);
    pid->pre_error=error;
    pid->pre_feedback=feedback;
    
    
    float temp=P+I+D;
    if(temp>pid->output_limit)
    return pid->output_limit;
    if(temp<-pid->output_limit)
    return -pid->output_limit;
    return temp;
}