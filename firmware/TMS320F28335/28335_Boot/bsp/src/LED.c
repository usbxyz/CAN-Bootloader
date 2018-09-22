/*
 * LED.c
 *
 *  Created on: 2017Äê5ÔÂ21ÈÕ
 *      Author: admin
 */
#include "LED.h"
UPDATA_INFO updata_info =
{
		.time_cnt = 0,
		.time_out_flag = 0,
};
void LED_GPIO_Config(void)
{
	   EALLOW;
		   GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
		   GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;
		   GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0x00;
		   GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;
		   GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;
		   GpioCtrlRegs.GPBDIR.bit.GPIO58 = 1;
		   GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;
		   GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;
		   GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
		   GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
	   EDIS;
	   GpioDataRegs.GPBCLEAR.all = 0xFFFFFFFF;
}
void LED_Timer_Config(void)
{
	EALLOW;
		SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1;
	EDIS;
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0, 150, 100000);//250ms
	CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	 CpuTimer0.InterruptCount = 0;


}
interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
   if(updata_info.time_out_flag == 0)
   {
	   if( CpuTimer0.InterruptCount == 10)
	     {
	  	   updata_info.time_cnt++;
	  	   CpuTimer0.InterruptCount = 0;
	  	   if(updata_info.time_cnt == 10)
	  	   {
	  		   updata_info.time_out_flag = 1;
	  	   }
	     }
   }

   GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1; // Toggle GPIO32 once per 500 milliseconds
   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


