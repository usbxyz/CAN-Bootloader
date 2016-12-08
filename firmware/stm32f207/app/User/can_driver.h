/**
  ******************************************************************************
  * @file    can.h
  * $Author: wdluo $
  * $Revision: 17 $
  * $Date:: 2012-07-06 11:16:48 +0800 #$
  * @brief   CAN总线收发函数.
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
#ifndef __CAN_DRIVER_H
#define __CAN_DRIVER_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
/* Functions -----------------------------------------------------------------*/
void CAN_Configuration(CAN_TypeDef* CANx,uint32_t BaudRate);
uint8_t CAN_WriteData(CAN_TypeDef* CANx,CanTxMsg *TxMessage);
void CAN_ConfigFilter(CAN_TypeDef* CANx,uint8_t FilterNumber,uint16_t can_addr);
void BOOT_DelayMs(uint32_t ms);
#endif /*__CAN_H */

/***********************************文件结束***********************************/
