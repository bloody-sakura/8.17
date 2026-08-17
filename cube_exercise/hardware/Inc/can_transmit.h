#ifndef CAN1_H
#define CAN1_H


#include "uart_transmit.h"
#include "can.h"
extern uint8_t RxData[];
extern CAN_RxHeaderTypeDef RxHeader;
extern volatile uint8_t Float_Data[8];
extern volatile uint8_t buzzer_times;
extern volatile uint8_t Vofa_Transmit_Signal;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void can_transmit();

#endif