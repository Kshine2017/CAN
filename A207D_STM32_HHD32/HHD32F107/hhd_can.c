/*****************************************************************************
 *   hhd_can.C:  Sourse file for  MCU CAN application
 *
 *   Copyright(C) 2015,  Technology
 *   All rights reserved.
 *
 *   History
 *   2015.08.14  ver 1.00
 *
******************************************************************************/
#include "hhd32f1xx.h"
#include "hhd_sys.h"
#include "hhd_can.h"

extern uint32_t GetTick(void);


/*
模式控制寄存器(MOD)，地址：0x000h
bit0 RM 复位模式       1 复位模式，任何收到和发送的消息会被丢弃
bit1 LOM 侦听模式      1 Listen Only 模式                        0 正常模式。
bit2 STM 自测模式      1 自测模式使能  
bit3 AFM 接受过滤模式  1 数据接受采用 4-byte 过滤器              0 数据接受采用 2 个 shoter 过滤器
bit4 SM 休眠模式       1 CAN 控制器进入休眠模式                  0 工作模式
*/
void CAN_SetMode(HHD32F1_CAN_TypeDef *can, uint32_t mode)
{
    can->MOD.all = mode;

    return;
}

/*
	状态寄存器(SR)，地址：0x008h
	bit0 RBS 接受 buffer状态          1 接受 buffer已被释放，CPU 可以读取接受新接受到的消息    0 接受 buffer 为空，无消息可读
	bit1 DOS 数据过载状态             1 数据过载，由于空间不足导致有消息丢失                   0 无数据过载，无数据由于空间不足导致有消息丢失
	bit2 TBS 传输 buffer状态          1 发送 buffer 已被释放，CPU 可以写入新消息               0 发送 buffer 被锁定，buffer 中消息正准备被发送
	bit3 TCS 传输完成状态             1 最后一笔数据已被发送完成                               0 最后一笔数据未被发送完成
	bit4 RS 接受状态                  1 CAN 控制器处于接受消息进程中                           0 无消息在接受中
	bit5 TS 传输状态                  1 CAN 控制器处于发送消息进程中                           0 无消息在发送中
	bit6 ES 错误状态                  1 至少一种错误计数器达到 EWL寄存器设置的错误数           0 两种错误计数器均未达到报警线
	bit7 BS 总线状态                  1 CAN 控制器是总线 off 态，未参与总线交易                0 CAN 控制参与总线交易
	*/
int CAN_GetStatus(HHD32F1_CAN_TypeDef *can)
{


    return (can->SR.all);
}

void CAN_ClearStatus(HHD32F1_CAN_TypeDef *can, uint32_t clrbit)
{

    can->SR.all &= ~clrbit;
    return ;
}


	/*
	 Command 命令寄存器(CMR)，地址：0x004h
	bit0 TR 发送请求        设置 1 发送一笔消息
	bit1 AT 传输丢弃        设置 1 用于取消下一笔传输请求
	bit2 RRB 释放接受buffer 设置 1 用于释放接受 buffer
	bit3 CDO 清数据过载     设置 1 用于清除数据过载标志
	bit4 SRR 自接受请求     同时发送和接受消息时，该 bit 设 1
	*/
void CAN_SetCMD(HHD32F1_CAN_TypeDef *can, uint32_t cmddata)
{

    can->CMR.all |= cmddata;
    return;
}

void CAN_ClrCMD(HHD32F1_CAN_TypeDef *can, uint32_t cmddata)
{

    can->CMR.all &= ~cmddata;
    return;
}

/*
 中断寄存器(IR)  地址：0x00Ch  只读寄存器
  当下面有 1 位为 1 时，控制器向 CPU 发送中断请求
bit0 RI  接受中断     当有接受中断发生时 为 1
bit1 TI  传输中断     当有传输中断发生时 为 1
bit2 EI  错误报警中断 当有错误报警发生时 为 1
bit3 DOI 数据过载中断 当有数据过载发生时 为 1
bit4 WUI 唤醒中断     当有总线错误发生时 为 1
bit5 EPI 错误被动中断 当有错误被动发生时 为 1
bit6 ALI 仲裁丢失中断 当有仲裁丢失发生时 为 1
bit7 BEI 总线错误中断 当有总线错误发生时 为 1

 中断使能寄存器(IER)  地址：0x010h
bit0 RIE  接受中断使能
bit1 TIE  传输中断使能
bit2 EIE  错误报警中断使能
bit3 DOIE 数据过载中断使能
bit4 WUIE 唤醒中断使能
bit5 EPIE 错误被动中断使能
bit6 ALIE 仲裁丢失中断使能
bit7 BEIE 总线错误中断使能


*/
void CAN_EnableInt(HHD32F1_CAN_TypeDef *can, uint32_t intbit)
{

    can->IER.all |= intbit ; ///int enable
    //NVIC_EnableIRQ(CAN_IRQn);
    if(can == CAN1)
    {
        NVIC_EnableIRQ(CAN1_IRQn);
    }
    else if(can == CAN2)
    {
        NVIC_EnableIRQ(CAN2_IRQn);
    }
    else return;
}

void CAN_DisableInt(HHD32F1_CAN_TypeDef *can, uint32_t intbit)
{
    can->IER.all  &= ~intbit ; ///int disable
    if(can == CAN1)
    {
        NVIC_DisableIRQ(CAN1_IRQn);
    }
    else if(can == CAN2)
    {
        NVIC_DisableIRQ(CAN2_IRQn);
    }
    else return;
}


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
	else  //其他波特率   ，需要计算
	{
		can->BTR0.bit.BRP = 5;   
		can->BTR1.bit.TSEG1 = 14;
		can->BTR1.bit.TSEG2 = 3;
	}
 	can->BTR1.bit.SAM 	= 0;   //0 总线采样1 次
	
	
	//3. 设置滤波器
	can->MOD.bit.AFM 	= 0x01;  //1 数据接受采用 4-byte 过滤器(32位过滤器)
														 //0 数据接受采用 2 个 shoter 过滤器（2个16位过滤器）
// 设置接收帧过滤，可以接收任何标识符
	
	if(filterID <= 0x7FF)
	{
		/*
		ACR/AMR屏蔽字属性:
		ACR用于指定CAN ID二进制中关注位和忽略位；
		AMR用于规定ACR中那些二进制位生效，哪些无效.AMR中0代表有效位，1代表无效位；
		*/
		// Acceptance Code Register 
		can->DATAINFO.FILTER.ACR[0].all  = ((filterID << 5) >> 8 ) & 0xFF;   //Identifier
		can->DATAINFO.FILTER.ACR[1].all  = (filterID << 5) &0xE0 ;  		 //Identifier
		can->DATAINFO.FILTER.ACR[2].all  = 0xFF;
		can->DATAINFO.FILTER.ACR[3].all  = 0xFF;

		//Acceptance Mask Register
		can->DATAINFO.FILTER.AMR[0].all = 0xFF;//((mask << 5) >> 8 ) & 0xFF;
		can->DATAINFO.FILTER.AMR[1].all = 0xFF;//(mask << 5) &0xE0 ;
		can->DATAINFO.FILTER.AMR[2].all = 0xff;
		can->DATAINFO.FILTER.AMR[3].all = 0xff; 
		
	}
	else
	{
		can->DATAINFO.FILTER.ACR[0].all = ((filterID << 3) >> 24) & 0xFF;   	//Identifier
		can->DATAINFO.FILTER.ACR[1].all  = ((filterID<< 3) >> 16) & 0xFF;  		//Identifier
		can->DATAINFO.FILTER.ACR[2].all  = ((filterID << 3) >> 8) & 0xFF;   	//Identifier
		can->DATAINFO.FILTER.ACR[3].all  = (filterID << 3) & 0xF8;  		    //Identifier
		
		can->DATAINFO.FILTER.AMR[0].all = ((mask << 3) >> 24) & 0xFF;   	//Identifier;
		can->DATAINFO.FILTER.AMR[1].all = ((mask<< 3) >> 16) & 0xFF;
		can->DATAINFO.FILTER.AMR[2].all = ((mask << 3) >> 8) & 0xFF;
		can->DATAINFO.FILTER.AMR[3].all = (mask << 3) & 0xF8; 
	}

// 开接收中断	
	CAN_EnableInt(can, RIE); //开启接收中断

#ifdef SELF_LOOP
	CAN_SetMode(can, ACCFMode | SelfTestMode);
#else
	CAN_SetMode(can, ACCFMode | ActiveMode);	
#endif	

}
#include "include.h"
/***********************************************************************************************
*
*
*	CAN 写数据
*
*
************************************************************************************************/
int can_write(HHD32F1_CAN_TypeDef *can, enum FRMAT_TYPE type, union UN_CAN_IDENTIFY *Identifier,  uint8_t *data, int len)
{
	int i = 0;
	int count = len / 8;									//发送帧数
	int other = len % 8;									//最后一帧数据长度
	uint8_t start = 2;
	while((CAN_GetStatus(can) & RecSt) == RecSt)OSTimeDly(1);
	while((CAN_GetStatus(can) & TranComplete) != TranComplete)OSTimeDly(1);
	while((CAN_GetStatus(can) & TranBufSt) != TranBufSt)OSTimeDly(1);
	
	/*
		TxDATAINFO 指的是 帧头信息
		bit[31~bit8] 24位预留
		bit[7] FF IDE 帧类型 扩展帧还是标准帧
		bit[6] RTR
		bit[]
	  bit[3:0] DLC 数据字节数
	*/
	can->DATAINFO.TxDATAINFO.TxFRAME.all = 0x08 |(uint8_t)type;		    // tyoe 决定bit7 是不是扩展帧
	can->DATAINFO.TxDATAINFO.TxDATA[0].all = Identifier->data[0];  		//Identifier
	can->DATAINFO.TxDATAINFO.TxDATA[1].all = Identifier->data[1];  		//Identifier
	
	if(type == EXT_FF)
	{
		can->DATAINFO.TxDATAINFO.TxDATA[2].all = Identifier->data[2];  		//Identifier
		can->DATAINFO.TxDATAINFO.TxDATA[3].all = Identifier->data[3];  		//Identifier
		start = 4;
	}
	
	
	for(i = 0; i < count; i++)
	{
		
		can->DATAINFO.TxDATAINFO.TxDATA[start+0].all = data[i*8+0];  //data 1
		can->DATAINFO.TxDATAINFO.TxDATA[start+1].all = data[i*8+1];  //data 2
		can->DATAINFO.TxDATAINFO.TxDATA[start+2].all = data[i*8+2];  //data 3
		can->DATAINFO.TxDATAINFO.TxDATA[start+3].all = data[i*8+3];  //data 4
		can->DATAINFO.TxDATAINFO.TxDATA[start+4].all = data[i*8+4];  //data 5
		can->DATAINFO.TxDATAINFO.TxDATA[start+5].all = data[i*8+5];  //data 5
		can->DATAINFO.TxDATAINFO.TxDATA[start+6].all = data[i*8+6];  //data 7
		can->DATAINFO.TxDATAINFO.TxDATA[start+7].all = data[i*8+7];  //data 8
#ifdef SELF_LOOP		
		CAN_SetCMD(can, SelfRecReq);                             	// 启动发送
#else		
		CAN_SetCMD(can, TransReq);                             		// 启动发送
#endif		
		while((CAN_GetStatus(can) & TranComplete) != TranComplete)OSTimeDly(1);	//等待发送完成
		while((CAN_GetStatus(can) & TranBufSt) != TranBufSt)OSTimeDly(1);	    // 等待发送缓冲被释放
	
	}
	if(other > 0)
	{
		can->DATAINFO.TxDATAINFO.TxFRAME.all = other;    			// 帧长度

		for(i = 0; i < other; i++)
		{
			can->DATAINFO.TxDATAINFO.TxDATA[2+i].all = data[count*8+i];  //data 
		}

		CAN_SetCMD(can, TransReq);                             		// 启动发送
		while((CAN_GetStatus(can) & TranComplete) != TranComplete)OSTimeDly(1);	//等待发送完成
		while((CAN_GetStatus(can) & TranBufSt) != TranBufSt)OSTimeDly(1);	    // 等待发送缓冲被释放
	
	}
	
	return len;
}

int Ext_can_write(HHD32F1_CAN_TypeDef *can, union UN_CAN_IDENTIFY *Identifier,  uint8_t *data, int len)
{
	int i = 0;
	uint8_t start = 2;
	if(len>8)return -1;
	while((CAN_GetStatus(can) & RecSt) == RecSt)OSTimeDly(1);
	while((CAN_GetStatus(can) & TranComplete) != TranComplete)OSTimeDly(1);
	while((CAN_GetStatus(can) & TranBufSt) != TranBufSt)OSTimeDly(1);
	
	/*
		TxDATAINFO 指的是 帧头信息
		bit[31~bit8] 24位预留
		bit[7] FF IDE 帧类型 扩展帧还是标准帧
		bit[6] RTR
		bit[]
	  bit[3:0] DLC 数据字节数
	*/
	can->DATAINFO.TxDATAINFO.TxFRAME.all = len |0x80;		    // tyoe 决定bit7 是不是扩展帧
	//这里似乎不能够 按位操作，可能是因为寄存器
//	can->DATAINFO.TxDATAINFO.TxFRAME.bit.DLC = len;
//	can->DATAINFO.TxDATAINFO.TxFRAME.bit.FF = 1;//扩展帧
//	can->DATAINFO.TxDATAINFO.TxFRAME.bit.RTR = 0;//不是远程帧
	
	can->DATAINFO.TxDATAINFO.TxDATA[0].all = Identifier->data[0];  		//Identifier
	can->DATAINFO.TxDATAINFO.TxDATA[1].all = Identifier->data[1];  		//Identifier
	can->DATAINFO.TxDATAINFO.TxDATA[2].all = Identifier->data[2];  		//Identifier
	can->DATAINFO.TxDATAINFO.TxDATA[3].all = Identifier->data[3];  		//Identifier
	for(i = 0; i < len; i++)
	{
			can->DATAINFO.TxDATAINFO.TxDATA[4+i].all = data[i];  //data 
	}
		CAN_SetCMD(can, TransReq);                             		// 启动发送
		while((CAN_GetStatus(can) & TranComplete) != TranComplete)OSTimeDly(1);	//等待发送完成
		while((CAN_GetStatus(can) & TranBufSt) != TranBufSt)OSTimeDly(1);	    // 等待发送缓冲被释放
	return len;
}
/***********************************************************************************************
*
*
*	CAN 发送一帧数据
*
*
************************************************************************************************/
uint32_t can_timeout = 1000;
int CAN_Transmit(HHD32F1_CAN_TypeDef *can, CanTxMsg *TxMessage)
{
	uint8_t start = 2;
	uint8_t i = 0;
	uint32_t time = 0;
	can->DATAINFO.TxDATAINFO.TxFRAME.all = TxMessage->DLC | TxMessage->IDE |TxMessage->RTR;		// 帧长度
	if(TxMessage->IDE == CAN_Id_Standard)
	{
		can->DATAINFO.TxDATAINFO.TxDATA[0].all = ((TxMessage->StdId << 5) >> 8 ) & 0xFF;   		//Identifier
		can->DATAINFO.TxDATAINFO.TxDATA[1].all = (TxMessage->StdId << 5) &0xE0 ;  		//Identifier
	}
	else
	{
		can->DATAINFO.TxDATAINFO.TxDATA[0].all = ((TxMessage->ExtId << 3) >> 24) & 0xFF;   		//Identifier
		can->DATAINFO.TxDATAINFO.TxDATA[1].all = ((TxMessage->ExtId << 3) >> 16) & 0xFF;  		//Identifier
		can->DATAINFO.TxDATAINFO.TxDATA[2].all = ((TxMessage->ExtId << 3) >> 8) & 0xFF;   		//Identifier
		can->DATAINFO.TxDATAINFO.TxDATA[3].all = (TxMessage->ExtId << 3) & 0xF8;  		//Identifier
		start = 4;
	}
	
	for(i = 0; i < TxMessage->DLC; i++)
	{
		
		can->DATAINFO.TxDATAINFO.TxDATA[start+i].all = TxMessage->Data[i];  //data 1
	}
#ifdef SELF_LOOP		
		CAN_SetCMD(can, SelfRecReq);                             	// 启动发送
#else		
		CAN_SetCMD(can, TransReq);                             		// 启动发送
#endif	
	#if 1
		time = GetTick();	
		while(((CAN_GetStatus(can) & TranComplete) != TranComplete) && 
			((GetTick() - time) < can_timeout) )OSTimeDly(1);	//等待发送完成
		if((GetTick() - time) >= can_timeout)
		{
			return CAN_TxStatus_Failed;
		}
		time = GetTick();	
		while(((CAN_GetStatus(can) & TranBufSt) != TranBufSt) && 
			((GetTick() - time) < can_timeout))OSTimeDly(1);	    // 等待发送缓冲被释放
		
		if((GetTick() - time) >= can_timeout)
		{
			return CAN_TxStatus_Failed;
		}
	#endif
	
	return CAN_TxStatus_Ok;
}

/***********************************************************************************************
*
*
*	CAN 接收一帧数数据
*
*
************************************************************************************************/
int CAN_Receive(HHD32F1_CAN_TypeDef *can, uint8_t FIFONumber, CanRxMsg* RxMessage)
{
	uint8_t start = 2;
	uint8_t i = 0;
	
	RxMessage->DLC = can->DATAINFO.RxDATAINFO.RxFRAME.all & 0x0F;
	RxMessage->IDE = can->DATAINFO.RxDATAINFO.RxFRAME.all & 0x80;
	RxMessage->RTR = can->DATAINFO.RxDATAINFO.RxFRAME.all & 0x40;
	
	if(RxMessage->IDE == CAN_Id_Extended)
	{
		
		RxMessage->ExtId = ((can->DATAINFO.RxDATAINFO.RxDATA[0].all & 0xFF) << 24 ) | 
						   ((can->DATAINFO.RxDATAINFO.RxDATA[1].all & 0xFF) << 16 ) |
						   ((can->DATAINFO.RxDATAINFO.RxDATA[2].all & 0xFF) << 8  ) |
						   ((can->DATAINFO.RxDATAINFO.RxDATA[3].all & 0xF8) << 0);
	
		RxMessage->ExtId = RxMessage->ExtId >> 3;
		
		start = 4;
	
	}
	else
	{
		RxMessage->StdId = ((can->DATAINFO.RxDATAINFO.RxDATA[0].all & 0xFF )<< 8 ) | 
						   ((can->DATAINFO.RxDATAINFO.RxDATA[1].all & 0xE0 )<< 0 ) ;
		
		RxMessage->StdId = RxMessage->StdId >> 5;
	}
	
	for(i = 0; i < RxMessage->DLC; i++)
	{
		RxMessage->Data[i] =  can->DATAINFO.RxDATAINFO.RxDATA[start+i].all;
	}

	RxMessage->FMI = 0;
	
	return CAN_TxStatus_Ok;


}

/***********************************************************************************************
*
*
*	CAN 回写收到的数据
*
*
************************************************************************************************/

void can_write_back(HHD32F1_CAN_TypeDef *can,  uint8_t *data)
{
	int i = 0;
  uint32_t time = 0;
//	while((CAN_GetStatus(can) & RecSt) == RecSt);
//	while((CAN_GetStatus(can) & TranComplete) != TranComplete);
//	while((CAN_GetStatus(can) & TranBufSt) != TranBufSt);
	
	   
	can->DATAINFO.TxDATAINFO.TxFRAME.all = data[0] ;		// 帧长度

	for(i = 0; i < 12; i++)
	{
		can->DATAINFO.TxDATAINFO.TxDATA[i].all = data[i+1];
	
	}
	
#ifdef SELF_LOOP		
		CAN_SetCMD(can, SelfRecReq);                             	// 启动发送
#else		
		CAN_SetCMD(can, TransReq);                             		// 启动发送
#endif		

		#if 1
		time = GetTick();	
		while(((CAN_GetStatus(can) & TranComplete) != TranComplete) && 
			((GetTick() - time) < can_timeout) );	//等待发送完成
		if((GetTick() - time) >= can_timeout)
		{
			return;
		}
		time = GetTick();	
		while(((CAN_GetStatus(can) & TranBufSt) != TranBufSt) && 
			((GetTick() - time) < can_timeout));	    // 等待发送缓冲被释放
		
		if((GetTick() - time) >= can_timeout)
		{
			return;
		}
	#endif
}





