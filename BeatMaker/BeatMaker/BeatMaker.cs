using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SGP;
using Microsoft.Xna.Framework.Audio;



namespace BeatMaker
{   

    public partial class BeatMaker : Form
    {

        public ManagedDirect3D D3D = ManagedDirect3D.Instance;

        public BeatMaker()
        {
            InitializeComponent();

            // Init D3D
            D3D.InitManagedDirect3D(this.TrackPanel, true);
        }
        

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void TrackPanel_Paint(object sender, PaintEventArgs e)
        {
            D3D.DeviceBegin();

            bool tool;
            tool = D3D.DrawLine(TrackPanel.Right / 2, TrackPanel.Top, TrackPanel.Right / 2, TrackPanel.Bottom, Color.White);
            //D3D.DrawLine(TrackPanel.Size.Width / 2, 0, TrackPanel.Size.Width / 2, TrackPanel.Size.Height,Color.White);

            D3D.DeviceEnd();
        }
      
    }
}
