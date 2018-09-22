/*
 * LED.h
 *
 *  Created on: 2017Äê5ÔÂ21ÈÕ
 *      Author: admin
 */
#ifndef BSP_INC_LED_H_
#define BSP_INC_LED_H_
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "data.h"
#include "delay.h"
#include "stdint.h"
void LED_GPIO_Config(void);
void LED_Timer_Config(void);
interrupt void cpu_timer0_isr(void);
#endif /* BSP_INC_LED_H_ */
