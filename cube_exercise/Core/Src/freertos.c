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
typedef StaticTask_t osStaticThreadDef_t;
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
/* Definitions for LEDTASK */
osThreadId_t LEDTASKHandle;
const osThreadAttr_t LEDTASK_attributes = {
  .name = "LEDTASK",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for BUZZERTASK */
osThreadId_t BUZZERTASKHandle;
uint32_t BUZZERTASKBuffer[ 128 ];
osStaticThreadDef_t BUZZERTASKControlBlock;
const osThreadAttr_t BUZZERTASK_attributes = {
  .name = "BUZZERTASK",
  .cb_mem = &BUZZERTASKControlBlock,
  .cb_size = sizeof(BUZZERTASKControlBlock),
  .stack_mem = &BUZZERTASKBuffer[0],
  .stack_size = sizeof(BUZZERTASKBuffer),
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for transmit_task */
osThreadId_t transmit_taskHandle;
const osThreadAttr_t transmit_task_attributes = {
  .name = "transmit_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for IDLE_TASK */
osThreadId_t IDLE_TASKHandle;
const osThreadAttr_t IDLE_TASK_attributes = {
  .name = "IDLE_TASK",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal7,
};
/* Definitions for vofatransmit */
osThreadId_t vofatransmitHandle;
const osThreadAttr_t vofatransmit_attributes = {
  .name = "vofatransmit",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void LED_Task(void *argument);
void Beep_Task(void *argument);
void Transmit_Task(void *argument);
void Idle_Task(void *argument);
void Vofa_Transmit(void *argument);

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
  /* creation of LEDTASK */
  LEDTASKHandle = osThreadNew(LED_Task, NULL, &LEDTASK_attributes);

  /* creation of BUZZERTASK */
  BUZZERTASKHandle = osThreadNew(Beep_Task, NULL, &BUZZERTASK_attributes);

  /* creation of transmit_task */
  transmit_taskHandle = osThreadNew(Transmit_Task, NULL, &transmit_task_attributes);

  /* creation of IDLE_TASK */
  IDLE_TASKHandle = osThreadNew(Idle_Task, NULL, &IDLE_TASK_attributes);

  /* creation of vofatransmit */
  vofatransmitHandle = osThreadNew(Vofa_Transmit, NULL, &vofatransmit_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_LED_Task */
/**
  * @brief  Function implementing the LEDTASK thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_LED_Task */
__weak void LED_Task(void *argument)
{
  /* USER CODE BEGIN LED_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END LED_Task */
}

/* USER CODE BEGIN Header_Beep_Task */
/**
* @brief Function implementing the BUZZERTASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Beep_Task */
__weak void Beep_Task(void *argument)
{
  /* USER CODE BEGIN Beep_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Beep_Task */
}

/* USER CODE BEGIN Header_Transmit_Task */
/**
* @brief Function implementing the transmit_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Transmit_Task */
__weak void Transmit_Task(void *argument)
{
  /* USER CODE BEGIN Transmit_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Transmit_Task */
}

/* USER CODE BEGIN Header_Idle_Task */
/**
* @brief Function implementing the IDLE_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Idle_Task */
__weak void Idle_Task(void *argument)
{
  /* USER CODE BEGIN Idle_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Idle_Task */
}

/* USER CODE BEGIN Header_Vofa_Transmit */
/**
* @brief Function implementing the vofatransmit thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Vofa_Transmit */
__weak void Vofa_Transmit(void *argument)
{
  /* USER CODE BEGIN Vofa_Transmit */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Vofa_Transmit */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

