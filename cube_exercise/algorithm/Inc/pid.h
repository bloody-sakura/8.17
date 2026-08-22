#ifndef PID_H
#define PID_H

#include "main.h"
#include "DJIDrive.h"
#include "can_transmit.h"




float PID_Process(PID_Typedef *pid,float target,float feedback);


#endif
