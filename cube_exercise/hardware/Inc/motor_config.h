#ifndef MOTORCONFIG_H
#define MOTORCONFIG_H

#include "main.h"
#include <stdbool.h>
#include "ZDrive.h"
typedef struct
{
    float Angle;
    float Speed;
    int16_t Torque;
}MotorVal;

typedef struct
{
    int16_t MaxSpeed;
    int16_t MaxCurrent;
    int16_t GearRatio;
}MotorParam;

typedef enum
{
    IDLE=0,
    SPEED,
    DEGREE,
} MotorMode;


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

typedef struct
{
    MotorVal set_motor_val;
    MotorParam motor_param;
    MotorMode motor_mode;
    PID_Typedef vel_pid;
    PID_Typedef pos_pid;
    MotorMode cur_motor_mode;
}Motor;



void Get_MotorVal_Feedback(volatile MotorVal* Rotor ,MotorParam* Motor);
void motor_set_init(Motor* motor,MotorMode motor_mode,float set_motor_angle,float set_motor_speed,float set_motor_torque);
void motor_pid_init(PID_Typedef *pid,float kp,float ki,float kd);



#endif