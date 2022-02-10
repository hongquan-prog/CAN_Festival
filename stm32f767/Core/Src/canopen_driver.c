#include "canfestival.h"
#include "main.h"
#include "can.h"
#include "tim.h"
#include "cmsis_os.h"

static TIMEVAL last_counter_val = 0;
static TIMEVAL elapsed_time = 0;

extern osMessageQueueId_t CANSendQueueHandle;

//Set the timer for the next alarm.
static void setTimerFunc(TIMEVAL value)
{
  uint32_t timer_cnt = __HAL_TIM_GET_COUNTER(&htim7);
	
	elapsed_time += timer_cnt - last_counter_val;
	last_counter_val = htim7.Init.Period - value;
	
	__HAL_TIM_SET_COUNTER(&htim7, htim7.Init.Period - value);
	HAL_TIM_Base_Start_IT(&htim7);
}

//Return the elapsed time to tell the Stack how much time is spent since last call.
static TIMEVAL getElapsedTimeFunc(void)
{
  uint32_t timer_cnt = __HAL_TIM_GET_COUNTER(&htim7);
	
	if(timer_cnt < last_counter_val)
	{
		timer_cnt += htim7.Init.Period;
	}
	TIMEVAL elapsed = timer_cnt - last_counter_val + elapsed_time;
	
	return elapsed;
}

void canTimerCallbackFunc(void)
{
	last_counter_val = 0;
	elapsed_time = 0;
	TimeDispatch();
}

static unsigned char canSendFunc(CAN_PORT notused, Message *m)
{
	osMessageQueuePut(CANSendQueueHandle, m, NULL, 0);
	return 0;
}

static void CAN1_Init(void)
{
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterBank = 0;
	sFilterConfig.SlaveStartFilterBank = 14;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	sFilterConfig.FilterIdHigh = 0;
	sFilterConfig.FilterIdLow = 0;
	sFilterConfig.FilterMaskIdHigh = 0;
	sFilterConfig.FilterMaskIdLow = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterActivation = ENABLE;
	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);

	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_ERROR);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY);
	
	HAL_CAN_Start(&hcan1);
}

void canFestivalInit(void)
{
  canSend = canSendFunc;
	setTimer = setTimerFunc;
	getElapsedTime = getElapsedTimeFunc;
	canTimerCallback = canTimerCallbackFunc;
	
	CAN1_Init();
	HAL_TIM_Base_Start_IT(&htim7);
}
