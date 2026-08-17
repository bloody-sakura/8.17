#ifndef UART
#define UART

#include "main.h"
#include "usart.h"
#include "string.h"
#include "math.h"
#include "myostask.h"
#include "can1.h"
extern volatile uint8_t rx_buffer[];
extern volatile uint8_t tx_buffer[];

void UART_Start_Receive(void);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void vofa_transmit();


#endif