/*
 * main.c
 *
 *  Created on: 2017年4月16日
 *      Author: admin
 */
/**
 *整体的思路是:
 * 第一步:样子FLASH的数据写入和擦除;
 * 第二步:验证CAN总线接收数据;
 * 第三步:根据前面的步骤进行最后综合
 * 第一步验证FLASH功能函数基本结束;
 * 需要添加两个功能函数:从某个地址写入和从某个地址读出的函数
 ***************************************/
#include "main.h"
#include "BootLoader.h"
#include "LED.h"
int main(void)
{
	u8 i = 0;
	DINT;
	DRTM;
	InitSysCtrl();
	CAN_GPIO_Config(CANA);
	LED_GPIO_Config();
	
	CsmUnlock();
	InitPieCtrl();
	IER = 0x0000;	// Disable CPU interrupts and clear all CPU interrupt flags:
	IFR = 0x0000;
	InitPieVectTable();
	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	PieVectTable.ECAN1INTA = &Ecana_isr1;
	EDIS;
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd,&Flash28_API_RunStart);
	InitFlash();
	FlashAPI_Init();
	CAN_Config(CANA);
	CAN_Rx_Config();
	CAN_Rx_IT_Concig();
	//配置LED指示灯
	LED_Timer_Config();
	//------------------------------------
	//配置中断
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx6 = 1; //CANA的中断1
	IER |= M_INT9;
	IER |= M_INT1;
	__enable_irq();
	for(i = 0; i< 8;i++)
	{
		can_tx_msg.CAN_Tx_msg_data.msg_byte.data[i] =i;
	}
	can_tx_msg.CAN_num  = CANA;
	can_tx_msg.DLC = 5;
	can_tx_msg.ExtId.bit.ExtId = 0x1134522;
	can_tx_msg.ExtId.bit.resved = 0x00;
	can_tx_msg.IDE = CAN_ID_EXT;
	can_tx_msg.MBox_num = 0x01;
	can_tx_msg.SAE_J1939_Flag = 0;
	can_tx_msg.StdId.all = 0x00;
	can_tx_msg.Tx_timeout_cnt = 0x00;
	while (1)
	{
		if(updata_info.time_out_flag == 0)
		{
			if(can_rx_msg.rx_update == UPDATE)
			{
				if(CpuTimer0Regs.TCR.bit.TSS == 0)
				{
					CpuTimer0Regs.TCR.bit.TSS = 1;
				}
				updata_info.time_out_flag = 0;
				can_rx_msg.rx_update = NON_CHANGE;
				CAN_BOOT_ExecutiveCommand(&can_rx_msg);
				 GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
			}
		}
		else
		{
			CAN_BOOT_JumpToApplication(APP_START_ADDR);
		}

	}
}
