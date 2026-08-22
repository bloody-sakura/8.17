#ifndef CAN1_H
#define CAN1_H


#include "uart_transmit.h"
#include "can.h"
#include "ZDrive.h"
#include "string.h"
extern uint8_t RxData[];
extern CAN_RxHeaderTypeDef RxHeader;
extern volatile uint8_t ESC_Data[8];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void can_transmit();

#endif