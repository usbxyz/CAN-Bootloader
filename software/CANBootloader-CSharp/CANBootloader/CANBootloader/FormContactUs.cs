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
    public partial class FormContactUs : Form
    {
        public FormContactUs()
        {
            InitializeComponent();
        }

        private void linkLabelWebsite_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.usbxyz.com/"); 
        }

        private void linkLabelForum_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.embed-net.com/"); 
        }

        private void linkLabelTaobao_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://usbxyz.taobao.com/"); 
        }

    }
}
