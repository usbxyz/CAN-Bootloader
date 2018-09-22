/*
 * CAN_APP.c
 *
 *  Created on: 2017年5月21日
 *      Author: admin
 */
#include "CAN_APP.h"
typedef  void (*pFunction)(void);

Boot_CMD_LIST cmd_list =
{
	.Erase = 0x00,      //擦除APP区域数据
	.WriteInfo = 0x01,  //设置多字节写数据相关参数（写起始地址，数据量）
	.Write = 0x02,      //以多字节形式写数据
	.Check = 0x03,      //检测节点是否在线，同时返回固件信息
	.SetBaudRate = 0x04,//设置节点波特率
	.Excute = 0x05,     //执行固件
	.CmdSuccess = 0x08, //命令执行成功
	.CmdFaild = 0x09,   //命令执行失败
};

Device_INFO DEVICE_INFO =
{
	.FW_TYPE = CAN_BL_APP,
	.FW_Version = 0x0010001,
};

void __disable_irq(void)
{
	DINT;
	DRTM;
}
void __enable_irq(void)
{
	EINT;
	ERTM;
}
void __set_PRIMASK(u8 state)
{
	if(state == 1)
	{
		__disable_irq();
	}
	else if(state == 0)
	{
		__enable_irq();
	}
	else
	{
		return;
	}
}

void CAN_BOOT_JumpToApplication(uint32_t Addr)
{
	(*((void(*)(void))(Addr)))();
}

#pragma CODE_SECTION(CAN_BOOT_ExecutiveCommand,"ramfuncs");
void CAN_BOOT_ExecutiveCommand(CanRxMsg *pRxMessage)
{
	bootloader_data Boot_ID_info;
	u8 can_cmd    = 0x00;//ID的bit0~bit3位为命令码
	u16 can_addr  = 0x00;//ID的bit4~bit15位为节点地址
	uint32_t exe_type = 0x00;
	CanTxMsg TxMessage;//发送对应消息
	TxMessage.CAN_num = CANA;
	TxMessage.DLC = 1;
	TxMessage.ExtId.bit.resved = 0x00;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.MBox_num = 0x03;
	TxMessage.Tx_timeout_cnt = 0x00;
	TxMessage.SAE_J1939_Flag = 0;
	//获取地址信息
	Boot_ID_info.ExtId.all = pRxMessage->ExtId;
	can_cmd  = Boot_ID_info.ExtId.bit.cmd;
	can_addr = Boot_ID_info.ExtId.bit.addr;
	//判断接收的数据地址是否和本节点地址匹配，若不匹配则直接返回，不做任何事情
	if((can_addr!=CAN_BOOT_GetAddrData())&&(can_addr!=0)){
		return;
	}
	//CMD_List.Check，节点在线检测
	//节点收到该命令后返回固件版本信息和固件类型，
	//该命令在Bootloader程序和APP程序都必须实现
	if(can_cmd == cmd_list.Check)//DSP中尚未实现,相对比较容易实现,主要是为实现APP再次更新应用程序
	{
		if(can_addr != 0x00)
		{
			DEVICE_INFO.Device_addr.bits.Device_addr = CAN_BOOT_GetAddrData();
			TxMessage.ExtId.bit.ExtId = (DEVICE_INFO.Device_addr.bits.Device_addr<<CMD_WIDTH)|cmd_list.CmdSuccess;
			TxMessage.CAN_Tx_msg_data.msg_byte.data[0] = (u8)(DEVICE_INFO.FW_Version>>24);;//主版本号，两字节
			TxMessage.CAN_Tx_msg_data.msg_byte.data[1] = (u8)(DEVICE_INFO.FW_Version>>16);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[2] = (u8)(DEVICE_INFO.FW_Version>>8);//次版本号，两字节
			TxMessage.CAN_Tx_msg_data.msg_byte.data[3] = (u8)(DEVICE_INFO.FW_Version>>0);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[4] = (u8)(DEVICE_INFO.FW_TYPE>>24);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[5] = (u8)(DEVICE_INFO.FW_TYPE>>16);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[6] = (u8)(DEVICE_INFO.FW_TYPE>>8);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[7] = (u8)(DEVICE_INFO.FW_TYPE>>0);
			TxMessage.DLC = 8;
			CAN_Tx_Msg(&TxMessage);
		}
	}
	//CMD_List.Excute，控制程序跳转到指定地址执行
	//该命令在Bootloader和APP程序中都必须实现
	if(can_cmd == cmd_list.Excute)//该命令在DSP中已经实现
	{
		exe_type  = (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[0])&0xFFFFFFFF)<<24)|\
					(((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[1])&0x00FFFFFF)<<16)|\
					(((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[2])&0x0000FFFF)<<8)|\
					(((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[3])&0x000000FF)<<0);
		if(exe_type == CAN_BL_BOOT)
		{
			if((*((uint32_t *)BOOT_START_ADDR)!=0xFFFFFFFF))
			{
				CAN_BOOT_JumpToApplication(BOOT_START_ADDR);
			}
		}
		return;
	}
	return;

}


