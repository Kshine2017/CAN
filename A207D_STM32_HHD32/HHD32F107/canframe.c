#include "include.h"
#include "typedef.h"

extern void CAN1_SEND_WITH_CMD(uint32_t ID,uint8_t cmd);

/* 定义一个消息队列事件指针 */
OS_EVENT *MessageQ;
/* 定义消息队列的空间大小 */
void *MessageStorage[20];

void init_MSGBOX(void)
{
	/* 创建消息队列, 含20个存储单元(存储的内容是指针不是数据) */
  MessageQ = OSQCreate(&MessageStorage[0], 20);

}
static int8_t cntMalloc =0;

int ThrowIntoMsgQueue(uint32_t ID,uint8_t cmd)
{
	  S_msgbox* msg;
		INT8U  ret=0;
		if(cntMalloc>=20)return -1;
    msg	= ( S_msgbox*)malloc(sizeof(S_msgbox));
    if(msg==NULL) return -2;
		
    msg->CAN_ID = ID;
		msg->CMD = cmd;
		
    ret = OSQPost (MessageQ, (void*)msg);
	  if(ret == OS_ERR_NONE)
		{
			cntMalloc++;
			return 0;
		}
		else if(ret == OS_ERR_Q_FULL)
		{
			free(msg);
			return -3;
		}
		
	return 0;
}

//线程：将消息队列的信息，通过CAN发出
void Report(void *  a)
{
		INT8U err;
    S_msgbox* msg=NULL;

	//刚上电，等待1s，数据刷新
  OSTimeDlyHMSM(0, 0, 1, 0);

	while(1)
	{
		 msg = (S_msgbox *)OSQPend(MessageQ, 0, &err);
		 if( msg!=NULL && err == OS_ERR_NONE)
		 {
				CAN1_SEND_WITH_CMD(msg->CAN_ID,msg->CMD);
			  free(msg);
			  cntMalloc--;
		 }
		 
			msg = NULL;
		 
			OSTimeDlyHMSM(0, 0, 0, 1);
	}
}


















