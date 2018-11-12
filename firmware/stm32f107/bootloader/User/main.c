/**
  ******************************************************************************
  * @file    main.c
  * $Author: wdluo $
  * $Revision: 17 $
  * $Date:: 2012-07-06 11:16:48 +0800 #$
  * @brief   主函数.
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
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//命令参数必须跟上位机软件的命令参数一致
CBL_CMD_LIST CMD_List = 
{
  .Erase = 0x00,      //擦除APP区域数据
  .WriteInfo = 0x01,  //设置多字节写数据相关参数（写起始地址，数据量）
  .Write = 0x02,      //以多字节形式写数据
  .Check = 0x03,      //检测节点是否在线，同时返回固件信息
  .SetBaudRate = 0x04,//设置节点波特率
  .Excute = 0x05,     //执行固件
  .CmdSuccess = 0x08, //命令执行成功
  .CmdFaild = 0x09,   //命令执行失败
};
extern CanRxMsg CAN1_RxMessage;
extern volatile uint8_t CAN1_CanRxMsgFlag;//接收到CAN数据后的标志
extern volatile uint8_t TimeOutFlag;				///<定时器超时标志
volatile uint16_t BOOT_TimeOutCount;
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
/**
  * @brief  根据地址配置引脚设置程序启动方式
  * @param  None
  * @retval None
  */
void BOOT_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	/*Configure Addr Pin*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	if(CAN_BOOT_GetAddrData()==0x00){
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		// 使能外设时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
		/* 时钟及分频设置 */
		/* 30M/30000 = 1ms */
		TIM_TimeBaseStructure.TIM_Prescaler = 72-1;// 预分频，分频后的频率为1K    
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数模式:向上计数
		// TIM的重载值，决定定时时间长度，
		TIM_TimeBaseStructure.TIM_Period =1000;	  // 计数重载值，向上计数时，计数到该值溢出，向下计数时，从该值开始计数
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 与数字滤波器的采样率有关       
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  //重新计数的起始值
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		TimeOutFlag = 0;
		
		/* Disable the TIM Counter */
		TIM2->CR1 &= (uint16_t)(~TIM_CR1_CEN);

		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //开启溢出中断
		
		/* Enable the TIM2 global Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;							// 中断源
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	// 先占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					// 从优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						// 使能中断
		NVIC_Init(&NVIC_InitStructure);		
		
		/* 	Enable the TIM Counter */
		TIM2->CR1 |= (uint16_t)TIM_CR1_CEN;
		while(CAN_BOOT_GetAddrData()==0x00){
			if(TimeOutFlag){
				TimeOutFlag = 0;
				__set_PRIMASK(1);//关闭所有中断
				FLASH_Unlock();
				CAN_BOOT_ErasePage(APP_EXE_FLAG_START_ADDR,APP_START_ADDR);
				__set_PRIMASK(0);//开启所有中断
			}
		}
		/* Disable the TIM Counter */
		TIM2->CR1 &= (uint16_t)(~TIM_CR1_CEN);
	}
	if((*((uint32_t *)APP_EXE_FLAG_START_ADDR)==0x12345678)&&(*((uint32_t *)APP_START_ADDR)!=0xFFFFFFFF)){
		CAN_BOOT_JumpToApplication(APP_START_ADDR);	
	}
	__set_PRIMASK(0);//开启总中断
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000); //重新映射中断向量表
}

/**
  * @brief  TIM定时器超时中断处理函数
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)== SET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update); //清楚溢出标志
		BOOT_TimeOutCount++;
	}
}

/**
  * @brief  根据地址配置引脚设置程序启动方式
  * @param  None
  * @retval None
  */
void BOOT_Delay_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	__set_PRIMASK(0);//开启总中断
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000); //重新映射中断向量表
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	// 使能外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	/* 时钟及分频设置 */
	/* 30M/60000 = 1ms */
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;// 预分频，分频后的频率为1M   
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数模式:向上计数
	// TIM的重载值，决定定时时间长度，1
	TIM_TimeBaseStructure.TIM_Period =1000;	  // 计数重载值，向上计数时，计数到该值溢出，向下计数时，从该值开始计数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 与数字滤波器的采样率有关       
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  //重新计数的起始值
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TimeOutFlag = 0;
		
	/* Disable the TIM Counter */
	TIM3->CR1 &= (uint16_t)(~TIM_CR1_CEN);

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //开启溢出中断
	
	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;							// 中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	// 先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					// 从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						// 使能中断
	NVIC_Init(&NVIC_InitStructure);		
		
	/* 	Enable the TIM Counter */
	TIM3->CR1 |= (uint16_t)TIM_CR1_CEN;
	BOOT_TimeOutCount = 0;
}

/**
  * @brief  主函数，实现LED灯的闪烁
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32fxxx_xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32fxxx.c file
  */
	//时钟配置函数在system_stm32f10x.c文件的SetSysClockTo72()函数中
  if(*((uint32_t *)APP_EXE_FLAG_ADDR)==0x78563412){
    CAN_BOOT_JumpToApplication(APP_START_ADDR);
  }
  __set_PRIMASK(0);//开启总中断
  CAN_Configuration(1000000);

// 	/* Enable the Flash option control register access */
// 	FLASH_OB_Unlock();
// 	/* Enable FLASH_WRP_SECTORS write protection */
// 	FLASH_OB_WRPConfig(OB_WRP_Sector_0, ENABLE); 
// 	FLASH_OB_Launch();
// 	FLASH_OB_Lock();	
	
// 	if(FLASH_OB_GetRDP() != SET)
// 	{
// 		FLASH_OB_Unlock();
// 		FLASH_OB_RDPConfig(OB_RDP_Level_1); 
// 		FLASH_OB_Launch();
// 		FLASH_OB_Lock();		
// 	}
  while (1)
  {
    if(CAN1_CanRxMsgFlag){
      CAN_BOOT_ExecutiveCommand(&CAN1_RxMessage);
      CAN1_CanRxMsgFlag = 0;
    }
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/***********************************文件结束***********************************/
