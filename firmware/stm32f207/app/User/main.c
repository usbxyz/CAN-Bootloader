/**
  ******************************************************************************
  * @file    app.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   This file provides all the Application firmware functions.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/ 
#include	"main.h"
#include "can_app.h"
#include "delay.h"
extern CanRxMsg CAN_RxMessage;
extern volatile uint8_t CAN_CanRxMsgFlag;//接收到CAN数据后的标志
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
/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup APP_HID 
  * @brief Mass storage application module
  * @{
  */ 

/** @defgroup APP_HID_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup APP_HID_Private_Defines
  * @{
  */ 


/**
  * @}
  */ 


/** @defgroup APP_HID_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup APP_HID_Private_Variables
  * @{
  */ 
 
/**
  * @}
  */ 


/** @defgroup APP_HID_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */ 


/** @defgroup APP_HID_Private_Functions
  * @{
  */ 

/**
  * @brief  Program entry point
  * @param  None
  * @retval None
  */
int main(void)
{
//	int i;
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32fxxx_xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32fxxx.c file
  */
  if(*((uint32_t *)APP_EXE_FLAG_ADDR)==0xFFFFFFFF){
    __align(4) static unsigned char data[4]={0x12,0x34,0x56,0x78};
    FLASH_Unlock();
    CAN_BOOT_ProgramDatatoFlash(APP_EXE_FLAG_ADDR,data,4);
    FLASH_Lock();
  }
  __set_PRIMASK(0);//开启总中断
  delay_init(120);
  CAN_Configuration(1000000);
  //设置读保护
// 	if(FLASH_OB_GetRDP() != SET)
// 	{
// 		FLASH_OB_Unlock();
// 		FLASH_OB_RDPConfig(OB_RDP_Level_1); 
// 		FLASH_OB_Launch();
// 		FLASH_OB_Lock();		
// 	}
  while (1)
  {
    if(CAN_CanRxMsgFlag){
      CAN_CanRxMsgFlag = 0;
      CAN_BOOT_ExecutiveCommand(&CAN_RxMessage);
    }
  }
} 

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
