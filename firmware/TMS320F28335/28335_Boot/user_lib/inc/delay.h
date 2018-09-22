#ifndef _DSP_DELAY_H__
#define _DSP_DELAY_H__
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"
#include "data.h"
#include "config.h"
void delay_us(unsigned long int nus);
void delay_ms(unsigned long int nms);
void delay_s(unsigned long int ns);
void delay(unsigned long int i);
#endif
