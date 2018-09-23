/*
 * LED.c
 *
 *  Created on: 2017年5月21日
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
       GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
	   EDIS;
	   GpioDataRegs.GPACLEAR.all = 0xFFFFFFFF;
}
void LED_Timer_Config(void)
{
	EALLOW;
	//*	SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;
	EDIS;
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0, 150, 50000);//250ms
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

   GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1;  // Toggle GPIO32 once per 500 milliseconds
   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


