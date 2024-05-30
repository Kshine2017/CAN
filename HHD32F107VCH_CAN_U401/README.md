
# 1.初始化
```
    ZL_CanInit(1);
    ZL_CanInit(2);
```
## 1.1 FIFO初始化
- port=1或者2
```
init_CanFifo(port,1024);
```

## 1.2 底层驱动初始化
- 在hhd_can.can文件中，can_init函数，包括了硬件过滤器的设置方法
```
/*******************************************************************************
*
* CAN 接口初始化
*
*
* CAN使用的时钟CAN_CLK是APB1时钟，当前APB1为主频时钟;
* 波特率基于“原子时间”计算，及 一个“原子时间”的周期TQ = CAN_CLK/(2*(1+can->BTR0.bit.BRP);
* 例如 APB1 = 60M，can->BTR0.bit.BRP = 3; 1/TQ = (60/2) / ((3+1)) = 7.5M
* 波特率 BIT Period 以TQ作为单位，计算方法 Tsyncseg + Ttseg1 + Ttseg2 + 2
* Tsyncseg = 1；
* 例如 Ttseg1 = 7；Ttseg2 = 5；
  则波特率  = 7.5M/（3+7+5）= 500Kbps
********************************************************************************/
/*
模式控制寄存器(MOD)，地址：0x000h
bit0 RM 复位模式       1 复位模式，任何收到和发送的消息会被丢弃
bit1 LOM 侦听模式      1 Listen Only 模式                        0 正常模式。
bit2 STM 自测模式      1 自测模式使能  
bit3 AFM 接受过滤模式  1 数据接受采用 4-byte 过滤器（32位过滤器）              0 数据接受采用 2 个 shoter 过滤器（16位过滤器）
bit4 SM 休眠模式       1 CAN 控制器进入休眠模式                  0 工作模式
*/
void can_init(HHD32F1_CAN_TypeDef *can, EN_BAUD baud, uint32_t filterID, uint32_t mask)
{
	if(can == CAN1)
	{
		// 模块时钟	
		SYSCON->SYSAPB1CLKCTRL.bit.CAN1CLK = 1;
		SYSCON->APB1RESET.bit.CAN1RSTN=1;
		SYSCON->APB1RESET.bit.CAN1RSTN=0;
		//	引脚
	
		HHD_AFIO -> PA11 = CAN1_RX_PA11;  			//CAN1 RX config
		HHD_AFIO -> PA12 = CAN1_TX_PA12;  			//CAN1 TX config
		// 使模块进入配置模式	
	}
	else if(can == CAN2)
	{
		SYSCON->SYSAPB1CLKCTRL.bit.CAN2CLK = 1;
		SYSCON->APB1RESET.bit.CAN2RSTN=1;
		SYSCON->APB1RESET.bit.CAN2RSTN=0;
		HHD_AFIO -> PB5 = CAN2_RX_PB5;  			//CAN2 RX config
		HHD_AFIO -> PB6 = CAN2_TX_PB6;  			//CAN2 TX config		
	}
	
	//1. 进入复位模式
	do{
		CAN_SetMode(can, ResetMode);
	}while(can->MOD.bit.RM !=  ResetMode);//等待成功进入复位模式
	
  //2. 配置波特率	
	/*
	BIT Period= Tsyncseg + Ttseg1 + Ttseg2 = 1+(7+1)+(5+1) = 15
	*/
	//TQ = 2x(1/60MHz)x2 = (1/15)us

	
	/*
	总线时序寄存器 0(BTR0)  地址：0x018h
	bit5:0 BRP 波特率分频器 数值为 时间原子 TQ 对应 CAN 工作时钟的个数
	bit7:6 SJW 同步跳跃宽度 该值定义的是，最大缩短或延长的 TQ 数量
	*/
	/*
	总线时序寄存器 1(BTR1)  地址：0x01Ch
	bit3:0 TSEG1 Bit周期长度1    Tseg1=TQx(4TSEG2.2+2TSEG2.1+ TSEG2.0+1)
	bit6:4 TSEG2 Bit周期长度2    Tseg2=TQx(8TSEG1.3+4TSEG1.2+ 2TSEG1.1+TSEG1.0+1)
	bit7   SAM                   1 总线采样3 次    0 总线采样1 次
	*/
 	can->BTR0.bit.SJW = 0x00; //同步跳跃宽度 最大缩短或延长的 TQ =0
	if(baud == CAN_BAUD_1M)	
	{
	
		can->BTR0.bit.BRP = 1;   //波特率分频器
		can->BTR1.bit.TSEG1 = 9;
		can->BTR1.bit.TSEG2 = 3;	
	}
	else if(baud == CAN_BAUD_500K)
	{
		can->BTR0.bit.BRP = 2; 
		can->BTR1.bit.TSEG1 = 14;
		can->BTR1.bit.TSEG2 = 3;
	}
	else if(baud == CAN_BAUD_250K) 
	{
		
		can->BTR0.bit.BRP = 5;   
		can->BTR1.bit.TSEG1 = 14;
		can->BTR1.bit.TSEG2 = 3;	
	}
    else if(baud == CAN_BAUD_200K) 
	{
		
		can->BTR0.bit.BRP = 14;   
		can->BTR1.bit.TSEG1 = 7;
		can->BTR1.bit.TSEG2 = 0;	
	}
	else  //其他波特率   ，需要计算
	{
		can->BTR0.bit.BRP = 15;   
		can->BTR1.bit.TSEG1 = 7;
		can->BTR1.bit.TSEG2 = 0;
	}
 	can->BTR1.bit.SAM 	= 0;   //0 总线采样1 次
	
	
	//3. 设置滤波器
	can->MOD.bit.AFM 	= 0x01;  //1 数据接受采用 4-byte 过滤器(32位过滤器)
														 //0 数据接受采用 2 个 shoter 过滤器（2个16位过滤器）
// 设置接收帧过滤，可以接收任何标识符
	

		/*
		ACR/AMR屏蔽字属性:
		ACR用于指定CAN ID二进制中关注位和忽略位；
		AMR用于规定ACR中那些二进制位生效，哪些无效.AMR中0代表有效位，1代表无效位；
		*/
		// Acceptance Code Register 
		can->DATAINFO.FILTER.ACR[0].all  = 0x02;
		can->DATAINFO.FILTER.ACR[1].all  = 0x00;
		can->DATAINFO.FILTER.ACR[2].all  = 0x4D;//MCU CAN ID
		can->DATAINFO.FILTER.ACR[3].all  = 0xFF;

		//Acceptance Mask Register
		can->DATAINFO.FILTER.AMR[0].all = 0x00;//全8位 有效
		can->DATAINFO.FILTER.AMR[1].all = 0x1F;//高3位 有效
		can->DATAINFO.FILTER.AMR[2].all = 0x00;//全8位 有效
		can->DATAINFO.FILTER.AMR[3].all = 0xff;//
		

// 开接收中断	
	CAN_EnableInt(can, RIE); //开启接收中断

#ifdef SELF_LOOP
	CAN_SetMode(can, ACCFMode | SelfTestMode);
#else
	CAN_SetMode(can, ACCFMode | ActiveMode);	
#endif	

}
```

# 2.发送
```
static void CAN_ACK_0x4D(void)
{
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg0.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg1.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg2.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg3.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg4.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg5.array,8);
    ZL_CANX_Send8Byte(1,0x4D ,can_pkg6.array,8);
}    

```

# 3.接收，从FIFO中读取数据
```
void CAN(void)
{ 
    int i=1;
    if(CurrentStatus==WORK_MODE_STANDBY)
        return;
    feeddog_H();
    #if 1
    for(i=1;i<=2;i++)
    {
       uint8_t* frame = (uint8_t*) ZL_CanFIFORead16Bytes(i);
      
        //读到数据
       if(frame)
        {
//          uint32_t srcID =*((uint32_t*)(frame+2));
//          uint8_t code  = *((uint8_t*)(frame+6));//byte0 //0x4Dh
            uint8_t mode  = *((uint8_t*)(frame+7));//byte1 //0X1
            switch(mode)//模式字	SWO1
            {
                case 0x01:
                {
                  CAN_ACK_0x4D();
                }
                break;
        
                default:
                
                break;
            }
        }
    }
    #endif
    feeddog_L();
        
}

```
