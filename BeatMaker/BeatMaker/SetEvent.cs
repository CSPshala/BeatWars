using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BeatMaker
{
    public partial class SetEvent : Form
    {
        string szEventName = "";

        public string EventName
        {
            get { return szEventName; }
            set { szEventName = value; }
        }

        public SetEvent()
        {
            InitializeComponent();
        }

        private void SetEvent_Shown(object sender, EventArgs e)
        {
            EventNameTextBox.Text = szEventName;
        }

        private void OKButton_Click(object sender, EventArgs e)
        {
            szEventName = EventNameTextBox.Text;

            this.Close();
        }
    }
}
