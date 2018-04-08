using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using USB2XXX;
namespace CANBootloader
{
    public partial class FormMain : Form
    {
        UInt64 DevType;
        UInt64 DevTypeUSB2CANB = ((UInt64)'U' << 56) | ((UInt64)'S' << 48) | ((UInt64)'B' << 40) | ((UInt64)'2' << 32) | ((UInt64)'C' << 24) | ((UInt64)'A' << 16) | ((UInt64)'N' << 8) | ((UInt64)'B' << 0);
        //定义CAN波特率参数表
        struct CAN_BAUD_RATE
        {
            public byte SJW;
            public byte BS1;
            public byte BS2;
            public short PreScale;
            public int BaudRate;//前面4个参数对应的波特率，单位为Hz
            public CAN_BAUD_RATE(byte SJW, byte BS1, byte BS2, short PreScale, int BaudRate)
            {
                this.SJW = SJW;
                this.BS1 = BS1;
                this.BS2 = BS2;
                this.PreScale = PreScale;
                this.BaudRate = BaudRate;
            }
        };
        readonly CAN_BAUD_RATE[]  CANBaudRateTab = {
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
        readonly CAN_BAUD_RATE[] CANBaudRateTab42M = {
            //SJW,BS1,BS2,BRP
            new CAN_BAUD_RATE(1,15,5,2,1000000),       // 1M
            new CAN_BAUD_RATE(1,16,6,2,900000),       // 900K
            new CAN_BAUD_RATE(1,2,1,13,800000),       // 800K
            new CAN_BAUD_RATE(1,16,4,3,666000),       // 666K
            new CAN_BAUD_RATE(1,7,2,7,600000),       // 600K
            new CAN_BAUD_RATE(1,16,4,4,500000),       // 500K
            new CAN_BAUD_RATE(1,12,2,7,400000),       // 400K
            new CAN_BAUD_RATE(1,5,1,20,300000),      // 300K
            new CAN_BAUD_RATE(1,6,1,21,250000),      // 250K
            new CAN_BAUD_RATE(1,14,2,11,225000),      // 225K
            new CAN_BAUD_RATE(1,5,1,30,200000),      // 200K
            new CAN_BAUD_RATE(1,6,1,33,160000),      // 160K
            new CAN_BAUD_RATE(1,6,1,35,150000),      // 150K
            new CAN_BAUD_RATE(1,3,1,58,144000),      // 144K
            new CAN_BAUD_RATE(1,6,1,42,125000),     // 125K
            new CAN_BAUD_RATE(1,8,1,35,120000),     // 120K
            new CAN_BAUD_RATE(1,15,4,21,100000),      // 100K
            new CAN_BAUD_RATE(1,15,2,26,90000),     // 90K
            new CAN_BAUD_RATE(1,3,1,105,80000),     // 80K
            new CAN_BAUD_RATE(1,6,1,70,75000),      // 75K
            new CAN_BAUD_RATE(1,16,3,35,60000),      // 60K
            new CAN_BAUD_RATE(1,6,1,105,50000),      // 50K
            new CAN_BAUD_RATE(1,5,1,150,40000),      // 40K
            new CAN_BAUD_RATE(1,6,1,175,30000),     // 30K
            new CAN_BAUD_RATE(1,12,2,140,20000),     // 20K
            new CAN_BAUD_RATE(1,6,1,525,10000),     // 10K
            new CAN_BAUD_RATE(1,13,2,525,5000),     // 5K
        };
        //
        short ScanStartAddr = 0, ScanEndAddr = 0;
        int DeviceIndex = 0;
        int CANIndex = 0;
        Int32[] DevHandles = new Int32[20];

        public FormMain()
        {
            InitializeComponent();
            init();
        }
        //初始化界面控件值
        private void init()
        {
            this.comboBoxDeviceIndex.SelectedIndex = 0;
            this.comboBoxCANIndex.SelectedIndex = 0;
            this.comboBoxBaudRate.SelectedIndex = 0;
            this.comboBoxNewBaudRate.SelectedIndex = 0;
        }
        /// <summary>
        /// 根据波特率获取波特率参数索引值
        /// </summary>
        /// <param name="BaudRate">波特率值，单位为Hz</param>
        /// <returns>该波特率在波特率索引表中的索引值</returns>
        private int GetBaudRateIndex(int BaudRate)
        {
            for (int i = 0; i < CANBaudRateTab.Length; i++)
            {
                if (BaudRate == CANBaudRateTab[i].BaudRate)
                {
                    return i;
                }
            }
            return 0;
        }
        /// <summary>
        /// 初始化配置USB2XXX CAN适配器
        /// </summary>
        /// <returns></returns>
        private bool configDevice()
        {
            int ret,DevNum;
            bool state;
            DeviceIndex = this.comboBoxDeviceIndex.SelectedIndex;
            CANIndex = this.comboBoxCANIndex.SelectedIndex;
            //扫描查找设备
            DevNum = usb_device.USB_ScanDevice(DevHandles);
            if (DevNum <= 0)
            {
                MessageBox.Show(this, "无设备连接！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }
            //打开设备
            state = usb_device.USB_OpenDevice(DevHandles[DeviceIndex]);
            if (!state)
            {
                MessageBox.Show(this, "打开设备失败！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }
            //获取设备类型
            usb_device.DEVICE_INFO DevInfo = new usb_device.DEVICE_INFO();
            StringBuilder FuncStr = new StringBuilder(256);
            state = usb_device.DEV_GetDeviceInfo(DevHandles[DeviceIndex],ref DevInfo,FuncStr);
            if(!state){
                MessageBox.Show(this, "获取设备信息失败！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }
            DevType = ((UInt64)DevInfo.SerialNumber[0]<<32)|(DevInfo.SerialNumber[1]);

            USB2CAN.CBL_CMD_LIST CMD_List = new USB2CAN.CBL_CMD_LIST();
            String[] cmdStr={"Erase","WriteInfo","Write","Check","SetBaudRate","Execute","CmdSuccess","CmdFaild"};
            byte[] cmdData = new byte[cmdStr.Length];
            for(int i=0;i<this.listViewCmdList.Items.Count;i++)
            {
                ListViewItem item = listViewCmdList.Items[i];
                if(item.SubItems[0].Text == cmdStr[i])
                {
                    cmdData[i] = byte.Parse(item.SubItems[1].Text);
                }
            }
            CMD_List.Erase = cmdData[0];
            CMD_List.WriteInfo = cmdData[1];
            CMD_List.Write = cmdData[2];
            CMD_List.Check = cmdData[3];
            CMD_List.SetBaudRate = cmdData[4];
            CMD_List.Execute = cmdData[5];
            CMD_List.CmdSuccess = cmdData[6];
            CMD_List.CmdFaild = cmdData[7];
            USB2CAN.CAN_INIT_CONFIG CAN_InitConfig = new USB2CAN.CAN_INIT_CONFIG();
            int BaudRate = int.Parse(this.comboBoxBaudRate.Text.Substring(0,this.comboBoxBaudRate.Text.Length-4));
            if (DevType == DevTypeUSB2CANB)
            {
                CAN_InitConfig.CAN_BRP = (UInt32)CANBaudRateTab42M[GetBaudRateIndex(BaudRate)].PreScale;
                CAN_InitConfig.CAN_SJW = CANBaudRateTab42M[GetBaudRateIndex(BaudRate)].SJW;
                CAN_InitConfig.CAN_BS1 = CANBaudRateTab42M[GetBaudRateIndex(BaudRate)].BS1;
                CAN_InitConfig.CAN_BS2 = CANBaudRateTab42M[GetBaudRateIndex(BaudRate)].BS2;
            }
            else
            {
                CAN_InitConfig.CAN_BRP = (UInt32)CANBaudRateTab[GetBaudRateIndex(BaudRate)].PreScale;
                CAN_InitConfig.CAN_SJW = CANBaudRateTab[GetBaudRateIndex(BaudRate)].SJW;
                CAN_InitConfig.CAN_BS1 = CANBaudRateTab[GetBaudRateIndex(BaudRate)].BS1;
                CAN_InitConfig.CAN_BS2 = CANBaudRateTab[GetBaudRateIndex(BaudRate)].BS2;
            }
            ret = USB2CAN.CAN_BL_Init(DevHandles[DeviceIndex], CANIndex, ref CAN_InitConfig, ref CMD_List);
            if(ret!=USB2CAN.CAN_SUCCESS)
            {
                MessageBox.Show(this, "初始化设备失败！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }
            return true;
        }
        /// <summary>
        /// 选择固件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonOpenFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog fd = new OpenFileDialog();
            fd.Filter = "二进制文件(*.bin)|*.bin";
            if (fd.ShowDialog() == DialogResult.OK)
            {
                this.textBoxFilePath.Text = fd.FileName;
            }
        }
        /// <summary>
        /// 升级固件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonUpgrate_Click(object sender, EventArgs e)
        {
            if (configDevice())
            {
                //如果文件不存在，就提示错误
                if (File.Exists(this.textBoxFilePath.Text))
                {
                    FormProgress upgrateFormProgress = new FormProgress();
                    upgrateFormProgress.DoWork += new FormProgress.DoWorkEventHandler(upgrateFirmware);
                    upgrateFormProgress.ProgressBar.Maximum = (int)new FileInfo(this.textBoxFilePath.Text).Length;
                    upgrateFormProgress.Text = "固件升级";
                    //ScanNode.Argument = checkBoxThrowException.Checked;
                    if (upgrateFormProgress.ShowDialog() == DialogResult.Cancel)
                    {
                        upgrateFormProgress.Close();
                    }
                }
                else
                {
                    MessageBox.Show(this, "无法打开固件文件，是否选择了固件文件？", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }
        }

        void upgrateFirmware(FormProgress sender, DoWorkEventArgs e)
        {
            int FirmwareFileSize;
            UInt16 NodeAddr;
            int ret;
            //如果文件不存在，就提示错误
            if (File.Exists(this.textBoxFilePath.Text))
            {
                BinaryReader br;
                FirmwareFileSize = (int)new FileInfo(this.textBoxFilePath.Text).Length;
                try
                {
                    //将固件文件读取到数据缓冲区中
                    br = new BinaryReader(new FileStream(this.textBoxFilePath.Text, FileMode.Open));
                    //判断当前所选择的节点
                    if (this.listViewNodeList.Items.Count <= 0)
                    {
                        //当前节点列表中没有节点
                        MessageBox.Show(this, "节点列表中无节点，请先扫描节点！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        br.Close();
                        return;
                    }
                    else
                    {
                        if (this.checkBoxAllNode.Checked)
                        {
                            NodeAddr = 0;
                        }
                        else
                        {
                            if (this.listViewNodeList.SelectedIndices[0] < 0)
                            {
                                //没有选择任何节点
                                MessageBox.Show(this, "请先选择节点！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                                br.Close();
                                return;
                            }
                            else
                            {
                                NodeAddr = UInt16.Parse(this.listViewNodeList.Items[this.listViewNodeList.SelectedIndices[0]].SubItems[0].Text);
                            }
                        }
                    }

                    //MessageBox.Show(this, "NodeAddr = " + NodeAddr, "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    //发送跳转到Bootloader的命令
                    sender.SetProgress(0, "跳转到Bootloader...");
                    ret = USB2CAN.CAN_BL_Excute(DevHandles[DeviceIndex], CANIndex, NodeAddr, USB2CAN.CAN_BL_BOOT);
                    if (ret != USB2CAN.CAN_SUCCESS)
                    {
                        MessageBox.Show(this, "执行固件跳转命令失败！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        br.Close();
                        return;
                    }
                    
                    //延时
                    System.Threading.Thread.Sleep(500);
                    //检测当前节点是否为Bootloader
                    if (!this.checkBoxAllNode.Checked)
                    {
                        UInt32[] appVersion = new UInt32[1], appType = new UInt32[1];
                        ret = USB2CAN.CAN_BL_NodeCheck(DevHandles[DeviceIndex], CANIndex, NodeAddr, appVersion, appType, 10);
                        if (ret != USB2CAN.CAN_SUCCESS)
                        {
                            MessageBox.Show(this, "节点检测失败！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            br.Close();
                            return;
                        }
                        else
                        {
                            if (appType[0] != USB2CAN.CAN_BL_BOOT)
                            {
                                MessageBox.Show(this, "当前节点固件类型不是Bootloader，请重新点击升级固件！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                                br.Close();
                                return;
                            }
                        }
                    }
                    //发送擦出固件命令
                    sender.SetProgress(0, "擦出固件...");
                    ret = USB2CAN.CAN_BL_Erase(DevHandles[DeviceIndex], CANIndex, NodeAddr, (UInt32)FirmwareFileSize, 10000);
                    if(ret != USB2CAN.CAN_SUCCESS){
                        MessageBox.Show(this, "擦出固件失败！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        br.Close();
                        return;
                    }
                    if (this.checkBoxAllNode.Checked)
                    {
                        System.Threading.Thread.Sleep(1000);
                    }
                    //循环发送数据
                    UInt32 AddrOffset = 0;
                    UInt32 PackSize = 512;
                    byte[] DataBuffer = new byte[PackSize];
                    for (AddrOffset = 0; AddrOffset < FirmwareFileSize; AddrOffset += PackSize)
                    {
                        UInt32 read_data_num = (UInt32)br.Read(DataBuffer, 0, (int)PackSize);
                        ret = USB2CAN.CAN_BL_Write(DevHandles[DeviceIndex], CANIndex, NodeAddr, AddrOffset, DataBuffer, read_data_num, 1000);
                        if(ret != USB2CAN.CAN_SUCCESS){
                            MessageBox.Show(this, "写数据失败！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            br.Close();
                            return;
                        }
                        if (this.checkBoxAllNode.Checked)
                        {
                            System.Threading.Thread.Sleep(20);
                        }
                        sender.SetProgress((int)(AddrOffset+read_data_num), "正在发送固件数据...");
                    }
                    //执行固件
                    ret = USB2CAN.CAN_BL_Excute(DevHandles[DeviceIndex], CANIndex, NodeAddr, USB2CAN.CAN_BL_APP);
                    if (ret != USB2CAN.CAN_SUCCESS)
                    {
                        MessageBox.Show(this, "执行固件跳转命令失败！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        br.Close();
                        return;
                    }
                    System.Threading.Thread.Sleep(100);
                    //获取固件信息
                    if (!this.checkBoxAllNode.Checked)
                    {
                        UInt32[] appVersion = new UInt32[1], appType = new UInt32[1];
                        ret = USB2CAN.CAN_BL_NodeCheck(DevHandles[DeviceIndex], CANIndex, NodeAddr, appVersion, appType, 10);
                        if (ret == USB2CAN.CAN_SUCCESS)
                        {
                            string AppTypeStr;
                            if (appType[0] == USB2CAN.CAN_BL_BOOT)
                            {
                                AppTypeStr = "BOOT";
                            }
                            else
                            {
                                AppTypeStr = "APP";
                            }
                            this.listViewNodeList.Items[this.listViewNodeList.SelectedIndices[0]].SubItems[1].Text = AppTypeStr;
                            string AppVersionStr;
                            AppVersionStr = String.Format("v{0}.{1}", (((appVersion[0] >> 24) & 0xFF) * 10) + (appVersion[0] >> 16) & 0xFF, (((appVersion[0] >> 8) & 0xFF) * 10) + appVersion[0] & 0xFF);
                            this.listViewNodeList.Items[this.listViewNodeList.SelectedIndices[0]].SubItems[2].Text = AppVersionStr;
                        }
                    }
                    br.Close();
                }
                catch (IOException ep)
                {
                    MessageBox.Show(this, ep.Message, "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }
            }
            else
            {
                MessageBox.Show(this, "无法打开固件文件，是否选择了固件文件？", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void buttonSetNewBaudRate_Click(object sender, EventArgs e)
        {
            UInt16 NodeAddr;
            if (configDevice())
            {
                //判断当前所选择的节点
                if (this.listViewNodeList.Items.Count <= 0)
                {
                    //当前节点列表中没有节点
                    MessageBox.Show(this, "节点列表中无节点，请先扫描节点！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }
                else
                {
                    if (this.checkBoxAllNode.Checked)
                    {
                        NodeAddr = 0;
                    }
                    else
                    {
                        if (this.listViewNodeList.SelectedIndices[0] < 0)
                        {
                            //没有选择任何节点
                            MessageBox.Show(this, "请先选择节点！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            return;
                        }
                        else
                        {
                            NodeAddr = UInt16.Parse(this.listViewNodeList.Items[this.listViewNodeList.SelectedIndices[0]].SubItems[0].Text);
                        }
                    }
                }
                USB2CAN.CAN_INIT_CONFIG CAN_InitConfig = new USB2CAN.CAN_INIT_CONFIG();
                int BaudRate = int.Parse(this.comboBoxNewBaudRate.Text.Substring(0, this.comboBoxNewBaudRate.Text.Length - 4));
                if (DevType == DevTypeUSB2CANB)
                {
                    CAN_InitConfig.CAN_BRP = (UInt32)CANBaudRateTab42M[GetBaudRateIndex(BaudRate)].PreScale;
                    CAN_InitConfig.CAN_SJW = CANBaudRateTab42M[GetBaudRateIndex(BaudRate)].SJW;
                    CAN_InitConfig.CAN_BS1 = CANBaudRateTab42M[GetBaudRateIndex(BaudRate)].BS1;
                    CAN_InitConfig.CAN_BS2 = CANBaudRateTab42M[GetBaudRateIndex(BaudRate)].BS2;
                }
                else
                {
                    CAN_InitConfig.CAN_BRP = (UInt32)CANBaudRateTab[GetBaudRateIndex(BaudRate)].PreScale;
                    CAN_InitConfig.CAN_SJW = CANBaudRateTab[GetBaudRateIndex(BaudRate)].SJW;
                    CAN_InitConfig.CAN_BS1 = CANBaudRateTab[GetBaudRateIndex(BaudRate)].BS1;
                    CAN_InitConfig.CAN_BS2 = CANBaudRateTab[GetBaudRateIndex(BaudRate)].BS2;
                }
                int ret = USB2CAN.CAN_BL_SetNewBaudRate(DevHandles[DeviceIndex], CANIndex, NodeAddr, ref CAN_InitConfig, (UInt32)BaudRate, 100);
                if (ret != USB2CAN.CAN_SUCCESS)
                {
                    MessageBox.Show(this, "设置新波特率失败！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
                else
                {
                    this.comboBoxBaudRate.SelectedIndex = this.comboBoxNewBaudRate.SelectedIndex;
                }
            }
        }
        /// <summary>
        /// 扫描当前CAN总线上的节点，然后添加到节点列表里面
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void 扫描节点ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormStartScanNode StartScanNode = new FormStartScanNode();
            if (StartScanNode.ShowDialog() == DialogResult.OK)
            {
                this.listViewNodeList.Items.Clear();
                if (configDevice())
                {
                    FormProgress ScanNode = new FormProgress();
                    ScanNode.DoWork += new FormProgress.DoWorkEventHandler(scanNode);
                    ScanStartAddr = (short)StartScanNode.StartAddr;
                    ScanEndAddr = (short)StartScanNode.EndAddr;
                    ScanNode.ProgressBar.Maximum = ScanEndAddr - ScanStartAddr+1;
                    ScanNode.Text = "节点扫描";
                    //ScanNode.Argument = checkBoxThrowException.Checked;
                    if (ScanNode.ShowDialog() == DialogResult.Cancel)
                    {
                        ScanNode.Close();
                    }
                }
            }
            StartScanNode.Close();
        }
        /// <summary>
        /// 扫描节点
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void scanNode(FormProgress sender, DoWorkEventArgs e)
        {
            for (int i = ScanStartAddr; i <= ScanEndAddr; i++)
            {
                int ret;
                UInt32[] appVersion = new UInt32[1], appType = new UInt32[1];
                ret = USB2CAN.CAN_BL_NodeCheck(DevHandles[DeviceIndex], CANIndex, (UInt16)i, appVersion, appType, 100);
                if (ret == USB2CAN.CAN_SUCCESS)
                {
                    //Ctrl+F5启动程序这里运行正常，F5启动程序运行不正常，能扫描到节点，但是无法在界面上显示出来，原因不明
                    this.listViewNodeList.BeginUpdate();  //数据更新，UI暂时挂起，直到EndUpdate绘制控件，可以有效避免闪烁并大大提高加载速度  
                    ListViewItem lvi = new ListViewItem();
                    lvi.Text = i.ToString();
                    if (appType[0] == USB2CAN.CAN_BL_BOOT)
                    {
                        lvi.SubItems.Add("ROOT");
                    }
                    else
                    {
                        lvi.SubItems.Add("APP");
                    }
                    lvi.SubItems.Add(String.Format("v{0}.{1}", (((appVersion[0] >> 24) & 0xFF) * 10) + (appVersion[0] >> 16) & 0xFF, (((appVersion[0] >> 8) & 0xFF) * 10) + appVersion[0] & 0xFF));
                    this.listViewNodeList.Items.Add(lvi);
                    this.listViewNodeList.EndUpdate();  //结束数据处理，UI界面一次性绘制。
                }
                sender.SetProgress(i - ScanStartAddr + 1, "正在检测节点：" + i.ToString());
                if (sender.CancellationPending)
                {
                    e.Cancel = true;
                    return;
                }
            }
        }

        private void 关于ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox AboutBoxDialog = new AboutBox();
            AboutBoxDialog.ShowDialog();
        }

        private void 打开文件ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog fd = new OpenFileDialog();
            fd.Filter = "二进制文件(*.bin)|*.bin";
            if (fd.ShowDialog() == DialogResult.OK)
            {
                this.textBoxFilePath.Text = fd.FileName;
            }
        }

        private void 退出ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void 联系我们ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormContactUs ContactUs = new FormContactUs();
            ContactUs.Show();
        }
    }
}
