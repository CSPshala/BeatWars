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
        //private bool bHasStarted = false;
        private bool bIsFastforwarding = false;
        private bool bIsRewinding = false;
        private bool bPaused = false;
        private bool bFreq = false;
        //private int nChannelsplaying = 0;

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

           // fmodChannel.getSpectrum(
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

            // Drawing running time to test timer
            D3D.DrawText(theTimer.RunningTime.Seconds.ToString(), 10, 10, Color.Red);

            // Drawing song line and count in track window
            DrawSong();

            // Cutting down on divide ops
            int Halfsies = TrackPanel.Right / 2;

            // Drawing current time pointer over everything
            D3D.DrawLine(Halfsies, TrackPanel.Top, Halfsies, TrackPanel.Bottom, Color.WhiteSmoke);

            D3D.LineEnd();
            D3D.SpriteEnd();
            D3D.DeviceEnd();

            D3D.Present();
        }

        //***************UTILITY FUNCTIONS********************//

        // Draws line for song, along with time
        private void DrawSong()
        {
            if (fmodChannel != null)
            {
                // Pixel offset for numbers / tics original position
                int xoffset = 34;     

                // Pixel offset for scrolling speed based on current song time                                  (1.258f works really well so far)
                                                                                                 //V-- tweak this for speed (divide = move faster, mult = slower, negative = move backwards) I'm so cool 
                float songoffset = (nCurrentPositionMS * 0.001f) * (1000.0f / (float)(xoffset / 1.258f)); 

                // Cutting down on divide ops
                int Halfsies = TrackPanel.Bottom / 2;
                int XHalfsies = TrackPanel.Right / 2;

                // Getting tics that should be on screen at a time
                int Tics = TrackPanel.Right / xoffset;
                // Getting total tics for the song
                int totalTics = (int)(nLengthMS / 1000) + xoffset;
             

                // Drawing Horizontal song line
                D3D.DrawLine(TrackPanel.Left, Halfsies, TrackPanel.Right, Halfsies, Color.LawnGreen);                           
                
                // Premade first tic
                float x1 = TrackPanel.Left + xoffset;
                float y1 = Halfsies - 10;
                float x2 = x1;
                float y2 = Halfsies + 10;

                // Total seconds for use in drawing numbers
                int nTotalSeconds = (int)(nLengthMS / 1000);

                for (int i = 0; i < totalTics; ++i)
                {

                    if(!bFreq)
                        // Drawing Tics
                        D3D.DrawLineF((x1 * i) - songoffset , y1, (x2 * i) - songoffset, y2, Color.LawnGreen);                 
                        

                    // Draws numbers from original half screen position till end of song
                    //   v--- this retardation is just tweaking to get shit lined up right
                    D3D.DrawText(i.ToString(), (XHalfsies + (xoffset * i) + (i * 3)) - (int)songoffset, (int)y2 + 5, Color.LawnGreen);
                }             

              
            }
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
