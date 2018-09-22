;; TI File $Revision: /main/1 $
;; Checkin $Date: July 30, 2007   10:29:23 $
;;###########################################################################
;;
;; FILE:    ADC_cal.asm
;;
;; TITLE:   2833x Boot Rom ADC Cal routine.
;;
;; Functions:
;;
;;     _ADC_cal - Copies device specific calibration data into ADCREFSEL and ADCOFFTRIM registers 
;; Notes:
;;
;;###########################################################################
;; $TI Release: 2833x/2823x Header Files and Peripheral Examples V133 $
;; $Release Date: June 8, 2012 $
;;###########################################################################

    .def _ADC_cal
	.asg "0x711C",   ADCREFSEL_LOC

;-----------------------------------------------
; _ADC_cal
;-----------------------------------------------
;-----------------------------------------------
; This is the ADC cal routine.This routine is programmed into 
; reserved memory by the factory. 0xAAAA and 0xBBBB are place- 
; holders for calibration data.  
;The actual values programmed by TI are device specific. 
;
; This function assumes that the clocks have been
; enabled to the ADC module.
;-----------------------------------------------

    .sect ".adc_cal"

_ADC_cal
    MOVW  DP,   #ADCREFSEL_LOC >> 6
    MOV   @28,  #0xAAAA            ; actual value may not be 0xAAAA
    MOV   @29,  #0xBBBB             ; actual value may not be 0xBBBB
    LRETR
;eof ----------
