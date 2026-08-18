#ifndef MYOSTASK
#define MYOSTASK

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


#include "can_transmit.h"
#include "pid.h"
typedef enum
{
    IDLE=0,
    SPEED,
    DEGREE,
} STATE_MODE;


#endif