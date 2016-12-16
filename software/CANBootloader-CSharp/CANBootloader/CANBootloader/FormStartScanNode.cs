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
    public partial class FormStartScanNode : Form
    {
        public int StartAddr = -1,EndAddr = -1;
        public FormStartScanNode()
        {
            InitializeComponent();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            if ((this.numericUpDownStartAddr.Value.ToString() != "") && (this.numericUpDownEndAddr.Value.ToString() != ""))
            {
                StartAddr = int.Parse(this.numericUpDownStartAddr.Value.ToString());
                EndAddr = int.Parse(this.numericUpDownEndAddr.Value.ToString());
            }
            this.DialogResult = DialogResult.OK;
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void numericUpDownStartAddr_ValueChanged(object sender, EventArgs e)
        {
            if (this.numericUpDownStartAddr.Value > this.numericUpDownEndAddr.Value)
            {
                this.numericUpDownEndAddr.Value = this.numericUpDownStartAddr.Value;
            }
        }

        private void numericUpDownEndAddr_ValueChanged(object sender, EventArgs e)
        {
            if (this.numericUpDownStartAddr.Value > this.numericUpDownEndAddr.Value)
            {
                this.numericUpDownStartAddr.Value = this.numericUpDownEndAddr.Value;
            }
        }
    }
}
