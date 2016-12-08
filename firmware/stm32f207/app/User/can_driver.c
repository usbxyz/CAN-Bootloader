/**
  ******************************************************************************
  * @file    can_driver.c
  * $Author: wdluo $
  * $Revision: 17 $
  * $Date:: 2012-07-06 11:16:48 +0800 #$
  * @brief   CAN总线收发相关函数.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can_app.h"
/* Private typedef -----------------------------------------------------------*/
typedef  struct {
  unsigned char   SJW;
  unsigned char   BS1;
  unsigned char   BS2;
  unsigned short  PreScale;
} tCAN_BaudRate;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
CanRxMsg CAN1_RxMessage,CAN2_RxMessage;
volatile uint8_t CAN1_CanRxMsgFlag=0;//接收到CAN数据后的标志
volatile uint8_t CAN2_CanRxMsgFlag=0;
volatile uint8_t TimeOutFlag;				///<定时器超时标志
tCAN_BaudRate  CAN_BaudRateInitTab[]= {      // CLK=30MHz
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_8tq,2},     // 1M
   {CAN_SJW_1tq,CAN_BS1_7tq,CAN_BS2_3tq,3},     // 900K
   {CAN_SJW_2tq,CAN_BS1_13tq,CAN_BS2_5tq,2},     // 800K
   {CAN_SJW_1tq,CAN_BS1_7tq,CAN_BS2_1tq,5},     // 666K
   {CAN_SJW_1tq,CAN_BS1_7tq,CAN_BS2_2tq,5},     // 600K
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_8tq,4},     // 500K
   {CAN_SJW_1tq,CAN_BS1_12tq,CAN_BS2_2tq,5},     // 400K
   {CAN_SJW_1tq,CAN_BS1_16tq,CAN_BS2_3tq,5},    // 300K
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_1tq,15},    // 250K
   {CAN_SJW_1tq,CAN_BS1_15tq,CAN_BS2_3tq,7},	// 225K
   {CAN_SJW_1tq,CAN_BS1_12tq,CAN_BS2_2tq,10},    // 200K
   {CAN_SJW_1tq,CAN_BS1_14tq,CAN_BS2_2tq,11},	// 160K
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_1tq,25},    // 150K
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_1tq,26},	// 144K
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_1tq,30},   // 125K
   {CAN_SJW_1tq,CAN_BS1_7tq,CAN_BS2_2tq,25},	// 120K
   {CAN_SJW_1tq,CAN_BS1_12tq,CAN_BS2_2tq,20},    // 100K
   {CAN_SJW_1tq,CAN_BS1_7tq,CAN_BS2_1tq,37},   // 90K
   {CAN_SJW_1tq,CAN_BS1_3tq,CAN_BS2_1tq,75},   // 80K
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_1tq,50},	// 75K
   {CAN_SJW_1tq,CAN_BS1_16tq,CAN_BS2_3tq,25},    // 60K
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_1tq,75},    // 50K
   {CAN_SJW_1tq,CAN_BS1_12tq,CAN_BS2_2tq,50},    // 40K
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_1tq,125},   // 30K
   {CAN_SJW_1tq,CAN_BS1_7tq,CAN_BS2_2tq,150},   // 20K
};
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  CAN时钟配置表，CLK=36MHz 
  */
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq,   6,     // 1M     36/(1+3+2)/6 =1M  
// CAN_SJW_1tq, CAN_BS1_4tq , CAN_BS2_3tq,   5,     // 900K   36/(1+4+3)/5 =0.9  
// CAN_SJW_1tq, CAN_BS1_5tq , CAN_BS2_3tq,   5,     // 800K   36/(1+5+3)/5 =0.8  
// CAN_SJW_1tq, CAN_BS1_6tq , CAN_BS2_3tq,   6,     // 600K   36/(1+6+3)/6=0.6  
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq,  12,     // 500K     
// CAN_SJW_1tq, CAN_BS1_5tq , CAN_BS2_3tq,  10,     // 400K     
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq,  20,     // 300K   36/(1+3+2)/20  
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq,  24,     // 250K     
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq,  30,     // 200K    
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq,  40,     // 150K   36/(1+3+2)/40  
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq,  48,     // 125K   36/(1+3+2)/48  
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq,  60,     // 100K     
// CAN_SJW_1tq, CAN_BS1_4tq , CAN_BS2_3tq,  50,     //  90K   36/(1+4+3)/50 =0.09  
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq,  75,     //  80K    
// CAN_SJW_1tq, CAN_BS1_6tq , CAN_BS2_3tq,  60,     //  60K    
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq, 120,     //  50K    
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq, 150,     //  40K   
// CAN_SJW_1tq, CAN_BS1_6tq , CAN_BS2_3tq, 120,     //  30K    
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq, 300,     //  20K       
// CAN_SJW_1tq, CAN_BS1_3tq , CAN_BS2_2tq, 600,     //  10K  
// CAN_SJW_2tq, CAN_BS1_6tq , CAN_BS2_4tq, 600,     //  5K   36/(2+6+4)/600=5K   
// CAN_SJW_2tq, CAN_BS1_6tq , CAN_BS2_4tq, 1000,    //  3K   36/(2+6+4)/1000  
// CAN_SJW_2tq, CAN_BS1_10tq, CAN_BS2_6tq, 1000     //  2K   36/(2+10+6)/1000=2K   

uint32_t CAN_GetBaudRateNum(uint32_t BaudRate)
{
    switch(BaudRate){
        case 1000000 :return 0;
        case 900000 :return 1;
        case 800000 :return 2;
        case 666000 :return 3;
        case 600000 :return 4;
        case 500000 :return 5;
        case 400000 :return 6;
        case 300000 :return 7;
        case 250000 :return 8;
        case 225000:return 9;
        case 200000 :return 10;
        case 160000:return 11;
        case 150000 :return 12;
        case 144000:return 13;
        case 125000 :return 14;
        case 120000:return 15;
        case 100000 :return 16;
        case 90000 :return 17;
        case 80000 :return 18;
        case 75000:return 19;
        case 60000 :return 20;
        case 50000 :return 21;
        case 40000 :return 22;
        case 30000 :return 23;
        case 20000 :return 24;
        default:return 0;
    }
}

/**
  * @brief  TIM定时器设置，用于CAN数据发送超时判断
  * @param  None
  * @retval None
  */
void BOOT_TIM_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	// 使能外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	/* 时钟及分频设置 */
	/* 30M/30000 = 1ms */
	TIM_TimeBaseStructure.TIM_Prescaler = 60000-1;// 预分频，分频后的频率为1K    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数模式:向上计数
	// TIM的重载值，决定定时时间长度，400*0.5ms=200ms
	TIM_TimeBaseStructure.TIM_Period =0;	  // 计数重载值，向上计数时，计数到该值溢出，向下计数时，从该值开始计数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 与数字滤波器的采样率有关       
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  //重新计数的起始值
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TimeOutFlag = 0;
	
	/* Disable the TIM Counter */
	TIM2->CR1 &= (uint16_t)(~TIM_CR1_CEN);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //开启溢出中断
	
	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;							// 中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	// 先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					// 从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						// 使能中断
	NVIC_Init(&NVIC_InitStructure);		
}

void BOOT_DelayMs(uint32_t ms)
{
	TimeOutFlag = 0;
	TIM2->ARR = ms;
	/* Enable the TIM Counter */
	TIM2->CR1 |= (uint16_t)TIM_CR1_CEN;	
	while(!TimeOutFlag);
	/* Disable the TIM Counter */
	TIM2->CR1 &= (uint16_t)(~TIM_CR1_CEN);
}
/**
  * @brief  TIM定时器超时中断处理函数
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)== SET)
	{
		TimeOutFlag = 1;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //清楚溢出标志
	}
}


/**
  * @brief  CAN引脚配置
  * @param  CANx CAN通道号
  * @retval None
  */
void CAN_GPIO_Configuration(CAN_TypeDef* CANx)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	if(CANx == CAN2){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_CAN2);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_CAN2);
	}else{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1);
	}
	
  /* Configure CAN1 RX pin */
	if(CANx == CAN2){
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	}else{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	}
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	if(CANx == CAN2){
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}else{
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
//	/*Configure Addr Pin*/
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOG, &GPIO_InitStructure);
  /* CAN1 and CAN2 periph clock enable */
	if(CANx == CAN2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	}else{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);	
	}		
}
/**
  * @brief  CAN接收中断配置
  * @param  CANx CAN通道号
  * @retval None
  */
void CAN_NVIC_Configuration(CAN_TypeDef* CANx)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable CAN1 RX0 interrupt IRQ channel */
	if(CANx == CAN1){
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	}else{
		NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	}
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  配置CAN接收过滤器
  * @param  CANx CAN通道号
  * @param  FilterNumber 过滤器号
  * @param  can_addr CAN节点地址
  * @retval None
  */
void CAN_ConfigFilter(CAN_TypeDef* CANx,uint8_t FilterNumber,uint16_t can_addr)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	if(CANx == CAN2){
		FilterNumber += 14;
	}
	//设置CAN接收过滤器
  CAN_FilterInitStructure.CAN_FilterNumber=FilterNumber;//过滤器1
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//屏蔽位模式
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//32bit模式
	//以下4个为0表示接收任何数据
  CAN_FilterInitStructure.CAN_FilterIdHigh=can_addr>>(16-CMD_WIDTH-3);
  CAN_FilterInitStructure.CAN_FilterIdLow=(can_addr<<(CMD_WIDTH+3))|0x04;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=ADDR_MASK>>(16-CMD_WIDTH-3);;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=(ADDR_MASK<<(CMD_WIDTH+3))|0x04;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//使能过滤器
  CAN_FilterInit(&CAN_FilterInitStructure);	
}
/**
  * @brief  初始化CAN
  * @param  None
  * @retval None
  */
void CAN_Configuration(CAN_TypeDef* CANx,uint32_t BaudRate)
{
  CAN_InitTypeDef        CAN_InitStructure;
  /* CAN register init */
  CAN_NVIC_Configuration(CANx);
  CAN_GPIO_Configuration(CANx);

  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = ENABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_BaudRateInitTab[CAN_GetBaudRateNum(BaudRate)].SJW;//配置波特率为1M
  CAN_InitStructure.CAN_BS1 = CAN_BaudRateInitTab[CAN_GetBaudRateNum(BaudRate)].BS1;
  CAN_InitStructure.CAN_BS2 = CAN_BaudRateInitTab[CAN_GetBaudRateNum(BaudRate)].BS2;
  CAN_InitStructure.CAN_Prescaler = CAN_BaudRateInitTab[CAN_GetBaudRateNum(BaudRate)].PreScale;

  CAN_Init(CANx,&CAN_InitStructure);
	//设置CAN接收过滤器
	CAN_ConfigFilter(CANx,0,0x00);//广播地址，接受广播命令
	CAN_ConfigFilter(CANx,1,CAN_BOOT_GetAddrData());//本节点真实地址
	
	//使能接收中断
	CAN_ITConfig(CANx,CAN_IT_FMP0, ENABLE);
	//配置定时器
	BOOT_TIM_Config();
}


/**
  * @brief  发送一帧CAN数据
  * @param  CANx CAN通道号
	* @param  TxMessage CAN消息指针
  * @retval None
  */
uint8_t CAN_WriteData(CAN_TypeDef* CANx,CanTxMsg *TxMessage)
{
	uint8_t TransmitMailbox;   
	uint32_t	TimeOut=0;
  TransmitMailbox = CAN_Transmit(CANx,TxMessage);
	while(CAN_TransmitStatus(CANx,TransmitMailbox)!=CAN_TxStatus_Ok){
		TimeOut++;
		if(TimeOut > 10000000){
			return 1;
		}
	}
	return 0;
}
/**
  * @brief  CAN接收中断处理函数
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	CAN_Receive(CAN1,CAN_FIFO0, &CAN1_RxMessage);
  CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	CAN1_CanRxMsgFlag = 1;
}

/**
  * @brief  CAN接收中断处理函数
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void)
{
	CAN_Receive(CAN2,CAN_FIFO0, &CAN2_RxMessage);
  CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);
	CAN2_CanRxMsgFlag = 1;
}

/*********************************END OF FILE**********************************/
