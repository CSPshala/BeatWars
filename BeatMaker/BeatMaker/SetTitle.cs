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
    public partial class SetTitle : Form
    {
        string szSongName = "";

        public string SongName
        {
            get { return szSongName; }
            set { szSongName = value; }
        }

        public SetTitle()
        {
            InitializeComponent();
        }

        private void OKButton_Click(object sender, EventArgs e)
        {
            szSongName = SongTitleTextBox.Text;

            this.Close();
        }

        private void SetTitle_Shown(object sender, EventArgs e)
        {
            SongTitleTextBox.Text = szSongName;
        }
       
    }
}
