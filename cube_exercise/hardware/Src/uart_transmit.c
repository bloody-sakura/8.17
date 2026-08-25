#include "uart_transmit.h"
#include "usart.h"
#include "string.h"
#include "math.h"
#include "myostask.h"
#include "can_transmit.h"
uint8_t rx_buffer[5] = {0};



void UART_Start_Receive(void)//开启中断函数
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rx_buffer,sizeof(rx_buffer));//空闲中断开启函数
    
}


//空闲中断回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1) 
    {
        if(rx_buffer[0]==0xA5&&rx_buffer[4]==0x5A)
        {
        
        
        

        CAN_TxHeaderTypeDef TxHeader;
                uint32_t TxMailbox;
                uint8_t TxData[5] ;
                TxData[0]=rx_buffer[0];
                TxData[1]=rx_buffer[1];
                TxData[2]=rx_buffer[2];
                TxData[3]=rx_buffer[3];
                TxData[4]=rx_buffer[4];

                TxHeader.StdId = (0x021);
                TxHeader.RTR = CAN_RTR_DATA;
                TxHeader.IDE = CAN_ID_STD;
                TxHeader.DLC = 5;
                TxHeader.TransmitGlobalTime = DISABLE;

                HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);}

        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buffer, sizeof(rx_buffer));
        //重新开启空闲回调

    }
}






void vofa_transmit(uint8_t* Data)
{
    HAL_UART_Transmit_DMA(&huart1,Data,sizeof(Data));
}

