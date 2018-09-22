/*
 * BootLoader.h
 *
 *  Created on: 2017年4月19日
 *      Author: admin
 */

#ifndef BOOTLOADER_BOOTLOADER_H_
#define BOOTLOADER_BOOTLOADER_H_
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "data.h"
#include "delay.h"
#include "stdint.h"
#include "CANA.h"
#include "Flash.h"
#define APP_START_ADDR  ((uint32_t)0x310000)
#define CAN_BL_APP      0xAAAAAAAA
#define CAN_BL_BOOT     0x55555555
//#define FW_TYPE         CAN_BL_BOOT
#define	DATA_PACK_SIZE	(1024+2)//包含2字节CRC16校验
#define DEVICE_ADDR 0x134//设备地址
#define CAN_BOOT_GetAddrData() DEVICE_ADDR
#define CMD_WIDTH 4
#define ADDR_WIDTH 12
typedef struct _Device_INFO
{
	union
	{
		unsigned short int all;
		struct
		{
			unsigned short int Device_addr:	12;
			unsigned short int reserve:	4;
		}bits;//设备地址
	}Device_addr;
	unsigned long int FW_TYPE;//固件类型
	unsigned long int FW_Version;//固件版本
}Device_INFO;
typedef struct _bootloader_data
{
	union
	{
		u32 all;
		struct
		{
			u16 cmd :CMD_WIDTH; //命令
			u16 addr :ADDR_WIDTH; //设备地址
			u16 reserve :16; //保留位
		} bit;
	} ExtId; //扩展帧ID
	unsigned char IDE;   //帧类型,可为：CAN_ID_STD(标准帧),CAN_ID_EXT(扩展帧)
	unsigned char DLC;  //数据长度，可为0到8;
	u8 data[8];
} bootloader_data;
typedef struct _Boot_CMD_LIST
{
	//Bootloader相关命令
	unsigned char Erase;        //擦出APP储存扇区数据
	unsigned char Write;        //以多字节形式写数据
	unsigned char Check;        //检测节点是否在线，同时返回固件信息
	unsigned char Excute;       //执行固件
	unsigned char WriteInfo;    //设置多字节写数据相关参数（写起始地址，数据量）
	unsigned char SetBaudRate;  //设置节点波特率
	//节点返回状态,关键
	unsigned char CmdFaild;     //命令执行失败
	unsigned char CmdSuccess;   //命令执行成功


} Boot_CMD_LIST;
extern Boot_CMD_LIST cmd_list;
extern bootloader_data Bootloader_data;
extern Device_INFO DEVICE_INFO;
void __disable_irq(void);
void __enable_irq(void);
void __set_PRIMASK(u8 state);

unsigned short int CRCcalc16 (unsigned char *data,unsigned short int len);
void CAN_BOOT_JumpToApplication(uint32_t Addr);
void CAN_BOOT_ExecutiveCommand(CanRxMsg *pRxMessage);
#endif /* BOOTLOADER_BOOTLOADER_H_ */
