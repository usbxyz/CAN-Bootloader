;// TI File $Revision: /main/3 $
;// Checkin $Date: June 26, 2007   16:41:07 $
;//###########################################################################
;//
;// FILE:	DSP2833x_CSMPasswords.asm
;//
;// TITLE:	DSP2833x Code Security Module Passwords.
;// 
;// DESCRIPTION:
;//
;//         This file is used to specify password values to
;//         program into the CSM password locations in Flash
;//         at 0x33FFF8 - 0x33FFFF.
;//
;//         In addition, the reserved locations 0x33FF80 - 0X33fff5 are 
;//         all programmed to 0x0000
;//
;//###########################################################################
;// $TI Release: F2833x/F2823x Header Files and Peripheral Examples V141 $
;// $Release Date: November  6, 2015 $
;// $Copyright: Copyright (C) 2007-2015 Texas Instruments Incorporated -
;//             http://www.ti.com/ ALL RIGHTS RESERVED $
;//###########################################################################

; The "csmpasswords" section contains the actual CSM passwords that will be
; linked and programmed into to the CSM password locations (PWL) in flash.  
; These passwords must be known in order to unlock the CSM module. 
; All 0xFFFF's (erased) is the default value for the password locations (PWL).

; It is recommended that all passwords be left as 0xFFFF during code
; development.  Passwords of 0xFFFF do not activate code security and dummy 
; reads of the CSM PWL registers is all that is required to unlock the CSM.  
; When code development is complete, modify the passwords to activate the
; code security module.

      .sect "csmpasswds"

      .int	0xFFFF		;PWL0 (LSW of 128-bit password)
      .int	0xFFFF		;PWL1
      .int	0xFFFF		;PWL2
      .int	0xFFFF		;PWL3
      .int	0xFFFF		;PWL4
      .int	0xFFFF		;PWL5
      .int	0xFFFF		;PWL6
      .int	0xFFFF		;PWL7 (MSW of 128-bit password)
	
;----------------------------------------------------------------------

; For code security operation, all addresses between 0x33FF80 and
; 0X33fff5 cannot be used as program code or data.  These locations
; must be programmed to 0x0000 when the code security password locations
; (PWL) are programmed.  If security is not a concern, then these addresses
; can be used for code or data.  

; The section "csm_rsvd" can be used to program these locations to 0x0000.

        .sect "csm_rsvd"
        .loop (33FFF5h - 33FF80h + 1)
              .int 0x0000
        .endloop

;//===========================================================================
;// End of file.
;//===========================================================================

      
