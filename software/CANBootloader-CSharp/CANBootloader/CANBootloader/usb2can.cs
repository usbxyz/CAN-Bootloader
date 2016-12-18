using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace USB2XXX
{
    class USB2CAN
    {
        //1.CAN信息帧的数据类型定义
        public struct CAN_MSG
        {
            public UInt32  ID;          //报文ID。
            public UInt32  TimeStamp;   //接收到信息帧时的时间标识，从CAN 控制器初始化开始计时。
            public Byte    RemoteFlag;  //是否是远程帧
            public Byte    ExternFlag;  //是否是扩展帧
            public Byte    DataLen;     //数据长度(<=8)，即Data 的长度。
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public Byte[]  Data;        //报文的数据。
        };

        //2.初始化CAN的数据类型定义
        public struct CAN_INIT_CONFIG
        {
            //CAN波特率 = 100MHz/(CAN_BRP)/(CAN_SJW+CAN_BS1+CAN_BS2)
            public UInt32   CAN_BRP;    //取值范围1~1024
            public Byte     CAN_SJW;    //取值范围1~4
            public Byte     CAN_BS1;    //取值范围1~16
            public Byte     CAN_BS2;    //取值范围1~8
            public Byte     CAN_Mode;   //CAN工作模式，0-正常模式，1-环回模式，2-静默模式，3-静默环回模式
            public Byte     CAN_ABOM;   //自动离线管理，0-禁止，1-使能
            public Byte     CAN_NART;   //报文重发管理，0-使能报文重传，1-禁止报文重传
            public Byte     CAN_RFLM;   //FIFO锁定管理，0-新报文覆盖旧报文，1-丢弃新报文
            public Byte     CAN_TXFP;   //发送优先级管理，0-标识符决定，1-发送请求顺序决定
        };

        //3.CAN 滤波器设置数据类型定义
        public struct CAN_FILTER_CONFIG{
            public Byte     Enable;         //使能该过滤器，1-使能，0-禁止
            public Byte     FilterIndex;    //过滤器索引号，取值范围为0到13
            public Byte     FilterMode;     //过滤器模式，0-屏蔽位模式，1-标识符列表模式
            public Byte     ExtFrame;       //过滤的帧类型标志，为1 代表要过滤的为扩展帧，为0 代表要过滤的为标准帧。
            public UInt32   ID_Std_Ext;     //验收码ID
            public UInt32   ID_IDE;         //验收码IDE
            public UInt32   ID_RTR;         //验收码RTR
            public UInt32   MASK_Std_Ext;   //屏蔽码ID，该项只有在过滤器模式为屏蔽位模式时有用
            public UInt32   MASK_IDE;       //屏蔽码IDE，该项只有在过滤器模式为屏蔽位模式时有用
            public UInt32   MASK_RTR;       //屏蔽码RTR，该项只有在过滤器模式为屏蔽位模式时有用
        };

        //4.CAN总线状态数据类型定义
        public struct CAN_STATUS{
            public UInt32   TSR;        //发送状态寄存器
            public UInt32   ESR;        //错误状态寄存器
            public Byte     RECounter;  //CAN 控制器接收错误寄存器。
            public Byte     TECounter;  //CAN 控制器发送错误寄存器。
            public Byte     LECode;     //最后的错误代码
        };

        //5.定义CAN Bootloader命令列表
        public struct CBL_CMD_LIST{
            //Bootloader相关命令
            public Byte Erase;          //擦出APP储存扇区数据
            public Byte WriteInfo;      //设置多字节写数据相关参数（写起始地址，数据量）
            public Byte Write;          //以多字节形式写数据
            public Byte Check;          //检测节点是否在线，同时返回固件信息
            public Byte SetBaudRate;    //设置节点波特率
            public Byte Excute;         //执行固件
            //节点返回状态
            public Byte CmdSuccess;     //命令执行成功
            public Byte CmdFaild;       //命令执行失败
        };

        public const UInt32  CAN_BL_BOOT   =  0x55555555;
        public const UInt32 CAN_BL_APP = 0xAAAAAAAA;

        //6.函数返回错误代码定义
        public const Int32 CAN_SUCCESS             = (0);   //函数执行成功
        public const Int32 CAN_ERR_NOT_SUPPORT     = (-1);  //适配器不支持该函数
        public const Int32 CAN_ERR_USB_WRITE_FAIL  = (-2);  //USB写数据失败
        public const Int32 CAN_ERR_USB_READ_FAIL   = (-3);  //USB读数据失败
        public const Int32 CAN_ERR_CMD_FAIL        = (-4);  //命令执行失败
        public const Int32 CAN_BL_ERR_CONFIG       = (-20); //配置设备错误
        public const Int32 CAN_BL_ERR_SEND         = (-21); //发送数据出错
        public const Int32 CAN_BL_ERR_TIME_OUT     = (-22); //超时错误
        public const Int32 CAN_BL_ERR_CMD          = (-23); //执行命令失败
        //USB2CAN相关函数定义
        [DllImport("USB2XXX.dll")]
        public static extern Int32 CAN_Init(Int32 DevIndex, Byte CANIndex, ref CAN_INIT_CONFIG pCanConfig);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 CAN_Filter_Init(Int32 DevIndex, Byte CANIndex, ref CAN_FILTER_CONFIG pFilterConfig);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 CAN_SendMsg(Int32 DevIndex, Byte CANIndex, ref CAN_MSG pCanSendMsg,UInt32 SendMsgNum);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 CAN_GetMsg(Int32 DevIndex, Byte CANIndex, ref CAN_MSG pCanGetMsg);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 CAN_GetStatus(Int32 DevIndex, Byte CANIndex, ref CAN_STATUS pCANStatus);

        //CAN Bootloader相关函数
        [DllImport("USB2XXX.dll")]
        public static extern Int32 CAN_BL_Init(Int32 DevIndex,Int32 CANIndex,ref CAN_INIT_CONFIG pInitConfig,ref CBL_CMD_LIST pCmdList);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 CAN_BL_NodeCheck(Int32 DevIndex, Int32 CANIndex, UInt16 NodeAddr, UInt32[] pVersion, UInt32[] pType, UInt32 TimeOut);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 CAN_BL_Erase(Int32 DevIndex,Int32 CANIndex,UInt16 NodeAddr,UInt32 FlashSize,UInt32 TimeOut);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 CAN_BL_Write(Int32 DevIndex,Int32 CANIndex,UInt16 NodeAddr,UInt32 AddrOffset,Byte[] pData,UInt32 DataNum,UInt32 TimeOut);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 CAN_BL_Excute(Int32 DevIndex,Int32 CANIndex,UInt16 NodeAddr,UInt32 Type);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 CAN_BL_SetNewBaudRate(Int32 DevIndex, Int32 CANIndex, UInt16 NodeAddr, ref CAN_INIT_CONFIG pInitConfig, UInt32 NewBaudRate, UInt32 TimeOut);

    }
}
