/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
//CAN_TxHeaderTypeDef     TxMeg
#include "typedef.h"
extern uint8_t GetGA(void);
#include "rtthread.h"
extern rt_mq_t GetMsgQ_CAN(void);
extern void SetFpgaReg(uint8_t offset,uint16_t dd);
extern uint16_t GetFpgaReg(uint8_t offset);

/*
    如果想实现多个地址可以添加多个过滤器组
    stm32103 有0-13  共14个过滤器 
    在32位模式下可以过滤14个地址
    16为模式下可以过滤28个地址
    如果需要更多 那就要用屏蔽位模式，用屏蔽位模式就需要通过软件来判断一下地址

*/

//2个3级深度的FIFO
#define   CAN1FIFO   CAN_RX_FIFO0	           
#define   CAN2FIFO   CAN_RX_FIFO1  // 
CAN_TxHeaderTypeDef     TxMeg;
CAN_RxHeaderTypeDef     RxMeg;

void CAN_User_Init2(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  TxMeg.IDE=CAN_ID_EXT; //扩展帧
  TxMeg.RTR=CAN_RTR_DATA;//数据帧
	sFilterConfig.FilterMode =  CAN_FILTERMODE_IDLIST;  //设为列表模式    
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterFIFOAssignment = CAN1FIFO;      //接收到的报文放入到FIFO0中 
  sFilterConfig.FilterActivation = ENABLE;  
  sFilterConfig.SlaveStartFilterBank  = 14; 
	//过滤器0
    sFilterConfig.FilterBank = 0;                       
    sFilterConfig.FilterIdHigh = ((PC_ID<<3)>>16)&0xffff;
    sFilterConfig.FilterIdLow  = ((PC_ID<<3)&0xffff)|CAN_ID_EXT; 
    sFilterConfig.FilterMaskIdHigh =((ZIBAN_ID1<<3)>>16)&0xffff;
    sFilterConfig.FilterMaskIdLow  =((ZIBAN_ID1<<3)&0xffff)|CAN_ID_EXT; 
    HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
  	//过滤器1
    sFilterConfig.FilterBank = 1;
    sFilterConfig.FilterIdHigh     = ((ZIBAN_ID2<<3)>>16)&0xffff;
    sFilterConfig.FilterIdLow      = ((ZIBAN_ID2<<3)&0xffff)|CAN_ID_EXT; 
    sFilterConfig.FilterMaskIdHigh =((ZIBAN_ID2<<3)>>16)&0xffff;
    sFilterConfig.FilterMaskIdLow  =((ZIBAN_ID2<<3)&0xffff)|CAN_ID_EXT; 
    HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
	
	
    //CAN2 过滤器设置
    sFilterConfig.FilterFIFOAssignment = CAN2FIFO; 
    sFilterConfig.FilterBank = 14;
    sFilterConfig.FilterIdHigh = 0x020 <<5;
    sFilterConfig.FilterIdLow  = 0x021 <<5; 
    sFilterConfig.FilterMaskIdHigh =0x022<<5;
    sFilterConfig.FilterMaskIdLow  =0x023 <<5; 
    HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);
  
    //过滤器15
    sFilterConfig.FilterBank = 15;
    sFilterConfig.FilterIdHigh     = 0x030 <<5;
    sFilterConfig.FilterIdLow      = 0x031 <<5; 
    sFilterConfig.FilterMaskIdHigh =0x032<<5;
    sFilterConfig.FilterMaskIdLow  =0x033 <<5; 
    HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
  
    HAL_CAN_Start(&hcan1);  
    HAL_CAN_ActivateNotification(&hcan1,   CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_Start(&hcan2);  
    HAL_CAN_ActivateNotification(&hcan2,   CAN_IT_RX_FIFO1_MSG_PENDING);
}

void CAN_User_Init(void)   //用户初始化函数
{
   CAN_FilterTypeDef  sFilterConfig;
	
  TxMeg.IDE=CAN_ID_STD;   //CAN_ID_EXT;//标准帧，扩展帧
  TxMeg.RTR=CAN_RTR_DATA; //CAN_RTR_REMOTE;//数据帧，远程帧
	
    sFilterConfig.FilterMode =  CAN_FILTERMODE_IDLIST;  //设为列表模式    
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterFIFOAssignment = CAN1FIFO;      //接收到的报文放入到FIFO0中 
    sFilterConfig.FilterActivation = ENABLE;  
    sFilterConfig.SlaveStartFilterBank  = 14; 
    
    //过滤器0
    sFilterConfig.FilterBank = 0;                       
    sFilterConfig.FilterIdHigh = PC_ID <<5;
    sFilterConfig.FilterIdLow  = ZIBAN_ID2 <<5; //2021年7月23日 用于获取时钟状态，方便显示
    sFilterConfig.FilterMaskIdHigh =ZIBAN_ID1<<5;
    sFilterConfig.FilterMaskIdLow  =ZIBAN_ID1 <<5; 
    HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
  
    //过滤器1
//    sFilterConfig.FilterBank = 1;
//    sFilterConfig.FilterIdHigh     = ZIBAN_ID1 <<5;
//    sFilterConfig.FilterIdLow      = ZIBAN_ID1 <<5; 
//    sFilterConfig.FilterMaskIdHigh =ZIBAN_ID1<<5;
//    sFilterConfig.FilterMaskIdLow  =ZIBAN_ID1 <<5; 
//    HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
    
    //CAN2 过滤器设置
    sFilterConfig.FilterFIFOAssignment = CAN2FIFO; 
    sFilterConfig.FilterBank = 14;
    sFilterConfig.FilterIdHigh = 0x020 <<5;
    sFilterConfig.FilterIdLow  = 0x021 <<5; 
    sFilterConfig.FilterMaskIdHigh =0x022<<5;
    sFilterConfig.FilterMaskIdLow  =0x023 <<5; 
    HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);
  
    //过滤器15
    sFilterConfig.FilterBank = 15;
    sFilterConfig.FilterIdHigh     = 0x030 <<5;
    sFilterConfig.FilterIdLow      = 0x031 <<5; 
    sFilterConfig.FilterMaskIdHigh =0x032<<5;
    sFilterConfig.FilterMaskIdLow  =0x033 <<5; 
    HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
  
    HAL_CAN_Start(&hcan1);  
    HAL_CAN_ActivateNotification(&hcan1,   CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_Start(&hcan2);  
    HAL_CAN_ActivateNotification(&hcan2,   CAN_IT_RX_FIFO1_MSG_PENDING);
  
 }
uint32_t mail=0;
//发送数据函数
uint8_t CANx_SendNormalData(CAN_HandleTypeDef* hcan,uint16_t ID,uint8_t *pData,uint16_t Len)
{
	HAL_StatusTypeDef	HAL_RetVal;
    uint16_t SendTimes,SendCNT=0;
	uint8_t  FreeTxNum=0;
	TxMeg.StdId=ID;
	if(!hcan || ! pData ||!Len)  return 1;
	SendTimes=Len/8+(Len%8?1:0);
	FreeTxNum=HAL_CAN_GetTxMailboxesFreeLevel(hcan);
	TxMeg.DLC=8;
	while(SendTimes--){
		if(0==SendTimes){
			if(Len%8)
				TxMeg.DLC=Len%8;
		}
		while(0==FreeTxNum){
			FreeTxNum=HAL_CAN_GetTxMailboxesFreeLevel(hcan);
		}
		HAL_Delay(1);   //没有延时很有可能会发送失败
		HAL_RetVal=HAL_CAN_AddTxMessage(hcan,&TxMeg,pData+SendCNT,(uint32_t*)&mail); 
		if(HAL_RetVal!=HAL_OK)
		{
			return 2;
		}
		SendCNT+=8;
	}
	
  return 0;
}
void CAN1_Send8Byte(uint32_t ID,uint8_t *pData,uint16_t Len,uint8_t IDE)
{
    
    if(Len>8)return;
    //可以是扩展帧或者标准帧
	
	  TxMeg.ExtId=ID;
	  TxMeg.StdId=ID;

    TxMeg.IDE =IDE;
		TxMeg.RTR = CAN_RTR_DATA;
    TxMeg.DLC=Len;
    /*
    CAN_TX_BOX0是作为一个类似于接收返回值的作用
    每当上述函数发送完毕时，即CAN发送一帧后，CAN_TX_BOX0里放的的是这个数据帧发送时所使用的邮箱号
    */
    HAL_CAN_AddTxMessage(&hcan1,&TxMeg,pData,(uint32_t*)&mail); 
}
void CAN2_Send8Byte(uint16_t ID,uint8_t *pData,uint16_t Len)
{
    
    if(Len>8)return;
    TxMeg.StdId=ID;
    TxMeg.DLC=Len;
    /*
    CAN_TX_BOX0是作为一个类似于接收返回值的作用
    每当上述函数发送完毕时，即CAN发送一帧后，CAN_TX_BOX0里放的的是这个数据帧发送时所使用的邮箱号
    */
    HAL_CAN_AddTxMessage(&hcan2,&TxMeg,pData,(uint32_t*)&mail); 
}
void test_CAN(void)
{
    //uint8_t C1[8] ={0x12,0x23,0x34,0x45,0x56,0x67,0x78,0x89};
    //uint8_t C2[8] ={0x11,0x22,0x33,0x44,0x55,0x44,0x22,0x11};
    uint8_t C3[8] ={0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88};
    static uint16_t ccc=0;
    if(ccc++<100)return;
   //CANx_SendNormalData(&hcan1,1,C3,8);
   //CANx_SendNormalData(&hcan2,2,C3,8);
    
    CAN2_Send8Byte(1,C3,8);
    
    ccc=0;
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)  //接收回调函数
{
  uint8_t  Data[8]={0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE};
  HAL_StatusTypeDef	HAL_RetVal;
  if(hcan ==&hcan1){	
    HAL_RetVal=HAL_CAN_GetRxMessage(hcan,  CAN1FIFO, &RxMeg,  Data);
    if ( HAL_OK==HAL_RetVal){
			uint32_t getID = 0x0;
			if(RxMeg.IDE == CAN_ID_EXT) 
			{
				getID = RxMeg.ExtId;
			}
			else if(RxMeg.IDE == CAN_ID_STD)
			{
				getID = RxMeg.StdId;	
			}
			if(getID == PC_ID ||getID==ZIBAN_ID2||getID==ZIBAN_ID1)
			{
				 //发送到消息队列
				 rt_mq_send_with_num_length(GetMsgQ_CAN(), Data , RxMeg.DLC, getID,RxMeg.IDE);
			}
    }
  }

}
 
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)  //接收回调函数
{
    uint8_t  Data[8]={0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE};
    HAL_StatusTypeDef	HAL_RetVal;
    if(hcan ==&hcan2){	
        HAL_RetVal=HAL_CAN_GetRxMessage(hcan,  CAN2FIFO, &RxMeg,  Data);
        if ( HAL_OK==HAL_RetVal){                              			
          //在这里接收数据
           //CAN_Send8Byte(01,Data,RxMeg.DLC);
           CAN2_Send8Byte(RxMeg.StdId,Data,RxMeg.DLC);
            
        }
     }
}



void CAN1_UnframeAndSend(uint32_t ID,uint8_t DLC,uint32_t IDE,uint8_t* Data)
{
	  uint8_t cmd = Data[0];
	  uint8_t CC[8]={0};
		uint8_t len=8;
		uint32_t sendID = MUBAN_ID1;
		if(ID == PC_ID)
		{
			switch(cmd)
			{
                case ...:
				break;

				default:
				return;
			}
		}else if(ID == ZIBAN_ID1)
		{
				if(cmd == CMD_STATE_4)
				{
					if(DLC!=6)return;
					g_MI.son100MLock = Data[5];
				}
				return;
		
		}
		else if(ID==ZIBAN_ID2)
		{
			
			switch(cmd){
				case ...://母板作为接收
				
				break;
	
		   default:
					return;

			}

		}
		else
			return;
		
		CAN1_Send8Byte(sendID,CC,len,IDE);
}




/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 6;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* CAN2 init function */
void MX_CAN2_Init(void)
{

  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 6;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }

}

static uint32_t HAL_RCC_CAN1_CLK_ENABLED=0;

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */
    
      
  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */
  
  
  
  /* USER CODE END CAN1_MspInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */

  /* USER CODE END CAN2_MspInit 0 */
    /* CAN2 clock enable */
    __HAL_RCC_CAN2_CLK_ENABLE();
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN2 interrupt Init */
    HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
  /* USER CODE BEGIN CAN2_MspInit 1 */

  /* USER CODE END CAN2_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN2_CLK_DISABLE();
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    /* CAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
