#include "pid.h"





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