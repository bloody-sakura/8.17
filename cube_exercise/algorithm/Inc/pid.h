#ifndef PID_H
#define PID_H

#include "main.h"
#include "can_transmit.h"

typedef struct
{
    float Angle;
    float Speed;
    int16_t Torque;
}rotor;

typedef struct
{
    int16_t MaxSpeed;
    int16_t MaxCurrent;
    int16_t GearRatio;
}motor;

typedef struct
{
    float kp;
    float ki;
    float kd;
    float integral;
    float pre_error;
    float pre_feedback;
    float integral_limit;
    float output_limit;
}PID_Typedef;

void get_real_rotor(volatile rotor* Rotor,motor* Motor);
float PID_Process(volatile PID_Typedef *pid,float target,float feedback);


#endif
