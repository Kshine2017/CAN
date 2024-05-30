# 1. 初始化
- can_init函数内，包括过滤器的设置
```
void InitCAN(void)
{
	can_init(CAN1, CAN_BAUD_500K, PROJ_CAN_SRC_NODE, 0x0000000);//	can_init(CAN1, CAN_BAUD_1M, 0x7ff, 0x00);
	memset(&Can1RxFrame, 0, sizeof(CanRxMsg));
	Can1RxFrame.IDE = CAN_ID_EXT; //默认扩展帧形式发送数据
	Can1RxFrame.RTR = CAN_RTR_DATA;
	
	//can2 备份
	can_init(CAN2, CAN_BAUD_500K, PROJ_CAN_SRC_NODE, 0x0000000);
	
}

```



# 2. 发送
- 在hhd_can.c文件中
```
//扩展帧发送
int Ext_can_write(HHD32F1_CAN_TypeDef *can, union UN_CAN_IDENTIFY *Identifier,  uint8_t *data, int len)
//标准帧发送
int can_write(HHD32F1_CAN_TypeDef *can, enum FRMAT_TYPE type, union UN_CAN_IDENTIFY *Identifier,  uint8_t *data, int len)

```

# 3.中断接收
```
int RxMsgFromCan1(void)
{		
	CAN_Receive(CAN1, 8, &Can1RxFrame);//读取数据
	if(Can1RxFrame.DLC != 0)
	{
				unframe();
	}
	
	return 0;
}

void CAN1_IRQHandler(void)
{
  OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	
	RxMsgFromCan1();//CAN接收执行函数

	CAN_SetCMD(CAN1, ReleaseRecBuf); // release recieve buffer

	OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}

void CAN2_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;
	
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	
  if(CAN2->IR.bit.RI == 0x01) //接收中断
	{
		
#if 1
//#ifndef CFG_USING_CAN1_WRITE_BACK
		int i;
		uint8_t can_buff[15] = {0};
		can_buff[0] = CAN2->DATAINFO.RxDATAINFO.RxFRAME.all;
		for(i = 0; i<12; i++)
		{
			can_buff[i+1] =  CAN2->DATAINFO.RxDATAINFO.RxDATA[i].all;
			
		}
		
		can_write_back(CAN2,  can_buff);
#endif	
	}
//////////////////////////////////////////////////////////////////////////////////////////	
//我的CAN接收执行函数
	
/////////////////////////////////////////////////////////////////////////////////////////	
	CAN_SetCMD(CAN2, ReleaseRecBuf); // release recieve buffer
	
	OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}

```