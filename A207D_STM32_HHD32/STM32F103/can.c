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
		if(ID == PC_ID){
		
		switch(cmd)
		{

				/*
				将CAN盒依次接在测试工装CAN0和1的位置
				
				选择模块自检，等待综合控制模块上报所有模块工作正常
				将CAN盒接在测试工装CAN1的位置，切换槽位识别号GA0~3,GAP，改变高低电平，重复以上步骤,观察能否正确上报状态
				正常情况下能够自检正常，错误情况下能够上报自检错误，同时则判定综合控制模块自检功能合格。
				板上工作电压、温度状态能正确上报则判定综合控制模块上报温度电压状态合格。
				槽位识别能正确上报则判定综合控制模块槽位识别号合格。
				*/
				case CMD_START_CAN://can槽位识别 测试结果上报
				{
				  len = 2;
					CC[0]=0x8D;
					CC[1]=GetGA();
				}
				break;
				
				/*
				(1)	将综合控制模块定义成LVTTL数据发送，测试工装定义成LVTTL数据接收，观察数据高低翻转收发是否一致
				(2) 将综合控制模块定义成LVTTL数据接收，测试工装定义成LVTTL数据发送，观察数据高低翻转收发是否一致
				*/
				case CMD_START_LVTTL://LVTTL测试结果上报
				{
					if(DLC!=2 || Data[1]!=CMD_MUBAN_SEND)return;//必须是母板发送
					SetFpgaReg(0xA1,MUBAN_REG_SEND);//自己配置为发送
					sendID = MUBAN_ID2;//通知子板配置为接收
					len = 2;
					CC[0]=CMD_START_LVTTL;
					CC[1]=Data[1];
					
				}
				break;
				case CMD_START_TTL://TTL测试结果上报
				{
					if(DLC!=2 || Data[1]!=CMD_MUBAN_SEND)return;//必须是母板发送
					SetFpgaReg(0xA2,MUBAN_REG_SEND);//设置为发送
					sendID = MUBAN_ID2;//通知子板配置为接收
					len = 2;
					CC[0]=CMD_START_TTL;
					CC[1]=Data[1];
				}
				
				break;

				
				/*
				控制MLVDS驱动器收发方向使能脚，将综合控制模块定义成MLVDS数据发送，产生依次递增的数据，数据速率为200Mbps，测试工装定义成MLVDS数据接收，观察数据收发是否一致
				控制MLVDS驱动器收发方向使能脚，将综合控制模块定义成MLVDS数据接收，测试工装定义成MLVDS数据发送，产生依次递增的数据，数据速率为200Mbps，改变MLVDS驱动器中的FSEN脚，看数据收发是否一致
				TypeI与TypeII工作方式是否可控
				
				正常情况下综合控制模块和测试工装FPGA收发数据一致，则判定11路MLVDS输出信号收发正常，TypeI与TypeII工作方式可控
				*/
				case CMD_START_MLVDS://MLVDS测试结果上报
				{
					if(DLC!=3 || Data[1]!=CMD_MUBAN_SEND)return;//必须是母板发送
					
					//设置模式：type1或者type2
					g_MI.typeflag = Data[2];
		
					if(g_MI.typeflag==1)
					{
						SetFpgaReg(0xA4,MUBAN_REG_TYPE1);//type1
					} 
					else if(g_MI.typeflag==2)
					{
						SetFpgaReg(0xA4,MUBAN_REG_TYPE2);//type2
					}
					rt_thread_mdelay(1);
					
					//设置方向：母板发送
					SetFpgaReg(0xA3,MUBAN_REG_SEND);//自己配置为发送
					
					//通知子板配置为接收
					sendID = MUBAN_ID2;
					len =3;
					CC[0]=CMD_START_MLVDS;
					CC[1]=Data[1];
					CC[2]=Data[2];//type测试
					//g_MI.typeflag = Data[2];
				}
				break;

				case CMD_START_RESET:
					SetFpgaReg(0xA5,0x0001);
					rt_thread_mdelay(1);
					SetFpgaReg(0xA5,0x0000);
				return;//不需要回报
				
				/*
				收到上位机测试LVDS，母板先自测，并向子板报告自己的结果
				*/
				case CMD_START_LVDS:
				{
				  g_MI.spi.a.LVDS =GetFpgaReg(0x18);
					len =2;
				  sendID = MUBAN_ID2;
					CC[0]=CMD_START_LVDS;
					if(g_MI.son100MLock==0 || g_MI.spi.a.LVDS)CC[1]=0x02;//故障
					else CC[1]=0x01;//正常
				}
				break;
								/*
				收到上位机测试RS422，母板先自测，并向子板报告自己的结果
				*/
				case CMD_START_RS422:
				{
				  g_MI.spi.a.RS422 =GetFpgaReg(0x1b);
					len =2;
				  sendID = MUBAN_ID2;
					CC[0]=CMD_START_RS422;
					if(g_MI.son100MLock==0 || g_MI.spi.a.RS422)CC[1]=0x02;//故障
					else CC[1]=0x01;//正常
				}
				break;
				/*
				收到上位机测试gtx，母板先自测，并向子板报告自己的结果
				*/
				case CMD_START_GTX:
				{
				  g_MI.spi.a.GTX =GetFpgaReg(0x1d);
					len =2;
				  sendID = MUBAN_ID2;
					CC[0]=CMD_START_GTX;
					if(g_MI.son100MLock==0 || g_MI.spi.a.GTX)CC[1]=0x02;//故障
					else CC[1]=0x01;//正常
				}
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
				case CMD_START_LVTTL://母板作为接收
				{
					if(DLC!=2 || Data[1]!=CMD_MUBAN_RECV)return;
					
				  sendID = MUBAN_ID1;
					SetFpgaReg(0xA1,MUBAN_REG_RECV);
					
					rt_thread_mdelay(1);
					
					g_MI.spi.a.LVTTL =GetFpgaReg(0x16);
					len =3;
					CC[0]=CMD_RESULT_LVTTL;
					CC[1]=0x02;//母板作为接收
					if(g_MI.son100MLock==0 || g_MI.spi.a.LVTTL)CC[2]=0x02;//故障
					else CC[2]=0x01;//正常
					
				}
				break;
				
				case CMD_START_TTL://母板作为接收
				{
					if(DLC!=2 || Data[1]!=CMD_MUBAN_RECV)return;
					
					sendID = MUBAN_ID1;
					SetFpgaReg(0xA2,MUBAN_REG_RECV);
					
					rt_thread_mdelay(1);
					
					g_MI.spi.a.TTL =GetFpgaReg(0x17);
					len =3;
					CC[0]=CMD_RESULT_TTL;
					CC[1]=0x02;//母板作为接收
				  if(g_MI.son100MLock==0 || g_MI.spi.a.TTL)CC[2]=0x02;//故障
					else CC[2]=0x01;//正常
				}
					
				break;
				
				case CMD_START_MLVDS://母板作为接收
				{
					if(DLC!=3 || Data[1]!=CMD_MUBAN_RECV)return;

					sendID = MUBAN_ID1;
					
					//设置模式：type1或者type2
					g_MI.typeflag = Data[2];
		
					if(g_MI.typeflag==1)
					{
						SetFpgaReg(0xA4,MUBAN_REG_TYPE1);//type1
					} 
					else if(g_MI.typeflag==2)
					{
						SetFpgaReg(0xA4,MUBAN_REG_TYPE2);//type2
					}
					rt_thread_mdelay(1);
					
					//设置方向：母板接收
					SetFpgaReg(0xA3,MUBAN_REG_RECV);
					rt_thread_mdelay(1);
					
					//读取数据
					g_MI.spi.a.MLVDS =GetFpgaReg(0x19);
					
					len =4;
					CC[0]=CMD_RESULT_MLVDS;
					CC[1]=CMD_MUBAN_RECV;//母板作为接收
					CC[2]=g_MI.typeflag;//type
					
					if(g_MI.son100MLock==0 || g_MI.spi.a.MLVDS)CC[3]=0x02;//故障
					else CC[3]=0x01;//正常
					
					g_MI.typeflag =0;
				}
				
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
