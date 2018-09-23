#include "relay.h"
void ReLay_GPIO_Config(void)
{
	EALLOW;
	GpioCtrlRegs.GPAPUD.bit.GPIO28 = GPIO_PuPd_UP;
	GpioCtrlRegs.GPAPUD.bit.GPIO26 = GPIO_PuPd_UP;
	GpioCtrlRegs.GPADIR.bit.GPIO26 = GPIO_Mode_OUT;
	GpioCtrlRegs.GPADIR.bit.GPIO28 = GPIO_Mode_OUT;
	EDIS;
	GpioDataRegs.GPASET.bit.GPIO28 = 1;
	GpioDataRegs.GPASET.bit.GPIO26 = 1;
}
void Relay_ON(void)
{
	GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;//低电平有效
	GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;
}
void Relay_OFF(void)
{
	GpioDataRegs.GPASET.bit.GPIO28 = 1;
	GpioDataRegs.GPASET.bit.GPIO26 = 1;
}
