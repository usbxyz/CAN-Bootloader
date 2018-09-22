// TI File $Revision: /main/7 $
// Checkin $Date: September 9, 2005   13:59:40 $
//###########################################################################
//
// FILE:   DSP280x_EPwm.h
//
// TITLE:  DSP280x Enhanced PWM Module Register Bit Definitions.
//
//###########################################################################
// $TI Release: DSP280x C/C++ Header Files V1.70 $
// $Release Date: July 27, 2009 $
//###########################################################################

#ifndef DSP280x_EPWM_H
#define DSP280x_EPWM_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
// Time base control register bit definitions */                                    
struct TBCTL_BITS {          // bits   description
   Uint16 CTRMODE:2;         // 1:0    Counter Mode
   Uint16 PHSEN:1;           // 2      Phase load enable
   Uint16 PRDLD:1;           // 3      Active period load
   Uint16 SYNCOSEL:2;        // 5:4    Sync output select
   Uint16 SWFSYNC:1;         // 6      Software force sync pulse
   Uint16 HSPCLKDIV:3;       // 9:7    High speed time pre-scale
   Uint16 CLKDIV:3;          // 12:10  Timebase clock pre-scale
   Uint16 PHSDIR:1;          // 13     Phase Direction
   Uint16 FREE_SOFT:2;       // 15:14  Emulation mode 
};

union TBCTL_REG {
   Uint16              all;
   struct TBCTL_BITS   bit;
};

//----------------------------------------------------
// Time base status register bit definitions */                                    
struct TBSTS_BITS {          // bits   description
   Uint16 CTRDIR:1;          // 0      Counter direction status
   Uint16 SYNCI:1;           // 1      External input sync status
   Uint16 CTRMAX:1;          // 2      Counter max latched status
   Uint16 rsvd1:13;          // 15:3   reserved
};

union TBSTS_REG {
   Uint16              all;
   struct TBSTS_BITS   bit;
};

//----------------------------------------------------
// Compare control register bit definitions */                                    
struct CMPCTL_BITS {          // bits   description
   Uint16 LOADAMODE:2;        // 0:1    Active compare A
   Uint16 LOADBMODE:2;        // 3:2    Active compare B
   Uint16 SHDWAMODE:1;        // 4      Compare A block operating mode
   Uint16 rsvd1:1;            // 5      reserved
   Uint16 SHDWBMODE:1;        // 6      Compare B block operating mode
   Uint16 rsvd2:1;            // 7      reserved
   Uint16 SHDWAFULL:1;        // 8      Compare A Shadow registers full Status
   Uint16 SHDWBFULL:1;        // 9      Compare B Shadow registers full Status
   Uint16 rsvd3:6;            // 15:10  reserved
};


union CMPCTL_REG {
   Uint16                all;
   struct CMPCTL_BITS    bit;
};

//----------------------------------------------------
// Action qualifier register bit definitions */                                    
struct AQCTL_BITS {           // bits   description
   Uint16 ZRO:2;              // 1:0    Action Counter = Zero
   Uint16 PRD:2;              // 3:2    Action Counter = Period
   Uint16 CAU:2;              // 5:4    Action Counter = Compare A up
   Uint16 CAD:2;              // 7:6    Action Counter = Compare A down
   Uint16 CBU:2;              // 9:8    Action Counter = Compare B up
   Uint16 CBD:2;              // 11:10  Action Counter = Compare B down
   Uint16 rsvd:4;             // 15:12  reserved
};

union AQCTL_REG {
   Uint16                all;
   struct AQCTL_BITS     bit;
};

//----------------------------------------------------
// Action qualifier SW force register bit definitions */                                    
struct AQSFRC_BITS {           // bits   description
   Uint16 ACTSFA:2;            // 1:0    Action when One-time SW Force A invoked
   Uint16 OTSFA:1;             // 2      One-time SW Force A output
   Uint16 ACTSFB:2;            // 4:3    Action when One-time SW Force B invoked
   Uint16 OTSFB:1;             // 5      One-time SW Force A output
   Uint16 RLDCSF:2;            // 7:6    Reload from Shadow options
   Uint16 rsvd1:8;             // 15:8   reserved
};

union AQSFRC_REG {
   Uint16                 all;
   struct AQSFRC_BITS     bit;
};

//----------------------------------------------------
// Action qualifier continuous SW force register bit definitions */                                    
struct AQCSFRC_BITS {          // bits   description
   Uint16 CSFA:2;              // 1:0    Continuous Software Force on output A
   Uint16 CSFB:2;              // 3:2    Continuous Software Force on output B
   Uint16 rsvd1:12;            // 15:4   reserved
};

union AQCSFRC_REG {
   Uint16                  all;
   struct AQCSFRC_BITS     bit;
};


// As of version 1.1
// Changed the MODE bit-field to OUT_MODE
// Added the bit-field IN_MODE
// This corresponds to changes in silicon as of F280x devices
// Rev A silicon.
//----------------------------------------------------
// Dead-band generator control register bit definitions                                    
struct DBCTL_BITS {          // bits   description
   Uint16 OUT_MODE:2;      	 // 1:0    Dead Band Output Mode Control 
   Uint16 POLSEL:2;          // 3:2    Polarity Select Control 
   Uint16 IN_MODE:2;         // 5:4    Dead Band Input Select Mode Control
   Uint16 rsvd1:10;          // 15:4   reserved
};

union DBCTL_REG {
   Uint16                  all;
   struct DBCTL_BITS       bit;
};


//----------------------------------------------------
// Trip zone select register bit definitions                                   
struct TZSEL_BITS {           // bits   description
   Uint16  CBC1:1;            // 0      TZ1 CBC select
   Uint16  CBC2:1;            // 1      TZ2 CBC select
   Uint16  CBC3:1;            // 2      TZ3 CBC select
   Uint16  CBC4:1;            // 3      TZ4 CBC select
   Uint16  CBC5:1;            // 4      TZ5 CBC select
   Uint16  CBC6:1;            // 5      TZ6 CBC select
   Uint16  rsvd1:2;           // 7:6    reserved
   Uint16  OSHT1:1;           // 8      One-shot TZ1 select             
   Uint16  OSHT2:1;           // 9      One-shot TZ2 select             
   Uint16  OSHT3:1;           // 10     One-shot TZ3 select             
   Uint16  OSHT4:1;           // 11     One-shot TZ4 select             
   Uint16  OSHT5:1;           // 12     One-shot TZ5 select             
   Uint16  OSHT6:1;           // 13     One-shot TZ6 select             
   Uint16  rsvd2:2;           // 15:14  reserved
};

union TZSEL_REG {
   Uint16                  all;
   struct TZSEL_BITS       bit;
};


//----------------------------------------------------
// Trip zone control register bit definitions */                                    
struct TZCTL_BITS {         // bits   description
   Uint16 TZA:2;            // 1:0    TZ1 to TZ6 Trip Action On EPWMxA
   Uint16 TZB:2;            // 3:2    TZ1 to TZ6 Trip Action On EPWMxB
   Uint16 rsvd:12;          // 15:4   reserved
};

union TZCTL_REG {
   Uint16                  all;
   struct TZCTL_BITS       bit;
};


//----------------------------------------------------
// Trip zone control register bit definitions */                                    
struct TZEINT_BITS {         // bits   description
   Uint16  rsvd1:1;          // 0      reserved
   Uint16  CBC:1;            // 1      Trip Zones Cycle By Cycle Int Enable
   Uint16  OST:1;            // 2      Trip Zones One Shot Int Enable
   Uint16  rsvd2:13;         // 15:3   reserved
};   


union TZEINT_REG {
   Uint16                  all;
   struct TZEINT_BITS      bit;
};


//----------------------------------------------------
// Trip zone flag register bit definitions */                                    
struct TZFLG_BITS {         // bits   description
   Uint16  INT:1;           // 0      Global status
   Uint16  CBC:1;           // 1      Trip Zones Cycle By Cycle Int
   Uint16  OST:1;           // 2      Trip Zones One Shot Int
   Uint16  rsvd2:13;        // 15:3   reserved
};

union TZFLG_REG {
   Uint16                  all;
   struct TZFLG_BITS       bit;
};

//----------------------------------------------------
// Trip zone flag clear register bit definitions */                                    
struct TZCLR_BITS {         // bits   description
   Uint16  INT:1;           // 0      Global status
   Uint16  CBC:1;           // 1      Trip Zones Cycle By Cycle Int
   Uint16  OST:1;           // 2      Trip Zones One Shot Int
   Uint16  rsvd2:13;        // 15:3   reserved
};

union TZCLR_REG {
   Uint16                  all;
   struct TZCLR_BITS       bit;
};

//----------------------------------------------------
// Trip zone flag force register bit definitions */                                    
struct TZFRC_BITS {         // bits   description
   Uint16  rsvd1:1;         // 0      reserved
   Uint16  CBC:1;           // 1      Trip Zones Cycle By Cycle Int
   Uint16  OST:1;           // 2      Trip Zones One Shot Int
   Uint16  rsvd2:13;        // 15:3   reserved
};

union TZFRC_REG {
   Uint16                  all;
   struct TZFRC_BITS       bit;
};

//----------------------------------------------------
// Event trigger select register bit definitions */                                    
struct ETSEL_BITS {         // bits   description
   Uint16  INTSEL:3;        // 2:0    EPWMxINTn Select
   Uint16  INTEN:1;         // 3      EPWMxINTn Enable
   Uint16  rsvd1:4;         // 7:4    reserved
   Uint16  SOCASEL:3;       // 10:8   Start of conversion A Select
   Uint16  SOCAEN:1;        // 11     Start of conversion A Enable
   Uint16  SOCBSEL:3;       // 14:12  Start of conversion B Select
   Uint16  SOCBEN:1;        // 15     Start of conversion B Enable
};

union ETSEL_REG {
   Uint16                  all;
   struct ETSEL_BITS       bit;
};


//----------------------------------------------------
// Event trigger pre-scale register bit definitions */                                    
struct ETPS_BITS {         // bits   description
   Uint16  INTPRD:2;       // 1:0    EPWMxINTn Period Select
   Uint16  INTCNT:2;       // 3:2    EPWMxINTn Counter Register
   Uint16  rsvd1:4;        // 7:4    reserved
   Uint16  SOCAPRD:2;      // 9:8    EPWMxSOCA Period Select
   Uint16  SOCACNT:2;      // 11:10  EPWMxSOCA Counter Register
   Uint16  SOCBPRD:2;      // 13:12  EPWMxSOCB Period Select
   Uint16  SOCBCNT:2;      // 15:14  EPWMxSOCB Counter Register
};

union ETPS_REG {
   Uint16                  all;
   struct ETPS_BITS        bit;
};

//----------------------------------------------------
// Event trigger Flag register bit definitions */                                    
struct ETFLG_BITS {         // bits   description
   Uint16  INT:1;           // 0	EPWMxINTn Flag
   Uint16  rsvd1:1;         // 1	reserved
   Uint16  SOCA:1;          // 2	EPWMxSOCA Flag
   Uint16  SOCB:1;          // 3	EPWMxSOCB Flag
   Uint16  rsvd2:12;        // 15:4	reserved
};

union ETFLG_REG {
   Uint16                   all;
   struct ETFLG_BITS        bit;
};


//----------------------------------------------------
// Event trigger Clear register bit definitions */                                    
struct ETCLR_BITS {         // bits   description
   Uint16  INT:1;           // 0	EPWMxINTn Clear
   Uint16  rsvd1:1;         // 1	reserved
   Uint16  SOCA:1;          // 2	EPWMxSOCA Clear
   Uint16  SOCB:1;          // 3	EPWMxSOCB Clear
   Uint16  rsvd2:12;        // 15:4	reserved
};

union ETCLR_REG {
   Uint16                   all;
   struct ETCLR_BITS        bit;
};

//----------------------------------------------------
// Event trigger Force register bit definitions */                                    
struct ETFRC_BITS {         // bits   description
   Uint16  INT:1;           // 0	EPWMxINTn Force
   Uint16  rsvd1:1;         // 1	reserved
   Uint16  SOCA:1;          // 2	EPWMxSOCA Force
   Uint16  SOCB:1;          // 3	EPWMxSOCB Force
   Uint16  rsvd2:12;        // 15:4	reserved
};

union ETFRC_REG {
   Uint16                  all;
   struct ETFRC_BITS        bit;
};
//----------------------------------------------------
// PWM chopper control register bit definitions */                                    
struct PCCTL_BITS {         // bits   description
   Uint16  CHPEN:1;         // 0      PWM chopping enable
   Uint16  OSHTWTH:4;       // 4:1    One-shot pulse width
   Uint16  CHPFREQ:3;       // 7:5    Chopping clock frequency
   Uint16  CHPDUTY:3;       // 10:8   Chopping clock Duty cycle
   Uint16  rsvd1:5;         // 15:11  reserved
};


union PCCTL_REG {
   Uint16                  all;
   struct PCCTL_BITS       bit;
};

struct HRCNFG_BITS {       	// bits   description
   Uint16  EDGMODE:2;     	// 1:0    Edge Mode select Bits
   Uint16  CTLMODE:1;     	// 2      Control mode Select Bit
   Uint16  HRLOAD:1;      	// 3      Shadow mode Select Bit
   Uint16  rsvd1:12;      	// 15:4   reserved
};

union HRCNFG_REG {
   Uint16                  	all;
   struct HRCNFG_BITS       bit;
};


struct TBPHS_HRPWM_REG {   	// bits   description
   Uint16  TBPHSHR;     	// 15:0   Extension register for HRPWM Phase (8 bits)
   Uint16  TBPHS;           // 31:16  Phase offset register
};

union TBPHS_HRPWM_GROUP {
   Uint32                  all;
   struct TBPHS_HRPWM_REG  half;
};

struct CMPA_HRPWM_REG {   	// bits   description
   Uint16  CMPAHR;     	    // 15:0   Extension register for HRPWM compare (8 bits)
   Uint16  CMPA;            // 31:16  Compare A reg
};

union CMPA_HRPWM_GROUP {
   Uint32                 all;
   struct CMPA_HRPWM_REG  half;
};


struct EPWM_REGS {
   union  TBCTL_REG           TBCTL;   // 
   union  TBSTS_REG           TBSTS;   // 
   union  TBPHS_HRPWM_GROUP   TBPHS;   // Union of TBPHS:TBPHSHR
   Uint16                     TBCTR;   // Counter
   Uint16                     TBPRD;   // Period register set 
   Uint16                     rsvd1;   // 
   union  CMPCTL_REG          CMPCTL;  // Compare control
   union  CMPA_HRPWM_GROUP    CMPA;    // Union of CMPA:CMPAHR
   Uint16                     CMPB;    // Compare B reg
   union  AQCTL_REG           AQCTLA;  // Action qual output A
   union  AQCTL_REG           AQCTLB;  // Action qual output B
   union  AQSFRC_REG          AQSFRC;  // Action qual SW force
   union  AQCSFRC_REG         AQCSFRC; // Action qualifier continuous SW force 
   union  DBCTL_REG           DBCTL;   // Dead-band control
   Uint16                     DBRED;   // Dead-band rising edge delay
   Uint16                     DBFED;   // Dead-band falling edge delay
   union  TZSEL_REG           TZSEL;   // Trip zone select
   Uint16                     rsvd2;   
   union  TZCTL_REG           TZCTL;   // Trip zone control
   union  TZEINT_REG          TZEINT;  // Trip zone interrupt enable
   union  TZFLG_REG           TZFLG;   // Trip zone interrupt flags
   union  TZCLR_REG           TZCLR;   // Trip zone clear   
   union  TZFRC_REG    	      TZFRC;   // Trip zone force interrupt
   union  ETSEL_REG           ETSEL;   // Event trigger selection
   union  ETPS_REG            ETPS;    // Event trigger pre-scaler
   union  ETFLG_REG           ETFLG;   // Event trigger flags
   union  ETCLR_REG           ETCLR;   // Event trigger clear   
   union  ETFRC_REG           ETFRC;   // Event trigger force
   union  PCCTL_REG           PCCTL;   // PWM chopper control
   Uint16                     rsvd3;   // 
   union  HRCNFG_REG          HRCNFG;  // HRPWM Config Reg
};
    
 

//---------------------------------------------------------------------------
// External References & Function Declarations:
//
extern volatile struct EPWM_REGS EPwm1Regs;
extern volatile struct EPWM_REGS EPwm2Regs;
extern volatile struct EPWM_REGS EPwm3Regs;
extern volatile struct EPWM_REGS EPwm4Regs;
extern volatile struct EPWM_REGS EPwm5Regs;
extern volatile struct EPWM_REGS EPwm6Regs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP280x_EPWM_H definition

//===========================================================================
// End of file.
//===========================================================================
