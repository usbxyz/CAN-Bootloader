/**
  ******************************************************************************
  * @file    can_bootloader.h
  * $Author: wdluo $
  * $Revision: 17 $
  * $Date:: 2012-07-06 11:16:48 +0800 #$
  * @brief   基于CAN总线的Bootloader程序.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_BOOTLOADER_H
#define __CAN_BOOTLOADER_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can_driver.h"
/* Private typedef -----------------------------------------------------------*/
#define CMD_WIDTH   4         //不要修改
#define CMD_MASK    0xF       //不要修改
#define CAN_ID_TYPE 1         //1为扩展帧，0为标准帧，不要修改
#define ADDR_MASK   0x1FFFFFF //不要修改

#define APP_EXE_FLAG_START_ADDR    ((uint32_t)0x08004000)
#define APP_START_ADDR             ((uint32_t)0x08008000)
#define CAN_BL_BOOT     0x55555555
#define CAN_BL_APP      0xAAAAAAAA
#define FW_TYPE         CAN_BL_BOOT

typedef struct _CBL_CMD_LIST{
  //Bootloader相关命令
  unsigned char Erase;        //擦出APP储存扇区数据
  unsigned char WriteInfo;    //设置多字节写数据相关参数（写起始地址，数据量）
  unsigned char Write;        //以多字节形式写数据
  unsigned char Check;        //检测节点是否在线，同时返回固件信息
  unsigned char SetBaudRate;  //设置节点波特率
  unsigned char Excute;       //执行固件
  //节点返回状态
  unsigned char CmdSuccess;   //命令执行成功
  unsigned char CmdFaild;     //命令执行失败
} CBL_CMD_LIST,*PCBL_CMD_LIST; 
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint32_t GetSector(uint32_t Address);
FLASH_Status CAN_BOOT_ErasePage(uint32_t StartPageAddr,uint32_t EndPageAddr);
uint16_t CAN_BOOT_GetAddrData(void);
void CAN_BOOT_ExecutiveCommand(CanRxMsg *pRxMessage);
void CAN_BOOT_JumpToApplication(__IO uint32_t Addr);

#endif
/*********************************END OF FILE**********************************/

