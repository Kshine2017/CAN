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
ģʽ���ƼĴ���(MOD)����ַ��0x000h
bit0 RM ��λģʽ       1 ��λģʽ���κ��յ��ͷ��͵���Ϣ�ᱻ����
bit1 LOM ����ģʽ      1 Listen Only ģʽ                        0 ����ģʽ��
bit2 STM �Բ�ģʽ      1 �Բ�ģʽʹ��  
bit3 AFM ���ܹ���ģʽ  1 ���ݽ��ܲ��� 4-byte ������              0 ���ݽ��ܲ��� 2 �� shoter ������
bit4 SM ����ģʽ       1 CAN ��������������ģʽ                  0 ����ģʽ
*/
void CAN_SetMode(HHD32F1_CAN_TypeDef *can, uint32_t mode)
{
    can->MOD.all = mode;

    return;
}

/*
	״̬�Ĵ���(SR)����ַ��0x008h
	bit0 RBS ���� buffer״̬          1 ���� buffer�ѱ��ͷţ�CPU ���Զ�ȡ�����½��ܵ�����Ϣ    0 ���� buffer Ϊ�գ�����Ϣ�ɶ�
	bit1 DOS ���ݹ���״̬             1 ���ݹ��أ����ڿռ䲻�㵼������Ϣ��ʧ                   0 �����ݹ��أ����������ڿռ䲻�㵼������Ϣ��ʧ
	bit2 TBS ���� buffer״̬          1 ���� buffer �ѱ��ͷţ�CPU ����д������Ϣ               0 ���� buffer ��������buffer ����Ϣ��׼��������
	bit3 TCS �������״̬             1 ���һ�������ѱ��������                               0 ���һ������δ���������
	bit4 RS ����״̬                  1 CAN ���������ڽ�����Ϣ������                           0 ����Ϣ�ڽ�����
	bit5 TS ����״̬                  1 CAN ���������ڷ�����Ϣ������                           0 ����Ϣ�ڷ�����
	bit6 ES ����״̬                  1 ����һ�ִ���������ﵽ EWL�Ĵ������õĴ�����           0 ���ִ����������δ�ﵽ������
	bit7 BS ����״̬                  1 CAN ������������ off ̬��δ�������߽���                0 CAN ���Ʋ������߽���
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
	 Command ����Ĵ���(CMR)����ַ��0x004h
	bit0 TR ��������        ���� 1 ����һ����Ϣ
	bit1 AT ���䶪��        ���� 1 ����ȡ����һ�ʴ�������
	bit2 RRB �ͷŽ���buffer ���� 1 �����ͷŽ��� buffer
	bit3 CDO �����ݹ���     ���� 1 ����������ݹ��ر�־
	bit4 SRR �Խ�������     ͬʱ���ͺͽ�����Ϣʱ���� bit �� 1
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
 �жϼĴ���(IR)  ��ַ��0x00Ch  ֻ���Ĵ���
  �������� 1 λΪ 1 ʱ���������� CPU �����ж�����
bit0 RI  �����ж�     ���н����жϷ���ʱ Ϊ 1
bit1 TI  �����ж�     ���д����жϷ���ʱ Ϊ 1
bit2 EI  ���󱨾��ж� ���д��󱨾�����ʱ Ϊ 1
bit3 DOI ���ݹ����ж� �������ݹ��ط���ʱ Ϊ 1
bit4 WUI �����ж�     �������ߴ�����ʱ Ϊ 1
bit5 EPI ���󱻶��ж� ���д��󱻶�����ʱ Ϊ 1
bit6 ALI �ٲö�ʧ�ж� �����ٲö�ʧ����ʱ Ϊ 1
bit7 BEI ���ߴ����ж� �������ߴ�����ʱ Ϊ 1

 �ж�ʹ�ܼĴ���(IER)  ��ַ��0x010h
bit0 RIE  �����ж�ʹ��
bit1 TIE  �����ж�ʹ��
bit2 EIE  ���󱨾��ж�ʹ��
bit3 DOIE ���ݹ����ж�ʹ��
bit4 WUIE �����ж�ʹ��
bit5 EPIE ���󱻶��ж�ʹ��
bit6 ALIE �ٲö�ʧ�ж�ʹ��
bit7 BEIE ���ߴ����ж�ʹ��


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
* CAN �ӿڳ�ʼ��
*
*
* CANʹ�õ�ʱ��CAN_CLK��APB1ʱ�ӣ���ǰAPB1Ϊ��Ƶʱ��;
* �����ʻ��ڡ�ԭ��ʱ�䡱���㣬�� һ����ԭ��ʱ�䡱������TQ = CAN_CLK/(2*(1+can->BTR0.bit.BRP);
* ���� APB1 = 60M��can->BTR0.bit.BRP = 3; 1/TQ = (60/2) / ((3+1)) = 7.5M
* ������ BIT Period ��TQ��Ϊ��λ�����㷽�� Tsyncseg + Ttseg1 + Ttseg2 + 2
* Tsyncseg = 1��
* ���� Ttseg1 = 7��Ttseg2 = 5��
  ������  = 7.5M/��3+7+5��= 500Kbps
********************************************************************************/
/*
ģʽ���ƼĴ���(MOD)����ַ��0x000h
bit0 RM ��λģʽ       1 ��λģʽ���κ��յ��ͷ��͵���Ϣ�ᱻ����
bit1 LOM ����ģʽ      1 Listen Only ģʽ                        0 ����ģʽ��
bit2 STM �Բ�ģʽ      1 �Բ�ģʽʹ��  
bit3 AFM ���ܹ���ģʽ  1 ���ݽ��ܲ��� 4-byte ��������32λ��������              0 ���ݽ��ܲ��� 2 �� shoter ��������16λ��������
bit4 SM ����ģʽ       1 CAN ��������������ģʽ                  0 ����ģʽ
*/
void can_init(HHD32F1_CAN_TypeDef *can, EN_BAUD baud, uint32_t filterID, uint32_t mask)
{
	if(can == CAN1)
	{
		// ģ��ʱ��	
		SYSCON->SYSAPB1CLKCTRL.bit.CAN1CLK = 1;
		SYSCON->APB1RESET.bit.CAN1RSTN=1;
		SYSCON->APB1RESET.bit.CAN1RSTN=0;
		//	����
	
		HHD_AFIO -> PA11 = CAN1_RX_PA11;  			//CAN1 RX config
		HHD_AFIO -> PA12 = CAN1_TX_PA12;  			//CAN1 TX config
		// ʹģ���������ģʽ	
	}
	else if(can == CAN2)
	{
		SYSCON->SYSAPB1CLKCTRL.bit.CAN2CLK = 1;
		SYSCON->APB1RESET.bit.CAN2RSTN=1;
		SYSCON->APB1RESET.bit.CAN2RSTN=0;
		HHD_AFIO -> PB5 = CAN2_RX_PB5;  			//CAN2 RX config
		HHD_AFIO -> PB6 = CAN2_TX_PB6;  			//CAN2 TX config		
	}
	
	//1. ���븴λģʽ
	do{
		CAN_SetMode(can, ResetMode);
	}while(can->MOD.bit.RM !=  ResetMode);//�ȴ��ɹ����븴λģʽ
	
  //2. ���ò�����	
	/*
	BIT Period= Tsyncseg + Ttseg1 + Ttseg2 = 1+(7+1)+(5+1) = 15
	*/
	//TQ = 2x(1/60MHz)x2 = (1/15)us

	
	/*
	����ʱ��Ĵ��� 0(BTR0)  ��ַ��0x018h
	bit5:0 BRP �����ʷ�Ƶ�� ��ֵΪ ʱ��ԭ�� TQ ��Ӧ CAN ����ʱ�ӵĸ���
	bit7:6 SJW ͬ����Ծ��� ��ֵ������ǣ�������̻��ӳ��� TQ ����
	*/
	/*
	����ʱ��Ĵ��� 1(BTR1)  ��ַ��0x01Ch
	bit3:0 TSEG1 Bit���ڳ���1    Tseg1=TQx(4TSEG2.2+2TSEG2.1+ TSEG2.0+1)
	bit6:4 TSEG2 Bit���ڳ���2    Tseg2=TQx(8TSEG1.3+4TSEG1.2+ 2TSEG1.1+TSEG1.0+1)
	bit7   SAM                   1 ���߲���3 ��    0 ���߲���1 ��
	*/
 	can->BTR0.bit.SJW = 0x00; //ͬ����Ծ��� ������̻��ӳ��� TQ =0
	if(baud == CAN_BAUD_1M)	
	{
	
		can->BTR0.bit.BRP = 1;   //�����ʷ�Ƶ��
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
	else  //����������   ����Ҫ����
	{
		can->BTR0.bit.BRP = 5;   
		can->BTR1.bit.TSEG1 = 14;
		can->BTR1.bit.TSEG2 = 3;
	}
 	can->BTR1.bit.SAM 	= 0;   //0 ���߲���1 ��
	
	
	//3. �����˲���
	can->MOD.bit.AFM 	= 0x01;  //1 ���ݽ��ܲ��� 4-byte ������(32λ������)
														 //0 ���ݽ��ܲ��� 2 �� shoter ��������2��16λ��������
// ���ý���֡���ˣ����Խ����κα�ʶ��
	
	if(filterID <= 0x7FF)
	{
		/*
		ACR/AMR����������:
		ACR����ָ��CAN ID�������й�עλ�ͺ���λ��
		AMR���ڹ涨ACR����Щ������λ��Ч����Щ��Ч.AMR��0������Чλ��1������Чλ��
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

// �������ж�	
	CAN_EnableInt(can, RIE); //���������ж�

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
*	CAN д����
*
*
************************************************************************************************/
int can_write(HHD32F1_CAN_TypeDef *can, enum FRMAT_TYPE type, union UN_CAN_IDENTIFY *Identifier,  uint8_t *data, int len)
{
	int i = 0;
	int count = len / 8;									//����֡��
	int other = len % 8;									//���һ֡���ݳ���
	uint8_t start = 2;
	while((CAN_GetStatus(can) & RecSt) == RecSt)OSTimeDly(1);
	while((CAN_GetStatus(can) & TranComplete) != TranComplete)OSTimeDly(1);
	while((CAN_GetStatus(can) & TranBufSt) != TranBufSt)OSTimeDly(1);
	
	/*
		TxDATAINFO ָ���� ֡ͷ��Ϣ
		bit[31~bit8] 24λԤ��
		bit[7] FF IDE ֡���� ��չ֡���Ǳ�׼֡
		bit[6] RTR
		bit[]
	  bit[3:0] DLC �����ֽ���
	*/
	can->DATAINFO.TxDATAINFO.TxFRAME.all = 0x08 |(uint8_t)type;		    // tyoe ����bit7 �ǲ�����չ֡
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
		CAN_SetCMD(can, SelfRecReq);                             	// ��������
#else		
		CAN_SetCMD(can, TransReq);                             		// ��������
#endif		
		while((CAN_GetStatus(can) & TranComplete) != TranComplete)OSTimeDly(1);	//�ȴ��������
		while((CAN_GetStatus(can) & TranBufSt) != TranBufSt)OSTimeDly(1);	    // �ȴ����ͻ��屻�ͷ�
	
	}
	if(other > 0)
	{
		can->DATAINFO.TxDATAINFO.TxFRAME.all = other;    			// ֡����

		for(i = 0; i < other; i++)
		{
			can->DATAINFO.TxDATAINFO.TxDATA[2+i].all = data[count*8+i];  //data 
		}

		CAN_SetCMD(can, TransReq);                             		// ��������
		while((CAN_GetStatus(can) & TranComplete) != TranComplete)OSTimeDly(1);	//�ȴ��������
		while((CAN_GetStatus(can) & TranBufSt) != TranBufSt)OSTimeDly(1);	    // �ȴ����ͻ��屻�ͷ�
	
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
		TxDATAINFO ָ���� ֡ͷ��Ϣ
		bit[31~bit8] 24λԤ��
		bit[7] FF IDE ֡���� ��չ֡���Ǳ�׼֡
		bit[6] RTR
		bit[]
	  bit[3:0] DLC �����ֽ���
	*/
	can->DATAINFO.TxDATAINFO.TxFRAME.all = len |0x80;		    // tyoe ����bit7 �ǲ�����չ֡
	//�����ƺ����ܹ� ��λ��������������Ϊ�Ĵ���
//	can->DATAINFO.TxDATAINFO.TxFRAME.bit.DLC = len;
//	can->DATAINFO.TxDATAINFO.TxFRAME.bit.FF = 1;//��չ֡
//	can->DATAINFO.TxDATAINFO.TxFRAME.bit.RTR = 0;//����Զ��֡
	
	can->DATAINFO.TxDATAINFO.TxDATA[0].all = Identifier->data[0];  		//Identifier
	can->DATAINFO.TxDATAINFO.TxDATA[1].all = Identifier->data[1];  		//Identifier
	can->DATAINFO.TxDATAINFO.TxDATA[2].all = Identifier->data[2];  		//Identifier
	can->DATAINFO.TxDATAINFO.TxDATA[3].all = Identifier->data[3];  		//Identifier
	for(i = 0; i < len; i++)
	{
			can->DATAINFO.TxDATAINFO.TxDATA[4+i].all = data[i];  //data 
	}
		CAN_SetCMD(can, TransReq);                             		// ��������
		while((CAN_GetStatus(can) & TranComplete) != TranComplete)OSTimeDly(1);	//�ȴ��������
		while((CAN_GetStatus(can) & TranBufSt) != TranBufSt)OSTimeDly(1);	    // �ȴ����ͻ��屻�ͷ�
	return len;
}
/***********************************************************************************************
*
*
*	CAN ����һ֡����
*
*
************************************************************************************************/
uint32_t can_timeout = 1000;
int CAN_Transmit(HHD32F1_CAN_TypeDef *can, CanTxMsg *TxMessage)
{
	uint8_t start = 2;
	uint8_t i = 0;
	uint32_t time = 0;
	can->DATAINFO.TxDATAINFO.TxFRAME.all = TxMessage->DLC | TxMessage->IDE |TxMessage->RTR;		// ֡����
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
		CAN_SetCMD(can, SelfRecReq);                             	// ��������
#else		
		CAN_SetCMD(can, TransReq);                             		// ��������
#endif	
	#if 1
		time = GetTick();	
		while(((CAN_GetStatus(can) & TranComplete) != TranComplete) && 
			((GetTick() - time) < can_timeout) )OSTimeDly(1);	//�ȴ��������
		if((GetTick() - time) >= can_timeout)
		{
			return CAN_TxStatus_Failed;
		}
		time = GetTick();	
		while(((CAN_GetStatus(can) & TranBufSt) != TranBufSt) && 
			((GetTick() - time) < can_timeout))OSTimeDly(1);	    // �ȴ����ͻ��屻�ͷ�
		
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
*	CAN ����һ֡������
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
*	CAN ��д�յ�������
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
	
	   
	can->DATAINFO.TxDATAINFO.TxFRAME.all = data[0] ;		// ֡����

	for(i = 0; i < 12; i++)
	{
		can->DATAINFO.TxDATAINFO.TxDATA[i].all = data[i+1];
	
	}
	
#ifdef SELF_LOOP		
		CAN_SetCMD(can, SelfRecReq);                             	// ��������
#else		
		CAN_SetCMD(can, TransReq);                             		// ��������
#endif		

		#if 1
		time = GetTick();	
		while(((CAN_GetStatus(can) & TranComplete) != TranComplete) && 
			((GetTick() - time) < can_timeout) );	//�ȴ��������
		if((GetTick() - time) >= can_timeout)
		{
			return;
		}
		time = GetTick();	
		while(((CAN_GetStatus(can) & TranBufSt) != TranBufSt) && 
			((GetTick() - time) < can_timeout));	    // �ȴ����ͻ��屻�ͷ�
		
		if((GetTick() - time) >= can_timeout)
		{
			return;
		}
	#endif
}





