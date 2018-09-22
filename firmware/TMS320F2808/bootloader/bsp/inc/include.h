/*
 * include.h
 *
 *  Created on: 2016年10月17日
 *      Author: Administrator
 */

#ifndef TMS320F2808_USER_INC_INCLUDE_H_
#define TMS320F2808_USER_INC_INCLUDE_H_
#include "DSP280x_Device.h"     // DSP280x Headerfile Include File
#include "DSP280x_Examples.h"   // DSP280x Examples Include File
#include "IQmathLib.h"
#include "define.h"
#include "delay.h"
typedef struct
{
	u16 sys_time_cnt;//表示系统运行时间;
	u8  sys_state;//表示系统运行状态
	u16 ADC_sample_cnt;
}Sys_Register;
extern Sys_Register sys_reg;
#endif /* TMS320F2808_USER_INC_INCLUDE_H_ */
