#include "can_transmmit.h"

CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];
volatile uint8_t Float_Data[8]={0};
volatile uint8_t buzzer_times=0;
volatile uint8_t Vofa_Transmit_Signal=0;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance == CAN1)
    {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
        {
            if (RxHeader.StdId == 0x011)//接收float数据
            {
                memcpy(Float_Data,RxData,sizeof(Float_Data));
                
            }
            else if (RxHeader.StdId == 0x022)//接收蜂鸣器信号
            {
                state_mode=RxData[0];
                buzzer_times=RxData[1];

                
            }
        }
    }
}

void can_transmit()
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox2;
    uint8_t TxData[2] = {'O', 'K'};

    TxHeader.StdId = (0x012);
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 2;
    TxHeader.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox2);
}