#include "include.h"
#include "typedef.h"

CanRxMsg Can1RxFrame;

/*
解帧
*/
void unframe(void)
{
	uint32_t getID =0x0;

	if(Can1RxFrame.IDE == CAN_ID_EXT)
		getID = Can1RxFrame.ExtId;
	else if(Can1RxFrame.IDE == CAN_ID_STD)
		getID = Can1RxFrame.StdId;
	
	if(getID == PC_ID)//上位机
	{
		switch(Can1RxFrame.Data[0])//命令码
		{
			case ...
		}
	}
	else if(getID == MUBAN_ID2)//母板BB 子板AA
	{
			switch(Can1RxFrame.Data[0])
			{	
				case ...
			}
	}
	

}

//CAN1通道接收，并（在CAN1通道）回复状态信息
int RxMsgFromCan1(void)
{		
	CAN_Receive(CAN1, 8, &Can1RxFrame);//读取数据
	if(Can1RxFrame.DLC != 0)
	{
				unframe();
	}
	
	return 0;
}



//CAN1 发送
void CAN1_SEND_WITH_CMD(uint32_t ID,uint8_t cmd)
{
		union UN_CAN_IDENTIFY Identifier;
	  uint8_t len=8;
	  uint8_t CC[8]={0};
		
		//收到什么类型的帧，就发送什么类型的帧
		if(Can1RxFrame.IDE == CAN_ID_EXT)
		{
			//ID = 0x12345678;
			Identifier.ext_id.ID_bit0_1 = (ID)&0x3;
			Identifier.ext_id.ID_bit2_4 = (ID>>2)&0x7;
			Identifier.ext_id.ID_bit5_12 = (ID>>5)&0xff;
			Identifier.ext_id.ID_bit13_15 = (ID>>13)&0x7;
			Identifier.ext_id.ID_bit16_20 = (ID>>16)&0x1f;
			Identifier.ext_id.ID_bit21_28 = (ID>>21)&0xff;
		}
		else if(Can1RxFrame.IDE == CAN_ID_STD)
		{
		  Identifier.std_id.id_high=ID>>3;  //高8位
	    Identifier.std_id.id_low =ID&0x07;//低3位
		
		}

		uint16_t data=0;
		CC[0] = cmd;//命令码
		CC[1]=  0x02;//测试故障
		switch(cmd)
		{
				case ...:
				{
				    len = 7;
				    CC[1] = ...;
				    CC[2] = ...;
					CC[3] = ...;
					CC[4] = ...;
				    CC[5] = ...;
					CC[6] = ...;
				    break;
				}
				default:
					return;
		}
		if(Can1RxFrame.IDE == CAN_ID_EXT)
		  Ext_can_write(CAN1,&Identifier , CC,len);//扩展帧发送
		else if(Can1RxFrame.IDE == CAN_ID_STD)
	    can_write(CAN1, STD_FF,&Identifier , CC,len);
}


