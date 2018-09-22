// TI File $Revision: /main/4 $
// Checkin $Date: July 2, 2007   09:24:03 $
//###########################################################################
//
// FILE:   DSP280x_SysCtrl.h
//
// TITLE:  DSP280x Device System Control Register Definitions.
//
//###########################################################################
// $TI Release: DSP280x C/C++ Header Files V1.70 $
// $Release Date: July 27, 2009 $
//###########################################################################

#ifndef DSP280x_SYS_CTRL_H
#define DSP280x_SYS_CTRL_H


#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// System Control Individual Register Bit Definitions:
//

// XCLKOUT Control
struct XCLK_BITS   {    // bits  description
   Uint16 XCLKOUTDIV:2; // 1:0   XCLKOUT Divide Ratio
   Uint16 XCLKOUTDAT:1; // 2     reserved for TI use only
   Uint16 X1DAT:1;      // 3     reserved for TI use only
   Uint16 XCLKINDAT:1;  // 4     reserved for TI use only
   Uint16 rsvd1:3;      // 7:5   reserved
   Uint16 X1CNT:4;      // 11:8  reserved for TI use only
   Uint16 XCLKINCNT:4;  // 15:12 reserved for TI use only
};

union XCLK_REG {
   Uint16              all;
   struct XCLK_BITS    bit;
};

// PLL Status Register
struct PLLSTS_BITS   {    // bits  description
   Uint16 PLLLOCKS:1;     // 0     PLL lock status
   Uint16 CLKINDIV:1;     // 1     CLKIN div by 2
   Uint16 PLLOFF:1;       // 2     PLL off bit
   Uint16 MCLKSTS:1;      // 3     Missing clock status bit
   Uint16 MCLKCLR:1;      // 4     Missing clock clear bit
   Uint16 OSCOFF:1;       // 5     Oscillator clock off
   Uint16 MCLKOFF:1;      // 6     Missing clock detect
   Uint16 rsvd2:9;        // 15:7  reserved
};

union PLLSTS_REG {
   Uint16              all;
   struct PLLSTS_BITS  bit;
};

// High speed peripheral clock register bit definitions:
struct HISPCP_BITS  {   // bits  description
   Uint16 HSPCLK:3;     // 2:0   Rate relative to SYSCLKOUT
   Uint16 rsvd1:13;     // 15:3  reserved
};

union HISPCP_REG {
   Uint16              all;
   struct HISPCP_BITS  bit;
};

// Low speed peripheral clock register bit definitions:
struct LOSPCP_BITS  {   // bits  description
   Uint16 LSPCLK:3;     // 2:0   Rate relative to SYSCLKOUT
   Uint16 rsvd1:13;     // 15:3  reserved
};

union LOSPCP_REG {
   Uint16              all;
   struct LOSPCP_BITS  bit;
};

// Peripheral clock control register 0 bit definitions:
struct PCLKCR0_BITS  {  // bits  description
   Uint16 rsvd1:2;      // 1:0   reserved
   Uint16 TBCLKSYNC:1;  // 2     EWPM Module TBCLK enable/sync
   Uint16 ADCENCLK:1;   // 3     Enable high speed clk to ADC
   Uint16 I2CAENCLK:1;  // 4     Enable SYSCLKOUT to I2C-A
   Uint16 rsvd2:1;      // 5     reserved
   Uint16 SPICENCLK:1;  // 6     Enable low speed clk to SPI-C
   Uint16 SPIDENCLK:1;  // 7     Enable low speed clk to SPI-D
   Uint16 SPIAENCLK:1;  // 8     Enable low speed clk to SPI-A
   Uint16 SPIBENCLK:1;  // 9     Enable low speed clk to SPI-B
   Uint16 SCIAENCLK:1;  // 10    Enable low speed clk to SCI-A
   Uint16 SCIBENCLK:1;  // 11    Enable low speed clk to SCI-B
   Uint16 rsvd3:2;      // 13:12 reserved
   Uint16 ECANAENCLK:1; // 14    Enable system clk to eCAN-A
   Uint16 ECANBENCLK:1; // 15    Enable system clk to eCAN-B
};

union PCLKCR0_REG {
   Uint16              all;
   struct PCLKCR0_BITS bit;
};

// Peripheral clock control register 1 bit definitions:
struct PCLKCR1_BITS  {    // bits  description
   Uint16 EPWM1ENCLK:1;   // 0     Enable SYSCLKOUT to EPWM1
   Uint16 EPWM2ENCLK:1;   // 1     Enable SYSCLKOUT to EPWM2
   Uint16 EPWM3ENCLK:1;   // 2     Enable SYSCLKOUT to EPWM3
   Uint16 EPWM4ENCLK:1;   // 3     Enable SYSCLKOUT to EPWM4
   Uint16 EPWM5ENCLK:1;   // 4     Enable SYSCLKOUT to EPWM5
   Uint16 EPWM6ENCLK:1;   // 5     Enable SYSCLKOUT to EPWM6
   Uint16 rsvd1:2;        // 7:6   reserved
   Uint16 ECAP1ENCLK:1;   // 8     Enable SYSCLKOUT to ECAP1
   Uint16 ECAP2ENCLK:1;   // 9     Enable SYSCLKOUT to ECAP2
   Uint16 ECAP3ENCLK:1;   // 10    Enable SYSCLKOUT to ECAP3
   Uint16 ECAP4ENCLK:1;   // 11    Enable SYSCLKOUT to ECAP4
   Uint16 rsvd2:2;        // 13:12 reserved
   Uint16 EQEP1ENCLK:1;   // 11    Enable SYSCLKOUT to EQEP1
   Uint16 EQEP2ENCLK:1;   // 11    Enable SYSCLKOUT to EQEP2
};

union PCLKCR1_REG {
   Uint16              all;
   struct PCLKCR1_BITS bit;
};


// PLL control register bit definitions:
struct PLLCR_BITS {      // bits  description
   Uint16 DIV:4;         // 3:0   Set clock ratio for the PLL
   Uint16 rsvd1:12;      // 15:4  reserved
};

union PLLCR_REG {
   Uint16             all;
   struct PLLCR_BITS  bit;
};

// Low Power Mode 0 control register bit definitions:
struct LPMCR0_BITS {     // bits  description
   Uint16 LPM:2;         // 1:0   Set the low power mode
   Uint16 QUALSTDBY:6;   // 7:2   Qualification
   Uint16 rsvd1:7;       // 14:8  reserved
   Uint16 WDINTE:1;      // 15    Enables WD to wake the device from STANDBY
};

union LPMCR0_REG {
   Uint16              all;
   struct LPMCR0_BITS  bit;
};

//---------------------------------------------------------------------------
// System Control Register File:
//
struct SYS_CTRL_REGS {
   union   XCLK_REG    XCLK;      // 0
   union   PLLSTS_REG  PLLSTS;    // 1
   Uint16              rsvd1[8];  // 2-9
   union   HISPCP_REG  HISPCP;    // 10: High-speed peripheral clock pre-scaler
   union   LOSPCP_REG  LOSPCP;    // 11: Low-speed peripheral clock pre-scaler
   union   PCLKCR0_REG PCLKCR0;   // 12: Peripheral clock control register
   union   PCLKCR1_REG PCLKCR1;   // 13: Peripheral clock control register
   union   LPMCR0_REG  LPMCR0;    // 14: Low-power mode control register 0
   Uint16              rsvd2;     // 15: reserved
   Uint16              rsvd3;     // 16: reserved
   union   PLLCR_REG   PLLCR;     // 17: PLL control register
   // No bit definitions are defined for SCSR because
   // a read-modify-write instruction can clear the WDOVERRIDE bit
   Uint16              SCSR;      // 18: System control and status register
   Uint16              WDCNTR;    // 19: WD counter register
   Uint16              rsvd4;     // 20
   Uint16              WDKEY;     // 21: WD reset key register
   Uint16              rsvd5[3];  // 22-24
   // No bit definitions are defined for WDCR because
   // the proper value must be written to the WDCHK field
   // whenever writing to this register.
   Uint16              WDCR;      // 25: WD timer control register
   Uint16              rsvd6[6];  // 26-31
};


/* --------------------------------------------------- */
/* CSM Registers                                       */
/*                                                     */
/* ----------------------------------------------------*/

/* CSM Status & Control register bit definitions */
struct  CSMSCR_BITS {      // bit   description
   Uint16     SECURE:1;    // 0     Secure flag
   Uint16     rsvd1:14;    // 14-1  reserved
   Uint16     FORCESEC:1;  // 15    Force Secure control bit

};

/* Allow access to the bit fields or entire register */
union CSMSCR_REG {
   Uint16             all;
   struct CSMSCR_BITS bit;
};

/* CSM Register File */
struct  CSM_REGS {
   Uint16           KEY0;    // KEY reg bits 15-0
   Uint16           KEY1;    // KEY reg bits 31-16
   Uint16           KEY2;    // KEY reg bits 47-32
   Uint16           KEY3;    // KEY reg bits 63-48
   Uint16           KEY4;    // KEY reg bits 79-64
   Uint16           KEY5;    // KEY reg bits 95-80
   Uint16           KEY6;    // KEY reg bits 111-96
   Uint16           KEY7;    // KEY reg bits 127-112
   Uint16           rsvd1;   // reserved
   Uint16           rsvd2;   // reserved
   Uint16           rsvd3;   // reserved
   Uint16           rsvd4;   // reserved
   Uint16           rsvd5;   // reserved
   Uint16           rsvd6;   // reserved
   Uint16           rsvd7;   // reserved
   union CSMSCR_REG CSMSCR;  // CSM Status & Control register
};

/* Password locations */
struct  CSM_PWL {
   Uint16   PSWD0;  // PSWD bits 15-0
   Uint16   PSWD1;  // PSWD bits 31-16
   Uint16   PSWD2;  // PSWD bits 47-32
   Uint16   PSWD3;  // PSWD bits 63-48
   Uint16   PSWD4;  // PSWD bits 79-64
   Uint16   PSWD5;  // PSWD bits 95-80
   Uint16   PSWD6;  // PSWD bits 111-96
   Uint16   PSWD7;  // PSWD bits 127-112
};



/* Flash Registers */

#define FLASH_SLEEP   0x0000;
#define FLASH_STANDBY 0x0001;
#define FLASH_ACTIVE  0x0003;


/* Flash Option Register bit definitions */
struct  FOPT_BITS {       // bit   description
   Uint16     ENPIPE:1;   // 0     Enable Pipeline Mode
   Uint16     rsvd:15;    // 1-15  reserved
};

/* Allow access to the bit fields or entire register */
union FOPT_REG {
   Uint16           all;
   struct FOPT_BITS bit;
};

/* Flash Power Modes Register bit definitions */
struct  FPWR_BITS {       // bit   description
   Uint16     PWR:2;      // 0-1   Power Mode bits
   Uint16     rsvd:14;    // 2-15  reserved
};

/* Allow access to the bit fields or entire register */
union FPWR_REG {
   Uint16           all;
   struct FPWR_BITS bit;
};


/* Flash Status Register bit definitions */
struct  FSTATUS_BITS {       // bit   description
   Uint16     PWRS:2;        // 0-1   Power Mode Status bits
   Uint16     STDBYWAITS:1;  // 2     Bank/Pump Sleep to Standby Wait Counter Status bits
   Uint16     ACTIVEWAITS:1; // 3     Bank/Pump Standby to Active Wait Counter Status bits
   Uint16     rsvd1:4;       // 4-7   reserved
   Uint16     V3STAT:1;      // 8     VDD3V Status Latch bit
   Uint16     rsvd2:7;       // 9-15  reserved
};

/* Allow access to the bit fields or entire register */
union FSTATUS_REG {
   Uint16              all;
   struct FSTATUS_BITS bit;
};

/* Flash Sleep to Standby Wait Counter Register bit definitions */
struct  FSTDBYWAIT_BITS {    // bit   description
   Uint16     STDBYWAIT:9;   // 0-8   Bank/Pump Sleep to Standby Wait Count bits
   Uint16     rsvd:7;        // 9-15  reserved
};

/* Allow access to the bit fields or entire register */
union FSTDBYWAIT_REG {
   Uint16                 all;
   struct FSTDBYWAIT_BITS bit;
};

/* Flash Standby to Active Wait Counter Register bit definitions */
struct  FACTIVEWAIT_BITS {   // bit   description
   Uint16     ACTIVEWAIT:9;  // 0-8   Bank/Pump Standby to Active Wait Count bits
   Uint16     rsvd:7;        // 9-15  reserved
};

/* Allow access to the bit fields or entire register */
union FACTIVEWAIT_REG {
   Uint16                  all;
   struct FACTIVEWAIT_BITS bit;
};

/* Bank Read Access Wait State Register bit definitions */
struct  FBANKWAIT_BITS {     // bit   description
   Uint16     RANDWAIT:4;    // 0-3   Flash Random Read Wait State bits
   Uint16     rsvd1:4;       // 4-7   reserved
   Uint16     PAGEWAIT:4;    // 8-11  Flash Paged Read Wait State bits
   Uint16     rsvd2:4;       // 12-15 reserved
};

/* Allow access to the bit fields or entire register */
union FBANKWAIT_REG {
   Uint16                all;
   struct FBANKWAIT_BITS bit;
};

/* OTP Read Access Wait State Register bit definitions */
struct  FOTPWAIT_BITS {      // bit   description
   Uint16     OTPWAIT:5;     // 0-4   OTP Read Wait State bits
   Uint16     rsvd:11;       // 5-15  reserved
};

/* Allow access to the bit fields or entire register */
union FOTPWAIT_REG {
   Uint16               all;
   struct FOTPWAIT_BITS bit;
};


struct FLASH_REGS {
   union FOPT_REG        FOPT;        // Option Register
   Uint16                rsvd1;       // reserved
   union FPWR_REG        FPWR;        // Power Modes Register
   union FSTATUS_REG     FSTATUS;     // Status Register
   union FSTDBYWAIT_REG  FSTDBYWAIT;  // Pump/Bank Sleep to Standby Wait State Register
   union FACTIVEWAIT_REG FACTIVEWAIT; // Pump/Bank Standby to Active Wait State Register
   union FBANKWAIT_REG   FBANKWAIT;   // Bank Read Access Wait State Register
   union FOTPWAIT_REG    FOTPWAIT;    // OTP Read Access Wait State Register
};

//---------------------------------------------------------------------------
// System Control External References & Function Declarations:
//
extern volatile struct SYS_CTRL_REGS SysCtrlRegs;
extern volatile struct CSM_REGS CsmRegs;
extern volatile struct CSM_PWL CsmPwl;
extern volatile struct FLASH_REGS FlashRegs;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP280x_SYS_CTRL_H definition

//===========================================================================
// End of file.
//===========================================================================

