using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Windows.Forms;


using SGP;


namespace BeatMaker
{   
    public partial class BeatMaker : Form
    {
        public enum SYMBOL { LEFT, RIGHT, UP, DOWN, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT,WKEY,AKEY,SKEY,DKEY };

        //************MEMBERS***************************

        // D3D
        public ManagedDirect3D D3D = ManagedDirect3D.Instance;
        public ManagedTextureManager TEXMAN = ManagedTextureManager.Instance;

        //FMOD
        private FMOD.System fmodSystem = null;
        private FMOD.Sound theSong = null;
        private FMOD.Channel fmodChannel = null;
        private string szSongName = "";
        private string szSongFileName = "";
        private uint nCurrentPositionMS = 0;
        private uint nLengthMS = 0;
        private bool bPlaying = false;        
        private bool bIsFastforwarding = false;
        private bool bIsRewinding = false;
        private bool bPaused = false;
        //private bool bFreq = false;  
   
        // MISC FORMS
        SetTitle setTitleWindow = null;

        // GAME LOOP
        public bool bRunning = true;
        public bool bListChanged = false;          

        // BEAT LIST
        List<Beat> listBeats = new List<Beat>();

        // BEAT IMAGE NAMES
        private string szAKeyImage = "fire_icon&32.png";
        private string szWKeyImage = "paper_airplane_icon&32.png";
        private string szSKeyImage = "heart_empty_icon&32.png";
        private string szDKeyImage = "lighting_icon&32.png";

        // BEAT SCALING (FOR EASY DISPLAY)
        private float scaleX = 0.0f;
        private float scaleY = 0.0f;
            // Used for zooming in on beat track
        private float fZoom = 1.0f;

        // TEXTURE MANAGER INDICES
        int ArrowLeft, ArrowRight, ArrowUp, ArrowDown, ArrowUpLeft, ArrowUpRight, ArrowDownLeft, ArrowDownRight;
        int KeyW, KeyA, KeyS, KeyD;

        // MY TIMER
        JCTimer theTimer = new JCTimer();
      
        //************CONSTRUCTION / CLEANUP************//
        public BeatMaker()
        {
            InitializeComponent();

            D3D.InitManagedDirect3D(this.TrackPanel, true);
            TEXMAN.InitManagedTextureManager(D3D.Device, D3D.Sprite);

            // Adding Arrows to texture manager
            ArrowLeft = TEXMAN.LoadTexture("Resources/leftarrowblue.png", 0);
            ArrowRight = TEXMAN.LoadTexture("Resources/rightarrowblue.png", 0);
            ArrowUp = TEXMAN.LoadTexture("Resources/uparrowblue.png", 0);
            ArrowDown = TEXMAN.LoadTexture("Resources/downarrowblue.png", 0);
            ArrowUpLeft = TEXMAN.LoadTexture("Resources/upleftarrowblue.png", 0);
            ArrowUpRight = TEXMAN.LoadTexture("Resources/uprightarrowblue.png", 0);
            ArrowDownLeft = TEXMAN.LoadTexture("Resources/downleftarrowblue.png", 0);
            ArrowDownRight = TEXMAN.LoadTexture("Resources/downrightarrowblue.png", 0);

            KeyW = TEXMAN.LoadTexture("Resources/paper_airplane_icon&32.png", 0);
            KeyA = TEXMAN.LoadTexture("Resources/fire_icon&32.png", 0);
            KeyS = TEXMAN.LoadTexture("Resources/heart_empty_icon&32.png", 0);
            KeyD = TEXMAN.LoadTexture("Resources/lighting_icon&32.png", 0);

            scaleX = 0.5f;
            scaleY = 0.5f;

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

                // Killing Texture Manager
                TEXMAN.ShutdownManagedTextureManager();

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
                if (fmodChannel != null)
                {
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
            }

            // If the list has changed, we sort it
            if (bListChanged)
            {
                BeatCountLabel.Text = listBeats.Count.ToString();

                listBeats.Sort();
                bListChanged = false;
            }

            // Updating timer
            theTimer.Update();

            base.Update();
        }

        public void Input()
        {

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

            // Drawing beats and arrows
            DrawBeats();
            
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
                
        private void DrawSong()
        {
            if (fmodChannel != null)
            {             
                //*********************DRAWING MATH********************************//               
                // Pixel offset for numbers / tics original position
                // Effectively these are the seconds displayed
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
             

                // Drawing Horizontal arrow line
                D3D.DrawLine(TrackPanel.Left, Halfsies + 30, TrackPanel.Right, Halfsies + 30, Color.CornflowerBlue);   
                
                // Drawing Horizontal beat note line
                D3D.DrawLine(TrackPanel.Left, Halfsies - 30, TrackPanel.Right, Halfsies - 30, Color.Crimson);
                
                // Premade first tic
                float x1 = TrackPanel.Left + xoffset;
                float y1 = TrackPanel.Bottom - 20;
                float x2 = x1;
                float y2 = TrackPanel.Bottom;

                // Total seconds for use in drawing numbers
                int nTotalSeconds = (int)(nLengthMS / 1000);

                //***********************DRAWING BEGIN********************************//

                // Putting up song name at top
                if (szSongName.Length > 0)
                    D3D.DrawText(szSongName, TrackPanel.Right - (szSongName.Length * 10), TrackPanel.Top, Color.LawnGreen);

                // Drawing Tic Marks and Second Numbers
                for (int i = 0; i < totalTics; ++i)
                {

                    // Frequency check
                    //if(!bFreq)
                        // Drawing Tics
                        D3D.DrawLineF((x1 * i) - songoffset , y1, (x2 * i) - songoffset, y2, Color.LawnGreen);                 
                        

                    // Draws numbers from original half screen position till end of song
                    //   v--- this retardation is just tweaking to get shit lined up right
                    D3D.DrawText(i.ToString(), (XHalfsies + (xoffset * i) + (i * 3)) - (int)songoffset, (int)y1 - 20, Color.LawnGreen);
                }          
                

               // Spectrum stuff I might mess with if given the time
               // float[] spec = new float[200];

               // fmodChannel.getSpectrum(spec, 100, 0, FMOD.DSP_FFT_WINDOW.RECT);

              
            }
        }

        private void DrawBeats()
        {
            if (fmodChannel != null)
            {
                //*********************DRAWING MATH********************************//               
                // Pixel offset for numbers / tics original position
                // Effectively these are the seconds displayed
                int xoffset = 34;

                // Pixel offset for scrolling speed based on current song time                                  (1.258f works really well so far)
                //V-- tweak this for speed (divide = move faster, mult = slower, negative = move backwards) I'm so cool 
                float songoffset = (nCurrentPositionMS * 0.001f) * (1000.0f / (float)(xoffset / 1.258f));

                // Cutting down on divide ops
                int Halfsies = TrackPanel.Bottom / 2;
                int XHalfsies = TrackPanel.Right / 2;

                // Getting scope to display beats
                float displayFrontArea = nCurrentPositionMS * 0.001f + 15;
                float displayBackArea = displayFrontArea - 30;             

               

                // Cycling through beat list
                for (int i = 0; i < listBeats.Count; ++i)
                {
                    // Time of beat in MS float style
                    float timeofbeat = listBeats[i].TimeOfBeat * 0.001f;

                    // Trying to cull notes so we don't display everything off screen
                    if (timeofbeat <= displayFrontArea)
                    {
                        if (timeofbeat >= displayBackArea)
                        {
                            if (listBeats[i].Completion == BEATIS.ARROW || listBeats[i].Completion == BEATIS.COMPLETE)
                            {
                                // Current note's offset based on the time of the beat.  Don't ask me where I come up with this shit.
                                float noteOffset = (listBeats[i].TimeOfBeat * 0.001f) *(1000.0f / (float)(xoffset / 1.258f));

                                TEXMAN.Draw(listBeats[i].TextureIndex, XHalfsies + (int)(noteOffset - songoffset), Halfsies + 50, 1.0f, 1.0f, Rectangle.Empty, 0, 0, 0, 0);

                                // Debug Text Draw for placement
                                // D3D.DrawText("|", (XHalfsies + xoffset * i) - (int)songoffset, Halfsies + 50,Color.Red);
                            }

                            if (listBeats[i].Completion == BEATIS.KEY || listBeats[i].Completion == BEATIS.COMPLETE)
                            {
                                float scaleX = 1.0f;
                                float scaleY = 1.0f;                                

                                // Current note's offset based on the time of the beat.  Don't ask me where I come up with this shit.
                                float noteOffset = (listBeats[i].TimeOfBeat * 0.001f) * (1000.0f / (float)(xoffset / 1.258f));

                                TEXMAN.Draw(listBeats[i].TextureIndex, XHalfsies + (int)(noteOffset - songoffset), Halfsies - 60, scaleX, scaleY, Rectangle.Empty, 0, 0, 0,0);
                            }
                        }
                    }
                    else
                        break;
                }
            }

        }

        private void ResetEverything()
        {
            // Stopping Music
            if(fmodChannel != null)
                fmodChannel.stop();

            fmodSystem = null;
            fmodChannel = null;
            theSong = null;
            szSongName = "";
            nCurrentPositionMS = 0;
            nLengthMS = 0;
            bPlaying = false;
            bIsFastforwarding = false;
            bIsRewinding = false;
            bPaused = false;
            //bFreq = false;
            bRunning = true;
            bListChanged = false;

            listBeats = new List<Beat>();

            TimeCurrentLabel.Text = "0(s)";
            TimeLengthLabel.Text = "0(s)";
            BeatCountLabel.Text = "0";
            BPMCurrentLabel.Text = "0";

            // Clean shit up RIGHT NAO!!!
            GC.Collect();
        }

        private void AddBeat(Keys dir)
        {
            Beat tempBeat = new Beat();

            switch (dir)
            {
                // Left Arrow
                case Keys.NumPad4:
                    {
                        LeftPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "left";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.TextureIndex = ArrowLeft;
                    }
                    break;

                // Right Arrow
                case Keys.NumPad6:
                    {
                        RightPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "right";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.TextureIndex = ArrowRight;
                    }
                    break;

                // Up Arrow
                case Keys.NumPad8:
                    {
                        UpPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "up";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.TextureIndex = ArrowUp;
                    }
                    break;

                // Down Arrow
                case Keys.NumPad2:
                    {
                        DownPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "down";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.TextureIndex = ArrowDown;
                    }
                    break;

                // Down Left Arrow
                case Keys.NumPad1:
                    {
                        DownLeftPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "leftdown";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.TextureIndex = ArrowDownLeft;
                    }
                    break;

                // Down Right Arrow
                case Keys.NumPad3:
                    {
                        DownRightPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "rightdown";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.TextureIndex = ArrowDownRight;
                    }
                    break;

                // Up Left Arrow
                case Keys.NumPad7:
                    {
                        UpLeftPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "leftup";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.TextureIndex = ArrowUpLeft;
                    }
                    break;

                // Up Right Arrow
                case Keys.NumPad9:
                    {
                        UpRightPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "rightup";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.TextureIndex = ArrowUpRight;
                    }
                    break;

                // W Key
                case Keys.W:
                    {
                        WKeyPictureBox.BackColor = Color.Crimson;
                        tempBeat.KeyPress = 'w';
                        tempBeat.Image = szWKeyImage;
                        tempBeat.Completion = BEATIS.KEY;
                        tempBeat.TextureIndex = KeyW;
                    }
                    break;

                // A Key
                case Keys.A:
                    {
                        AKeyPictureBox.BackColor = Color.Crimson;
                        tempBeat.KeyPress = 'a';
                        tempBeat.Image = szAKeyImage;
                        tempBeat.Completion = BEATIS.KEY;
                        tempBeat.TextureIndex = KeyA;
                    }
                    break;

                // S Key
                case Keys.S:
                    {    
                        SKeyPictureBox.BackColor = Color.Crimson;
                        tempBeat.KeyPress = 's';
                        tempBeat.Image = szSKeyImage;
                        tempBeat.Completion = BEATIS.KEY;
                        tempBeat.TextureIndex = KeyS;
                    }
                    break;

                // D Key
                case Keys.D:
                    {
                        DKeyPictureBox.BackColor = Color.Crimson;
                        tempBeat.KeyPress = 'd';
                        tempBeat.Image = szDKeyImage;
                        tempBeat.Completion = BEATIS.KEY;
                        tempBeat.TextureIndex = KeyD;
                    }
                    break;

                default:
                    return;
            }


            // Only setting beat time if it's not assigned yet
            fmodChannel.getPosition(ref tempBeat.nTimeOfBeat, FMOD.TIMEUNIT.MS);
            tempBeat.Width = 32;
            tempBeat.Height = 32;

            listBeats.Add(tempBeat);

            Console.WriteLine(tempBeat.TimeOfBeat.ToString());

            Console.WriteLine(tempBeat.TimeOfBeatS.ToString());

            bListChanged = true;
        }

        //****************MISC EVENTS************************//
        private void BeatMaker_KeyDown(object sender, KeyEventArgs e)
        {
            AddBeat(e.KeyCode); 
        }

        private void BeatMaker_KeyUp(object sender, KeyEventArgs e)
        {

            // Setting background of key up back to normal
             switch (e.KeyCode)
            {

                    // Left Arrow
            case Keys.NumPad4:
                {
                    LeftPictureBox.BackColor = SystemColors.Control;
                }
                break;

                    // Right Arrow
            case Keys.NumPad6:
                {
                    RightPictureBox.BackColor = SystemColors.Control;
                }
                break;

                    // Up Arrow
            case Keys.NumPad8:
                {
                    UpPictureBox.BackColor = SystemColors.Control;
                }
                break;

                    // Down Arrow
            case Keys.NumPad2:
                {
                    DownPictureBox.BackColor = SystemColors.Control;
                }
                break;

                    // Down Left Arrow
            case Keys.NumPad1:
                {
                    DownLeftPictureBox.BackColor = SystemColors.Control;
                }
                break;

                    // Down Right Arrow
            case Keys.NumPad3:
                {
                    DownRightPictureBox.BackColor = SystemColors.Control;
                }
                break;

                    // Up Left Arrow
            case Keys.NumPad7:
                {
                    UpLeftPictureBox.BackColor = SystemColors.Control;
                }
                break;

                    // Up Right Arrow
            case Keys.NumPad9:
                {
                    UpRightPictureBox.BackColor = SystemColors.Control;
                }
                break;

                    // W Key
            case Keys.W:
                {
                    WKeyPictureBox.BackColor = SystemColors.Control;
                }
                break;

                    // A Key
            case Keys.A:
                {
                    AKeyPictureBox.BackColor = SystemColors.Control;
                }
                break;

                    // D Key
            case Keys.D:
                {
                    DKeyPictureBox.BackColor = SystemColors.Control;
                }
                break;
                    
                    // S Key
            case Keys.S:
                {
                    SKeyPictureBox.BackColor = SystemColors.Control;
                }
                break;

            }
        }

        void setTitleWindow_FormClosed(object sender, FormClosedEventArgs e)
        {
            szSongName = setTitleWindow.SongName;
            setTitleWindow = null;
        }
       

        //*****************PLAYBACK AND EDITOR BUTTONS*******//

        private void PlayButton_Click(object sender, EventArgs e)
        {
            if(!bPlaying && fmodSystem != null)
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
            if (fmodChannel != null)
            {
                if (!bPaused)
                    fmodChannel.setPaused(true);
                else
                    fmodChannel.setPaused(false);
            }
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            // Stopping song'
            if(fmodChannel != null)
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
        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "XML Files | *.xml;*.XML";
            dlg.Title = "Save Active Beat List";
            dlg.FilterIndex = 2;
            dlg.DefaultExt = "xml";
            dlg.AddExtension = true;

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                XElement xRoot = new XElement("Song");

                if(szSongName == "")
                    if (System.Windows.Forms.MessageBox.Show("The song currently has no title.  Please set.") == DialogResult.OK)
                    {
                        setSongTitleToolStripMenuItem_Click(null, null);
                    }

                // Setting song attributes
                XAttribute xFile = new XAttribute("file",szSongFileName);
                XAttribute xSongName = new XAttribute("name", szSongName);
                XAttribute xDuration = new XAttribute("duration", nLengthMS);

                // Adding attributes to song element
                xRoot.Add(xFile);
                xRoot.Add(xSongName);
                xRoot.Add(xDuration);

                for (int i = 0; i < listBeats.Count; ++i)
                {
                    XElement xBeat = new XElement("Beat");

                    // Getting and adding all beat stats
                    XAttribute xTimeofbeat = new XAttribute("timeofbeat",listBeats[i].TimeOfBeat);
                    xBeat.Add(xTimeofbeat);

                    XAttribute xDirection = new XAttribute("direction", listBeats[i].Direction);
                    xBeat.Add(xDirection);

                    XAttribute xKey = new XAttribute("key", listBeats[i].KeyPress);
                    xBeat.Add(xKey);

                    XAttribute xImage = new XAttribute("image", listBeats[i].Image);
                    xBeat.Add(xImage);

                    XAttribute xDifficulty = new XAttribute("difficulty", listBeats[i].Difficulty);
                    xBeat.Add(xDifficulty);

                    XAttribute xWidth = new XAttribute("width", listBeats[i].Width);
                    xBeat.Add(xWidth);

                    XAttribute xHeight = new XAttribute("height", listBeats[i].Height);
                    xBeat.Add(xHeight);

                    XAttribute xBeatIs = new XAttribute("beatis", (int)listBeats[i].Completion);
                    xBeat.Add(xBeatIs);

                    XAttribute xIndex = new XAttribute("imageindex", listBeats[i].TextureIndex);
                    xBeat.Add(xIndex);

                    // Adding beat to Song
                    xRoot.Add(xBeat);
                }




                // We've gotten all the beats, and now saving to the file path specified
                xRoot.Save(dlg.FileName);


                // Cleaning up b/c of all the news and I'm anal like that
                GC.Collect();
            }
        }

        private void beatListxmlToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.OpenFileDialog openDlg = new OpenFileDialog();

            openDlg.Filter = "All Files(*.*)|*.*|XML Files(*.xml)|*.xml";
            openDlg.FilterIndex = 2;
            // Set the default extension
            openDlg.DefaultExt = "xml";

            //*****************OPENING DIALOG***********************//
            if (openDlg.ShowDialog() == DialogResult.OK)
            {


                XElement root = XElement.Load(openDlg.FileName);

                string associatedFilePath = "";
                string associatedFileName = "";

                //******************SETTING FILEPATH FROM SETTINGS *********************//
                if (Properties.Settings.Default.AssociatedFileDirectory != null)
                {
                    // The Associated File Directory is saves in the programms settings
                    // this is retreiving the directory from the programs settings and setting it up for use                    
                    associatedFilePath = Properties.Settings.Default.AssociatedFileDirectory;
                }


                //******************GETTING SONG FILENAME*****************************//
                XAttribute fileName = root.Attribute("file");

                if (fileName != null)
                {
                    associatedFileName = Convert.ToString(fileName.Value);
                    szSongFileName = associatedFileName;
                }

                XAttribute songName = root.Attribute("name");

                if (songName != null)
                    szSongName = Convert.ToString(songName.Value);

                


                //*****************LOADING MUSIC FILE***********************************//
                if (!System.IO.File.Exists(associatedFilePath + associatedFileName))
                {
                    // the file was not found so we must reaquire the associated file
                    if (System.Windows.Forms.MessageBox.Show("\"" + associatedFileName + "\" can not be found in \"" + associatedFilePath + "\". Please relocate " + associatedFileName) == DialogResult.OK)
                    {
                        musicTrackToolStripMenuItem_Click(null, null);
                    }
                }
                else
                {
                    LoadMusic(associatedFilePath + associatedFileName);

                    szSongFileName = associatedFileName;
                }              


                //*****************LOAD BEATS******************************************//
                IEnumerable<XElement> xBeats = root.Elements("Beat");

                if (xBeats != null)
                {
                    foreach (XElement xBeat in xBeats)
                    {
                        Beat aBeat = new Beat();

                        // Getting and adding all beat stats
                        XAttribute xTimeofbeat = xBeat.Attribute("timeofbeat");
                        aBeat.TimeOfBeat = Convert.ToUInt32(xTimeofbeat.Value);

                        XAttribute xDirection = xBeat.Attribute("direction");
                        aBeat.Direction = xDirection.Value;

                        XAttribute xKey = xBeat.Attribute("key");
                        aBeat.KeyPress = Convert.ToChar(xKey.Value);

                        XAttribute xImage = xBeat.Attribute("image");
                        aBeat.Image = xImage.Value;

                        XAttribute xDifficulty = xBeat.Attribute("difficulty");
                        aBeat.Difficulty = xDifficulty.Value;

                        XAttribute xWidth = xBeat.Attribute("width");
                        aBeat.Width = Convert.ToInt32(xWidth.Value);

                        XAttribute xHeight = xBeat.Attribute("height");
                        aBeat.Height = Convert.ToInt32(xHeight.Value);

                        XAttribute xBeatIs = xBeat.Attribute("beatis");
                        aBeat.Completion = (BEATIS)Convert.ToInt32(xBeatIs.Value);

                        XAttribute xIndex = xBeat.Attribute("imageindex");
                        aBeat.TextureIndex = Convert.ToInt32(xIndex.Value);

                        // Adding beat to beatList
                        listBeats.Add(aBeat);
                    }
                }

                // Making sure the
                bListChanged = true;

                // Again..... being anal
                GC.Collect();
            }
        }

        private void musicTrackToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.OpenFileDialog openDlg = new OpenFileDialog();

            PauseButton_Click(null, null);

            openDlg.Filter = "All Files(*.*)|*.*|MP3 Files(*.mp3)|*.mp3|WAV Files(*.wav)|*.wav|WMA Files(*.wma)|*.wma|OGG Files(*.ogg)|*.ogg";
            openDlg.FilterIndex = 2;
            // Set the default extension
            openDlg.DefaultExt = ".mp3";

            //*****************OPENING DIALOG***********************//
            if (openDlg.ShowDialog() == DialogResult.OK)
            {
                StopButton_Click(null, null);
                LoadMusic(openDlg.FileName);

                szSongFileName = openDlg.SafeFileName;                

                // Saving file path
                Properties.Settings.Default.AssociatedFileDirectory = openDlg.FileName.Replace(openDlg.SafeFileName,"");
                Properties.Settings.Default.Save();
            }        
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void setSongTitleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (setTitleWindow == null)
            {
                setTitleWindow = new SetTitle();
                setTitleWindow.FormClosed += new FormClosedEventHandler(setTitleWindow_FormClosed);
                setTitleWindow.SongName = szSongName;
                setTitleWindow.ShowDialog();
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ResetEverything();
        }

        private void loadWImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.OpenFileDialog openDlg = new OpenFileDialog();

            openDlg.Filter = "All Files(*.*)|*.*|PNG Files(*.png)|*.png|JPG Files(*.jpg)|*.jpg|BMP Files(*.bmp)|*.bmp";
            openDlg.FilterIndex = 2;
            // Set the default extension
            openDlg.DefaultExt = "xml";

            //*****************OPENING DIALOG***********************//
            if (openDlg.ShowDialog() == DialogResult.OK)
            {  
                szWKeyImage = openDlg.SafeFileName;

                WKeyPictureBox.BackgroundImage = new Bitmap(openDlg.FileName);

                KeyW =  TEXMAN.LoadTexture(openDlg.FileName,0);

                // Saving file path
                Properties.Settings.Default.IconFilePath = openDlg.FileName.Replace(openDlg.SafeFileName, "");
                Properties.Settings.Default.Save();


                if (TEXMAN.GetTextureWidth(KeyW) >= 32) 
                scaleX = 0.5f;
                if (TEXMAN.GetTextureHeight(KeyW) >= 32) 
                scaleY = 0.5f;
            }
        }

        private void loadAImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.OpenFileDialog openDlg = new OpenFileDialog();

            openDlg.Filter = "All Files(*.*)|*.*|PNG Files(*.png)|*.png|JPG Files(*.jpg)|*.jpg|BMP Files(*.bmp)|*.bmp";
            openDlg.FilterIndex = 2;
            // Set the default extension
            openDlg.DefaultExt = "xml";

            //*****************OPENING DIALOG***********************//
            if (openDlg.ShowDialog() == DialogResult.OK)
            {
                szAKeyImage = openDlg.SafeFileName;

                KeyA = TEXMAN.LoadTexture(openDlg.FileName, 0);

                AKeyPictureBox.BackgroundImage = new Bitmap(openDlg.FileName);

                // Saving file path
                Properties.Settings.Default.IconFilePath = openDlg.FileName.Replace(openDlg.SafeFileName, "");
                Properties.Settings.Default.Save();

                scaleX = TEXMAN.GetTextureWidth(KeyA);
                scaleY = TEXMAN.GetTextureHeight(KeyA);
            }
        }

        private void loadSImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.OpenFileDialog openDlg = new OpenFileDialog();

            openDlg.Filter = "All Files(*.*)|*.*|PNG Files(*.png)|*.png|JPG Files(*.jpg)|*.jpg|BMP Files(*.bmp)|*.bmp";
            openDlg.FilterIndex = 2;
            // Set the default extension
            openDlg.DefaultExt = "xml";

            //*****************OPENING DIALOG***********************//
            if (openDlg.ShowDialog() == DialogResult.OK)
            {
                szSKeyImage = openDlg.SafeFileName;

                KeyS = TEXMAN.LoadTexture(openDlg.FileName, 0);

                SKeyPictureBox.BackgroundImage = new Bitmap(openDlg.FileName);

                // Saving file path
                Properties.Settings.Default.IconFilePath = openDlg.FileName.Replace(openDlg.SafeFileName, "");
                Properties.Settings.Default.Save();

                scaleX = TEXMAN.GetTextureWidth(KeyS);
                scaleY = TEXMAN.GetTextureHeight(KeyS);
            }
        }

        private void loadDImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.OpenFileDialog openDlg = new OpenFileDialog();

            openDlg.Filter = "All Files(*.*)|*.*|PNG Files(*.png)|*.png|JPG Files(*.jpg)|*.jpg|BMP Files(*.bmp)|*.bmp";
            openDlg.FilterIndex = 2;
            // Set the default extension
            openDlg.DefaultExt = "xml";

            //*****************OPENING DIALOG***********************//
            if (openDlg.ShowDialog() == DialogResult.OK)
            {
                szDKeyImage = openDlg.SafeFileName;

                KeyD = TEXMAN.LoadTexture(openDlg.FileName, 0);

                DKeyPictureBox.BackgroundImage = new Bitmap(openDlg.FileName);

                // Saving file path
                Properties.Settings.Default.IconFilePath = openDlg.FileName.Replace(openDlg.SafeFileName, "");
                Properties.Settings.Default.Save();

                scaleX = TEXMAN.GetTextureWidth(KeyD);
                scaleY = TEXMAN.GetTextureHeight(KeyD);
            }
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

        private void GetWaveform()
        {

        }

     
      
    
    }
}
