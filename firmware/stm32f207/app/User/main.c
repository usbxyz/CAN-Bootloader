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
CBL_CMD_LIST CMD_List;
extern CanRxMsg CAN1_RxMessage,CAN2_RxMessage;
extern volatile uint8_t CAN1_CanRxMsgFlag;//接收到CAN数据后的标志
extern volatile uint8_t CAN2_CanRxMsgFlag;
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
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000); //重新映射中断向量表
	if(*((uint32_t *)0x08004000)==0xFFFFFFFF){
		uint32_t addr = 0x08004000;
		__align(4) static unsigned char data[4]={0x12,0x34,0x56,0x78};
		FLASH_Unlock();
		CAN_BOOT_ProgramDatatoFlash(addr,data,4);
		FLASH_Lock();
	}
	__set_PRIMASK(0);//开启所有中断 
	CAN_Configuration(CAN1,1000000);
	CAN_Configuration(CAN2,1000000);
	
	CMD_List.OnlineCheck = 0x01;
	CMD_List.EraseFlash = 0x03;
	CMD_List.SetBaudRate = 0x04;
	CMD_List.BlockWriteInfo = 0x05;
	CMD_List.WriteBlockFlash = 0x06;
	CMD_List.BlockReadInfo = 0x07;
	CMD_List.ReadBlockFlash = 0x08;
	CMD_List.ExcuteApp = 0x09;
	CMD_List.CmdFaild = 0x00;
	CMD_List.CmdSuccess = 0x0A;
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
        CAN1_CanRxMsgFlag = 0;
        CAN_BOOT_ExecutiveCommand(CAN1,&CAN1_RxMessage);
      }
      if(CAN2_CanRxMsgFlag){
        CAN2_CanRxMsgFlag = 0;
        CAN_BOOT_ExecutiveCommand(CAN2,&CAN2_RxMessage);
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
