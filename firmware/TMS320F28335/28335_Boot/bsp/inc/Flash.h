/*
 * Flash.h
 *
 *  Created on: 2017Äê4ÔÂ16ÈÕ
 *      Author: admin
 */

#ifndef BSP_INC_FLASH_H_
#define BSP_INC_FLASH_H_
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "data.h"
#include "delay.h"
#include "stdint.h"
#include "Flash2833x_API_Library.h"
void FlashAPI_Init(void);
Uint16 Flash_RD(Uint32 addr, Uint16 *buffer, Uint16 leng);

Uint16 Flash_WR(Uint32 addr, Uint16 *buffer, Uint16 leng);
#endif /* BSP_INC_FLASH_H_ */
