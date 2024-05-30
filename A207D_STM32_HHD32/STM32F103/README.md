# 1. CAN初始化
```
MX_CAN1_Init();
MX_CAN2_Init();
#if CAN_B
  CAN_User_Init2();
#else
	CAN_User_Init();
#endif

```
## 1.1 CAN1
```
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
```
## 1.2 CAN2
```
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

```


## 1.3. CAN过滤器设置
- 标准帧，过滤器列表模式16位
```
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

```

- 扩展帧，过滤器列表模式32位
```
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

```

# 2. 接收
```
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)  //接收回调函数

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)  //接收回调函数

```

# 3. 发送
- mail变量必须是非局部变量


```
uint32_t mail=0;

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

```



