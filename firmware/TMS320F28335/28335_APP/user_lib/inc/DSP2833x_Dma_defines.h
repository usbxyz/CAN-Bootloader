//###########################################################################
//
// FILE:   DSP2833x_Dma_defines.h
//
// TITLE:  #defines used in DMA examples
//
//###########################################################################
// $TI Release: 2833x/2823x Header Files and Peripheral Examples V133 $
// $Release Date: June 8, 2012 $
//###########################################################################

#ifndef DSP2833x_DMA_DEFINES_H
#define DSP2833x_DMA_DEFINES_H


#ifdef __cplusplus
extern "C" {
#endif

// MODE
//==========================
// PERINTSEL bits
#define DMA_SEQ1INT 1
#define DMA_SEQ2INT 2
#define DMA_XINT1   3
#define DMA_XINT2   4
#define DMA_XINT3   5
#define DMA_XINT4   6
#define DMA_XINT5   7
#define DMA_XINT6   8
#define DMA_XINT7   9
#define DMA_XINT13  10
#define DMA_TINT0   11
#define DMA_TINT1   12
#define DMA_TINT2   13
#define DMA_MXEVTA  14
#define DMA_MREVTA  15
#define DMA_MXREVTB 16
#define DMA_MREVTB  17
// OVERINTE bit
#define	OVRFLOW_DISABLE	0x0
#define	OVEFLOW_ENABLE	0x1
// PERINTE bit
#define	PERINT_DISABLE	0x0
#define	PERINT_ENABLE   0x1
// CHINTMODE bits
#define	CHINT_BEGIN		0x0
#define	CHINT_END     	0x1
// ONESHOT bits
#define	ONESHOT_DISABLE	0x0
#define	ONESHOT_ENABLE	0x1
// CONTINOUS bit
#define	CONT_DISABLE	0x0
#define	CONT_ENABLE 	0x1
// SYNCE bit
#define	SYNC_DISABLE	0x0
#define	SYNC_ENABLE     0x1
// SYNCSEL bit
#define	SYNC_SRC		0x0
#define	SYNC_DST        0x1
// DATASIZE bit
#define	SIXTEEN_BIT    	0x0
#define	THIRTYTWO_BIT   0x1
// CHINTE bit
#define	CHINT_DISABLE	0x0
#define	CHINT_ENABLE   	0x1




#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // - end of DSP2833x_EPWM_DEFINES_H

//===========================================================================
// End of file.
//===========================================================================
