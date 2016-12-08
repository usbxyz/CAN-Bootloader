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
#ifndef __CAN_APP_H
#define __CAN_APP_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
typedef  struct  _CBL_CMD_LIST{
	uint8_t EraseFlash;			//擦出APP储存扇区数据
	uint8_t	WriteBlockFlash;	//以多字节形式写数据
	uint8_t	ReadBlockFlash;		//以多字节形式读数据
	uint8_t BlockWriteInfo;		//设置多字节写数据相关参数（写起始地址，数据量）
	uint8_t BlockReadInfo;		//设置多字节数据读相关参数（读起始地址，数据量）
	uint8_t	OnlineCheck;		//检测节点是否在线
	uint8_t	CmdSuccess;			//命令执行成功
	uint8_t	CmdFaild;			//命令执行失败
	uint8_t	SetBaudRate;		//设置节点波特率
	uint8_t	ExcuteApp;			//执行应用程序
	uint8_t	Undefine0;			//未定义
	uint8_t	Undefine1;			//未定义
	uint8_t	Undefine2;			//未定义
	uint8_t	Undefine3;			//未定义
} CBL_CMD_LIST,*PCBL_CMD_LIST; 
/* Private define ------------------------------------------------------------*/
#define CMD_WIDTH   4
#define CMD_MASK    0xF
#define ADDR_MASK   0x1FFFFFF

#define EXE_APP_FLAG					((uint32_t)0x08004000)
#define APP_START_ADDRESS			((uint32_t)0x08008000)	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

uint16_t CAN_BOOT_GetAddrData(void);
void CAN_BOOT_ExecutiveCommand(CAN_TypeDef* CANx,CanRxMsg *pRxMessage);
void CAN_BOOT_JumpToApplication(uint32_t Addr);
FLASH_Status CAN_BOOT_ProgramDatatoFlash(uint32_t StartAddress,uint8_t *pData,uint32_t DataNum);

#endif
/*********************************END OF FILE**********************************/

