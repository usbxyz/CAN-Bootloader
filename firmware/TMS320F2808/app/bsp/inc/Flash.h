/*
 * Flash.h
 *
 *  Created on: 2017年4月16日
 *      Author: admin
 */

#ifndef BSP_INC_FLASH_H_
#define BSP_INC_FLASH_H_
#include "include.h"
#include "data.h"
#include "delay.h"
#include "stdint.h"
#include "Flash280x_API_Library.h"
void FlashAPI_Init(void);
Uint16 Flash_RD(Uint32 addr, Uint16 *buffer, Uint16 leng);

Uint16 Flash_WR(Uint32 addr, Uint16 *buffer, Uint16 leng);
#endif /* BSP_INC_FLASH_H_ */
