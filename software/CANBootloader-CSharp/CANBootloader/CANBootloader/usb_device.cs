using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace USB2XXX
{
    class usb_device
    {
        public struct DEVICE_INFO
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public Byte[] FirmwareName;   //固件名称字符串
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public Byte[]   BuildDate;    //固件编译时间字符串
            public UInt32 HardwareVersion;//硬件版本号
            public UInt32 FirmwareVersion;//固件版本号
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
            public UInt32[] SerialNumber; //适配器序列号
            public UInt32 Functions;      //适配器当前具备的功能
        }
        //方法定义
        [DllImport("USB2XXX.dll")]
        public static extern Int32 USB_ScanDevice(Int32[] pDevNum);
        [DllImport("USB2XXX.dll")]
        public static extern bool USB_OpenDevice(Int32 DevIndex);
        [DllImport("USB2XXX.dll")]
        public static extern bool USB_CloseDevice(Int32 DevIndex);
        [DllImport("USB2XXX.dll")]
        public static extern bool USB_GetDeviceInfo(Int32 DevIndex, ref DEVICE_INFO pDevInfo, StringBuilder pFunctionStr);
    }
}
