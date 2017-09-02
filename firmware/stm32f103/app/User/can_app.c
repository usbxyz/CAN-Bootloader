/**
  ******************************************************************************
  * @file    can_app.c
  * $Author: wdluo $
  * $Revision: 17 $
  * $Date:: 2012-07-06 11:16:48 +0800 #$
  * @brief   基于CAN总线的Bootloader程序APP测试程序.
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
#include "crc16.h"
/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Private define ------------------------------------------------------------*/

/* Base address of the Flash sectors */
#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
 #define PAGE_SIZE                         (0x400)    /* 1 Kbyte */
 #define FLASH_SIZE                        (0x20000)  /* 128 KBytes */
#elif defined STM32F10X_CL
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x40000)  /* 256 KBytes */
#elif defined STM32F10X_HD
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x80000)  /* 512 KBytes */
#elif defined STM32F10X_XL
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x100000) /* 1 MByte */
#else 
 #error "Please select first the STM32 device to be used (in stm32f10x.h)"    
#endif  
/* Private macro -------------------------------------------------------------*/
extern CBL_CMD_LIST CMD_List;
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  将数据烧写到指定地址的Flash中 。
  * @param  Address Flash起始地址。
  * @param  Data 数据存储区起始地址。
  * @param  DataNum 数据字节数。
  * @retval 数据烧写状态。
  */
FLASH_Status CAN_BOOT_ProgramDatatoFlash(uint32_t StartAddress,uint8_t *pData,uint32_t DataNum) 
{
	FLASH_Status FLASHStatus = FLASH_COMPLETE;

	uint32_t i;

	if(StartAddress<APP_EXE_FLAG_START_ADDR){
		return FLASH_ERROR_PG;
	}
   /* Clear All pending flags */
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
	
  for(i=0;i<(DataNum>>2);i++)
	{
    FLASHStatus = FLASH_ProgramWord(StartAddress, *((uint32_t*)pData));
		if (FLASHStatus == FLASH_COMPLETE){
      StartAddress += 4;
      pData += 4;
    }else{ 
			return FLASHStatus;
    }
  }
	return	FLASHStatus;
}
/**
  * @brief  擦出指定扇区区间的Flash数据 。
  * @param  StartPage 起始扇区
  * @param  EndPage 结束扇区
  * @retval 扇区擦出状态  
  */
FLASH_Status CAN_BOOT_ErasePage(uint32_t StartPageAddr,uint32_t EndPageAddr)
{
  uint32_t i;
  FLASH_Status FLASHStatus=FLASH_COMPLETE;

  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

  for(i=StartPageAddr;i<=EndPageAddr;i+=PAGE_SIZE){
    FLASHStatus = FLASH_ErasePage(i);
    if(FLASHStatus!=FLASH_COMPLETE){
      FLASH_Lock();
      return	FLASHStatus;	
    }
  }
  FLASH_Lock();
  return FLASHStatus;
}





/**
  * @brief  从指定的Flash区读取一个字的数据
  * @param  Address 开始读取数据的地址。
	* @param  pData 数据储存地址。
  * @retval 程序跳转状态。
  */
uint16_t CAN_BOOT_GetAddrData(void)
{
  return Read_CAN_Address();
}
/**
  * @brief  控制程序跳转到指定位置开始执行 。
  * @param  Addr 程序执行地址。
  * @retval 程序跳转状态。
  */
void CAN_BOOT_JumpToApplication(__IO uint32_t Addr)
{
	pFunction Jump_To_Application;
	__IO uint32_t JumpAddress; 
	/* Test if user code is programmed starting from address "ApplicationAddress" */
	if (((*(__IO uint32_t*)Addr) & 0x2FFE0000 ) == 0x20000000)
	{ 
	  /* Jump to user application */
	  JumpAddress = *(__IO uint32_t*) (Addr + 4);
	  Jump_To_Application = (pFunction) JumpAddress;
		__set_PRIMASK(1);//关闭所有中断
	  /* Initialize user application's Stack Pointer */
	  __set_MSP(*(__IO uint32_t*)Addr);
	  Jump_To_Application();
	}
}


/**
  * @brief  执行主机下发的命令
  * @param  pRxMessage CAN总线消息
  * @retval 无
  */
void CAN_BOOT_ExecutiveCommand(CanRxMsg *pRxMessage)
{
  CanTxMsg TxMessage;
  uint8_t can_cmd = (pRxMessage->ExtId)&CMD_MASK;//ID的bit0~bit3位为命令码
  uint16_t can_addr = (pRxMessage->ExtId >> CMD_WIDTH);//ID的bit4~bit15位为节点地址
  uint32_t BaudRate;
  uint32_t exe_type;
  //判断接收的数据地址是否和本节点地址匹配，若不匹配则直接返回，不做任何事情
  if((can_addr!=CAN_BOOT_GetAddrData())&&(can_addr!=0)){
    return;
  }
  TxMessage.DLC = 0;
  TxMessage.ExtId = 0;
  TxMessage.IDE = CAN_Id_Extended;
  TxMessage.RTR = CAN_RTR_Data;
  
  //CMD_List.SetBaudRate，设置节点波特率，具体波特率信息存储在Data[0]到Data[3]中
  //更改波特率后，适配器也需要更改为相同的波特率，否则不能正常通信
  if(can_cmd == CMD_List.SetBaudRate){
    BaudRate = (pRxMessage->Data[0]<<24)|(pRxMessage->Data[1]<<16)|(pRxMessage->Data[2]<<8)|(pRxMessage->Data[3]<<0);
    CAN_Configuration(BaudRate);
    if(can_addr != 0x00){
      TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdSuccess;
      TxMessage.DLC = 0;
      BOOT_DelayMs(20);
      CAN_WriteData(&TxMessage);
    }
    return;
  }
  //CMD_List.Check，节点在线检测
  //节点收到该命令后返回固件版本信息和固件类型，该命令在Bootloader程序和APP程序都必须实现
  if(can_cmd == CMD_List.Check){
    if(can_addr != 0x00){
      TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdSuccess;
      TxMessage.Data[0] = 0;//主版本号，两字节
      TxMessage.Data[1] = 1;
      TxMessage.Data[2] = 0;//次版本号，两字节
      TxMessage.Data[3] = 0;
      TxMessage.Data[4] = (uint8_t)(FW_TYPE>>24);
      TxMessage.Data[5] = (uint8_t)(FW_TYPE>>16);
      TxMessage.Data[6] = (uint8_t)(FW_TYPE>>8);
      TxMessage.Data[7] = (uint8_t)(FW_TYPE>>0);
      TxMessage.DLC = 8;
      CAN_WriteData(&TxMessage);
    }
    return;
  }
  //CMD_List.Excute，控制程序跳转到指定地址执行
  //该命令在Bootloader和APP程序中都必须实现
  if(can_cmd == CMD_List.Excute){
    exe_type = (pRxMessage->Data[0]<<24)|(pRxMessage->Data[1]<<16)|(pRxMessage->Data[2]<<8)|(pRxMessage->Data[3]<<0);
    if(exe_type == CAN_BL_BOOT){
      FLASH_Unlock();
      CAN_BOOT_ErasePage(APP_EXE_FLAG_START_ADDR,APP_EXE_FLAG_START_ADDR);//擦除写入到Flash中的APP执行标志，复位运行后，即可执行Bootloader程序
      FLASH_Lock();
      __set_PRIMASK(1);//关闭所有中断
      NVIC_SystemReset();
    }
    return;
  }
}
/*********************************END OF FILE**********************************/

