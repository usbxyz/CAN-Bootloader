/*
 * LED.h
 *
 *  Created on: 2017��5��21��
 *      Author: admin
 */

#ifndef BSP_INC_LED_H_
#define BSP_INC_LED_H_
#include "include.h"
#include "data.h"
#include "delay.h"
#include "stdint.h"
typedef struct _UPDATA_INFO
{
	unsigned short int time_cnt;
	unsigned short int time_out_flag;
}UPDATA_INFO;
void LED_GPIO_Config(void);
void LED_Timer_Config(void);
extern UPDATA_INFO updata_info;
interrupt void cpu_timer0_isr(void);

#endif /* BSP_INC_LED_H_ */
