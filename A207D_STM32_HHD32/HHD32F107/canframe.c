#include "include.h"
#include "typedef.h"

extern void CAN1_SEND_WITH_CMD(uint32_t ID,uint8_t cmd);

/* ����һ����Ϣ�����¼�ָ�� */
OS_EVENT *MessageQ;
/* ������Ϣ���еĿռ��С */
void *MessageStorage[20];

void init_MSGBOX(void)
{
	/* ������Ϣ����, ��20���洢��Ԫ(�洢��������ָ�벻������) */
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

//�̣߳�����Ϣ���е���Ϣ��ͨ��CAN����
void Report(void *  a)
{
		INT8U err;
    S_msgbox* msg=NULL;

	//���ϵ磬�ȴ�1s������ˢ��
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


















