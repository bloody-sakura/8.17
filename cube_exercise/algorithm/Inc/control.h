#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"

/*typedef struct
{
    uint8_t ID;
    volatile bool Begin; //true运行MODE,false失能
    volatile DJmotor_mode_t MODE_Set;//DJ_Disable即失能
    volatile DJmotor_mode_t MODE_Cur;//实际运行模式，任务层可读

    DJmotorParam param;
    DJmotorVal valSet;
    DJmotorVal valNow;
    DJmotorVal valPre;
    DJmotorStatus statusFlag;
    DJmotorLimit limit;
    DJmotorArgum argum;
    DJmotorError error;
    PTDType posPID;
    PIDTYpe velPID;
}
DJMotor,*DJMotorPointer;

//#if USE_DJ
    extern DJMotor DJmotor[USE_DJNUM];

    void DJmotor_Init(void);
    void DJmotor_Func(void);
    void DJmotor_Receive(FDCAN_RxHeaderTypedef Rxheader,uint8_t *Rx_data);
    void DJmotor_PID_Reload(DJMotorPointer motor,DJmotorPID pid_reload);

typedef enum
{
    DJ_Disable=0,
    DJ_RPM=1,
    DJ_Position=2,
    DJ_Zero=3,
    DJ_Current=4,

}DJmotor_mode_t;

typedef struct 
{
    volatile int16_t current_raw;//直接设置电流
    volatile float angle_deg;//输出角度，degree
    volatile int16_t speed_rpm;//valSet:输出轴 rpm;valNow:转子 rpm原始反馈
    volatile float current_A;//反馈电流A
    volatile int16_t PulseRead;//raw encoder pulse
    volatile int16_t PulseGap;//pulse delta
    volatile int32_t PulseTotal;//accumulated pulse
    volatile int8_t temperature_C;//℃
}DJmotorVal;


typedef struct 
{
    uint32_t PulsePerRound;// 8191
    float Gear_ratio;//mechanism ratio
    float Reduction_ratio;//motor reducer ratio
    uint32_t ParamID;//CAN receive ID base
    int16_t CurrentLimit_raw;//output current limit,raw
}DJmotorParam;

typedef struct 
{
    bool PRMLimitFlag;
    bool PosAngleLimitFlag;
    bool PosRPMFlag;
    bool CurrentLimitFlag;
    float  MaxAngle_deg;
    float MinAngle_deg;
    int16_t SpeedRPMLimit;
    int32_t PosRPMLimit;
    int16_t ZeroRPMLimit;
    int16_t ZeroCurrentLimit_raw;
    bool IsLooseStuck;
}DJmotorLimit;*/


#endif
