using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SGP;


namespace BeatMaker
{   

    public partial class BeatMaker : Form
    {

        //************MEMBERS***************************//
        // D3D
        public ManagedDirect3D D3D = ManagedDirect3D.Instance;

        //FMOD
        private FMOD.System fmodSystem = null;
        private FMOD.Sound theSong = null;
        private FMOD.Channel fmodChannel = null;
        private uint nCurrentPositionMS = 0;
        private uint nLengthMS = 0;
        private bool bPlaying = false;
        private bool bHasStarted = false;
        private bool bIsFastforwarding = false;
        private bool bIsRewinding = false;
        private bool bPaused = false;
        private int nChannelsplaying = 0;

        // GAME LOOP
        public bool bRunning = true;

        // MY TIMER
        JCTimer theTimer = new JCTimer();
      
        //************CONSTRUCTION / CLEANUP************//
        public BeatMaker()
        {
            InitializeComponent();

            D3D.InitManagedDirect3D(this.TrackPanel, true);

            LoadMusic("levels.mp3");      
      
            // Init timer
            theTimer.InitTimer();
        }

        // Have to override dispose to properly kill FMOD stuff
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (components != null)
                    components.Dispose();

                if (theSong != null)
                    theSong.release();

                if (fmodSystem != null)
                    fmodSystem.release();

                // Killing D3D
                D3D.Shutdown();

            }
            base.Dispose(disposing);
        }

        private void BeatMaker_FormClosing(object sender, FormClosingEventArgs e)
        {
            bRunning = false;
        }   

        //***************GAME LOOP***************************//
        new public void Update()
        {
            // Updating FMOD variables
            if (fmodChannel != null)
            {         
                // Finding out if paused
                fmodChannel.getPaused(ref bPaused);

                // Finding out if playing
                fmodChannel.isPlaying(ref bPlaying);

                // Updating current song time in MS
                fmodChannel.getPosition(ref nCurrentPositionMS, FMOD.TIMEUNIT.MS);               

                // Updating labels in info pane

                    // Current Time label update (color change and time)
                    if (bPlaying && !bPaused)
                        TimeCurrentLabel.ForeColor = Color.Black;
                    else
                        TimeCurrentLabel.ForeColor = Color.Red;

                    TimeCurrentLabel.Text = ((nCurrentPositionMS * 0.001).ToString("F1")) + "(s)";

                // Checking for fastforward
                if (bIsFastforwarding)
                {
                    // Moving forward by 100 MS when held down
                    uint CurPos = 0;
                    fmodChannel.getPosition(ref CurPos, FMOD.TIMEUNIT.MS);
                    fmodChannel.setPosition(CurPos + 100, FMOD.TIMEUNIT.MS);
                }

                if (bIsRewinding)
                {
                    // Moving backwards by 100 MS when held down
                    uint CurPos = 0;
                    fmodChannel.getPosition(ref CurPos, FMOD.TIMEUNIT.MS);
                    fmodChannel.setPosition(CurPos - 100, FMOD.TIMEUNIT.MS);
                }

            }

            // Updating timer
            theTimer.Update();

            base.Update();
        }

        public void Render()
        {
            // Clearing to Dim Gray
            D3D.Clear(105, 105, 105);
            D3D.DeviceBegin();
            D3D.SpriteBegin();
            D3D.LineBegin();           

            // Drawing current
            D3D.DrawLine(TrackPanel.Right / 2, TrackPanel.Top, TrackPanel.Right / 2, TrackPanel.Bottom, Color.WhiteSmoke);

            // Drawing running time to test timer
            D3D.DrawText(theTimer.RunningTime.Seconds.ToString(), 10, 10, Color.Red);

            D3D.LineEnd();
            D3D.SpriteEnd();
            D3D.DeviceEnd();

            D3D.Present();
        }


        //*****************PLAYBACK AND EDITOR BUTTONS*******//

        private void PlayButton_Click(object sender, EventArgs e)
        {
            if(!bPlaying)
                fmodSystem.playSound(FMOD.CHANNELINDEX.FREE, theSong, false, ref fmodChannel);

            // Also allowing play to unpause just for user-friendlyness (new word)
            if (fmodChannel != null)
            {
                if (bPaused)
                    fmodChannel.setPaused(false); 
            }
        }

        private void PauseButton_Click(object sender, EventArgs e)
        {
            if (!bPaused)
                fmodChannel.setPaused(true);
            else
                fmodChannel.setPaused(false);
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            // Stopping song
            fmodChannel.stop();
        }

        private void FastForwardButton_MouseDown(object sender, MouseEventArgs e)
        {    
            bIsFastforwarding = true;
        }

        private void FastForwardButton_MouseUp(object sender, MouseEventArgs e)
        {
            bIsFastforwarding = false;
        }

        private void RewindButton_MouseDown(object sender, MouseEventArgs e)
        {
            bIsRewinding = true;
        }

        private void RewindButton_MouseUp(object sender, MouseEventArgs e)
        {
            bIsRewinding = false;
        }       

     

        //*****************MAIN MENU ITEM STUFF***********************//
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        //*****************FMOD SPECIFIC METHODS**************//
        private void LoadMusic(String szFilePath)
        {
            uint version = 0;           

            // Creating FMOD system
            FMOD.Factory.System_Create(ref fmodSystem);

            // Getting version
            fmodSystem.getVersion(ref version);

            // Making sure we're using the right FMOD version, else kill the application (for now)
            if (version < FMOD.VERSION.number)
            {
                MessageBox.Show("Error!  You are using an old version of FMOD " + version.ToString("X") + ".  This program requires " + FMOD.VERSION.number.ToString("X") + ".");
                Application.Exit();
            }

            // Initing FMOD system
            fmodSystem.init(32, FMOD.INITFLAGS.NORMAL, (IntPtr)null);

            // Loading the music
            fmodSystem.createSound(szFilePath, FMOD.MODE.HARDWARE, ref theSong);

            // Updating song length in info pane                 
            theSong.getLength(ref nLengthMS, FMOD.TIMEUNIT.MS);
            TimeLengthLabel.Text = ((nLengthMS * 0.001).ToString("F1")) + "(s)";

            
            // GOOD TO GO!
        }
       
    }
}
