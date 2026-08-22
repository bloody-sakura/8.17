#include "can_transmit.h"

CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];
volatile uint8_t ESC_Data[8] = {0};
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance == CAN1)
    {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
        {
            if (RxHeader.IDE == CAN_ID_STD)
            ZdriveReceive(RxHeader, RxData); // 接收Zdrive信号
        }
    }
}

void can_transmit()
{
    if (isQueueEmpty(&send_queue) == false)
    {
        CAN_TxHeaderTypeDef TxHeader;
        uint32_t TxMailbox2;
        uint8_t TxData[8] = {0};
        TxHeader.StdId = send_queue.TxBuffer[send_queue.head].TxHeader.StdId;
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.RTR = CAN_RTR_DATA;
        TxHeader.DLC = send_queue.TxBuffer[send_queue.head].TxHeader.DLC;
        TxHeader.TransmitGlobalTime = DISABLE;
        memcpy(TxData, send_queue.TxBuffer[send_queue.head].TxData, sizeof(send_queue.TxBuffer[send_queue.head].TxData));

        HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox2);
        DeQueue(&send_queue);
    }
}
