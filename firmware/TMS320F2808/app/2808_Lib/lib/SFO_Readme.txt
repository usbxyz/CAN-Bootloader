Updated November 2007
================
There are now two versions of the SFO library - SFO_TI_Build.lib and SFO_TI_Build_V5.lib (V2-V4 were INTERNAL TI releases):

SFO_TI_Build.lib - original SFO library which supports up to 4 HRPWM channels (ePWM 1-4)
SFO_TI_Build_V5.lib - SFO library which supports up to 16 HRPWM channels (ePWM 1-16)  with some optimizations

* The above versions of the SFO library support only the following HRPWM configuration:
	HRCNFG[HRLOAD]=0 (load on CTR=ZRO)
	HRCNFG[EDGMODE]=10(falling edge MEP control)

Upgrades  
======
11/2007 - SFO_TI_Build_V5.lib - SFO_TI_Build_V5B.lib upgrades the SFO_MepEn_V5(n) function in SFO_TI_Build_V5.lib 
	 to support all HRPWM configurations.  

--------------------------------------------------------------------------------------------------------------------
For a description of the two SFO library versions including differences, functionality, and constraints 
during usage, consult the HRPWM Reference Guide (literature number: spru924) 
