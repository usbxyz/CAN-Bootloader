
//###########################################################################
//
// FILE:   SFO_V5.H
//
// TITLE:  Scale Factor Optimizer Library V5 Interface Header
//
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.01| 09 Jan 2004 | TI   | New module
//  0.02| 22 Jun 2007 | TI   | New version (V5) with support for more channels
//###########################################################################


//============================================================================
// Description:		This header provides the function call interface
// 			        for the scale factor optimizer V5. For more
//                  information on the SFO function usage and
//                  limitations, see the HRPWM Reference Guide
//                  (spru924) on the TI website.
//============================================================================


//============================================================================
// Multiple include Guard
//============================================================================
#ifndef _SFO_V5_H
#define _SFO_V5_H

//============================================================================
// C++ namespace
//============================================================================
#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// USER MUST UPDATE THIS CONSTANT FOR NUMBER OF HRPWM CHANNELS USED + 1
//============================================================================
#define PWM_CH	7	// Equal # of HRPWM channels PLUS 1
                    // i.e. PWM_CH is 7 for 6 channels, 5 for 4 channels etc.

//============================================================================
// Function prototypes for MEP SFO
//============================================================================

int SFO_MepEn_V5(int nEpwmModule);  // MEP-Enable V5 Calibration Function
int SFO_MepDis_V5(int nEpwmModule); // MEP-Disable V5  Calibration Function

//============================================================================
// Useful Defines when Using SFO Functions
//============================================================================
#define SFO_INCOMPLETE      0
#define SFO_COMPLETE        1
#define SFO_OUTRANGE_ERROR  2

//============================================================================
// Multiple include Guard
//============================================================================
#endif // End: Multiple include Guard

//============================================================================
// C++ namespace
//============================================================================
#ifdef __cplusplus
}
#endif /* extern "C" */
