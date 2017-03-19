/**
  ******************************************************************************
  * @file    main.c
  * $Author: 飞鸿踏雪 $
  * $Revision: 17 $
  * $Date:: 2014-10-25 11:16:48 +0800 #$
  * @brief   主函数.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, EmbedNet</center>
  *<center><a href="http:\\www.embed-net.com">http://www.embed-net.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "delay.h"
#include "can_app.h"
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
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  串口打印输出
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
  if(*((uint32_t *)APP_EXE_FLAG_ADDR)==0xFFFFFFFF){
    __align(4) static unsigned char data[4]={0x12,0x34,0x56,0x78};
    FLASH_Unlock();
    CAN_BOOT_ProgramDatatoFlash(APP_EXE_FLAG_START_ADDR,data,4);
    FLASH_Lock();
  }
  __set_PRIMASK(0);//开启总中断
  delay_init(168);
  CAN_Configuration(1000000);
  while (1)
  {
    if(CAN1_CanRxMsgFlag){
      CAN_BOOT_ExecutiveCommand(&CAN1_RxMessage);
      CAN1_CanRxMsgFlag = 0;
    }
  }
} // end of main()


/*********************************END OF FILE**********************************/
