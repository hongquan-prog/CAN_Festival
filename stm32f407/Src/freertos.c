/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include <stdio.h>
#include "canfestival.h"
#include "TestSlave.h"
#include "can.h"
#include "tim.h"
#include "usart.h"
#include "app_info.h"
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
osEventFlagsId_t gCANopenEventGroup;
/* USER CODE END Variables */
/* Definitions for CANTimerTask */
osThreadId_t CANTimerTaskHandle;
const osThreadAttr_t CANTimerTask_attributes = {
  .name = "CANTimerTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for CANSendTask */
osThreadId_t CANSendTaskHandle;
const osThreadAttr_t CANSendTask_attributes = {
  .name = "CANSendTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for CANRecvTask */
osThreadId_t CANRecvTaskHandle;
const osThreadAttr_t CANRecvTask_attributes = {
  .name = "CANRecvTask",
  .priority = (osPriority_t) osPriorityRealtime,
  .stack_size = 256 * 4
};
/* Definitions for CANSendQueue */
osMessageQueueId_t CANSendQueueHandle;
const osMessageQueueAttr_t CANSendQueue_attributes = {
  .name = "CANSendQueue"
};
/* Definitions for CANRecvQueue */
osMessageQueueId_t CANRecvQueueHandle;
const osMessageQueueAttr_t CANRecvQueue_attributes = {
  .name = "CANRecvQueue"
};
/* Definitions for CANTimerMutex */
osMutexId_t CANTimerMutexHandle;
const osMutexAttr_t CANTimerMutex_attributes = {
  .name = "CANTimerMutex"
};
/* Definitions for CANMailSem */
osSemaphoreId_t CANMailSemHandle;
const osSemaphoreAttr_t CANMailSem_attributes = {
  .name = "CANMailSem"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
UNS32 OD1F50Callback(CO_Data* d, const indextable * bIndex, UNS8 bSubindex);
UNS32 OD1F51Callback(CO_Data* d, const indextable * bIndex, UNS8 bSubindex);
void canFestivalInit(void);
/* USER CODE END FunctionPrototypes */

void StartMainTask(void *argument);
void CANSendTaskEntry(void *argument);
void CANRecvEntry(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  gCANopenEventGroup = osEventFlagsNew(NULL);     
  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of CANTimerMutex */
  CANTimerMutexHandle = osMutexNew(&CANTimerMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of CANMailSem */
  CANMailSemHandle = osSemaphoreNew(3, 3, &CANMailSem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of CANSendQueue */
  CANSendQueueHandle = osMessageQueueNew (5, sizeof(Message), &CANSendQueue_attributes);

  /* creation of CANRecvQueue */
  CANRecvQueueHandle = osMessageQueueNew (5, sizeof(Message), &CANRecvQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of CANTimerTask */
  CANTimerTaskHandle = osThreadNew(StartMainTask, NULL, &CANTimerTask_attributes);

  /* creation of CANSendTask */
  CANSendTaskHandle = osThreadNew(CANSendTaskEntry, NULL, &CANSendTask_attributes);

  /* creation of CANRecvTask */
  CANRecvTaskHandle = osThreadNew(CANRecvEntry, NULL, &CANRecvTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartMainTask */
/**
  * @brief  Function implementing the MainTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartMainTask */
void StartMainTask(void *argument)
{
  /* USER CODE BEGIN StartMainTask */
	canFestivalInit();
	
	RegisterSetODentryCallBack(&TestSlave_Data,0x1F50,0x01,OD1F50Callback);
	RegisterSetODentryCallBack(&TestSlave_Data,0x1F51,0x01,OD1F51Callback);
	
	/* Defining the node Id */
	setNodeId(&TestSlave_Data, 0x01);

	/* init */
	setState(&TestSlave_Data, Initialisation);	
	
  /* Infinite loop */
  for(;;)
  {
    osEventFlagsWait(gCANopenEventGroup, CANOPEN_TIMER_EVENT_BIT, osFlagsWaitAny, osWaitForever);
		canTimerCallback();
  }
  /* USER CODE END StartMainTask */
}

/* USER CODE BEGIN Header_CANSendTaskEntry */
/**
* @brief Function implementing the CANSendTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CANSendTaskEntry */
void CANSendTaskEntry(void *argument)
{
  /* USER CODE BEGIN CANSendTaskEntry */
	Message msg;
	uint32_t TxMailbox;
	CAN_TxHeaderTypeDef tx_msg;
	
  /* Infinite loop */
  for(;;)
  {
		osMessageQueueGet(CANSendQueueHandle,&msg,NULL,osWaitForever);
		
		tx_msg.StdId = msg.cob_id;
		(msg.rtr) ? (tx_msg.RTR = CAN_RTR_REMOTE) : (tx_msg.RTR = CAN_RTR_DATA);
		tx_msg.IDE = CAN_ID_STD;
		tx_msg.DLC = msg.len;
		tx_msg.TransmitGlobalTime = DISABLE;
		
		osSemaphoreAcquire(CANMailSemHandle, osWaitForever);
		HAL_CAN_AddTxMessage(&hcan1, &tx_msg,msg.data, &TxMailbox);
  }
  /* USER CODE END CANSendTaskEntry */
}

/* USER CODE BEGIN Header_CANRecvEntry */
/**
* @brief Function implementing the CANRecvTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CANRecvEntry */
void CANRecvEntry(void *argument)
{
  /* USER CODE BEGIN CANRecvEntry */
	Message msg;
  /* Infinite loop */
  for(;;)
  {
    osMessageQueueGet(CANRecvQueueHandle,&msg,NULL,osWaitForever);
		canDispatch(&TestSlave_Data, &msg);
  }
  /* USER CODE END CANRecvEntry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
UNS32 OD1F50Callback(CO_Data* d, const indextable * bIndex, UNS8 bSubindex)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&bSubindex,1,100);
	if(1 == bSubindex)
	{
		
	}
	return 0;
}

UNS32 OD1F51Callback(CO_Data* d, const indextable * bIndex, UNS8 bSubindex)
{
	uint32_t cmd = *(uint8_t *)bIndex->pSubindex[bSubindex].pObject;
	
	HAL_UART_Transmit(&huart1,(uint8_t *)"",1,100);
	if(1 == bSubindex)
	{
		switch(cmd)
		{
			case CMD_STOP:
				break;
			case CMD_START:
				
				break;
			case CMD_RESET:
				HAL_NVIC_SystemReset();
				break;
			case CMD_CLEAR:
				APP_Erase(APP_1);
				break;
			case CMD_START_BOOTLOADER:
				break;
			case CMD_SET_SIGNATURE:
				break;
			case CMD_CLEAR_SIGNATURE:
				break;
		}
	}
	return 0;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
