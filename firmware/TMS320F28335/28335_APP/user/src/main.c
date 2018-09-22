#include "CAN_APP.h"
#include "LED.h"
void main(void)
{
	InitSysCtrl();
	LED_GPIO_Config();
	CAN_GPIO_Config(CANA);
	DINT;
	DRTM;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
	MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);
	InitFlash();
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	PieVectTable.ECAN1INTA = &Ecana_isr1;
	EDIS;
	LED_Timer_Config();

	CAN_Config(CANA);
	CAN_Rx_Config();
	CAN_Rx_IT_Concig();
	//------------------------------------
	//≈‰÷√÷–∂œ
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx6 = 1; //CANAµƒ÷–∂œ1
	IER |= M_INT9;
	IER |= M_INT1;
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM
   while(1)
   {
	   if(can_rx_msg.rx_update == UPDATE)
	   		{
	   			can_rx_msg.rx_update = NON_CHANGE;
	   			CAN_BOOT_ExecutiveCommand(&can_rx_msg);
	   		}
   }
}
