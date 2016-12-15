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

        }

    }
}
