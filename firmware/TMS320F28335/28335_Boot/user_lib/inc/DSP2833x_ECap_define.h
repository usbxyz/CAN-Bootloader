#ifndef _ECAP_DEFINE_H__
#define _ECAP_DEFINE_H__
//CAP_ECCTL1 控制寄存器1
//CAPxPOL bitset 捕获极性宏定义
#define EC_RISING   0x00
#define EC_FASLLING 0x01
//CTRRSTx bits  计数器模式宏定义
#define EC_ABS_MODE   0x00
#define EC_DELTA_MODE 0x01
//CAP_Presscal分频宏定义
#define EC_DIV_1  0x00
#define EC_DIV_2  0x01
#define EC_DIV_4  0x02
#define EC_DIV_6  0x03
#define EC_DIV_8  0x04
#define EC_DIV_10 0x05
#define EC_DIV_12 0x06
#define EC_DIV_14 0x07
//CAP_ECCTL2 控制寄存器2
//连续模式_单次模式
#define EC_CONTINUOUS 0x00
#define EC_ONESHOT    0x01
//STOPVALUE bit
#define EC_EVENT1 0x00
#define EC_EVENT2 0x01
#define EC_EVENT3 0x02
#define EC_EVENT4 0x03
//RE-ARM  bit
#define EC_ARM  0x01
//TSCTRSTOP  bit
#define EC_FREEZE 0x00
#define EC_RUN    0x01
//SYNCO_SEN  bit  同步事件输出使能
#define EC_SYNCIN    0x00
#define EC_CTR_PRD   0x01
#define EC_SYNCO_DIS 0x02
//CAP/APWM  模式
#define EC_CAP_MODE  0x00
#define EC_APWM_MODE 0x01
//APWM  极性配置
#define EC_ACTV_HI 0x00
#define EC_ACTV_LO 0x01
//Generic
#define EC_DISABLE 0x00
#define EC_ENABLE  0x01
#define EC_FORCE   0x02

#endif
