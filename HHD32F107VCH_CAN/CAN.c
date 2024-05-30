#include <stdint.h>
#include "hhd_can.h"
#include <string.h>
#include <stdio.h>

extern int init_CanFifo (uint8_t port,uint16_t Len);
extern int loopBuffWrite(uint8_t port,uint8_t rcvSnd,uint8_t* data,int len);
extern int loopBuffRead (uint8_t port,uint8_t rcvSnd,uint8_t* data,int len);

//1. ================== 初始化  ==================
//1. ================== 初始化  ==================
//1. ================== 初始化  ==================
static EN_BAUD CAN_BAUDRATE = CAN_BAUD_1M;

static uint32_t CAN_SOURCE_ID =0x01;
void ZL_SetCanSrcID(uint32_t ID)
{
    CAN_SOURCE_ID = ID;
}

//-------------------------------
#define PROJ_CAN_SRC_NODE		(0x01)
CanRxMsg Can1RxFrame;
CanRxMsg Can2RxFrame;
static void HHD_CAN1_Init(void)
{
    can_init(CAN1, CAN_BAUDRATE, PROJ_CAN_SRC_NODE, 0x0000000);//	can_init(CAN1, CAN_BAUD_1M, 0x7ff, 0x00);
	
	memset(&Can1RxFrame, 0, sizeof(CanRxMsg));
	
	Can1RxFrame.IDE = CAN_ID_STD;   //默认先以标准帧形式发送数据
	
	Can1RxFrame.RTR = CAN_RTR_DATA; //默认是数据帧
}

static void HHD_CAN2_Init(void)
{
	can_init(CAN2, CAN_BAUDRATE, PROJ_CAN_SRC_NODE, 0x0000000);
    
	memset(&Can1RxFrame, 0, sizeof(CanRxMsg));
	
	Can2RxFrame.IDE = CAN_ID_STD;   //默认先以标准帧形式发送数据
	
	Can2RxFrame.RTR = CAN_RTR_DATA; //默认是数据帧
	
}

typedef struct{
	uint8_t init1:1;//CAN1是否初始化
	uint8_t init2:1;//CAN2是否初始化
	uint8_t res:6;
}S_CANSTATUS;
S_CANSTATUS can_status;


extern void TIM4Init(void);
extern uint8_t flag_uartinit;

#define ERROR_HARDWARE (-1)
#define ERROR_HEAP     (-2)
#define ERROR_HANDSHAKE_SND  (-3)
#define ERROR_HANDSHAKE_RCV  (-4)
#define ERROR_CONTENT  (-5)

int ZL_CanInit(uint8_t port)
{
	  int ret=0;
	  init_CanFifo(port,1024);
	  
	  //TIM4Init();
	 CAN_BAUDRATE = CAN_BAUD_200K;
    //1. init hardware. (CAN SPI ...)
	  if(port == 1)
		{
	      HHD_CAN1_Init();
			  can_status.init1=1;
		}
	  else if(port == 2)
		{
		    HHD_CAN2_Init();
			  can_status.init2=1;
		}
	  else return ERROR_HARDWARE;
		
		if(flag_uartinit)
		{
			printf("-> CAN%d: ",port);
                 if(CAN_BAUDRATE == CAN_BAUD_1M)printf("1000K ");
			else if(CAN_BAUDRATE == CAN_BAUD_500K)printf("500K ");
            else if(CAN_BAUDRATE == CAN_BAUD_250K)printf("250K ");
            else if(CAN_BAUDRATE == CAN_BAUD_200K)printf("200K ");
			printf("\n\r");

		}
		
    return ret;
}


//2. ================== 发送================== 
//2. ================== 发送================== 
//2. ================== 发送================== 
int ZL_CANX_Send8Byte(uint8_t port,uint32_t ID_Code ,uint8_t *pData,uint16_t Len)
{
	  uint32_t ID = ID_Code;
		union UN_CAN_IDENTIFY Identifier;
	  HHD32F1_CAN_TypeDef *CANx;	
	  uint8_t IDE=0;
	  if(port==1)
		{
		    CANx = CAN1;
			  IDE  = Can1RxFrame.IDE;
		}
		else if(port==2)
		{
		    CANx = CAN2;
			  IDE  = Can2RxFrame.IDE;
		}
		else 
			return -1;
		int ret =0;
		Identifier.std_id.id_high=ID>>3;  //高8位
	    Identifier.std_id.id_low =ID&0x07;//低3位
			
		ret= can_write(CANx, STD_FF,&Identifier , pData,Len);

		return ret;    
}



//3. ================== 接收================== 
//3. ================== 接收================== 
//3. ================== 接收================== 
static void HHD_CAN1_ReleaseRcvBuff(void)
{
    CAN_SetCMD(CAN1, ReleaseRecBuf); // release recieve buffer
}

static void HHD_CAN2_ReleaseRcvBuff(void)
{
    CAN_SetCMD(CAN2, ReleaseRecBuf); // release recieve buffer
}

static uint8_t data[16]={0};
uint32_t ZL_CanFIFORead16Bytes(uint8_t port)
{
		//1 -rcv   2- snd
	  while(1)
	  {
				while(1)
				{
					if(loopBuffRead(port,1,data,1)<0)
					{
						return 0;
					}
					if(data[0]==0xAB)
					{
						break;
					}
				}
				
				if(loopBuffRead(port,1,data+1,1)<0)
				{
						return 0;
				}
				if(data[1]==0xCD)
				{
						break;
				}
	  }
		if(loopBuffRead(port,1,data+2,14)<0)
		{
				return 0;
	  }
    if(data[14]==0xFD&&data[15]==0xFD)
			return (uint32_t)data;
		return 0;
}
uint32_t testCAN = 0;
//中断服务函数，接收到CAN数据，写入FIFO
//中断服务函数，接收到CAN数据，写入FIFO
//中断服务函数，接收到CAN数据，写入FIFO


extern uint8_t CurrentStatus;
void CAN1_IRQHandler(void)
{
    
    CAN_Receive(CAN1, 8, &Can1RxFrame);	
    testCAN = Can1RxFrame.StdId;
   if( CurrentStatus != 3 && Can1RxFrame.IDE == CAN_ID_STD)
    {
        if(Can1RxFrame.StdId == 0x10 )
        {
            if(Can1RxFrame.Data[0] == 0x4D)
            {
            uint8_t  pp[16]={0};
            pp[0]=0xAB;
            pp[1]=0xCD;
							
            memcpy(pp+2,(uint8_t*)&Can1RxFrame.StdId,4);
            memcpy(pp+6,Can1RxFrame.Data,8);
							
            pp[14]=0xFD;
            pp[15]=0xFD;
							 
            if(loopBuffWrite(1,1,pp,16)!=0) //CAN1 recv
            {
                printf("Write FIFO1 error");
            }
        }
        }	
    }
    HHD_CAN1_ReleaseRcvBuff();
}

void CAN2_IRQHandler(void)
{
    CAN_Receive(CAN2, 8, &Can2RxFrame);	
    testCAN = Can2RxFrame.StdId;
    if(CurrentStatus != 3 && Can2RxFrame.IDE == CAN_ID_STD)
    {
        testCAN = Can2RxFrame.StdId;
        if(Can2RxFrame.StdId == 0x10)
        {
            uint8_t  pp[16]={0};
            pp[0]=0xAB;
            pp[1]=0xCD;
							
            memcpy(pp+2,(uint8_t*)&Can2RxFrame.StdId,4);
            memcpy(pp+6,Can2RxFrame.Data,8);
							
            pp[14]=0xFD;
            pp[15]=0xFD;
							
            if(loopBuffWrite(2,1,pp,16)!=0) //CAN1 recv
            {
                printf("Write FIFO1 error");
            }
        }
    }
    HHD_CAN2_ReleaseRcvBuff();
}




