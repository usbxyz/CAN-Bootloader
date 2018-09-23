// TI File $Revision: /main/6 $
// Checkin $Date: December 17, 2007   13:13:48 $
//###########################################################################
//
// FILE:   DSP280x_Device.h
//
// TITLE:  DSP280x Device Definitions.
//
//###########################################################################
// $TI Release: DSP280x C/C++ Header Files V1.70 $
// $Release Date: July 27, 2009 $
//###########################################################################

#ifndef DSP280x_DEVICE_H
#define DSP280x_DEVICE_H


#ifdef __cplusplus
extern "C" {
#endif


#define   TARGET   1
//---------------------------------------------------------------------------
// User To Select Target Device:

#define   DSP28_28015  0
#define   DSP28_28016  0
#define   DSP28_2809   0
#define   DSP28_2808   TARGET
#define   DSP28_2806   0
#define   DSP28_2802   0
#define   DSP28_2801   0

//---------------------------------------------------------------------------
// Common CPU Definitions:
//

extern cregister volatile unsigned int IFR;
extern cregister volatile unsigned int IER;

#define  EINT   asm(" clrc INTM")
#define  DINT   asm(" setc INTM")
#define  ERTM   asm(" clrc DBGM")
#define  DRTM   asm(" setc DBGM")
#define  EALLOW asm(" EALLOW")
#define  EDIS   asm(" EDIS")
#define  ESTOP0 asm(" ESTOP0")

#define M_INT1  0x0001
#define M_INT2  0x0002
#define M_INT3  0x0004
#define M_INT4  0x0008
#define M_INT5  0x0010
#define M_INT6  0x0020
#define M_INT7  0x0040
#define M_INT8  0x0080
#define M_INT9  0x0100
#define M_INT10 0x0200
#define M_INT11 0x0400
#define M_INT12 0x0800
#define M_INT13 0x1000
#define M_INT14 0x2000
#define M_DLOG  0x4000
#define M_RTOS  0x8000

#define BIT0    0x0001
#define BIT1    0x0002
#define BIT2    0x0004
#define BIT3    0x0008
#define BIT4    0x0010
#define BIT5    0x0020
#define BIT6    0x0040
#define BIT7    0x0080
#define BIT8    0x0100
#define BIT9    0x0200
#define BIT10   0x0400
#define BIT11   0x0800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000



//---------------------------------------------------------------------------
// For Portability, User Is Recommended To Use Following Data Type Size
// Definitions For 16-bit and 32-Bit Signed/Unsigned Integers:
//

#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
typedef int                 int16;
typedef long                int32;
typedef long long           int64;
typedef unsigned int        Uint16;
typedef unsigned long       Uint32;
typedef unsigned long long  Uint64;
typedef float               float32;
typedef long double         float64;
#endif


//---------------------------------------------------------------------------
// Include All Peripheral Header Files:
//

#include "DSP280x_Adc.h"                // ADC Registers
#include "DSP280x_DevEmu.h"             // Device Emulation Registers
#include "DSP280x_CpuTimers.h"          // 32-bit CPU Timers
#include "DSP280x_ECan.h"               // Enhanced eCAN Registers
#include "DSP280x_ECap.h"               // Enhanced Capture
#include "DSP280x_EPwm.h"               // Enhanced PWM
#include "DSP280x_EQep.h"               // Enhanced QEP
#include "DSP280x_Gpio.h"               // General Purpose I/O Registers
#include "DSP280x_I2c.h"                // I2C Registers
#include "DSP280x_PieCtrl.h"            // PIE Control Registers
#include "DSP280x_PieVect.h"            // PIE Vector Table
#include "DSP280x_Spi.h"                // SPI Registers
#include "DSP280x_Sci.h"                // SCI Registers
#include "DSP280x_SysCtrl.h"            // System Control/Power Modes
#include "DSP280x_XIntrupt.h"           // External Interrupts

#if DSP28_2809 || DSP28_2808
#define DSP28_EPWM1 1
#define DSP28_EPWM2 1
#define DSP28_EPWM3 1
#define DSP28_EPWM4 1
#define DSP28_EPWM5 1
#define DSP28_EPWM6 1
#define DSP28_ECAP1 1
#define DSP28_ECAP2 1
#define DSP28_ECAP3 1
#define DSP28_ECAP4 1
#define DSP28_EQEP1 1
#define DSP28_EQEP2 1
#define DSP28_ECANA 1
#define DSP28_ECANB 1
#define DSP28_SPIA  1
#define DSP28_SPIB  1
#define DSP28_SPIC  1
#define DSP28_SPID  1
#define DSP28_SCIA  1
#define DSP28_SCIB  1
#define DSP28_I2CA  1
#endif  // end DSP28_2809 || DSP_2808


#if DSP28_2806
#define DSP28_EPWM1 1
#define DSP28_EPWM2 1
#define DSP28_EPWM3 1
#define DSP28_EPWM4 1
#define DSP28_EPWM5 1
#define DSP28_EPWM6 1
#define DSP28_ECAP1 1
#define DSP28_ECAP2 1
#define DSP28_ECAP3 1
#define DSP28_ECAP4 1
#define DSP28_EQEP1 1
#define DSP28_EQEP2 1
#define DSP28_ECANA 1
#define DSP28_ECANB 0
#define DSP28_SPIA  1
#define DSP28_SPIB  1
#define DSP28_SPIC  1
#define DSP28_SPID  1
#define DSP28_SCIA  1
#define DSP28_SCIB  1
#define DSP28_I2CA  1
#endif  // end DSP28_2806


#if DSP28_2802 || DSP28_2801
#define DSP28_EPWM1 1
#define DSP28_EPWM2 1
#define DSP28_EPWM3 1
#define DSP28_EPWM4 0
#define DSP28_EPWM5 0
#define DSP28_EPWM6 0
#define DSP28_ECAP1 1
#define DSP28_ECAP2 1
#define DSP28_ECAP3 0
#define DSP28_ECAP4 0
#define DSP28_EQEP1 1
#define DSP28_EQEP2 0
#define DSP28_ECANA 1
#define DSP28_ECANB 0
#define DSP28_SPIA  1
#define DSP28_SPIB  1
#define DSP28_SPIC  0
#define DSP28_SPID  0
#define DSP28_SCIA  1
#define DSP28_SCIB  0
#define DSP28_I2CA  1
#endif  // end DSP28_2802 || DSP_2801


#if DSP28_28016
#define DSP28_EPWM1 1
#define DSP28_EPWM2 1
#define DSP28_EPWM3 1
#define DSP28_EPWM4 1
#define DSP28_EPWM5 0
#define DSP28_EPWM6 0
#define DSP28_ECAP1 1
#define DSP28_ECAP2 1
#define DSP28_ECAP3 0
#define DSP28_ECAP4 0
#define DSP28_EQEP1 0
#define DSP28_EQEP2 0
#define DSP28_ECANA 1
#define DSP28_ECANB 0
#define DSP28_SPIA  1
#define DSP28_SPIB  0
#define DSP28_SPIC  0
#define DSP28_SPID  0
#define DSP28_SCIA  1
#define DSP28_SCIB  0
#define DSP28_I2CA  1
#endif  // end DSP28_28016

#if DSP28_28015
#define DSP28_EPWM1 1
#define DSP28_EPWM2 1
#define DSP28_EPWM3 1
#define DSP28_EPWM4 1
#define DSP28_EPWM5 0
#define DSP28_EPWM6 0
#define DSP28_ECAP1 1
#define DSP28_ECAP2 1
#define DSP28_ECAP3 0
#define DSP28_ECAP4 0
#define DSP28_EQEP1 0
#define DSP28_EQEP2 0
#define DSP28_ECANA 0
#define DSP28_ECANB 0
#define DSP28_SPIA  1
#define DSP28_SPIB  0
#define DSP28_SPIC  0
#define DSP28_SPID  0
#define DSP28_SCIA  1
#define DSP28_SCIB  0
#define DSP28_I2CA  1
#endif  // end DSP28_28015



#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP280x_DEVICE_H definition


//===========================================================================
// End of file.
//===========================================================================
