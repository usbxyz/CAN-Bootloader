/*
 * define.h
 *
 *  Created on: 2016年10月17日
 *      Author: Administrator
 */

#ifndef TMS320F2808_USER_INC_DEFINE_H_
#define TMS320F2808_USER_INC_DEFINE_H_
typedef unsigned short int u16;
typedef unsigned long int u32;
typedef unsigned char u8;
#define	TB_CLKDIV1		           0x00
#define	TB_CLKDIV2		           0x01
#define	TB_CLKDIV4		           0x02
#define TB_CLKDIV8                 0x03
#define TB_CLKDIV16                0x04
#define TB_CLKDIV32                0x05
#define TB_CLKDIV64                0x06
#define TB_CLKDIV128               0x07
//HSPCLKDIV and
#define TB_HSPCLKDIV1              0x00
#define TB_HSPCLKDIV2              0x01
#define TB_HSPCLKDIV4              0x02
#define TB_HSPCLKDIV6              0x03
#define TB_HSPCLKDIV8              0x04
#define TB_HSPCLKDIV10             0x05
#define TB_HSPCLKDIV12             0x06
#define TB_HSPCLKDIV14             0x07
//GPIO 工作模式:输入/输出
#define GPIO_Mode_IN  0 /*!< GPIO Input Mode */
#define GPIO_Mode_OUT 1 /*!< GPIO Output Mode */
//GPIO上拉电阻
#define GPIO_PuPd_UP    0
#define GPIO_PuPd_DOWN  0
//GPIO的IO状态
#define GPIO_SET        1
#define GPIO_CLR        0
#endif /* TMS320F2808_USER_INC_DEFINE_H_ */
