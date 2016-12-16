using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CANBootloader
{
    public partial class FormMain : Form
    {
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

        private void buttonOpenFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog fd = new OpenFileDialog();
            fd.Filter = "二进制文件(*.bin)|*.bin";
            if (fd.ShowDialog() == DialogResult.OK)
            {
                this.textBoxFilePath.Text = fd.FileName;
            }
        }

        private void buttonUpgrate_Click(object sender, EventArgs e)
        {

        }

        private void buttonSetNewBaudRate_Click(object sender, EventArgs e)
        {

        }

        private void 扫描节点ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormStartScanNode StartScanNode = new FormStartScanNode();
            if (StartScanNode.ShowDialog() == DialogResult.OK)
            {
                FormProgress ScanNode = new FormProgress();
                ScanNode.DoWork += new FormProgress.DoWorkEventHandler(form_DoWork);
                ScanNode.ProgressBar.Maximum = 1000;
                ScanNode.Text = "节点扫描";
                //ScanNode.Argument = checkBoxThrowException.Checked;
                if (ScanNode.ShowDialog() == DialogResult.Cancel)
                {
                    MessageBox.Show("Operation has been cancelled");
                    ScanNode.Close();
                }
                else
                {
                }
            }
            StartScanNode.Close();
        }
        void form_DoWork(FormProgress sender, DoWorkEventArgs e)
        {
            //bool throwException = (bool)e.Argument;
            
            for (int i = 0; i < 1000; i++)
            {
                System.Threading.Thread.Sleep(10);
                //sender.SetProgress(i);
                sender.SetProgress(i, "正在检测节点：" + i.ToString() + " / " + sender.ProgressBar.Maximum + "...");
                if (sender.CancellationPending)
                {
                    e.Cancel = true;
                    return;
                }
                this.listViewNodeList.BeginUpdate();  //数据更新，UI暂时挂起，直到EndUpdate绘制控件，可以有效避免闪烁并大大提高加载速度  
                ListViewItem lvi = new ListViewItem();
                lvi.Text = i.ToString();
                lvi.SubItems.Add("ROOT");
                lvi.SubItems.Add("v1.0");
                this.listViewNodeList.Items.Add(lvi);
                this.listViewNodeList.EndUpdate();  //结束数据处理，UI界面一次性绘制。
            }
            
        }

        private void 关于ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox AboutBoxDialog = new AboutBox();
            //AboutBoxDialog.AssemblyProduct = "USB2XXX CAN Bootloader";
            AboutBoxDialog.ShowDialog();
        }
    }
}
