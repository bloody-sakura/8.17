#ifndef UART_TRANSMIT_H
#define UART_TRANSMIT_H

#include "main.h"
extern uint8_t rx_buffer[];


void UART_Start_Receive(void);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void vofa_transmit(uint8_t* Data);


#endif