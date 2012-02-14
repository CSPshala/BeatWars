using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data;
using SGP;


namespace BeatMaker
{   

    public partial class BeatMaker : Form
    {
        // D3D
        public ManagedDirect3D D3D = ManagedDirect3D.Instance;

        //FMOD
        private FMOD.System fmodSystem = null;
        private FMOD.Sound theSong = null;
        private FMOD.Channel fmodChannel = null;
        private uint CurrentPositionMS = 0;
        private uint LengthMS = 0;
        private bool playing = false;
        private bool hasStarted = false;
        private bool paused = false;
        private int channelsplaying = 0;

        // Timer
        private System.Windows.Forms.Timer theTime;

        public BeatMaker()
        {
            InitializeComponent();
            
            // Init D3D
            D3D.InitManagedDirect3D(this.TrackPanel, true);

            // Setting Timer
            theTime = this.timer1;
        }
        

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
        }      

        // Have to override dispose to properly kill FMOD since it's unmanaged
       void Dispose(bool disposing)
        {
            if (disposing)
            {              

                if (theSong != null)
                    theSong.release();              
                
            }
        }

        private void PlayButton_Click(object sender, EventArgs e)
        {
            
            if (!theTime.Enabled)
                theTime.Start();

            
        }
      
    }
}
