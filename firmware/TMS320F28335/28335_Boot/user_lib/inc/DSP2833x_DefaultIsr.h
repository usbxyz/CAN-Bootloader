//###########################################################################
//
// FILE:    DSP2833x_DefaultIsr.h
//
// TITLE:   DSP2833x Devices Default Interrupt Service Routines Definitions.
//
//###########################################################################
// $TI Release: 2833x/2823x Header Files and Peripheral Examples V133 $
// $Release Date: June 8, 2012 $
//###########################################################################

#ifndef DSP2833x_DEFAULT_ISR_H
#define DSP2833x_DEFAULT_ISR_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// Default Interrupt Service Routine Declarations:
// 
// The following function prototypes are for the 
// default ISR routines used with the default PIE vector table.
// This default vector table is found in the DSP2833x_PieVect.h 
// file.  
//

// Non-Peripheral Interrupts:
interrupt void INT13_ISR(void);     // XINT13 or CPU-Timer 1
interrupt void INT14_ISR(void);     // CPU-Timer2
interrupt void DATALOG_ISR(void);   // Datalogging interrupt
interrupt void RTOSINT_ISR(void);   // RTOS interrupt
interrupt void EMUINT_ISR(void);    // Emulation interrupt
interrupt void NMI_ISR(void);       // Non-maskable interrupt
interrupt void ILLEGAL_ISR(void);   // Illegal operation TRAP
interrupt void USER1_ISR(void);     // User Defined trap 1
interrupt void USER2_ISR(void);     // User Defined trap 2
interrupt void USER3_ISR(void);     // User Defined trap 3
interrupt void USER4_ISR(void);     // User Defined trap 4
interrupt void USER5_ISR(void);     // User Defined trap 5
interrupt void USER6_ISR(void);     // User Defined trap 6
interrupt void USER7_ISR(void);     // User Defined trap 7
interrupt void USER8_ISR(void);     // User Defined trap 8
interrupt void USER9_ISR(void);     // User Defined trap 9
interrupt void USER10_ISR(void);    // User Defined trap 10
interrupt void USER11_ISR(void);    // User Defined trap 11
interrupt void USER12_ISR(void);    // User Defined trap 12

// Group 1 PIE Interrupt Service Routines:
interrupt void  SEQ1INT_ISR(void);   // ADC Sequencer 1 ISR
interrupt void  SEQ2INT_ISR(void);   // ADC Sequencer 2 ISR
interrupt void  XINT1_ISR(void);     // External interrupt 1
interrupt void  XINT2_ISR(void);     // External interrupt 2
interrupt void  ADCINT_ISR(void);    // ADC
interrupt void  TINT0_ISR(void);     // Timer 0
interrupt void  WAKEINT_ISR(void);   // WD

// Group 2 PIE Interrupt Service Routines:
interrupt void EPWM1_TZINT_ISR(void);    // EPWM-1
interrupt void EPWM2_TZINT_ISR(void);    // EPWM-2
interrupt void EPWM3_TZINT_ISR(void);    // EPWM-3
interrupt void EPWM4_TZINT_ISR(void);    // EPWM-4
interrupt void EPWM5_TZINT_ISR(void);    // EPWM-5
interrupt void EPWM6_TZINT_ISR(void);    // EPWM-6
      
// Group 3 PIE Interrupt Service Routines:
interrupt void EPWM1_INT_ISR(void);    // EPWM-1
interrupt void EPWM2_INT_ISR(void);    // EPWM-2
interrupt void EPWM3_INT_ISR(void);    // EPWM-3
interrupt void EPWM4_INT_ISR(void);    // EPWM-4
interrupt void EPWM5_INT_ISR(void);    // EPWM-5
interrupt void EPWM6_INT_ISR(void);    // EPWM-6
      
// Group 4 PIE Interrupt Service Routines:
interrupt void ECAP1_INT_ISR(void);    // ECAP-1
interrupt void ECAP2_INT_ISR(void);    // ECAP-2
interrupt void ECAP3_INT_ISR(void);    // ECAP-3
interrupt void ECAP4_INT_ISR(void);    // ECAP-4
interrupt void ECAP5_INT_ISR(void);    // ECAP-5
interrupt void ECAP6_INT_ISR(void);    // ECAP-6
     
// Group 5 PIE Interrupt Service Routines:
interrupt void EQEP1_INT_ISR(void);    // EQEP-1
interrupt void EQEP2_INT_ISR(void);    // EQEP-2

// Group 6 PIE Interrupt Service Routines:
interrupt void SPIRXINTA_ISR(void);   // SPI-A
interrupt void SPITXINTA_ISR(void);   // SPI-A
interrupt void MRINTA_ISR(void);      // McBSP-A
interrupt void MXINTA_ISR(void);      // McBSP-A
interrupt void MRINTB_ISR(void);      // McBSP-B
interrupt void MXINTB_ISR(void);      // McBSP-B

// Group 7 PIE Interrupt Service Routines:
interrupt void DINTCH1_ISR(void);     // DMA-Channel 1
interrupt void DINTCH2_ISR(void);     // DMA-Channel 2
interrupt void DINTCH3_ISR(void);     // DMA-Channel 3
interrupt void DINTCH4_ISR(void);     // DMA-Channel 4
interrupt void DINTCH5_ISR(void);     // DMA-Channel 5
interrupt void DINTCH6_ISR(void);     // DMA-Channel 6

// Group 8 PIE Interrupt Service Routines: 
interrupt void I2CINT1A_ISR(void);     // I2C-A
interrupt void I2CINT2A_ISR(void);     // I2C-A
interrupt void SCIRXINTC_ISR(void);    // SCI-C
interrupt void SCITXINTC_ISR(void);    // SCI-C
   
// Group 9 PIE Interrupt Service Routines:
interrupt void SCIRXINTA_ISR(void);    // SCI-A
interrupt void SCITXINTA_ISR(void);    // SCI-A
interrupt void SCIRXINTB_ISR(void);    // SCI-B
interrupt void SCITXINTB_ISR(void);    // SCI-B
interrupt void ECAN0INTA_ISR(void);    // eCAN-A
interrupt void ECAN1INTA_ISR(void);    // eCAN-A
interrupt void ECAN0INTB_ISR(void);    // eCAN-B
interrupt void ECAN1INTB_ISR(void);    // eCAN-B

// Group 10 PIE Interrupt Service Routines:
 
// Group 11 PIE Interrupt Service Routines: 

// Group 12 PIE Interrupt Service Routines: 
interrupt void  XINT3_ISR(void);     // External interrupt 3
interrupt void  XINT4_ISR(void);     // External interrupt 4
interrupt void  XINT5_ISR(void);     // External interrupt 5
interrupt void  XINT6_ISR(void);     // External interrupt 6
interrupt void  XINT7_ISR(void);     // External interrupt 7
interrupt void  LVF_ISR(void);       // Latched overflow flag
interrupt void  LUF_ISR(void);       // Latched underflow flag

// Catch-all for Reserved Locations For testing purposes:
interrupt void PIE_RESERVED(void);       // Reserved for test
interrupt void rsvd_ISR(void);           // for test
interrupt void INT_NOTUSED_ISR(void);    // for unused interrupts

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif    // end of DSP2833x_DEFAULT_ISR_H definition

//===========================================================================
// End of file.
//===========================================================================
