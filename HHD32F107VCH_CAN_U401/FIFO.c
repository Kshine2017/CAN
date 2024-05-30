#include <stdint.h>
#include <stdio.h>
#include "string.h"
#include <stdlib.h>

//#define CAN_FIFO_MEMSIZE (1024)
static uint16_t  CAN1_FIFO_MEMSIZE=0;
static uint8_t*  CAN1_FIFO_RCV=NULL;
static int CAN1_FIFO_RcvLen=0;
static uint8_t  *CAN1_R=NULL; //数据开头位置

//static uint8_t*  CAN1_FIFO_SND=NULL;
//static int CAN1_FIFO_SndLen=0;
//static uint8_t  *CAN1_S=NULL; //数据开头位置

static uint16_t  CAN2_FIFO_MEMSIZE=0;
static uint8_t*  CAN2_FIFO_RCV=NULL;
static int CAN2_FIFO_RcvLen=0;
static uint8_t  *CAN2_R=NULL; //数据开头位置

//static uint8_t*  CAN2_FIFO_SND=NULL;
//static int CAN2_FIFO_SndLen=0;
//static uint8_t  *CAN2_S=NULL; //数据开头位置

extern uint8_t flag_uartinit;
int init_CanFifo(uint8_t port,uint16_t Len)
{
	  uint32_t addr1=0;
    if(port == 1)
		{
			  CAN1_FIFO_RCV = (uint8_t  *)malloc(Len);
		    //CAN1_FIFO_SND = (uint8_t  *)malloc(Len);
			  CAN1_FIFO_MEMSIZE = Len;
			  
			  CAN1_R = CAN1_FIFO_RCV;
		    //CAN1_S = CAN1_FIFO_SND;
        CAN1_FIFO_RcvLen = 0;
			  //CAN1_FIFO_SndLen = 0;
			  //------printf info-----------//
			  addr1 = (uint32_t)CAN1_FIFO_RCV;
			  //addr2 = (uint32_t)CAN1_FIFO_SND;
			  			  
		}
	  else if(port == 2)
		{  
			  CAN2_FIFO_RCV = (uint8_t  *)malloc(Len);
			  //CAN2_FIFO_SND = (uint8_t  *)malloc(Len);
			  CAN2_FIFO_MEMSIZE = Len;
			  
			  CAN2_R = CAN2_FIFO_RCV;
			  //CAN2_S = CAN2_FIFO_SND;
			  CAN2_FIFO_RcvLen = 0;
			  //CAN2_FIFO_SndLen = 0;
			  //------printf info-----------//
			  addr1 = (uint32_t)CAN2_FIFO_RCV;
			  //addr2 = (uint32_t)CAN2_FIFO_SND;
		}
	  else return -1;
		
		if(flag_uartinit)
			printf("-> FIFO%d: RcvAddr=0x%08X,Len=%dbytes\n\r",port,addr1,Len);
		return 0;
}



int loopBuffRead(uint8_t port,uint8_t rcvSnd,uint8_t* data,int len)
{
	uint8_t *fifoAddress=NULL;
	uint8_t *p=NULL;
	int fifoDataLen = 0;
	uint16_t totalLen =0;
	if(len <= 0) return -2;//-2 CAN接收错误；
	if(port == 1)
	{
	    if(rcvSnd == 1)
			{
			    fifoAddress = CAN1_FIFO_RCV;
				  fifoDataLen = CAN1_FIFO_RcvLen;
				  p           = CAN1_R;
				  
			}
//			else if(rcvSnd == 2)
//			{
//			    fifoAddress = CAN1_FIFO_SND;
//				  fifoDataLen = CAN1_FIFO_SndLen;
//				  p           = CAN1_S;
//				  //printf("CAN1_FIFO_SndLen %d \n\r",CAN1_FIFO_SndLen);
//			}
			else
				return -2;
			
			totalLen = CAN1_FIFO_MEMSIZE;
	
	}
	else if(port == 2)
	{
		  if(rcvSnd == 1)
			{
			    fifoAddress = CAN2_FIFO_RCV;
				  fifoDataLen = CAN2_FIFO_RcvLen;
				  p           = CAN2_R;
			}
//			else if(rcvSnd == 2)
//			{
//			    fifoAddress = CAN2_FIFO_SND;
//				  fifoDataLen = CAN2_FIFO_SndLen;
//				  p           = CAN2_S;
//			}
			else
				return -2;
			
			totalLen = CAN2_FIFO_MEMSIZE;
	}
	else
		return -2;
	
	if(p==NULL)return -1;
	//-------------------------------------------------//
	  
	  if(len>fifoDataLen) return -3; //-3 接收缓冲区长度小于CAN报文长度。
    int RR =  fifoAddress+totalLen - p;
	  if(RR <= 0)
		{
			RR  = totalLen;
			p   = fifoAddress;  
	  }
	  if(len < RR)
		{
		    memcpy(data,p,len);
			  p +=len;
		}
		else //len  >=  RR
		{
		    //数据分两边
			  memcpy(data,p,RR);
			  memcpy(data+RR,fifoAddress,len-RR);
			  p = fifoAddress + len-RR;
		}
	  
		fifoDataLen -= len;
	//-------------------------------------------------//
	if(port == 1)
	 {
	    if(rcvSnd == 1)
			{
				  CAN1_FIFO_RcvLen = fifoDataLen;
				  CAN1_R           = p;
			}
//			else if(rcvSnd == 2)
//			{
//				  CAN1_FIFO_SndLen = fifoDataLen;
//				  CAN1_S           = p;
//			}
	}
	else if(port == 2)
	{
		  if(rcvSnd == 1)
			{
				  CAN2_FIFO_RcvLen = fifoDataLen;
				  CAN2_R           = p;
			}
//			else if(rcvSnd == 2)
//			{
//				  CAN2_FIFO_SndLen = fifoDataLen;
//				  CAN2_S           = p;
//			}
	}	

		return 0;
}

int loopBuffWrite(uint8_t port,uint8_t rcvSnd,uint8_t* data,int len)
{
	uint8_t *fifoAddress=NULL;
	uint8_t *p=NULL;
	int fifoDataLen = 0;
	uint16_t totalLen =0;
	if(len <= 0) return -2;
	
	if(port == 1)
	{
	    if(rcvSnd == 1)
			{
				 
			    fifoAddress = CAN1_FIFO_RCV;
				  fifoDataLen = CAN1_FIFO_RcvLen;
				  p           = CAN1_R;
				  
			}
//			else if(rcvSnd == 2)
//			{
//				  
//			    fifoAddress = CAN1_FIFO_SND;
//				  fifoDataLen = CAN1_FIFO_SndLen;
//				  p           = CAN1_S;
//				
//			}
			else
				return -2;
			
			totalLen = CAN1_FIFO_MEMSIZE;
	
	}
	else if(port == 2)
	{
		  if(rcvSnd == 1)
			{
			    fifoAddress = CAN2_FIFO_RCV;
				  fifoDataLen = CAN2_FIFO_RcvLen;
				  p           = CAN2_R;
			}
//			else if(rcvSnd == 2)
//			{
//				 
//			    fifoAddress = CAN2_FIFO_SND;
//				  fifoDataLen = CAN2_FIFO_SndLen;
//				  p           = CAN2_S;
//			}
			else
				return -2;
			totalLen = CAN2_FIFO_MEMSIZE;
	}
	else
		return -2;
	if(p==NULL)return -1;
	//-------------------------------------------------//
    
	  if(len > totalLen)return -3;
	  if(len == totalLen)
		{
		    memcpy(fifoAddress,data,totalLen);
		    fifoDataLen = totalLen;
			  p = fifoAddress;
		}
	  else //len >0  ,len <CAN_FIFO_MEMSIZE
		{
		    int RR = fifoAddress+totalLen - p;
		    if(fifoDataLen <RR )
				{
					  if(len < RR - fifoDataLen)
				    {  
						    memcpy(p+fifoDataLen,data,len);
							  fifoDataLen += len;
						}
						else //len >= RR - fifoDataLen
						{
						    memcpy(p+fifoDataLen,data,RR-fifoDataLen);
							  memcpy(fifoAddress,data+RR-fifoDataLen,len-(RR-fifoDataLen));
						    fifoDataLen += len;
						}
				
				}
				else //fifoDataLen >= RR
				{
				    if(len <= totalLen - fifoDataLen)
						{
						   memcpy(fifoAddress+fifoDataLen-RR,data,len); 
						   fifoDataLen += len;
						}
						else //len > CAN_FIFO_MEMSIZE - fifoDataLen
            {
						    memcpy(fifoAddress+fifoDataLen-RR,data,totalLen - fifoDataLen);
						    memcpy(p,data+totalLen - fifoDataLen,len-(totalLen - fifoDataLen));
							  p+=len-(totalLen - fifoDataLen);
							  fifoDataLen = totalLen;
						}
				}
		}
		//-------------------------------------------------//
	if(port == 1)
	 {
	    if(rcvSnd == 1)
			{
				  CAN1_FIFO_RcvLen = fifoDataLen;
				  CAN1_R           = p;
				  
			}
//			else if(rcvSnd == 2)
//			{
//				  CAN1_FIFO_SndLen = fifoDataLen;
//				  CAN1_S           = p;
//				  //printf("CAN1_FIFO_SndLen %d\n\r",CAN1_FIFO_SndLen);
//				  
//			}
	}
	else if(port == 2)
	{
		  if(rcvSnd == 1)
			{
				  CAN2_FIFO_RcvLen = fifoDataLen;
				  CAN2_R           = p;
				  
			}
//			else if(rcvSnd == 2)
//			{
//				  CAN2_FIFO_SndLen = fifoDataLen;
//				  CAN2_S           = p;
//				  
//			}
	}	
    return 0;
}











