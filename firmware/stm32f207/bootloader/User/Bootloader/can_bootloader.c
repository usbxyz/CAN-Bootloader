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
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */ 
/* Private macro -------------------------------------------------------------*/
extern CBL_CMD_LIST CMD_List;
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }

  return sector;
}
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

	if(StartAddress<0x8004000){
		return FLASH_ERROR_PGS;
	}

  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
  for(i=0;i<DataNum;i++)
	{
    FLASHStatus = FLASH_ProgramByte(StartAddress, *pData);
    if (FLASHStatus == FLASH_COMPLETE){
      StartAddress++;
      pData++;
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
	FLASH_Status FLASHStatus=FLASH_COMPLETE;
	uint32_t StartSector, EndSector;
	uint32_t SectorCounter=0;
	if(StartPageAddr < 0x8004000){
		return FLASH_ERROR_PGA;
	}
  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
  /* Get the number of the start and end sectors */
  StartSector = GetSector(StartPageAddr);
  EndSector = GetSector(EndPageAddr);
  for (SectorCounter = StartSector; SectorCounter <= EndSector; SectorCounter += 8)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
		FLASHStatus = FLASH_EraseSector(SectorCounter, VoltageRange_3);
    if (FLASHStatus != FLASH_COMPLETE)
    { 
			/* Clear pending flags (if any) */  
			FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
											FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
			return FLASHStatus;
    }
  }
  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	return	FLASHStatus; 
}





/**
  * @brief  获取节点地址信息
  * @param  None
  * @retval 节点地址。
  */
uint16_t CAN_BOOT_GetAddrData(void)
{
	return 0x123;
}
/**
  * @brief  控制程序跳转到指定位置开始执行 。
  * @param  Addr 程序执行地址。
  * @retval 程序跳转状态。
  */
void CAN_BOOT_JumpToApplication(__IO uint32_t Addr)
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
		//CAN_ITConfig(CAN1,CAN_IT_FMP0, DISABLE);
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
void CAN_BOOT_ExecutiveCommand(CAN_TypeDef* CANx,CanRxMsg *pRxMessage)
{
	CanTxMsg TxMessage;
	uint8_t ret,i;
	uint8_t can_cmd = (pRxMessage->ExtId)&CMD_MASK;//ID的bit0~bit3位为命令码
	uint16_t can_addr = (pRxMessage->ExtId >> CMD_WIDTH);//ID的bit4~bit15位为节点地址
	uint32_t BaudRate;
	uint16_t crc_data;
	static uint32_t jump_addr;
	static uint32_t start_addr;
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
	switch (can_cmd)
	{
    //CMD_List.EraseFlash，擦除Flash中的数据，起始地址存储在Data[0]到Data[3]中，结束地址存储在Data[4]到Data[7]中
    //该命令必须在Bootloader程序中实现，在APP程序中可以不用实现
    case 0x03:
			__set_PRIMASK(1);
			FLASH_Unlock();
			ret = CAN_BOOT_ErasePage((pRxMessage->Data[0]<<24)|(pRxMessage->Data[1]<<16)|(pRxMessage->Data[2]<<8)|(pRxMessage->Data[3]<<0),
															 (pRxMessage->Data[4]<<24)|(pRxMessage->Data[5]<<16)|(pRxMessage->Data[6]<<8)|(pRxMessage->Data[7]<<0));
			FLASH_Lock();
      __set_PRIMASK(0);
			if(can_addr != 0x00){
				if(ret==FLASH_COMPLETE){
					TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.EraseFlash;
				}else{
					TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdFaild;
				}
				TxMessage.DLC = 0;
				CAN_WriteData(CANx,&TxMessage);
			}
			break;
    //CMD_List.SetBaudRate，设置节点波特率，具体波特率信息存储在Data[0]到Data[3]中
    //更改波特率后，适配器也需要更改为相同的波特率，否则不能正常通信
		case 0x04:
			BaudRate = (pRxMessage->Data[0]<<24)|(pRxMessage->Data[1]<<16)|(pRxMessage->Data[2]<<8)|(pRxMessage->Data[3]<<0);
			CAN_Configuration(CANx,BaudRate);
			if(can_addr != 0x00){
				TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.SetBaudRate;	
				TxMessage.DLC = 0;
				BOOT_DelayMs(20);
				CAN_WriteData(CANx,&TxMessage);
			}
			break;
    //CMD_List.BlockWriteInfo，设置写Flash数据的相关信息，比如数据起始地址，数据大小
    //数据起始地址存储在Data[0]到Data[3]中，数据大小存储在Data[4]到Data[7]中，该函数必须在Bootloader程序中实现，APP程序可以不用实现
		case 0x05:
			start_addr = (pRxMessage->Data[0]<<24)|(pRxMessage->Data[1]<<16)|(pRxMessage->Data[2]<<8)|(pRxMessage->Data[3]<<0);
			data_size = (pRxMessage->Data[4]<<24)|(pRxMessage->Data[5]<<16)|(pRxMessage->Data[6]<<8)|(pRxMessage->Data[7]<<0);
			data_index = 0;
			if(can_addr != 0x00){
				TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.BlockWriteInfo;
				TxMessage.DLC = 0;
				CAN_WriteData(CANx,&TxMessage);
			}
			break;
    //CMD_List.WriteBlockFlash，先将数据存储在本地缓冲区中，然后计算数据的CRC，若校验正确则写数据到Flash中
    //每次执行该数据，数据缓冲区的数据字节数会增加pRxMessage->DLC字节，当数据量达到data_size（包含2字节CRC校验码）字节后
    //对数据进行CRC校验，若数据校验无误，则将数据写入Flash中
    //该函数在Bootloader程序中必须实现，APP程序可以不用实现
		case 0x06:
			if((data_index<data_size)&&(data_index<1026)){
        __set_PRIMASK(1);
				for(i=0;i<pRxMessage->DLC;i++){
					data_temp[data_index++] = pRxMessage->Data[i];
				}
        __set_PRIMASK(0);
			}
			if((data_index>=data_size)||(data_index>=1026)){
				crc_data = crc16_ccitt(data_temp,data_size-2);
				if(crc_data==((data_temp[data_size-2]<<8)|(data_temp[data_size-1]))){
          __set_PRIMASK(1);
          FLASH_Unlock();
          ret = CAN_BOOT_ProgramDatatoFlash(start_addr,data_temp,data_index-2);
          FLASH_Lock();
          __set_PRIMASK(0);
					if(can_addr != 0x00){
						if(ret==FLASH_COMPLETE){
							TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.WriteBlockFlash;	
						}else{
							TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdFaild;
						}
						TxMessage.DLC = 0;
						CAN_WriteData(CANx,&TxMessage);
					}
				}else{
					TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.CmdFaild;
					TxMessage.DLC = 0;
					CAN_WriteData(CANx,&TxMessage);
				}
			}
			break;
    //CMD_List.OnlineCheck，节点在线检测
    //节点收到该命令后返回固件版本信息和固件类型，该命令在Bootloader程序和APP程序都必须实现
		case 0x01:
			if(can_addr != 0x00){
				TxMessage.ExtId = (CAN_BOOT_GetAddrData()<<CMD_WIDTH)|CMD_List.OnlineCheck;	
				TxMessage.Data[0] = 0;//主版本号，两字节
				TxMessage.Data[1] = 1;
				TxMessage.Data[2] = 0;//次版本号，两字节
				TxMessage.Data[3] = 0;
				TxMessage.Data[4] = 0;
				TxMessage.Data[5] = 0;
				TxMessage.Data[6] = 0;
				TxMessage.Data[7] = 0;//0：固件是Bootloader,1:固件是APP
				TxMessage.DLC = 8;
				CAN_WriteData(CANx,&TxMessage);
			}
			break;
    //CMD_List.ExcuteApp，控制程序跳转到指定地址执行
    //该命令在Bootloader和APP程序中都必须实现
		case 0x09:
			jump_addr = (pRxMessage->Data[0]<<24)|(pRxMessage->Data[1]<<16)|(pRxMessage->Data[2]<<8)|(pRxMessage->Data[3]<<0);
			if(jump_addr == APP_START_ADDRESS){	
				if(*((uint32_t *)EXE_APP_FLAG)!=0x12345678){
					uint32_t addr = EXE_APP_FLAG;
					uint32_t data = 0x12345678;
					CAN_BOOT_ErasePage(EXE_APP_FLAG,APP_START_ADDRESS-1);
					CAN_BOOT_ProgramDatatoFlash(addr,(uint8_t *)&data,4);//在Flash中写入标志数据，Bootloader程序在运行的时候可以判断该标志，若该标志存在则可以直接跳转到APP程序执行
					FLASH_Lock();
				}
			}
			if((*((uint32_t *)jump_addr)!=0xFFFFFFFF)){
				CAN_BOOT_JumpToApplication(jump_addr);	
			}
			//__set_PRIMASK(1);//关闭所有中断
			//NVIC_SystemReset();
			break;
		default:
			break;
	}
}
/*********************************END OF FILE**********************************/

