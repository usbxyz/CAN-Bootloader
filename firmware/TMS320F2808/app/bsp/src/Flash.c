/*
 * Flash.c
 *
 *  Created on: 2017年4月16日
 *      Author: admin
 */
#include "Flash.h"
void FlashAPI_Init(void)
{
	EALLOW;
	Flash_CPUScaleFactor = SCALE_FACTOR;
	Flash_CallbackPtr = NULL;
	EDIS;
}
#pragma CODE_SECTION(Flash_RD,"ramfuncs");
Uint16 Flash_RD(Uint32 addr, Uint16 *buffer, Uint16 leng)
{
	Uint16 i;
	Uint32 addr_temp = addr;
	for (i = 0; i < leng; i++)
	{
		*buffer = *(volatile Uint16*) addr_temp;
		buffer++;
		addr_temp += 1;
	}
	return STATUS_SUCCESS;

}
#pragma CODE_SECTION(Flash_WR,"ramfuncs");
Uint16 Flash_WR(Uint32 addr, Uint16 *buffer, Uint16 leng)
{
	Uint16 *addr_temp = 0x00;
	addr_temp = (Uint16*) addr;
	FLASH_ST Flash_status;
	Uint16 status = 0x0001;
	status = Flash_Program(addr_temp, buffer, leng, &Flash_status);
	if (status != STATUS_SUCCESS)
	{
		return status;
	}
	status = 0x0001;
	status = Flash_Verify(addr_temp, buffer, leng, &Flash_status);
	if (status != STATUS_SUCCESS)
	{
		return status;
	}
	return STATUS_SUCCESS;
}
