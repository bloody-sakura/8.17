#include "can_transmit.h"
#include "uart_transmit.h"
#include "can.h"
#include "ZDrive.h"
#include "string.h"

CAN_RxHeaderTypeDef RxHeader0;
uint8_t RxData0[8];

CAN_RxHeaderTypeDef RxHeader1;
uint8_t RxData1[8];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance == CAN1)
    {

        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader0, RxData0) == HAL_OK)
        {
            if (RxHeader0.StdId == 0x204) // 接收电调数据
            {
                memcpy(ESC_Data, RxData0, sizeof(RxData0));
            }

        }
    }
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance == CAN1)
    {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader1, RxData1) == HAL_OK)
        {
            if (RxHeader1.IDE == CAN_ID_STD)
                ZdriveReceive(RxHeader1, RxData1); // 接收Zdrive信号
        }
    }
}

void Zcan_transmit()
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

void DJIcan_transmit(uint16_t p)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox2;
    uint8_t TxData[8] = {0};
    TxData[6] = (((p) >> 8) & 0xFF);
    TxData[7] = ((p) & 0xFF);

    TxHeader.StdId = (0x200);
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 8;
    TxHeader.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox2);
}
