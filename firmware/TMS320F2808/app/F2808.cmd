MEMORY
{
PAGE 0:    /* Program Memory */
           /* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE1 for data allocation */
   RAML0       : origin = 0x008000, length = 0x001000     /* on-chip RAM block L0 */
   OTP         : origin = 0x3D7800, length = 0x000400     /* on-chip OTP */
   APP_INFO    : origin = 0x3E8000, length = 0x000010,fill = 0xFFFF
   BEGIN       : origin = 0x3E8010, length = 0x000002     /* Part of FLASHA.  Used for "boot to Flash" bootloader mode. */
   FLASHD      : origin = 0x3E8012, length = 0x003FEE     /* on-chip FLASH */
   FLASHC      : origin = 0x3EC000, length = 0x004000     /* on-chip FLASH */
   FLASHB      : origin = 0x3F0000, length = 0x004000     /* on-chip FLASH */
   CSM_RSVD    : origin = 0x3F7F80, length = 0x000076     /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */
   CSM_PWL     : origin = 0x3F7FF8, length = 0x000008     /* Part of FLASHA.  CSM password locations in FLASHA */
   ROM         : origin = 0x3FF000, length = 0x000FC0     /* Boot ROM */
   RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM  */
   VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM  */
PAGE 1:
   RAMM0       : origin = 0x000000, length = 0x000400     /* on-chip RAM block M0 */
   BOOT_RSVD   : origin = 0x000400, length = 0x000080     /* Part of M1, BOOT rom will use this for stack */
   RAMM1       : origin = 0x000480, length = 0x000380     /* on-chip RAM block M1 */
   RAML1       : origin = 0x009000, length = 0x001000     /* on-chip RAM block L1 */
   RAMH0       : origin = 0x3FA000, length = 0x002000     /* on-chip RAM block H0 */
}

/* Allocate sections to memory blocks.
   Note:
         codestart user defined section in DSP28_CodeStartBranch.asm used to redirect code 
                   execution when booting to flash
         ramfuncs  user defined section to store functions that will be copied from Flash into RAM
*/ 
 
SECTIONS
{
   /* Allocate program areas: */
   .cinit              : > FLASHD      PAGE = 0
   .pinit              : > FLASHD,     PAGE = 0
   .text               : > FLASHD      PAGE = 0
   codestart           : > BEGIN       PAGE = 0
   APP_info : > APP_INFO  PAGE = 0
      Flash28_API:
                   {
                     -l Flash2808_API_V302.lib(.econst)
                     -l Flash2808_API_V302.lib(.text)
                   }
                   LOAD = FLASHD
                   RUN  = RAML0
                   LOAD_START(_Flash28_API_LoadStart)
                   LOAD_END  (_Flash28_API_LoadEnd)
                   RUN_START (_Flash28_API_RunStart)
                   PAGE=0
   ramfuncs            : LOAD = FLASHD, 
                         RUN  = RAML0,
                         LOAD_START(_RamfuncsLoadStart),
                         LOAD_END(_RamfuncsLoadEnd),
                         RUN_START(_RamfuncsRunStart),
                         PAGE = 0   
   csmpasswds          : > CSM_PWL     PAGE = 0
   csm_rsvd            : > CSM_RSVD    PAGE = 0
   /* Allocate uninitalized data sections: */
   .stack              : > RAMM0       PAGE = 1
   .ebss               : > RAML1       PAGE = 1
   .esysmem            : > RAMH0       PAGE = 1
   /* Initalized sections go in Flash */
   /* For SDFlash to program these, they must be allocated to page 0 */
	.econst             : > FLASHD      PAGE = 0
	.switch             : > FLASHD      PAGE = 0

   /* Allocate IQ math areas: */
   IQmath              : > FLASHD      PAGE = 0                  /* Math Code */
   IQmathTables        : > ROM         PAGE = 0, TYPE = NOLOAD   /* Math Tables In ROM */

   /* .reset is a standard section used by the compiler.  It contains the */ 
   /* the address of the start of _c_int00 for C Code.   /*
   /* When using the boot ROM this section and the CPU vector */
   /* table is not needed.  Thus the default type is set here to  */
   /* DSECT  */ 
   .reset              : > RESET,      PAGE = 0, TYPE = DSECT
   vectors             : > VECTORS     PAGE = 0, TYPE = DSECT

}

/*
//===========================================================================
// End of file.
//===========================================================================
*/

