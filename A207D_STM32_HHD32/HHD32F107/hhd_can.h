/*****************************************************************************
 *   hhf_can.h:  Header file for  HHD32F1xx Family Microprocessors
 *
 *   Copyright(C) 2015,  tech. Ltd.
 *   All rights reserved.
 *
 *   History
 *   2015  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __HHD_CAN_H__
#define __HHD_CAN_H__
#include "hhd32f1xx.h"


#define ENABLECAN1CLK  (SYSCON->SYSAPB1CLKCTRL.all |= (1<<25))
#define ENABLECAN2CLK  (SYSCON->SYSAPB1CLKCTRL.all |= (1<<26))

#define DISABLECAN1CLK (SYSCON->SYSAPB1CLKCTRL.all &= ~(1<<25))
#define DISABLECAN2CLK (SYSCON->SYSAPB1CLKCTRL.all &= ~(1<<26))

#define CAN1_TX_PA12 0x15
#define CAN1_TX_PB9  0x15
#define CAN1_TX_PD1  0X15
#define CAN1_RX_PA11 0x15
#define CAN1_RX_PB8  0x15
#define CAN1_RX_PD0  0x15

#define CAN2_TX_PB6  0x16
#define CAN2_RX_PB5  0x16
#define CAN2_TX_PB13 0x13
#define CAN2_RX_PB12 0x13

//Mode bit
#define SleepMode				0x10
#define ACCFMode				0x08
#define SelfTestMode			0x04
#define ListenOnlyMode			0x02
#define ResetMode				0x01
#define ActiveMode				0x00

//Interrupt enable bit
#define BEIE 0x80
#define ALIE 0x40
#define EPIE 0x20
#define WUIE 0x10
#define DOIE 0x08
#define EIE  0x04
#define TIE  0x02
#define RIE  0x01


//CMD bit
#define SelfRecReq					0x10
#define	ClearDataOverrun			0x08
#define	ReleaseRecBuf				0x04
#define AbortTrans					0x02
#define	TransReq					0x01


//Status Register bit
#define BusSt					0x80
#define	ErrorSt					0x40
#define	TranSt					0x20
#define	RecSt					0x10
#define	TranComplete			0x08
#define	TranBufSt				0x04
#define	DataOvSt				0x02
#define	RecBufSt				0x01
#if 1
//--CAN Registers BITS Field---------------------------------------------------------------------------------------
struct CAN_MOD_BITS
{
    uint32_t RM: 1;				 // sleep mode
    uint32_t LOM: 1;             // Acceptance filter mode
    uint32_t STM: 1;             // Self test mode
    uint32_t AFM: 1;             // Listen only mode
    uint32_t SM: 1;              // Reset mode
    uint32_t RSVD: 27;			 // 31:5 reserved
};
union CAN_MOD_REG
{
    uint32_t					all;
    struct  CAN_MOD_BITS		bit;
};

struct CAN_CMR_BITS
{
    uint32_t TR: 1;               //Transmission Request
    uint32_t AT: 1;               //Abort Transmission
    uint32_t RRB: 1;              //Release Receive Buffer
    uint32_t CDO: 1;              //Clear Data Overrun
    uint32_t SRR: 1;              //Self Reception Request
    uint32_t RSVD: 27;            //31:5 reserved
};
union CAN_CMR_REG
{
    uint32_t					all;
    struct  CAN_CMR_BITS		bit;
};

struct CAN_SR_BITS
{
    uint32_t RBS: 1;             //Receive Buffer Staus
    uint32_t DOS: 1;             //Data Overrun Status
    uint32_t TBS: 1;             //Transmit Buffer Status
    uint32_t TCS: 1;             //Transmission Complete Status
    uint32_t RS: 1;              //Receive Status
    uint32_t TS: 1;              //Transmit Status
    uint32_t ES: 1;              //Error Status
    uint32_t BS: 1;              //Bus Status
    uint32_t RSVD: 24;           //31:8 reserved
};
union CAN_SR_REG
{
    uint32_t					all;
    struct  CAN_SR_BITS			bit;
};

struct CAN_IR_BITS
{
    uint32_t RI: 1;             //Receive Interrupt
    uint32_t TI: 1;             //Transmit Interrupt
    uint32_t EI: 1;             //Error Warning Interrupt
    uint32_t DOI: 1;            //Data Overrun Interrupt
    uint32_t WUI: 1;            //Wake-Up Interrupt
    uint32_t EPI: 1;            //Error Passive Interrupt
    uint32_t ALI: 1;            //Arbitration Lost Interrupt
    uint32_t BEI: 1;            //Bus Error Interrupt
    uint32_t RSVD: 24;          //31:8 reserved
};
union CAN_IR_REG
{
    uint32_t					all;
    struct  CAN_IR_BITS		bit;
};

struct CAN_IER_BITS
{
    uint32_t RIEN: 1;             //Receive Interrupt Enable
    uint32_t TIEN: 1;             //Transmit Interrupt Enable
    uint32_t EIEN: 1;             //Error Warning Interrupt Enable
    uint32_t DOIEN: 1;            //Data Overrun Interrupt Enable
    uint32_t WUIEN: 1;            //Wake-Up Interrupt Enable
    uint32_t EPIEN: 1;            //Error Passive Interrupt Enable
    uint32_t ALIEN: 1;            //Arbitration Lost Interrupt Enable
    uint32_t BEIEN: 1;            //Bus Error Interrupt Enable
    uint32_t RSVD: 24;          //31:8 reserved
};
union CAN_IER_REG
{
    uint32_t					all;
    struct  CAN_IER_BITS		bit;
};

struct CAN_BTR0_BITS
{
    uint32_t BRP: 6;             //Baud Rate Prescaler (BRP).
    uint32_t SJW: 2;             //Synchronization Jump Width (SJW)
    uint32_t RSVD: 24;	          //31:8 reserved
};
union CAN_BTR0_REG
{
    uint32_t					all;
    struct  CAN_BTR0_BITS		bit;
};

struct CAN_BTR1_BITS
{
    uint32_t TSEG1: 4;             //
    uint32_t TSEG2: 3;             //
    uint32_t SAM: 1;               //Sampling
    uint32_t RSVD: 24;	            //31:8 reserved
};
union CAN_BTR1_REG
{
    uint32_t					all;
    struct  CAN_BTR1_BITS		bit;
};
struct CAN_OCR_BITS
{
    uint32_t OCMODE: 2;             //Selection of two possible output driver configurations:
    uint32_t RSVD: 30;      					//31:3 reserved
};
union CAN_OCR_REG
{
    uint32_t					all;
    struct  CAN_OCR_BITS		bit;
};

struct CAN_ALC_BITS
{
uint32_t ALC:
    5;               //This read-only register records the bit position at which arbitration was lost
    uint32_t RSVD: 27;      					//31:5 reserved
};
union CAN_ALC_REG
{
    uint32_t					all;
    struct  CAN_ALC_BITS		bit;
};

struct CAN_ECC_BITS
{
    uint32_t SEGMENTCODE: 5;           //Segment coede err
uint32_t DIRECTION:
    1;             //If ??1?¡¥, the error occurred during reception. If ??0?¡¥, the error occurred during transmission.
uint32_t ERRORCODE:
    2;             //00:Bit erro, 01:Bit Form erro, 10:Stuff error 11:Some other type of error
    uint32_t RSVD: 24;	                //31:8 reserved
};
union CAN_ECC_REG
{
    uint32_t					all;
    struct  CAN_ECC_BITS		bit;
};

struct CAN_EWLR_BITS
{
uint32_t EWL:
    8;                  // Defines the number of errors after which an Error Warning Interrupt should be generated
    uint32_t RSVD: 24;
};
union CAN_EWLR_REG
{
    uint32_t					all;
    struct  CAN_EWLR_BITS		bit;
};

struct CAN_RXERR_BITS
{
    uint32_t RXERR: 8;                  // Receive error counter
    uint32_t RSVD: 24;
};
union CAN_RXERR_REG
{
    uint32_t					all;
    struct  CAN_RXERR_BITS		bit;
};

struct CAN_TXERR_BITS
{
    uint32_t TXERR_: 8;                  // Transmit error counter
    uint32_t RSVD: 24;
};
union CAN_TXERR_REG
{
    uint32_t					all;
    struct  CAN_RXERR_BITS		bit;
};

struct CAN_FRAMEINFO_BITS
{
	uint32_t DLC: 4;                   //Specify the number of data bytes included in the message being sent.
    uint32_t RSVD0: 2;             //4:5 reserved
    uint32_t RTR: 1;               //Remote transmission request.
    uint32_t FF: 1;                //The FF bit selects the type of frame format to be transmitted
    uint32_t RSVD1: 24;	           //31:8 reserved
};
union  CAN_FRAMEINFO_REG
{
    uint32_t					        all;
    struct  CAN_FRAMEINFO_BITS			bit;
};





struct CAN_DATAINFO_BITS
{
    uint32_t DATA: 8;              //
    uint32_t RSVD: 24;	          //31:8 reserved
};
union CAN_DATAINFO_REG
{
    uint32_t						all;
    struct  CAN_DATAINFO_BITS		bit;
};

struct CAN_ACR_BITS
{
    uint32_t ACR: 8;               //
    uint32_t RSVD: 24;	          //31:8 reserved
};
union CAN_ACR_REG
{
    uint32_t					all;
    struct  CAN_ACR_BITS		bit;
};

struct CAN_AMR_BITS
{
    uint32_t AMR: 8;               //
    uint32_t RSVD: 24;	          //31:8 reserved
};
union CAN_AMR_REG
{
    uint32_t					all;
    struct  CAN_AMR_BITS		bit;
};

struct CAN_RMC_BITS
{
    uint32_t RMC: 5;             //Receive message counter
    uint32_t RSVD: 27;	          //31:5 reserved
};
union CAN_RMC_REG
{
    uint32_t					all;
    struct  CAN_RMC_BITS		bit;
};

struct CAN_RBSA_BITS
{
    uint32_t RBSA: 6;            //Receive buffer start address
    uint32_t RSVD: 26;	          //31:6 reserved
};
union CAN_RBSA_REG
{
    uint32_t					all;
    struct  CAN_RBSA_BITS		bit;
};

struct CAN_CDR_BITS
{
uint32_t CDR:
    3;               //The bits CD.2 to CD.0 define the frequency at the external CLKOUT pin
    uint32_t CLKOFF: 1;            //Setting this bit allows the external CLKOUT signal to be disabled
    uint32_t RSVD: 28;	            //31:4 reserved
};
union CAN_CDR_REG
{
    uint32_t					all;
    struct  CAN_CDR_BITS		bit;
};


struct CAN_RXFIFO_BITS
{
    uint32_t RX_DATA: 8;
    uint32_t RSVD: 24;
};
union CAN_RXFIFO_REG
{
    uint32_t					all;
    struct  CAN_RXFIFO_BITS		bit;
};

struct CAN_TB_BITS
{
    uint32_t  TX_DATA: 8;
    uint32_t RSVD: 24;
};
union CAN_TB_REG
{
    uint32_t					all;
    struct  CAN_TB_BITS			bit;
};
// struct CAN_ACR_BITS{
// 				uint32_t ACR:8;
// 	      uint32_t RSVD:24;
// };
// union CAN_ACR_REG{
// 				uint32_t          all;
// 	      struct CAN_ACR_BITS bit;
// };

// struct CAN_AMR_BITS{
// 				uint32_t ACR:8;
// 	      uint32_t RSVD:24;
// };
// union CAN_AMR_REG{
// 				uint32_t          all;
// 	      struct CAN_AMR_BITS bit;
// };
struct CAN_ACCEPTANCE_FILTER
{
    union CAN_ACR_REG ACR[4];
    union CAN_AMR_REG AMR[4];
};

struct CAN_TxDATAINFO
{
    union  CAN_FRAMEINFO_REG TxFRAME;
    union  CAN_DATAINFO_REG  TxDATA[12];
};

struct CAN_RxDATAINFO
{
    union  CAN_FRAMEINFO_REG RxFRAME;
    union  CAN_DATAINFO_REG  RxDATA[12];
};

typedef struct
{
    __IO   union CAN_MOD_REG  MOD;          /*!< Offset: 0x000 MOD Register    (R/W) */
    __IO   union CAN_CMR_REG  CMR;          /*!< Offset: 0x004 Command Register    (W) */
    __IO   union CAN_SR_REG SR;           /*!< Offset: 0x008 Status Register     (R) */
    __IO   union CAN_IR_REG IR;           /*!< Offset: 0x00C Interrupt Register  (R) */
    __IO   union CAN_IER_REG IER;          /*!< Offset: 0x010 Interrupt Enable Register    (R/W) */
    uint32_t  RESERVED1;
    __IO   union CAN_BTR0_REG BTR0;         /*!< Offset: 0x018 Bus Timing 0 Register    (R) */
    __IO   union CAN_BTR1_REG BTR1;         /*!< Offset: 0x01C Bus Timing 1 Register    (R) */
    __IO   union CAN_OCR_REG OCR;          /*!< Offset: 0x020 Output Control Register  (R) */
    uint32_t  RESERVED2[2];
    __IO   union CAN_ALC_REG ALC;          /*!< Offset: 0x02C Arbitration Lost Capture Register    (R) */
    __IO   union CAN_ECC_REG ECC;          /*!< Offset: 0x030 Error Code Capture Register    (R) */
    __IO   union CAN_EWLR_REG EWLR;         /*!< Offset: 0x034 Error Warning Register    (R) */
    __IO   union CAN_RXERR_REG RXERR;        /*!< Offset: 0x038 Receive Error Counter Register    (R) */
    __IO   union CAN_TXERR_REG TXERR;        /*!< Offset: 0x03C Transmit Error Register    (R) */
    //__IO	 union CAN_FRAMEINFO_REG FRAMEINFO;    /*!< Offset: 0x040 Transmit Buffer (Frame Information) Register    (W) */
    // __IO   union CAN_DATAINFO_REG DATAINFO[12]; /*!< Offset: 0x044 - 0x70 Transmit Buffer (Frame Information) Register    (W) */
    union
    {
        __IO  struct CAN_ACCEPTANCE_FILTER  FILTER;
        __O   struct CAN_TxDATAINFO          TxDATAINFO;
        __I   struct CAN_RxDATAINFO         RxDATAINFO;
    } DATAINFO;
    __I    union CAN_RMC_REG    RMC;          /*!< Offset: 0x074 Receive Message Count Register    (R) */
    __I    union CAN_RBSA_REG   RBSA;         /*!< Offset: 0x078 Receive Buffer Start Address Register    (R) */
    __IO   union CAN_CDR_REG    CDR;          /*!< Offset: 0x07C Clock Divider Register    (R/W) */
    __I    union CAN_RXFIFO_REG RXFIFO[64];   /*!< Offset: 0x080 - 0x Receive FFIO    (R) */
    __O    union CAN_TB_REG     TB[13];       /*!< Offset: 0x000 Transmit Buffer    (R) */
} HHD32F1_CAN_TypeDef;

struct CAN_STD_IDENTIFY
{
	uint8_t id_high;      //  标准帧 bit[10：3] 
	uint8_t reserve :5;   
	uint8_t id_low  :3;   //  标准帧 bit[2：0]    

};
/*
2021年6月28日 改
struct CAN_EXT_IDENTIFY
{
	uint32_t std_identify :11;      //  标准帧 bit[11：0] 
	uint32_t ext_identify :18;      //  扩展准帧 bit[12：29]
	uint32_t reserve  :3;                

};
*/
struct CAN_EXT_IDENTIFY
{
	uint32_t ID_bit21_28:8;
	uint32_t ID_bit13_15:3;
	uint32_t ID_bit16_20:5;
	uint32_t ID_bit5_12:8;
	uint32_t res:3;
	uint32_t ID_bit0_1:2;
	uint32_t ID_bit2_4  :3;
};

union UN_CAN_IDENTIFY
{
	uint8_t data[4];
	struct CAN_STD_IDENTIFY std_id;
	struct CAN_EXT_IDENTIFY ext_id;
};

// 帧类型
enum FRMAT_TYPE
{
	STD_FF = 0,							
	EXT_FF = 0x80
};

/** 
  * @brief  CAN Tx message structure definition  
  */

typedef struct
{
  uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that 
                        will be transmitted. This parameter can be a value 
                        of @ref CAN_identifier_type */

  uint8_t RTR;     /*!< Specifies the type of frame for the message that will 
                        be transmitted. This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  uint8_t DLC;     /*!< Specifies the length of the frame that will be 
                        transmitted. This parameter can be a value between 
                        0 to 8 */

  uint8_t Data[8]; /*!< Contains the data to be transmitted. It ranges from 0 
                        to 0xFF. */
} CanTxMsg;

/** 
  * @brief  CAN Rx message structure definition  
  */

typedef struct
{
  uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that 
                        will be received. This parameter can be a value of 
                        @ref CAN_identifier_type */

  uint8_t RTR;     /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  uint8_t DLC;     /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */

  uint8_t Data[8]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */

  uint8_t FMI;     /*!< Specifies the index of the filter the message stored in 
                        the mailbox passes through. This parameter can be a 
                        value between 0 to 0xFF */
} CanRxMsg;


/** @defgroup CAN_identifier_type 
  * @{
  */

/** @defgroup CAN_sleep_constants 
  * @{
  */

#define CAN_InitStatus_Failed              ((uint8_t)0x00) /*!< CAN initialization failed */
#define CAN_InitStatus_Success             ((uint8_t)0x01) /*!< CAN initialization OK */

/**
  * @}
  */

/** @defgroup CAN_Mode 
  * @{
  */

#define CAN_Mode_Normal             ((uint8_t)0x00)  /*!< normal mode */
#define CAN_Mode_LoopBack           ((uint8_t)0x01)  /*!< loopback mode */
#define CAN_Mode_Silent             ((uint8_t)0x02)  /*!< silent mode */
#define CAN_Mode_Silent_LoopBack    ((uint8_t)0x03)  /*!< loopback combined with silent mode */

#define IS_CAN_MODE(MODE) (((MODE) == CAN_Mode_Normal) || \
                           ((MODE) == CAN_Mode_LoopBack)|| \
                           ((MODE) == CAN_Mode_Silent) || \
                           ((MODE) == CAN_Mode_Silent_LoopBack))
/**
  * @}
  */


/**
  * @defgroup CAN_Operating_Mode 
  * @{
  */  
#define CAN_OperatingMode_Initialization  ((uint8_t)0x00) /*!< Initialization mode */
#define CAN_OperatingMode_Normal          ((uint8_t)0x01) /*!< Normal mode */
#define CAN_OperatingMode_Sleep           ((uint8_t)0x02) /*!< sleep mode */


#define IS_CAN_OPERATING_MODE(MODE) (((MODE) == CAN_OperatingMode_Initialization) ||\
                                    ((MODE) == CAN_OperatingMode_Normal)|| \
																		((MODE) == CAN_OperatingMode_Sleep))
/**
  * @}
  */
  
/**
  * @defgroup CAN_Mode_Status
  * @{
  */  

#define CAN_ModeStatus_Failed    ((uint8_t)0x00)                /*!< CAN entering the specific mode failed */
#define CAN_ModeStatus_Success   ((uint8_t)!CAN_ModeStatus_Failed)   /*!< CAN entering the specific mode Succeed */


/**
  * @}
  */

/** @defgroup CAN_synchronisation_jump_width 
  * @{
  */

#define CAN_SJW_1tq                 ((uint8_t)0x00)  /*!< 1 time quantum */
#define CAN_SJW_2tq                 ((uint8_t)0x01)  /*!< 2 time quantum */
#define CAN_SJW_3tq                 ((uint8_t)0x02)  /*!< 3 time quantum */
#define CAN_SJW_4tq                 ((uint8_t)0x03)  /*!< 4 time quantum */

#define IS_CAN_SJW(SJW) (((SJW) == CAN_SJW_1tq) || ((SJW) == CAN_SJW_2tq)|| \
                         ((SJW) == CAN_SJW_3tq) || ((SJW) == CAN_SJW_4tq))
/**
  * @}
  */

/** @defgroup CAN_time_quantum_in_bit_segment_1 
  * @{
  */

#define CAN_BS1_1tq                 ((uint8_t)0x00)  /*!< 1 time quantum */
#define CAN_BS1_2tq                 ((uint8_t)0x01)  /*!< 2 time quantum */
#define CAN_BS1_3tq                 ((uint8_t)0x02)  /*!< 3 time quantum */
#define CAN_BS1_4tq                 ((uint8_t)0x03)  /*!< 4 time quantum */
#define CAN_BS1_5tq                 ((uint8_t)0x04)  /*!< 5 time quantum */
#define CAN_BS1_6tq                 ((uint8_t)0x05)  /*!< 6 time quantum */
#define CAN_BS1_7tq                 ((uint8_t)0x06)  /*!< 7 time quantum */
#define CAN_BS1_8tq                 ((uint8_t)0x07)  /*!< 8 time quantum */
#define CAN_BS1_9tq                 ((uint8_t)0x08)  /*!< 9 time quantum */
#define CAN_BS1_10tq                ((uint8_t)0x09)  /*!< 10 time quantum */
#define CAN_BS1_11tq                ((uint8_t)0x0A)  /*!< 11 time quantum */
#define CAN_BS1_12tq                ((uint8_t)0x0B)  /*!< 12 time quantum */
#define CAN_BS1_13tq                ((uint8_t)0x0C)  /*!< 13 time quantum */
#define CAN_BS1_14tq                ((uint8_t)0x0D)  /*!< 14 time quantum */
#define CAN_BS1_15tq                ((uint8_t)0x0E)  /*!< 15 time quantum */
#define CAN_BS1_16tq                ((uint8_t)0x0F)  /*!< 16 time quantum */

#define IS_CAN_BS1(BS1) ((BS1) <= CAN_BS1_16tq)
/**
  * @}
  */

/** @defgroup CAN_time_quantum_in_bit_segment_2 
  * @{
  */

#define CAN_BS2_1tq                 ((uint8_t)0x00)  /*!< 1 time quantum */
#define CAN_BS2_2tq                 ((uint8_t)0x01)  /*!< 2 time quantum */
#define CAN_BS2_3tq                 ((uint8_t)0x02)  /*!< 3 time quantum */
#define CAN_BS2_4tq                 ((uint8_t)0x03)  /*!< 4 time quantum */
#define CAN_BS2_5tq                 ((uint8_t)0x04)  /*!< 5 time quantum */
#define CAN_BS2_6tq                 ((uint8_t)0x05)  /*!< 6 time quantum */
#define CAN_BS2_7tq                 ((uint8_t)0x06)  /*!< 7 time quantum */
#define CAN_BS2_8tq                 ((uint8_t)0x07)  /*!< 8 time quantum */

#define IS_CAN_BS2(BS2) ((BS2) <= CAN_BS2_8tq)

/**
  * @}
  */

/** @defgroup CAN_clock_prescaler 
  * @{
  */

#define IS_CAN_PRESCALER(PRESCALER) (((PRESCALER) >= 1) && ((PRESCALER) <= 1024))

/**
  * @}
  */

/** @defgroup CAN_filter_number 
  * @{
  */
#ifndef STM32F10X_CL
  #define IS_CAN_FILTER_NUMBER(NUMBER) ((NUMBER) <= 13)
#else
  #define IS_CAN_FILTER_NUMBER(NUMBER) ((NUMBER) <= 27)
#endif /* STM32F10X_CL */ 
/**
  * @}
  */

/** @defgroup CAN_filter_mode 
  * @{
  */

#define CAN_FilterMode_IdMask       ((uint8_t)0x00)  /*!< identifier/mask mode */
#define CAN_FilterMode_IdList       ((uint8_t)0x01)  /*!< identifier list mode */

#define IS_CAN_FILTER_MODE(MODE) (((MODE) == CAN_FilterMode_IdMask) || \
                                  ((MODE) == CAN_FilterMode_IdList))
/**
  * @}
  */

/** @defgroup CAN_filter_scale 
  * @{
  */

#define CAN_FilterScale_16bit       ((uint8_t)0x00) /*!< Two 16-bit filters */
#define CAN_FilterScale_32bit       ((uint8_t)0x01) /*!< One 32-bit filter */

#define IS_CAN_FILTER_SCALE(SCALE) (((SCALE) == CAN_FilterScale_16bit) || \
                                    ((SCALE) == CAN_FilterScale_32bit))

/**
  * @}
  */

/** @defgroup CAN_filter_FIFO
  * @{
  */

#define CAN_Filter_FIFO0             ((uint8_t)0x00)  /*!< Filter FIFO 0 assignment for filter x */
#define CAN_Filter_FIFO1             ((uint8_t)0x01)  /*!< Filter FIFO 1 assignment for filter x */
#define IS_CAN_FILTER_FIFO(FIFO) (((FIFO) == CAN_FilterFIFO0) || \
                                  ((FIFO) == CAN_FilterFIFO1))
/**
  * @}
  */

/** @defgroup Start_bank_filter_for_slave_CAN 
  * @{
  */
#define IS_CAN_BANKNUMBER(BANKNUMBER) (((BANKNUMBER) >= 1) && ((BANKNUMBER) <= 27))
/**
  * @}
  */

/** @defgroup CAN_Tx 
  * @{
  */

#define IS_CAN_TRANSMITMAILBOX(TRANSMITMAILBOX) ((TRANSMITMAILBOX) <= ((uint8_t)0x02))
#define IS_CAN_STDID(STDID)   ((STDID) <= ((uint32_t)0x7FF))
#define IS_CAN_EXTID(EXTID)   ((EXTID) <= ((uint32_t)0x1FFFFFFF))
#define IS_CAN_DLC(DLC)       ((DLC) <= ((uint8_t)0x08))

/**
  * @}
  */

/** @defgroup CAN_identifier_type 
  * @{
  */

#define CAN_Id_Standard             ((uint32_t)0x00000000)  /*!< Standard Id */
#define CAN_Id_Extended             ((uint32_t)0x00000080)  /*!< Extended Id */
#define IS_CAN_IDTYPE(IDTYPE) (((IDTYPE) == CAN_Id_Standard) || \
                               ((IDTYPE) == CAN_Id_Extended))
/**
  * @}
  */

/** @defgroup CAN_remote_transmission_request 
  * @{
  */

#define CAN_RTR_Data                ((uint32_t)0x00000000)  /*!< Data frame */
#define CAN_RTR_Remote              ((uint32_t)0x00000040)  /*!< Remote frame */
#define IS_CAN_RTR(RTR) (((RTR) == CAN_RTR_Data) || ((RTR) == CAN_RTR_Remote))

/**
  * @}
  */

/** @defgroup CAN_transmit_constants 
  * @{
  */

#define CAN_TxStatus_Failed         ((uint8_t)0x00)/*!< CAN transmission failed */
#define CAN_TxStatus_Ok             ((uint8_t)0x01) /*!< CAN transmission succeeded */
#define CAN_TxStatus_Pending        ((uint8_t)0x02) /*!< CAN transmission pending */
#define CAN_TxStatus_NoMailBox      ((uint8_t)0x04) /*!< CAN cell did not provide an empty mailbox */

/**
  * @}
  */




/** @defgroup CAN_sleep_constants 
  * @{
  */

#define CAN_Sleep_Failed     ((uint8_t)0x00) /*!< CAN did not enter the sleep mode */
#define CAN_Sleep_Ok         ((uint8_t)0x01) /*!< CAN entered the sleep mode */

/**
  * @}
  */

/** @defgroup CAN_wake_up_constants 
  * @{
  */

#define CAN_WakeUp_Failed        ((uint8_t)0x00) /*!< CAN did not leave the sleep mode */
#define CAN_WakeUp_Ok            ((uint8_t)0x01) /*!< CAN leaved the sleep mode */

/**
  * @}
  */

/**
  * @defgroup   CAN_Error_Code_constants
  * @{
  */  
                                                                
#define CAN_ErrorCode_NoErr           ((uint8_t)0x00) /*!< No Error */ 
#define	CAN_ErrorCode_StuffErr        ((uint8_t)0x10) /*!< Stuff Error */ 
#define	CAN_ErrorCode_FormErr         ((uint8_t)0x20) /*!< Form Error */ 
#define	CAN_ErrorCode_ACKErr          ((uint8_t)0x30) /*!< Acknowledgment Error */ 
#define	CAN_ErrorCode_BitRecessiveErr ((uint8_t)0x40) /*!< Bit Recessive Error */ 
#define	CAN_ErrorCode_BitDominantErr  ((uint8_t)0x50) /*!< Bit Dominant Error */ 
#define	CAN_ErrorCode_CRCErr          ((uint8_t)0x60) /*!< CRC Error  */ 
#define	CAN_ErrorCode_SoftwareSetErr  ((uint8_t)0x70) /*!< Software Set Error */ 


/**
  * @}
  */

/** @defgroup CAN_flags 
  * @{
  */
/* If the flag is 0x3XXXXXXX, it means that it can be used with CAN_GetFlagStatus()
   and CAN_ClearFlag() functions. */
/* If the flag is 0x1XXXXXXX, it means that it can only be used with CAN_GetFlagStatus() function.  */

/* Transmit Flags */
#define CAN_FLAG_RQCP0             ((uint32_t)0x38000001) /*!< Request MailBox0 Flag */
#define CAN_FLAG_RQCP1             ((uint32_t)0x38000100) /*!< Request MailBox1 Flag */
#define CAN_FLAG_RQCP2             ((uint32_t)0x38010000) /*!< Request MailBox2 Flag */

/* Receive Flags */
#define CAN_FLAG_FMP0              ((uint32_t)0x12000003) /*!< FIFO 0 Message Pending Flag */
#define CAN_FLAG_FF0               ((uint32_t)0x32000008) /*!< FIFO 0 Full Flag            */
#define CAN_FLAG_FOV0              ((uint32_t)0x32000010) /*!< FIFO 0 Overrun Flag         */
#define CAN_FLAG_FMP1              ((uint32_t)0x14000003) /*!< FIFO 1 Message Pending Flag */
#define CAN_FLAG_FF1               ((uint32_t)0x34000008) /*!< FIFO 1 Full Flag            */
#define CAN_FLAG_FOV1              ((uint32_t)0x34000010) /*!< FIFO 1 Overrun Flag         */

/* Operating Mode Flags */
#define CAN_FLAG_WKU               ((uint32_t)0x31000008) /*!< Wake up Flag */
#define CAN_FLAG_SLAK              ((uint32_t)0x31000012) /*!< Sleep acknowledge Flag */
/* Note: When SLAK intterupt is disabled (SLKIE=0), no polling on SLAKI is possible. 
         In this case the SLAK bit can be polled.*/

/* Error Flags */
#define CAN_FLAG_EWG               ((uint32_t)0x10F00001) /*!< Error Warning Flag   */
#define CAN_FLAG_EPV               ((uint32_t)0x10F00002) /*!< Error Passive Flag   */
#define CAN_FLAG_BOF               ((uint32_t)0x10F00004) /*!< Bus-Off Flag         */
#define CAN_FLAG_LEC               ((uint32_t)0x30F00070) /*!< Last error code Flag */

#define IS_CAN_GET_FLAG(FLAG) (((FLAG) == CAN_FLAG_LEC)  || ((FLAG) == CAN_FLAG_BOF)   || \
                               ((FLAG) == CAN_FLAG_EPV)  || ((FLAG) == CAN_FLAG_EWG)   || \
                               ((FLAG) == CAN_FLAG_WKU)  || ((FLAG) == CAN_FLAG_FOV0)  || \
                               ((FLAG) == CAN_FLAG_FF0)  || ((FLAG) == CAN_FLAG_FMP0)  || \
                               ((FLAG) == CAN_FLAG_FOV1) || ((FLAG) == CAN_FLAG_FF1)   || \
                               ((FLAG) == CAN_FLAG_FMP1) || ((FLAG) == CAN_FLAG_RQCP2) || \
                               ((FLAG) == CAN_FLAG_RQCP1)|| ((FLAG) == CAN_FLAG_RQCP0) || \
                               ((FLAG) == CAN_FLAG_SLAK ))

#define IS_CAN_CLEAR_FLAG(FLAG)(((FLAG) == CAN_FLAG_LEC) || ((FLAG) == CAN_FLAG_RQCP2) || \
                                ((FLAG) == CAN_FLAG_RQCP1)  || ((FLAG) == CAN_FLAG_RQCP0) || \
                                ((FLAG) == CAN_FLAG_FF0)  || ((FLAG) == CAN_FLAG_FOV0) ||\
                                ((FLAG) == CAN_FLAG_FF1) || ((FLAG) == CAN_FLAG_FOV1) || \
                                ((FLAG) == CAN_FLAG_WKU) || ((FLAG) == CAN_FLAG_SLAK))
/**
  * @}
  */

  
/** @defgroup CAN_interrupts 
  * @{
  */


  
#define CAN_IT_TME                  ((uint32_t)0x00000001) /*!< Transmit mailbox empty Interrupt*/

/* Receive Interrupts */
#define CAN_IT_FMP0                 ((uint32_t)0x00000002) /*!< FIFO 0 message pending Interrupt*/
#define CAN_IT_FF0                  ((uint32_t)0x00000004) /*!< FIFO 0 full Interrupt*/
#define CAN_IT_FOV0                 ((uint32_t)0x00000008) /*!< FIFO 0 overrun Interrupt*/
#define CAN_IT_FMP1                 ((uint32_t)0x00000010) /*!< FIFO 1 message pending Interrupt*/
#define CAN_IT_FF1                  ((uint32_t)0x00000020) /*!< FIFO 1 full Interrupt*/
#define CAN_IT_FOV1                 ((uint32_t)0x00000040) /*!< FIFO 1 overrun Interrupt*/

/* Operating Mode Interrupts */
#define CAN_IT_WKU                  ((uint32_t)0x00010000) /*!< Wake-up Interrupt*/
#define CAN_IT_SLK                  ((uint32_t)0x00020000) /*!< Sleep acknowledge Interrupt*/

/* Error Interrupts */
#define CAN_IT_EWG                  ((uint32_t)0x00000100) /*!< Error warning Interrupt*/
#define CAN_IT_EPV                  ((uint32_t)0x00000200) /*!< Error passive Interrupt*/
#define CAN_IT_BOF                  ((uint32_t)0x00000400) /*!< Bus-off Interrupt*/
#define CAN_IT_LEC                  ((uint32_t)0x00000800) /*!< Last error code Interrupt*/
#define CAN_IT_ERR                  ((uint32_t)0x00008000) /*!< Error Interrupt*/

/* Flags named as Interrupts : kept only for FW compatibility */
#define CAN_IT_RQCP0   CAN_IT_TME
#define CAN_IT_RQCP1   CAN_IT_TME
#define CAN_IT_RQCP2   CAN_IT_TME


#define IS_CAN_IT(IT)        (((IT) == CAN_IT_TME) || ((IT) == CAN_IT_FMP0)  ||\
                             ((IT) == CAN_IT_FF0)  || ((IT) == CAN_IT_FOV0)  ||\
                             ((IT) == CAN_IT_FMP1) || ((IT) == CAN_IT_FF1)   ||\
                             ((IT) == CAN_IT_FOV1) || ((IT) == CAN_IT_EWG)   ||\
                             ((IT) == CAN_IT_EPV)  || ((IT) == CAN_IT_BOF)   ||\
                             ((IT) == CAN_IT_LEC)  || ((IT) == CAN_IT_ERR)   ||\
                             ((IT) == CAN_IT_WKU)  || ((IT) == CAN_IT_SLK))

#define IS_CAN_CLEAR_IT(IT) (((IT) == CAN_IT_TME) || ((IT) == CAN_IT_FF0)    ||\
                             ((IT) == CAN_IT_FOV0)|| ((IT) == CAN_IT_FF1)    ||\
                             ((IT) == CAN_IT_FOV1)|| ((IT) == CAN_IT_EWG)    ||\
                             ((IT) == CAN_IT_EPV) || ((IT) == CAN_IT_BOF)    ||\
                             ((IT) == CAN_IT_LEC) || ((IT) == CAN_IT_ERR)    ||\
                             ((IT) == CAN_IT_WKU) || ((IT) == CAN_IT_SLK))


/** @defgroup CAN_Legacy 
  * @{
  */
#define CANINITFAILED               CAN_InitStatus_Failed
#define CANINITOK                   CAN_InitStatus_Success
#define CAN_FilterFIFO0             CAN_Filter_FIFO0
#define CAN_FilterFIFO1             CAN_Filter_FIFO1
#define CAN_ID_STD                  CAN_Id_Standard           
#define CAN_ID_EXT                  CAN_Id_Extended
#define CAN_RTR_DATA                CAN_RTR_Data         
#define CAN_RTR_REMOTE              CAN_RTR_Remote
#define CANTXFAILE                  CAN_TxStatus_Failed
#define CANTXOK                     CAN_TxStatus_Ok
#define CANTXPENDING                CAN_TxStatus_Pending
#define CAN_NO_MB                   CAN_TxStatus_NoMailBox

typedef enum en_Baud{
	CAN_BAUD_500K ,
	CAN_BAUD_1M ,
	CAN_BAUD_250K 
} EN_BAUD;

#define CAN1       ((HHD32F1_CAN_TypeDef     *) HHD_CAN1_BASE)
#define CAN2       ((HHD32F1_CAN_TypeDef     *) HHD_CAN2_BASE)


void CAN_SetMode(HHD32F1_CAN_TypeDef *can, uint32_t mode);
void CAN_EnableInt(HHD32F1_CAN_TypeDef *can, uint32_t intbit);
void CAN_DisableInt(HHD32F1_CAN_TypeDef *can, uint32_t intbit);
int  CAN_GetStatus(HHD32F1_CAN_TypeDef *can);
void CAN_ClearStatus(HHD32F1_CAN_TypeDef *can, uint32_t clrbit);
void CAN_SetCMD(HHD32F1_CAN_TypeDef *can, uint32_t cmddata);
void CAN_ClrCMD(HHD32F1_CAN_TypeDef *can, uint32_t cmddata);

int can_write(HHD32F1_CAN_TypeDef *can, enum FRMAT_TYPE type, union UN_CAN_IDENTIFY *Identifier,  uint8_t *data, int len);
int Ext_can_write(HHD32F1_CAN_TypeDef *can, union UN_CAN_IDENTIFY *Identifier,  uint8_t *data, int len);
void can_write_back(HHD32F1_CAN_TypeDef *can,  uint8_t *data);

#endif

extern void can_init(HHD32F1_CAN_TypeDef *can, EN_BAUD baud, uint32_t filterID, uint32_t mask);
int CAN_Receive(HHD32F1_CAN_TypeDef *can, uint8_t FIFONumber, CanRxMsg* RxMessage);


#endif
 /* end __CAN */
/*****************************************************************************
**                            End Of File
******************************************************************************/
