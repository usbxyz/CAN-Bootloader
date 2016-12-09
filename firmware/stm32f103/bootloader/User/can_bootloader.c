/**
  ******************************************************************************
  * @file    can_bootloader.c
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can_bootloader.h"
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
  * @param  StartPage 起始扇区地址
  * @param  EndPage 结束扇区地址
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
  * @brief  获取节点地址信息
  * @param  None
  * @retval 节点地址。
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
void CAN_BOOT_JumpToApplication(uint32_t Addr)
{
  static pFunction Jump_To_Application;
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
  uint8_t ret,i;
  uint8_t can_cmd = (pRxMessage->ExtId)&CMD_MASK;//ID的bit0~bit3位为命令码
  uint16_t can_addr = (pRxMessage->ExtId >> CMD_WIDTH);//ID的bit4~bit15位为节点地址
  uint32_t BaudRate;
  uint16_t crc_data;
  uint32_t addr_offset;
  uint32_t exe_type;
  uint32_t FlashSize;
  static uint32_t start_addr = APP_START_ADDR;
  static uint32_t data_size=0;
  static uint32_t data_index=0;
  __align(4) static uint8_t	data_temp[1028];
  //判断接收的数据地址是否和本节点地址匹配，若不匹配则直接返回，不做任何事情
  if((can_addr!=CAN_BOOT_GetAddrData())&&(can_addr!=0)){
    return;
  }
  TxMessage.DLC = 0;
  TxMessage.ExtId = 0;
  TxMessage.IDE = CAN_Id_Extended;
  TxMessage.RTR = CAN_RTR_Data;
  //CMD_List.Erase，擦除Flash中的数据，需要擦除的Flash大小存储在Data[0]到Data[3]中
  //该命令必须在Bootloader程序中实现，在APP程序中可以不用实现
  if(can_cmd == CMD_List.Erase){
    __set_PRIMASK(1);
    FLASH_Unlock();
    FlashSize = (pRxMessage->Data[0]<<24)|(pRxMessage->Data[1]<<16)|(pRxMessage->Data[2]<<8)|(pRxMessage->Data[3]<<0);
    ret = CAN_BOOT_ErasePage(APP_EXE_FLAG_START_ADDR,APP_START_ADDR+FlashSize);
    FLASH_Lock();	
    __set_PRIMASK(0);
    if(can_addr != 0x00){
      if(ret==FLASH_COMPLETE){
        TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdSuccess;
      }else{
        TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdFaild;
      }
      TxMessage.DLC = 0;
      CAN_WriteData(&TxMessage);
    }
    start_addr = APP_START_ADDR;
    return;
  }
  //CMD_List.SetBaudRate，设置节点波特率，具体波特率信息存储在Data[0]到Data[3]中
  //更改波特率后，适配器也需要更改为相同的波特率，否则不能正常通信
  if(can_cmd == CMD_List.SetBaudRate){
    __set_PRIMASK(1);
    BaudRate = (pRxMessage->Data[0]<<24)|(pRxMessage->Data[1]<<16)|(pRxMessage->Data[2]<<8)|(pRxMessage->Data[3]<<0);
    __set_PRIMASK(0);
    CAN_Configuration(BaudRate);
    if(can_addr != 0x00){
      TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdSuccess;
      TxMessage.DLC = 0;
      BOOT_DelayMs(20);
      CAN_WriteData(&TxMessage);
    }
    return;
  }
  //CMD_List.WriteInfo，设置写Flash数据的相关信息，比如数据起始地址，数据大小
  //数据偏移地址存储在Data[0]到Data[3]中，数据大小存储在Data[4]到Data[7]中，该函数必须在Bootloader程序中实现，APP程序可以不用实现
  if(can_cmd == CMD_List.WriteInfo){
    __set_PRIMASK(1);
    addr_offset = (pRxMessage->Data[0]<<24)|(pRxMessage->Data[1]<<16)|(pRxMessage->Data[2]<<8)|(pRxMessage->Data[3]<<0);
    start_addr = APP_START_ADDR+addr_offset;
    data_size = (pRxMessage->Data[4]<<24)|(pRxMessage->Data[5]<<16)|(pRxMessage->Data[6]<<8)|(pRxMessage->Data[7]<<0);
    data_index = 0;
    __set_PRIMASK(0);
    if(can_addr != 0x00){
      TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdSuccess;
      TxMessage.DLC = 0;
      CAN_WriteData(&TxMessage);
    }
  }
  //CMD_List.Write，先将数据存储在本地缓冲区中，然后计算数据的CRC，若校验正确则写数据到Flash中
  //每次执行该数据，数据缓冲区的数据字节数会增加pRxMessage->DLC字节，当数据量达到data_size（包含2字节CRC校验码）字节后
  //对数据进行CRC校验，若数据校验无误，则将数据写入Flash中
  //该函数在Bootloader程序中必须实现，APP程序可以不用实现
  if(can_cmd == CMD_List.Write){
    if((data_index<data_size)&&(data_index<1026)){
      __set_PRIMASK(1);
      for(i=0;i<pRxMessage->DLC;i++){
        data_temp[data_index++] = pRxMessage->Data[i];
      }
      __set_PRIMASK(0);
    }
    if((data_index>=data_size)||(data_index>=1026)){
      crc_data = crc16_ccitt(data_temp,data_size-2);//对接收到的数据做CRC校验，保证数据完整性
      if(crc_data==((data_temp[data_size-2]<<8)|(data_temp[data_size-1]))){
        __set_PRIMASK(1);
        FLASH_Unlock();
        ret = CAN_BOOT_ProgramDatatoFlash(start_addr,data_temp,data_size-2);
        FLASH_Lock();	
        __set_PRIMASK(0);
        if(can_addr != 0x00){
          if(ret==FLASH_COMPLETE){
            crc_data = crc16_ccitt((const unsigned char*)(start_addr),data_size-2);//再次对写入Flash中的数据进行CRC校验，确保写入Flash的数据无误
            if(crc_data!=((data_temp[data_size-2]<<8)|(data_temp[data_size-1]))){
              TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdFaild;
            }else{
              TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdSuccess;
            }
          }else{
            TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdFaild;
          }
          TxMessage.DLC = 0;
          CAN_WriteData(&TxMessage);
        }
      }else{
        TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdFaild;
        TxMessage.DLC = 0;
        CAN_WriteData(&TxMessage);
      }
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
    if(exe_type == CAN_BL_APP){
      if((*((uint32_t *)APP_START_ADDR)!=0xFFFFFFFF)){
        CAN_BOOT_JumpToApplication(APP_START_ADDR);
      }
    }
    return;
  }
}
/*********************************END OF FILE**********************************/

