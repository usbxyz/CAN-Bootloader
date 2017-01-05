package com.usbxyz.canbootloader;

/**
 * Created by wdluo on 2017/1/4.
 */
public class CANBaudRate {
    public class CAN_BAUD_RATE {
        public byte SJW;
        public byte BS1;
        public byte BS2;
        public short PreScale;
        public int BaudRate;//前面4个参数对应的波特率，单位为Hz

        public CAN_BAUD_RATE(int SJW, int BS1, int BS2, int PreScale, int BaudRate) {
            this.SJW = (byte) SJW;
            this.BS1 = (byte) BS1;
            this.BS2 = (byte) BS2;
            this.PreScale = (short) PreScale;
            this.BaudRate = BaudRate;
        }
    }
    public CAN_BAUD_RATE[]  CANBaudRateTab = {
        //SJW,BS1,BS2,BRP,BAUD
        new CAN_BAUD_RATE(1,2,1,25,1000000),    // 1M
        new CAN_BAUD_RATE(1,2,1,28,900000),     // 900K
        new CAN_BAUD_RATE(1,3,1,25,800000),     // 800K
        new CAN_BAUD_RATE(1,3,1,30,666000),     // 666K
        new CAN_BAUD_RATE(1,4,1,28,600000),     // 600K
        new CAN_BAUD_RATE(1,6,1,25,500000),     // 500K
        new CAN_BAUD_RATE(1,8,1,25,400000),     // 400K
        new CAN_BAUD_RATE(1,7,1,37,300000),     // 300K
        new CAN_BAUD_RATE(1,6,1,50,250000),     // 250K
        new CAN_BAUD_RATE(1,3,1,89,225000),     // 225K
        new CAN_BAUD_RATE(1,16,3,25,200000),    // 200K
        new CAN_BAUD_RATE(1,6,1,78,160000),     // 160K
        new CAN_BAUD_RATE(1,15,2,37,150000),    // 150K
        new CAN_BAUD_RATE(1,6,1,87,144000),     // 144K
        new CAN_BAUD_RATE(1,6,1,100,125000),    // 125K
        new CAN_BAUD_RATE(1,6,1,104,120000),    // 120K
        new CAN_BAUD_RATE(1,16,3,50,100000),    // 100K
        new CAN_BAUD_RATE(1,6,1,139,90000),     // 90K
        new CAN_BAUD_RATE(1,1,3,278,80000),     // 80K
        new CAN_BAUD_RATE(1,6,1,167,75000),     // 75K
        new CAN_BAUD_RATE(1,6,1,208,60000),     // 60K
        new CAN_BAUD_RATE(1,6,1,250,50000),     // 50K
        new CAN_BAUD_RATE(1,6,1,312,40000),     // 40K
        new CAN_BAUD_RATE(1,6,1,417,30000),     // 30K
        new CAN_BAUD_RATE(1,6,1,625,20000),     // 20K
        new CAN_BAUD_RATE(1,13,2,625,10000),    // 10K
        new CAN_BAUD_RATE(1,16,3,1000,5000),    // 5K
        };
    public int GetBaudRateIndex(int BaudRate) {
        for (int i = 0; i < CANBaudRateTab.length; i++){
            if (BaudRate == CANBaudRateTab[i].BaudRate) {
                return i;
            }
        }
        return 0;
    }
}
