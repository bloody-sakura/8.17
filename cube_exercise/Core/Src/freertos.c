/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for ZDriveProcessTa */
osThreadId_t ZDriveProcessTaHandle;
const osThreadAttr_t ZDriveProcessTa_attributes = {
  .name = "ZDriveProcessTa",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CANTansmitTask */
osThreadId_t CANTansmitTaskHandle;
const osThreadAttr_t CANTansmitTask_attributes = {
  .name = "CANTansmitTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for DJIProcessTask */
osThreadId_t DJIProcessTaskHandle;
const osThreadAttr_t DJIProcessTask_attributes = {
  .name = "DJIProcessTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ZdriveProcess_Task(void *argument);
void CANTransmit_Task(void *argument);
void DJIProcess_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ZDriveProcessTa */
  ZDriveProcessTaHandle = osThreadNew(ZdriveProcess_Task, NULL, &ZDriveProcessTa_attributes);

  /* creation of CANTansmitTask */
  CANTansmitTaskHandle = osThreadNew(CANTransmit_Task, NULL, &CANTansmitTask_attributes);

  /* creation of DJIProcessTask */
  DJIProcessTaskHandle = osThreadNew(DJIProcess_Task, NULL, &DJIProcessTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ZdriveProcess_Task */
/**
  * @brief  Function implementing the ZDriveProcessTa thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ZdriveProcess_Task */
__weak void ZdriveProcess_Task(void *argument)
{
  /* USER CODE BEGIN ZdriveProcess_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ZdriveProcess_Task */
}

/* USER CODE BEGIN Header_CANTransmit_Task */
/**
* @brief Function implementing the CANTansmitTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CANTransmit_Task */
__weak void CANTransmit_Task(void *argument)
{
  /* USER CODE BEGIN CANTransmit_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END CANTransmit_Task */
}

/* USER CODE BEGIN Header_DJIProcess_Task */
/**
* @brief Function implementing the DJIProcessTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DJIProcess_Task */
__weak void DJIProcess_Task(void *argument)
{
  /* USER CODE BEGIN DJIProcess_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END DJIProcess_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

