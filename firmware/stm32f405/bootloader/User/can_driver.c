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
#include "can_bootloader.h"
/* Private typedef -----------------------------------------------------------*/
typedef  struct {
  unsigned char   SJW;
  unsigned char   BS1;
  unsigned char   BS2;
  unsigned short  PreScale;
} tCAN_BaudRate;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
CanRxMsg CAN1_RxMessage;
volatile uint8_t CAN1_CanRxMsgFlag=0;//接收到CAN数据后的标志

tCAN_BaudRate  CAN_BaudRateInitTab[]= {      // CLK=84MHz
   {CAN_SJW_1tq,CAN_BS1_2tq,CAN_BS2_1tq,21},     // 1M
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_1tq,21},     // 500K
   {CAN_SJW_1tq,CAN_BS1_4tq,CAN_BS2_1tq,70},    // 200K
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_1tq,105},    // 100K
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_1tq,210},    // 50K
   {CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_1tq,525},   // 20K
};
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  通过波特率的值获取波特率参数表索引值
  * @param  BaudRate CAN总线波特率，单位为bps
  * @retval 波特率参数表索引值
  */
uint32_t CAN_GetBaudRateNum(uint32_t BaudRate)
{
  switch(BaudRate){
    case 1000000 :return 0;
    case 500000 :return 1;
    case 200000 :return 2;
    case 100000 :return 3;
    case 50000 :return 4;
    case 20000 :return 5;
    default:return 0;
  }
}



/**
  * @brief  CAN引脚配置
  * @param  None
  * @retval None
  */
void CAN_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
  // CAN1 periph clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1);
  // Configure CAN1 pin PB8->CAN1_RX  PB9->CAN1_TX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PORTA时钟
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	

//  //初始化GPIO
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA11,PA12

//  //引脚复用映射配置
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11复用为CAN1
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12复用为CAN1 

}
/**
  * @brief  CAN接收中断配置
  * @param  None
  * @retval None
  */
void CAN_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  // Enable CAN1 RX0 interrupt IRQ channel
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  配置CAN接收过滤器
  * @param  FilterNumber 过滤器号
  * @param  can_addr CAN节点地址，该参数非常重要，同一个CAN总线网络其节点地址不能重复
  * @retval None
  */
void CAN_ConfigFilter(uint8_t FilterNumber,uint16_t can_addr)
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

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
  * @param  BaudRate CAN总线波特率
  * @retval None
  */
void CAN_Configuration(uint32_t BaudRate)
{
  CAN_InitTypeDef        CAN_InitStructure;
  /* CAN register init */
  CAN_NVIC_Configuration();
  CAN_GPIO_Configuration();

  CAN_Address_GPIO_Config();

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

  CAN_Init(CAN1,&CAN_InitStructure);
  //设置CAN接收过滤器
  CAN_ConfigFilter(0,0x00);//广播地址，接受广播命令
  CAN_ConfigFilter(1,CAN_BOOT_GetAddrData());//本节点真实地址

  //使能接收中断
  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);
}


/**
  * @brief  发送一帧CAN数据
  * @param  CANx CAN通道号
	* @param  TxMessage CAN消息指针
  * @retval None
  */
uint8_t CAN_WriteData(CanTxMsg *TxMessage)
{
  uint8_t   TransmitMailbox;   
  uint32_t  TimeOut=0;
  TransmitMailbox = CAN_Transmit(CAN1,TxMessage);
  while(CAN_TransmitStatus(CAN1,TransmitMailbox)!=CAN_TxStatus_Ok){
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
  * @brief  获取CAN节点地址，该函数根据自己的实际情况进行修改
  * @param  None
  * @retval None
  */
uint16_t Read_CAN_Address(void)
{
  return 0x123;//返回的地址值需要根据实际情况进行修改
}

/**
  * @brief  初始化配置获取CAN总线地址的GPIO引脚
  * @param  None
  * @retval None
  */
void CAN_Address_GPIO_Config(void)
{
  //根据自己的板子完成对应的程序
}

/*********************************END OF FILE**********************************/
