/*
 * main.c
 *
 *  Created on: 2017骞�4鏈�16鏃�
 *      Author: admin
 */
/*****************************************************************
 *鏁翠綋鐨勬�濊矾鏄�:
 * 绗竴姝�:鏍峰瓙FLASH鐨勬暟鎹啓鍏ュ拰鎿﹂櫎;
 * 绗簩姝�:楠岃瘉CAN鎬荤嚎鎺ユ敹鏁版嵁;
 * 绗笁姝�:鏍规嵁鍓嶉潰鐨勬楠よ繘琛屾渶鍚庣患鍚�
 * 绗竴姝ラ獙璇丗LASH鍔熻兘鍑芥暟鍩烘湰缁撴潫;
 * 闇�瑕佹坊鍔犱袱涓姛鑳藉嚱鏁�:浠庢煇涓湴鍧�鍐欏叆鍜屼粠鏌愪釜鍦板潃璇诲嚭鐨勫嚱鏁�
 *****************************************************************/
#include "main.h"
#include "BootLoader.h"
#include "LED.h"
int main(void)
{
	DINT;
	DRTM;
	InitSysCtrl();
	CAN_GPIO_Config(CANA);
	LED_GPIO_Config();
	CsmUnlock();
	InitPieCtrl();
	IER = 0x0000;
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
	/*
	FLASH_ST Flash_status;
	Uint16 status = 0x0001;
	status = Flash_Verify((Uint16*)APP_INFO_ADDR,app_check,3,&Flash_status);
	if(status == STATUS_SUCCESS)
	{
		CAN_BOOT_JumpToApplication(APP_START_ADDR);
	}
	*/
	CAN_Config(CANA,500);
	CAN_Rx_Config();
	CAN_Rx_IT_Concig();
	//閰嶇疆LED鎸囩ず鐏�
	LED_Timer_Config();
	//------------------------------------
	//閰嶇疆涓柇
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx6 = 1; //CANA鐨勪腑鏂�1
	IER |= M_INT9;
	IER |= M_INT1;
	__enable_irq();
	GpioDataRegs.GPADAT.bit.GPIO1 = 1;
	GpioDataRegs.GPADAT.bit.GPIO1 = 0;
	while (1)
	{
		if(updata_info.time_out_flag == 0)
		{
			if(can_rx_msg.rx_update == UPDATE)
			{
			    GpioDataRegs.GPADAT.bit.GPIO1 = 1;
				if(CpuTimer0Regs.TCR.bit.TSS == 0)
				{
					CpuTimer0Regs.TCR.bit.TSS = 1;
				}
				updata_info.time_out_flag = 0;
				can_rx_msg.rx_update = NON_CHANGE;
				CAN_BOOT_ExecutiveCommand(&can_rx_msg);
			}
		}
		else
		{
		    GpioDataRegs.GPADAT.bit.GPIO1 = 0;
		    GpioDataRegs.GPADAT.bit.GPIO0 = 0;
			CAN_BOOT_JumpToApplication(APP_START_ADDR);
		}
	}
}
