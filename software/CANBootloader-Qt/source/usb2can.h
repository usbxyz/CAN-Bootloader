/**
  ******************************************************************************
  * @file    usb2can.h
  * $Author: wdluo $
  * $Revision: 447 $
  * $Date:: 2013-06-29 18:24:57 +0800 #$
  * @brief   usb2can相关函数和数据类型定义.
  ******************************************************************************
  * @attention
  *
  *<center><a href="http:\\www.usbxyz.com">http://www.usbxyz.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
#ifndef __USB2CAN_H_
#define __USB2CAN_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif

//1.CAN信息帧的数据类型定义
typedef  struct  _CAN_MSG
{
    unsigned int    ID;            //报文ID。
    unsigned int    TimeStamp;    //接收到信息帧时的时间标识，从CAN 控制器初始化开始计时。
    unsigned char   RemoteFlag;    //是否是远程帧
    unsigned char   ExternFlag;    //是否是扩展帧
    unsigned char   DataLen;    //数据长度(<=8)，即Data 的长度。
    unsigned char   Data[8];    //报文的数据。
}CAN_MSG,*PCAN_MSG;

//2.初始化CAN的数据类型定义
typedef struct _CAN_INIT_CONFIG
{
    //CAN波特率 = 100MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
    unsigned int    CAN_BRP;    //取值范围1~1024
    unsigned char   CAN_SJW;    //取值范围1~4
    unsigned char   CAN_BS1;    //取值范围1~16
    unsigned char   CAN_BS2;    //取值范围1~8
    unsigned char   CAN_Mode;    //CAN工作模式，0-正常模式，1-环回模式，2-静默模式，3-静默环回模式
    unsigned char   CAN_ABOM;    //自动离线管理，0-禁止，1-使能
    unsigned char   CAN_NART;    //报文重发管理，0-使能报文重传，1-禁止报文重传
    unsigned char   CAN_RFLM;    //FIFO锁定管理，0-新报文覆盖旧报文，1-丢弃新报文
    unsigned char   CAN_TXFP;    //发送优先级管理，0-标识符决定，1-发送请求顺序决定
}CAN_INIT_CONFIG,*PCAN_INIT_CONFIG;


//3.CAN 滤波器设置数据类型定义
typedef struct _CAN_FILTER_CONFIG{
    unsigned char   Enable;            //使能该过滤器，1-使能，0-禁止
    unsigned char   FilterIndex;    //过滤器索引号，取值范围为0到13
    unsigned char   FilterMode;        //过滤器模式，0-屏蔽位模式，1-标识符列表模式
    unsigned char   ExtFrame;        //过滤的帧类型标志，为1 代表要过滤的为扩展帧，为0 代表要过滤的为标准帧。
    unsigned int    ID_Std_Ext;        //验收码ID
    unsigned int    ID_IDE;            //验收码IDE
    unsigned int    ID_RTR;            //验收码RTR
    unsigned int    MASK_Std_Ext;    //屏蔽码ID，该项只有在过滤器模式为屏蔽位模式时有用
    unsigned int    MASK_IDE;        //屏蔽码IDE，该项只有在过滤器模式为屏蔽位模式时有用
    unsigned int    MASK_RTR;        //屏蔽码RTR，该项只有在过滤器模式为屏蔽位模式时有用
} CAN_FILTER_CONFIG,*PCAN_FILTER_CONFIG;
//4.CAN总线状态数据类型定义
typedef struct _CAN_STATUS{
    unsigned int    TSR;
    unsigned int    ESR;
    unsigned char   RECounter;    //CAN 控制器接收错误寄存器。
    unsigned char   TECounter;    //CAN 控制器发送错误寄存器。
    unsigned char   LECode;     //最后的错误代码
}CAN_STATUS,*PCAN_STATUS;
//5.定义CAN Bootloader命令列表
typedef  struct  _CBL_CMD_LIST{
    //Bootloader相关命令
    unsigned char   Erase;            //擦出APP储存扇区数据
    unsigned char   WriteInfo;        //设置多字节写数据相关参数（写起始地址，数据量）
    unsigned char   Write;            //以多字节形式写数据
    unsigned char   Check;            //检测节点是否在线，同时返回固件信息
    unsigned char   SetBaudRate;    //设置节点波特率
    unsigned char   Excute;            //执行固件
    //节点返回状态
    unsigned char    CmdSuccess;        //命令执行成功
    unsigned char    CmdFaild;        //命令执行失败
} CBL_CMD_LIST,*PCBL_CMD_LIST; 

//6.函数返回错误代码定义
#define CAN_SUCCESS             (0)   //函数执行成功
#define CAN_ERR_NOT_SUPPORT     (-1)  //适配器不支持该函数
#define CAN_ERR_USB_WRITE_FAIL  (-2)  //USB写数据失败
#define CAN_ERR_USB_READ_FAIL   (-3)  //USB读数据失败
#define CAN_ERR_CMD_FAIL        (-4)  //命令执行失败
#define CAN_BL_ERR_CONFIG       (-20) //配置设备错误
#define CAN_BL_ERR_SEND         (-21) //发送数据出错
#define CAN_BL_ERR_TIME_OUT     (-22) //超时错误
#define CAN_BL_ERR_CMD          (-23) //执行命令失败

//7.CAN Bootloader固件类型
#define CAN_BL_BOOT     0x55555555
#define CAN_BL_APP      0xAAAAAAAA

#ifdef __cplusplus
extern "C"
{
#endif

int WINAPI CAN_Init(int DevHandle, unsigned char CANIndex, PCAN_INIT_CONFIG pCanConfig);
int WINAPI CAN_Filter_Init(int DevHandle, unsigned char CANIndex, PCAN_FILTER_CONFIG pFilterConfig);
int WINAPI CAN_SendMsg(int DevHandle, unsigned char CANIndex, PCAN_MSG pCanSendMsg,unsigned int SendMsgNum);
int WINAPI CAN_GetMsg(int DevHandle, unsigned char CANIndex, PCAN_MSG pCanGetMsg);
int WINAPI CAN_GetStatus(int DevHandle, unsigned char CANIndex, PCAN_STATUS pCANStatus);

int WINAPI CAN_BL_Init(int DevHandle,int CANIndex,PCAN_INIT_CONFIG pInitConfig,PCBL_CMD_LIST pCmdList);
int WINAPI CAN_BL_NodeCheck(int DevHandle,int CANIndex,unsigned short NodeAddr,unsigned int *pVersion,unsigned int *pType,unsigned int TimeOut);
int WINAPI CAN_BL_Erase(int DevHandle,int CANIndex,unsigned short NodeAddr,unsigned int FlashSize,unsigned int TimeOut);
int WINAPI CAN_BL_Write(int DevHandle,int CANIndex,unsigned short NodeAddr,unsigned int AddrOffset,unsigned char *pData,unsigned int DataNum,unsigned int TimeOut);
int WINAPI CAN_BL_Excute(int DevHandle,int CANIndex,unsigned short NodeAddr,unsigned int Type);
int WINAPI CAN_BL_SetNewBaudRate(int DevHandle,int CANIndex,unsigned short NodeAddr,PCAN_INIT_CONFIG pInitConfig,unsigned int NewBaudRate,unsigned int TimeOut);

#ifdef __cplusplus
}
#endif
#endif
