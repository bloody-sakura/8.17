#ifndef PID_H
#define PID_H

#include "main.h"
#include "motor_config.h"
#include "can_transmit.h"




float PID_Process(PID_Typedef *pid,float target,float feedback);


#endif
