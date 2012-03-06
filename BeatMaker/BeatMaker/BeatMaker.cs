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
        SetEvent setEventWindow = null;
        string szDifficulty = "easy";
        float playSpeed = 1.0f; // Play speed modifier in MS (2 = half etc)
        float originalFreq = 0.0f;

        // GAME LOOP
        public bool bRunning = true;
        public bool bListChanged = false;    
      
        // MOUSE STUFF
        public bool bMouseInTrack = false;
        bool bMouseSelect = false;
        public int nMouseSelectedIndex = -1;
        public int nMouseClickedIndex = -1;
        Point pMouseSelectedCoords;        
        Beat MouseAddBeat = new Beat();        
        int nMouseScroll = 0;
        Point MouseSelectStartPoint;
        Rectangle MouseSelectRect;
        List<int> SelectedBeats = new List<int>();
        string szClickedEventEdit;

        // COPY PASTE STUFF
        List<int> CopiedBeats = new List<int>();

        // BEAT LIST
        List<Beat> listBeats = new List<Beat>();
        

        // BEAT IMAGE NAMES
        private string szAKeyImage = "fire_icon&32.png";
        private string szWKeyImage = "paper_airplane_icon&32.png";
        private string szSKeyImage = "heart_empty_icon&32.png";
        private string szDKeyImage = "lighting_icon&32.png";

        // BEAT SCALING (FOR EASY DISPLAY)
        private float scaleX = 1.0f;
        private float scaleY = 1.0f;
            // Used for zooming in on beat track
        private int nZoom = 1;

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

            // Zoom mousewheel event
            this.MouseWheel += new MouseEventHandler(BeatMaker_MouseWheel);

            // Difficulty label value
            DifficultyValueLabel.Text = szDifficulty;
                        
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

            // If the list has changed, we sort it
            if (bListChanged)
            {
                BeatCountLabel.Text = listBeats.Count.ToString();

                listBeats.Sort();
                bListChanged = false;
            }

            // Updating timer
            theTimer.Update();

            // Setting Zoom back to 1 if user dropped it into the negatives
            if (nZoom < 1)
                nZoom = 1;

            // Checking mouse's position and if it's on a note / arrow
            if (bMouseInTrack)
            {
                nMouseScroll = Cursor.Position.X;
                nMouseSelectedIndex = CheckMousePos();
            }

            // Updating note info box
            UpdateNoteInfo();
           
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

            // Drawing zoom
            D3D.DrawText("Zoom: " + nZoom.ToString() + "X", 10, 10, Color.Red);
            D3D.DrawText("Speed: " + playSpeed.ToString() + "X", 130, 10, Color.Red);

            // Drawing song line and count in track window
            DrawSong();

            // Drawing beats and arrows
            DrawBeats();

            // Drawing mouse selected beat at mousepoint
            DrawBeatMousePlacement();   
         
            // Drawing Selection rectangle
            DrawSelectRect();
            
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
                float songoffset = (nCurrentPositionMS * 0.001f * nZoom) * (1000.0f / (float)(xoffset / 1.258f)); 

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
                        D3D.DrawLineF(((x1 * i * nZoom) - songoffset) , y1, ((x2 * i * nZoom) - songoffset) , y2, Color.LawnGreen);                 
                        

                    // Draws numbers from original half screen position till end of song
                    //   v--- this retardation is just tweaking to get shit lined up right
                    D3D.DrawText(i.ToString(), ((XHalfsies + (xoffset * i * nZoom) + (i * 3 * nZoom)) - (int)songoffset) , (int)y1 - 20, Color.LawnGreen);
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
                float songoffset = (nCurrentPositionMS * 0.001f * nZoom) * (1000.0f / (float)(xoffset / 1.258f));

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
                            // Current note's offset based on the time of the beat.  Don't ask me where I come up with this shit.
                            float noteOffset = (listBeats[i].TimeOfBeat * 0.001f * nZoom) * (1000.0f / (float)(xoffset / 1.258f));

                            // Points for line drawing on complete notes
                            Point pArrow = new Point();
                            Point pNote = new Point();

                            if (listBeats[i].Completion == BEATIS.ARROW || listBeats[i].Completion == BEATIS.COMPLETE) 
                            {
                                pArrow.X = XHalfsies + (int)(noteOffset - songoffset);
                                pArrow.Y = Halfsies + 50;

                                // Drawing rect if mouse is hovering over it
                                if (i == nMouseSelectedIndex)
                                    D3D.DrawEmptyRect(new Rectangle(pArrow.X - 5, pArrow.Y - 5, (listBeats[i].Width / 2) + 10, (listBeats[i].Height / 2) + 10), Color.CornflowerBlue);
                                // Drawing rect if note is clicked on
                                for(int x = 0; x < SelectedBeats.Count; ++x)
                                    if (i == SelectedBeats[x])
                                    {
                                        D3D.DrawEmptyRect(new Rectangle(pArrow.X - 5, pArrow.Y - 5, (listBeats[i].Width / 2) + 10, (listBeats[i].Height / 2) + 10), Color.Goldenrod);
                                        break;
                                    }

                                // Drawing Arrow Icon
                                TEXMAN.Draw(listBeats[i].ArrowTextureIndex,pArrow.X, pArrow.Y, 1.0f, 1.0f, Rectangle.Empty, 0, 0, 0, 0);                                 
                            }

                            if (listBeats[i].Completion == BEATIS.KEY || listBeats[i].Completion == BEATIS.COMPLETE)
                            {
                                pNote.X =  XHalfsies + (int)(noteOffset - songoffset);
                                pNote.Y =  Halfsies - 60;

                                // Drawing rect if mouse is hovering over it
                                if (i == nMouseSelectedIndex)
                                    D3D.DrawEmptyRect(new Rectangle(pNote.X - 5, pNote.Y - 5, (listBeats[i].Width / 2) + 10, (listBeats[i].Height / 2) + 10), Color.Crimson);
                                // Drawing rect if note is clicked on
                                // Drawing rect if note is clicked on
                                for (int x = 0; x < SelectedBeats.Count; ++x)
                                    if (i == SelectedBeats[x])
                                        D3D.DrawEmptyRect(new Rectangle(pNote.X - 5, pNote.Y - 5, (listBeats[i].Width / 2) + 10, (listBeats[i].Height / 2) + 10), Color.Goldenrod);

                                
                                // Drawing Note Icon
                                TEXMAN.Draw(listBeats[i].TextureIndex,pNote.X,pNote.Y, scaleX, scaleY, Rectangle.Empty, 0, 0, 0,0);                                
                            }

                            if (listBeats[i].Completion == BEATIS.COMPLETE)
                                //Drawing Line from complete note to complete arrow
                                D3D.DrawLine(pArrow.X + 8, pArrow.Y + 16, pNote.X + 8, pNote.Y, Color.WhiteSmoke);                                                
                            


                        }
                    }
                    else
                        break;

                }
            }

        }

        private void DrawBeatMousePlacement()
        {
            if (MouseAddBeat.Completion != BEATIS.EMPTY)
            {
                // Mouse point
                Point mosPos;

                mosPos = TrackPanel.PointToClient(Cursor.Position);

                TEXMAN.Draw(MouseAddBeat.TextureIndex, mosPos.X, mosPos.Y, scaleX, scaleY, Rectangle.Empty, 0, 0, 0, Color.FromArgb(128, Color.White).ToArgb());
            }
        }

        private void DrawSelectRect()
        {

            if (bMouseSelect)
            {
                //*********************DRAWING MATH********************************//               
                // Pixel offset for numbers / tics original position
                // Effectively these are the seconds displayed
                int xoffset = 34;

                // Pixel offset for scrolling speed based on current song time                                  (1.258f works really well so far)
                //V-- tweak this for speed (divide = move faster, mult = slower, negative = move backwards) I'm so cool 
                float songoffset = (nCurrentPositionMS * 0.001f * nZoom) * (1000.0f / (float)(xoffset / 1.258f));

                // Cutting down on divide ops
                int Halfsies = TrackPanel.Bottom / 2;
                int XHalfsies = TrackPanel.Right / 2;

                // Getting scope to display beats
                float displayFrontArea = nCurrentPositionMS * 0.001f + 15;
                float displayBackArea = displayFrontArea - 30;

                D3D.DrawEmptyRect(MouseSelectRect, Color.Goldenrod);

                Console.WriteLine("Left: " + MouseSelectRect.Left.ToString());
                Console.WriteLine("Top: " + MouseSelectRect.Top.ToString());
                Console.WriteLine("Right: " + MouseSelectRect.Right.ToString());
                Console.WriteLine("Bottom: " + MouseSelectRect.Bottom.ToString());
            }
        }

        private void UpdateNoteInfo()
        {
            // Putting image into note picture box

            if (listBeats.Count > 0)
            {
                if (nMouseClickedIndex >= 0)
                {
                    switch (listBeats[nMouseClickedIndex].KeyPress)
                    {
                        case 'w':
                            BeatPictureBox.BackgroundImage = WKeyPictureBox.BackgroundImage;
                            break;

                        case 'a':
                            BeatPictureBox.BackgroundImage = AKeyPictureBox.BackgroundImage;
                            break;

                        case 's':
                            BeatPictureBox.BackgroundImage = SKeyPictureBox.BackgroundImage;
                            break;

                        case 'd':
                            BeatPictureBox.BackgroundImage = DKeyPictureBox.BackgroundImage;
                            break;

                        case 'x':
                            break;
                    }

                    if (listBeats[nMouseClickedIndex].Completion == BEATIS.ARROW || listBeats[nMouseClickedIndex].Completion == BEATIS.COMPLETE)
                    {
                        if (listBeats[nMouseClickedIndex].Direction == "left")
                            BeatPictureBox.BackgroundImage = LeftPictureBox.BackgroundImage;
                        else if (listBeats[nMouseClickedIndex].Direction == "right")
                            BeatPictureBox.BackgroundImage = RightPictureBox.BackgroundImage;
                        else if (listBeats[nMouseClickedIndex].Direction == "up")
                            BeatPictureBox.BackgroundImage = UpPictureBox.BackgroundImage;
                        else if (listBeats[nMouseClickedIndex].Direction == "down")
                            BeatPictureBox.BackgroundImage = DownPictureBox.BackgroundImage;
                        else if (listBeats[nMouseClickedIndex].Direction == "leftdown")
                            BeatPictureBox.BackgroundImage = DownLeftPictureBox.BackgroundImage;
                        else if (listBeats[nMouseClickedIndex].Direction == "rightdown")
                            BeatPictureBox.BackgroundImage = DownRightPictureBox.BackgroundImage;
                        else if (listBeats[nMouseClickedIndex].Direction == "leftup")
                            BeatPictureBox.BackgroundImage = UpLeftPictureBox.BackgroundImage;
                        else if (listBeats[nMouseClickedIndex].Direction == "rightup")
                            BeatPictureBox.BackgroundImage = UpRightPictureBox.BackgroundImage;
                    }

                    BeatDifficultyValueLabel.Text = listBeats[nMouseClickedIndex].Difficulty;
                    BeatKeyValueLabel.Text = listBeats[nMouseClickedIndex].KeyPress.ToString();
                    BeatDirectionValueLabel.Text = listBeats[nMouseClickedIndex].Direction;
                    BeatTimeValueUpDown.Value = listBeats[nMouseClickedIndex].TimeOfBeat;

                    BeatEventValueLabel.Text = listBeats[nMouseClickedIndex].Event;
                }
                else
                {
                    BeatPictureBox.BackgroundImage = null;
                    BeatDifficultyValueLabel.Text = "none";
                    BeatKeyValueLabel.Text = "none";
                    BeatDirectionValueLabel.Text = "none";
                    BeatTimeValueUpDown.Value = 0;
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

            MouseAddBeat = new Beat();
            szClickedEventEdit = "";

            nMouseScroll = 0;

            MouseSelectStartPoint = new Point();

            MouseSelectRect = Rectangle.Empty;

            
            SelectedBeats.Clear();
            CopiedBeats.Clear();

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

            bool isBeat = false;

            if(BothRadio.Checked || ArrowsRadio.Checked)
            switch (dir)
            {
                // Left Arrow
                case Keys.NumPad4:
                    {
                        LeftPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "left";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.ArrowTextureIndex = ArrowLeft;
                        isBeat = true;
                    }
                    break;

                // Right Arrow
                case Keys.NumPad6:
                    {
                        RightPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "right";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.ArrowTextureIndex = ArrowRight;
                        isBeat = true;

                    }
                    break;

                // Up Arrow
                case Keys.NumPad8:
                    {
                        UpPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "up";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.ArrowTextureIndex = ArrowUp;
                        isBeat = true;

                    }
                    break;

                // Down Arrow
                case Keys.NumPad2:
                    {
                        DownPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "down";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.ArrowTextureIndex = ArrowDown;
                        isBeat = true;

                    }
                    break;

                // Down Left Arrow
                case Keys.NumPad1:
                    {
                        DownLeftPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "leftdown";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.ArrowTextureIndex = ArrowDownLeft;
                        isBeat = true;

                    }
                    break;

                // Down Right Arrow
                case Keys.NumPad3:
                    {
                        DownRightPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "rightdown";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.ArrowTextureIndex = ArrowDownRight;
                        isBeat = true;

                    }
                    break;

                // Up Left Arrow
                case Keys.NumPad7:
                    {
                        UpLeftPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "leftup";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.ArrowTextureIndex = ArrowUpLeft;
                        isBeat = true;

                    }
                    break;

                // Up Right Arrow
                case Keys.NumPad9:
                    {
                        UpRightPictureBox.BackColor = Color.CornflowerBlue;
                        tempBeat.Direction = "rightup";
                        tempBeat.Completion = BEATIS.ARROW;
                        tempBeat.ArrowTextureIndex = ArrowUpRight;
                        isBeat = true;

                    }
                    break;                
                
                default:
                    break;
               
            }

            if(BothRadio.Checked || NotesRadio.Checked)
            switch (dir)
            {
                // W Key
                case Keys.W:
                    {
                        WKeyPictureBox.BackColor = Color.Crimson;
                        tempBeat.KeyPress = 'w';
                        tempBeat.Image = szWKeyImage;
                        tempBeat.Completion = BEATIS.KEY;
                        tempBeat.TextureIndex = KeyW;
                        isBeat = true;

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
                        isBeat = true;

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
                        isBeat = true;

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
                        isBeat = true;

                    }
                    break;               

                default:
                    break;
            }

            if (!isBeat || fmodChannel == null)
                return;

            // Only setting beat time if it's not assigned yet
            fmodChannel.getPosition(ref tempBeat.nTimeOfBeat, FMOD.TIMEUNIT.MS);
            tempBeat.Width = 32;
            tempBeat.Height = 32;

            tempBeat.Difficulty = szDifficulty;

            listBeats.Add(tempBeat);            

            bListChanged = true;
        }

        private int CheckMousePos()
        {

            // Mouse point
            Point mosPos;

            mosPos = TrackPanel.PointToClient(Cursor.Position);

            //*********************DRAWING MATH********************************//               
            // Pixel offset for numbers / tics original position
            // Effectively these are the seconds displayed
            int xoffset = 34;

            // Pixel offset for scrolling speed based on current song time                                  (1.258f works really well so far)
            //V-- tweak this for speed (divide = move faster, mult = slower, negative = move backwards) I'm so cool 
            float songoffset = (nCurrentPositionMS * 0.001f * nZoom) * (1000.0f / (float)(xoffset / 1.258f));

            // Cutting down on divide ops
            int Halfsies = TrackPanel.Bottom / 2;
            int XHalfsies = TrackPanel.Right / 2;

            // Getting scope to display beats
            float displayFrontArea = nCurrentPositionMS * 0.001f + 15;
            float displayBackArea = displayFrontArea - 30;

            int ArrowTop = Halfsies + 50;
            int ArrowBottom = ArrowTop + 32;

            int SymbolTop = Halfsies - 60;
            int SymbolBottom = SymbolTop + TEXMAN.GetTextureHeight(KeyW);

            // Counter
            int i = 0;

            // Checking if mouse is in the panel
            if (mosPos.Y < ArrowBottom && mosPos.Y > ArrowTop)
            {
                for (i = 0; i < listBeats.Count; ++i)
                {
                     // Time of beat in MS float style
                    float timeofbeat = listBeats[i].TimeOfBeat * 0.001f;

                    // Trying to cull notes so we don't display everything off screen
                    if (timeofbeat <= displayFrontArea)                   
                        if (timeofbeat >= displayBackArea)                        
                            if (listBeats[i].Completion == BEATIS.ARROW || listBeats[i].Completion == BEATIS.COMPLETE)
                            {
                                // Current note's offset based on the time of the beat.  Don't ask me where I come up with this shit.
                                float noteOffset = (listBeats[i].TimeOfBeat * 0.001f * nZoom) * (1000.0f / (float)(xoffset / 1.258f));
                                Rectangle derp = new Rectangle(XHalfsies + (int)(noteOffset - songoffset), Halfsies + 50, listBeats[i].Width / 2, listBeats[i].Height / 2);

                                if (derp.Contains(mosPos))
                                {                                    
                                    Console.WriteLine(nMouseSelectedIndex.ToString());
                                    pMouseSelectedCoords.X = derp.Left;
                                    pMouseSelectedCoords.Y = derp.Top;
                                    return i;
                                }
                            }  
                }               
            }
            else if (mosPos.Y < SymbolBottom && mosPos.Y > SymbolTop)
            {
                for (i = 0; i < listBeats.Count; ++i)
                {
                    // Time of beat in MS float style
                    float timeofbeat = listBeats[i].TimeOfBeat * 0.001f;

                    // Trying to cull notes so we don't display everything off screen
                    if (timeofbeat <= displayFrontArea)
                        if (timeofbeat >= displayBackArea)
                            if (listBeats[i].Completion == BEATIS.KEY || listBeats[i].Completion == BEATIS.COMPLETE)
                            {

                                // Current note's offset based on the time of the beat.  Don't ask me where I come up with this shit.
                                float noteOffset = (listBeats[i].TimeOfBeat * 0.001f * nZoom) * (1000.0f / (float)(xoffset / 1.258f));

                                Rectangle note = new Rectangle(XHalfsies + (int)(noteOffset - songoffset), Halfsies - 60, listBeats[i].Width / 2, listBeats[i].Height / 2);

                                if (note.Contains(mosPos))
                                {                                    
                                    Console.WriteLine(nMouseSelectedIndex.ToString());
                                    pMouseSelectedCoords.X = note.Left;
                                    pMouseSelectedCoords.Y = note.Top;
                                    return i;
                                }
                            }
                }
            }

            return -1;
        }

        private void ResetButtonBackgrounds()
        {
            WKeyPictureBox.BackColor = SystemColors.Control;
            AKeyPictureBox.BackColor = SystemColors.Control;
            SKeyPictureBox.BackColor = SystemColors.Control;
            DKeyPictureBox.BackColor = SystemColors.Control;

            UpLeftPictureBox.BackColor = SystemColors.Control;
            UpPictureBox.BackColor = SystemColors.Control;
            UpRightPictureBox.BackColor = SystemColors.Control;
            RightPictureBox.BackColor = SystemColors.Control;
            DownRightPictureBox.BackColor = SystemColors.Control;
            DownPictureBox.BackColor = SystemColors.Control;
            DownLeftPictureBox.BackColor = SystemColors.Control;
            LeftPictureBox.BackColor = SystemColors.Control;
        }

        private void PlaceBeatAtMousePosition()
        {
            if (MouseAddBeat.Completion != BEATIS.EMPTY)
            {
                //*********************DRAWING MATH********************************//               
               
                // Cutting down on divide ops
                int Halfsies = TrackPanel.Bottom / 2;
                int XHalfsies = TrackPanel.Right / 2;

                // Mouse point
                Point mosPos;
                mosPos = TrackPanel.PointToClient(Cursor.Position);

               
                if (mosPos.X > XHalfsies)
                {
                    // More coordinate magic to get the song position based off mouse X position
                    MouseAddBeat.TimeOfBeat = nCurrentPositionMS + (uint)((mosPos.X * 1000 / 34 - (15000 + 118 * (mosPos.X / 34 - 15))));
                }
                else
                {
                    // More coordinate magic to get the song position based off mouse X position
                    int nTime = (int)(nCurrentPositionMS + (mosPos.X * 1000 / 34 - (15000 - 57 * (15 - mosPos.X / 34))));

                    if (nTime > 0)
                        MouseAddBeat.TimeOfBeat = (uint)nTime;                        
                }

                MouseAddBeat.Difficulty = szDifficulty;

                // Adding to list
                listBeats.Add(MouseAddBeat);

                bListChanged = true;

                Beat tBeat = MouseAddBeat;
                MouseAddBeat = new Beat(tBeat);               
            }
        }

        private void LinkBeatNodes()
        {
            bool success = false;

            for (int i = 0; i < SelectedBeats.Count; ++i)
            {
                if (nMouseSelectedIndex != SelectedBeats[i] && nMouseSelectedIndex >= 0)
                {
                    if (listBeats[nMouseSelectedIndex].Completion == BEATIS.ARROW || listBeats[nMouseSelectedIndex].Completion == BEATIS.COMPLETE)
                    {
                        // Can't link with other arrows
                        if (listBeats[SelectedBeats[i]].Completion != BEATIS.ARROW)
                        {
                            listBeats[SelectedBeats[i]].Direction = listBeats[nMouseSelectedIndex].Direction;                           
                            listBeats[SelectedBeats[i]].ArrowTextureIndex = listBeats[nMouseSelectedIndex].ArrowTextureIndex;
                            listBeats[SelectedBeats[i]].Completion = BEATIS.COMPLETE;

                            success = true;
                        }
                    }
                    else if (listBeats[nMouseSelectedIndex].Completion == BEATIS.KEY || listBeats[nMouseSelectedIndex].Completion == BEATIS.COMPLETE)
                    {
                        // Can't link with other keys
                        if (listBeats[SelectedBeats[i]].Completion != BEATIS.KEY)
                        {
                            listBeats[SelectedBeats[i]].KeyPress = listBeats[nMouseSelectedIndex].KeyPress;                         
                            listBeats[SelectedBeats[i]].TextureIndex = listBeats[nMouseSelectedIndex].TextureIndex;
                            listBeats[SelectedBeats[i]].Completion = BEATIS.COMPLETE;
                            success = true;
                        }
                    }                

                }
            }

            if (success)
            {

                // Getting rid of old arrow since complete notes now hold all relevant info
                // Only deletes if the original wasn't a complete note
                if(listBeats[nMouseSelectedIndex].Completion != BEATIS.COMPLETE)
                    listBeats.RemoveAt(nMouseSelectedIndex);


                // Killing mouse selection indicies because they might be invalid now
                nMouseClickedIndex = -1;
                nMouseSelectedIndex = -1;


                bListChanged = true;

                if (fmodChannel != null)
                    fmodChannel.setPaused(true);
            }
        }

        //****************MISC EVENTS************************//
        private void BeatMaker_KeyDown(object sender, KeyEventArgs e)
        {
            AddBeat(e.KeyCode);

            if (e.KeyCode == Keys.ShiftKey)
                bMouseSelect = true;

            if (e.KeyCode == Keys.Delete)
                DeleteSelectionButton_Click(null, null);
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

            case Keys.ControlKey:
                {
                    LinkBeatNodes();
                }
                break;

            case Keys.ShiftKey:
                bMouseSelect = false;
                MouseSelectRect = Rectangle.Empty;
                break;

            }
        }

        void setTitleWindow_FormClosed(object sender, FormClosedEventArgs e)
        {
            szSongName = setTitleWindow.SongName;
            setTitleWindow = null;
        }

        void setEventWindow_FormClosed(object sender, FormClosedEventArgs e)
        {
            listBeats[nMouseClickedIndex].Event = setEventWindow.EventName;
            setEventWindow = null;
        }

        private void BeatMaker_MouseWheel(object sender, MouseEventArgs e)
        {
            // Adding +/- 10 to zoom (can't go below 1)

            if (e.Delta > 0)
            {
                switch (nZoom)
                {
                    case 1:
                        nZoom = 2;
                        break;

                    case 2:
                        nZoom = 7;
                        break;

                    case 7:
                        nZoom = 14;
                        break;

                    default:
                        break;
                }
            }
            else if (e.Delta < 0)
            {

                switch (nZoom)
                {
                    case 2:
                        nZoom = 1;
                        break;

                    case 7:
                        nZoom = 2;
                        break;

                    case 14:
                        nZoom = 7;
                        break;

                    default:
                        break;
                }
            }
        }

        private void TrackPanel_MouseEnter(object sender, EventArgs e)
        {
            bMouseInTrack = true;            
        }

        private void TrackPanel_MouseLeave(object sender, EventArgs e)
        {
            bMouseInTrack = false;
        }

        private void TrackPanel_MouseDown(object sender, MouseEventArgs e)
        {
            SelectedBeats.Clear();

            if(nMouseSelectedIndex >= 0)
                SelectedBeats.Add(nMouseSelectedIndex);

            nMouseClickedIndex = nMouseSelectedIndex;

            if (nMouseClickedIndex == -1 && e.Button == MouseButtons.Left)
            {
                PlaceBeatAtMousePosition();
            }

            if (bMouseSelect)
            {
                MouseSelectStartPoint = new Point();
                MouseSelectStartPoint.X = e.X;
                MouseSelectStartPoint.Y = e.Y;
            }
        }

        private void TrackPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (bMouseSelect)
            {               

                //*********************DRAWING MATH********************************//               
                // Pixel offset for numbers / tics original position
                // Effectively these are the seconds displayed
                int xoffset = 34;

                // Pixel offset for scrolling speed based on current song time                                  (1.258f works really well so far)
                //V-- tweak this for speed (divide = move faster, mult = slower, negative = move backwards) I'm so cool 
                float songoffset = (nCurrentPositionMS * 0.001f * nZoom) * (1000.0f / (float)(xoffset / 1.258f));

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
                            // Current note's offset based on the time of the beat.  Don't ask me where I come up with this shit.
                            float noteOffset = (listBeats[i].TimeOfBeat * 0.001f * nZoom) * (1000.0f / (float)(xoffset / 1.258f));

                            if (listBeats[i].Completion == BEATIS.ARROW || listBeats[i].Completion == BEATIS.COMPLETE)
                            {
                               
                                // Points for line drawing on complete notes
                                Point pArrow = new Point();

                                pArrow.X = XHalfsies + (int)(noteOffset - songoffset);
                                pArrow.Y = Halfsies + 50;

                                Rectangle noteRect = new Rectangle(pArrow.X - 5, pArrow.Y - 5, (listBeats[i].Width / 2) + 10, (listBeats[i].Height / 2) + 10);

                                if (MouseSelectRect.Contains(noteRect) && SelectedBeats.Count > 0 && (listBeats[SelectedBeats[0]].Completion != BEATIS.KEY || listBeats[SelectedBeats[0]].Completion == BEATIS.COMPLETE))
                                    SelectedBeats.Add(i);
                                else if (SelectedBeats.Count == 0 && MouseSelectRect.Contains(noteRect))
                                    SelectedBeats.Add(i);
                            }
                            else if (listBeats[i].Completion == BEATIS.KEY || listBeats[i].Completion == BEATIS.COMPLETE)
                            {
                                Point pNote = new Point();

                                pNote.X = XHalfsies + (int)(noteOffset - songoffset);
                                pNote.Y = Halfsies - 60;

                                Rectangle keyRect = new Rectangle(pNote.X - 5, pNote.Y - 5, (listBeats[i].Width / 2) + 10, (listBeats[i].Height / 2) + 10);

                                if (MouseSelectRect.Contains(keyRect) && SelectedBeats.Count > 0 && (listBeats[SelectedBeats[0]].Completion != BEATIS.ARROW || listBeats[SelectedBeats[0]].Completion == BEATIS.COMPLETE))
                                    SelectedBeats.Add(i);
                                else if (SelectedBeats.Count == 0 && MouseSelectRect.Contains(keyRect))
                                    SelectedBeats.Add(i);

                            }
                            
                        }
                    }
                }
                               
            }
        }

        private void TrackPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (bMouseSelect && e.Button == MouseButtons.Left)
            {
                Rectangle tRect = new Rectangle(MouseSelectStartPoint.X, MouseSelectStartPoint.Y, e.X - MouseSelectStartPoint.X, e.Y - MouseSelectStartPoint.Y);
                MouseSelectRect = tRect;
            }
            else if (nMouseClickedIndex >= 0 && e.Button == MouseButtons.Right)
            {
                //*********************DRAWING MATH********************************//               
                         
                // Cutting down on divide ops
                int Halfsies = TrackPanel.Bottom / 2;
                int XHalfsies = TrackPanel.Right / 2;

                // Mouse point
                Point mosPos;
                mosPos = TrackPanel.PointToClient(Cursor.Position);                
               

                if (mosPos.X > XHalfsies)
                {
                    // More coordinate magic to get the song position based off mouse X position
                    listBeats[nMouseClickedIndex].TimeOfBeat = nCurrentPositionMS + (uint)((mosPos.X * 1000 / 34 - (15000 + 118 * (mosPos.X / 34 - 15))));
                }
                else
                {
                    // More coordinate magic to get the song position based off mouse X position
                    int nTime = (int)(nCurrentPositionMS + (mosPos.X * 1000 / 34 - (15000 - 57 * (15 - mosPos.X / 34))));

                    if (nTime > 0)
                        listBeats[nMouseClickedIndex].TimeOfBeat = (uint)nTime;
                }
            }            
            else if (e.Button == MouseButtons.Left && !bMouseSelect)
            {
                if (fmodChannel != null)
                {
                    int nScrollDelta = ((nMouseScroll * 1000 / 34) - (Cursor.Position.X * 1000 / 34));
                    uint nPos = 0;

                    fmodChannel.getPosition(ref nPos, FMOD.TIMEUNIT.MS);

                    if (nPos + nScrollDelta > 0)
                        fmodChannel.setPosition(nPos + (uint)nScrollDelta, FMOD.TIMEUNIT.MS);
                }
            }

        }

        private void BeatTimeValueUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (nMouseClickedIndex >= 0)
                listBeats[nMouseClickedIndex].TimeOfBeat = (uint)BeatTimeValueUpDown.Value;
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

        private void NoteCopyButton_Click(object sender, EventArgs e)
        {
            // Copying whatever beats are selected
            CopiedBeats = new List<int>(SelectedBeats);
        }

        private void NotePasteButton_Click(object sender, EventArgs e)
        {
            if (CopiedBeats.Count > 0)
            {
                // Previous beat to get time splice from
                Beat lastBeat = new Beat(listBeats[CopiedBeats[0]]);



                // Adding first beat to list
                lastBeat.TimeOfBeat = nCurrentPositionMS;

                listBeats.Add(lastBeat);

                for (int i = 1; i < CopiedBeats.Count; ++i)
                {
                   // Getting spread of last beat from original list and this beat, and adding current time to it
                   Beat nextBeat = new Beat(listBeats[CopiedBeats[i]]);

                   nextBeat.TimeOfBeat = (listBeats[CopiedBeats[i]].TimeOfBeat - listBeats[CopiedBeats[0]].TimeOfBeat) + nCurrentPositionMS;

                   // Adding new beat
                   listBeats.Add(nextBeat);
                }

                // Setting it so it gets sorted
                bListChanged = true;
            }
        }

        private void PlaySpeed1xButton_Click(object sender, EventArgs e)
        {
            NormalSpeedMenuItem_Click(null, null);
        }

        private void PlaySpeedHalfButton_Click(object sender, EventArgs e)
        {
            HalfSpeedMenuItem_Click(null, null);
        }

        private void PlaySpeedThirdButton_Click(object sender, EventArgs e)
        {
            ThirdSpeedMenuItem_Click(null, null);
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

                // Adding icon file names
                XElement xIconFileNames = new XElement("icons");

                XAttribute wKeyFile = new XAttribute("wkey",szWKeyImage);
                xIconFileNames.Add(wKeyFile);

                XAttribute aKeyFile = new XAttribute("akey", szAKeyImage);
                xIconFileNames.Add(aKeyFile);

                XAttribute sKeyFile = new XAttribute("skey", szSKeyImage);
                xIconFileNames.Add(sKeyFile);

                XAttribute dKeyFile = new XAttribute("dkey", szDKeyImage);
                xIconFileNames.Add(dKeyFile);

                xRoot.Add(xIconFileNames);

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
                    
                    XAttribute xDifficulty = new XAttribute("difficulty", listBeats[i].Difficulty);
                    xBeat.Add(xDifficulty);

                    XAttribute xWidth = new XAttribute("width", listBeats[i].Width);
                    xBeat.Add(xWidth);

                    XAttribute xHeight = new XAttribute("height", listBeats[i].Height);
                    xBeat.Add(xHeight);

                    XAttribute xBeatIs = new XAttribute("beatis", (int)listBeats[i].Completion);
                    xBeat.Add(xBeatIs);

                    XAttribute xEvent = new XAttribute("event", listBeats[i].Event);
                    xBeat.Add(xEvent);

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

                // Loading Icon Images
                XElement xIcons = root.Element("icons");

                XAttribute wIcon = xIcons.Attribute("wkey");
                szWKeyImage = wIcon.Value;

                XAttribute aIcon = xIcons.Attribute("akey");
                szAKeyImage = aIcon.Value;

                XAttribute sIcon = xIcons.Attribute("skey");
                szSKeyImage = sIcon.Value;

                XAttribute dIcon = xIcons.Attribute("dkey");
                szDKeyImage = dIcon.Value;

                //*******************LOADING ICONS***************************************//

                //**************W KEY****************************************//
                if (!System.IO.File.Exists(Properties.Settings.Default.IconFilePath + szWKeyImage))
                {
                    // the file was not found so we must reaquire the associated file
                    if (System.Windows.Forms.MessageBox.Show("\"" + szWKeyImage + "\" can not be found in \"" + Properties.Settings.Default.IconFilePath + "\". Please relocate " + szWKeyImage) == DialogResult.OK)
                    {
                        loadWImageToolStripMenuItem_Click(null, null);
                    }
                }
                else
                {
                    KeyW = TEXMAN.LoadTexture(Properties.Settings.Default.IconFilePath + szWKeyImage, 0);

                    WKeyPictureBox.BackgroundImage = new Bitmap(Properties.Settings.Default.IconFilePath + szWKeyImage);

                    if (TEXMAN.GetTextureWidth(KeyW) >= 20)
                        scaleX = 0.5f;
                    if (TEXMAN.GetTextureHeight(KeyW) >= 20)
                        scaleY = 0.5f;

                    if (TEXMAN.GetTextureWidth(KeyW) >= 50)
                        scaleX = 0.25f;
                    if (TEXMAN.GetTextureHeight(KeyW) >= 50)
                        scaleY = 0.25f;
                }


                //***************A KEY*****************************************//

                if (!System.IO.File.Exists(Properties.Settings.Default.IconFilePath + szAKeyImage))
                {
                    // the file was not found so we must reaquire the associated file
                    if (System.Windows.Forms.MessageBox.Show("\"" + szAKeyImage + "\" can not be found in \"" + Properties.Settings.Default.IconFilePath + "\". Please relocate " + szAKeyImage) == DialogResult.OK)
                    {
                        loadAImageToolStripMenuItem_Click(null, null);
                    }
                }
                else
                {
                    KeyA = TEXMAN.LoadTexture(Properties.Settings.Default.IconFilePath + szAKeyImage, 0);

                    AKeyPictureBox.BackgroundImage = new Bitmap(Properties.Settings.Default.IconFilePath + szAKeyImage);

                }


                //****************S KEY******************************************//
                if (!System.IO.File.Exists(Properties.Settings.Default.IconFilePath + szSKeyImage))
                {
                    // the file was not found so we must reaquire the associated file
                    if (System.Windows.Forms.MessageBox.Show("\"" + szSKeyImage + "\" can not be found in \"" + Properties.Settings.Default.IconFilePath + "\". Please relocate " + szSKeyImage) == DialogResult.OK)
                    {
                        loadSImageToolStripMenuItem_Click(null, null);
                    }
                }
                else
                {
                    KeyS = TEXMAN.LoadTexture(Properties.Settings.Default.IconFilePath + szSKeyImage, 0);

                    SKeyPictureBox.BackgroundImage = new Bitmap(Properties.Settings.Default.IconFilePath + szSKeyImage);

                }

                //*****************D KEY*******************************************//
                if (!System.IO.File.Exists(Properties.Settings.Default.IconFilePath + szDKeyImage))
                {
                    // the file was not found so we must reaquire the associated file
                    if (System.Windows.Forms.MessageBox.Show("\"" + szDKeyImage + "\" can not be found in \"" + Properties.Settings.Default.IconFilePath + "\". Please relocate " + szDKeyImage) == DialogResult.OK)
                    {
                        loadDImageToolStripMenuItem_Click(null, null);
                    }
                }
                else
                {
                    KeyD = TEXMAN.LoadTexture(Properties.Settings.Default.IconFilePath + szDKeyImage, 0);

                    DKeyPictureBox.BackgroundImage = new Bitmap(Properties.Settings.Default.IconFilePath + szDKeyImage);

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

                        XAttribute xDifficulty = xBeat.Attribute("difficulty");
                        aBeat.Difficulty = xDifficulty.Value;

                        XAttribute xWidth = xBeat.Attribute("width");
                        aBeat.Width = Convert.ToInt32(xWidth.Value);

                        XAttribute xHeight = xBeat.Attribute("height");
                        aBeat.Height = Convert.ToInt32(xHeight.Value);

                        XAttribute xBeatIs = xBeat.Attribute("beatis");
                        aBeat.Completion = (BEATIS)Convert.ToInt32(xBeatIs.Value);

                        XAttribute xEvent = xBeat.Attribute("event");
                        aBeat.Event = xEvent.Value;

                        if (BEATIS.ARROW == aBeat.Completion || BEATIS.COMPLETE == aBeat.Completion)
                        {
                            if (aBeat.Direction == "left")
                                aBeat.ArrowTextureIndex = ArrowLeft;
                            else if (aBeat.Direction == "right")
                                aBeat.ArrowTextureIndex = ArrowRight;
                            else if (aBeat.Direction == "up")
                                aBeat.ArrowTextureIndex = ArrowUp;
                            else if (aBeat.Direction == "down")
                                aBeat.ArrowTextureIndex = ArrowDown;
                            else if (aBeat.Direction == "leftdown")
                                aBeat.ArrowTextureIndex = ArrowDownLeft;
                            else if (aBeat.Direction == "rightdown")
                                aBeat.ArrowTextureIndex = ArrowDownRight;
                            else if (aBeat.Direction == "leftup")
                                aBeat.ArrowTextureIndex = ArrowUpLeft;
                            else if (aBeat.Direction == "rightup")
                                aBeat.ArrowTextureIndex = ArrowUpRight;
                        }

                        if (BEATIS.KEY == aBeat.Completion || BEATIS.COMPLETE == aBeat.Completion)
                        {
                            switch (aBeat.KeyPress)
                            {
                                case 'w':
                                    aBeat.TextureIndex = KeyW;
                                    break;

                                case 'a':
                                    aBeat.TextureIndex = KeyA;
                                    break;

                                case 's':
                                    aBeat.TextureIndex = KeyS;
                                    break;

                                case 'd':
                                    aBeat.TextureIndex = KeyD;
                                    break;
                            }
                        }

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

                for (int i = 0; i < listBeats.Count; ++i)
                    if (listBeats[i].KeyPress == 'w')
                        listBeats[i].TextureIndex = KeyW;


                if (TEXMAN.GetTextureWidth(KeyW) >= 20)
                    scaleX = 0.5f;
                if (TEXMAN.GetTextureHeight(KeyW) >= 20)
                    scaleY = 0.5f;

                if (TEXMAN.GetTextureWidth(KeyW) >= 50)
                    scaleX = 0.25f;
                if (TEXMAN.GetTextureHeight(KeyW) >= 50)
                    scaleY = 0.25f;
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

                for (int i = 0; i < listBeats.Count; ++i)
                    if (listBeats[i].KeyPress == 'a')
                        listBeats[i].TextureIndex = KeyA;

                if (TEXMAN.GetTextureWidth(KeyW) >= 20)
                    scaleX = 0.5f;
                if (TEXMAN.GetTextureHeight(KeyW) >= 20)
                    scaleY = 0.5f;

                if (TEXMAN.GetTextureWidth(KeyW) >= 50)
                    scaleX = 0.25f;
                if (TEXMAN.GetTextureHeight(KeyW) >= 50)
                    scaleY = 0.25f;
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

                for (int i = 0; i < listBeats.Count; ++i)
                    if (listBeats[i].KeyPress == 's')
                        listBeats[i].TextureIndex = KeyS;

                if (TEXMAN.GetTextureWidth(KeyW) >= 20)
                    scaleX = 0.5f;
                if (TEXMAN.GetTextureHeight(KeyW) >= 20)
                    scaleY = 0.5f;

                if (TEXMAN.GetTextureWidth(KeyW) >= 50)
                    scaleX = 0.25f;
                if (TEXMAN.GetTextureHeight(KeyW) >= 50)
                    scaleY = 0.25f;
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

                for (int i = 0; i < listBeats.Count; ++i)
                    if(listBeats[i].KeyPress == 'd')
                        listBeats[i].TextureIndex = KeyD;

                if (TEXMAN.GetTextureWidth(KeyW) >= 20)
                    scaleX = 0.5f;
                if (TEXMAN.GetTextureHeight(KeyW) >= 20)
                    scaleY = 0.5f;

                if (TEXMAN.GetTextureWidth(KeyW) >= 50)
                    scaleX = 0.25f;
                if (TEXMAN.GetTextureHeight(KeyW) >= 50)
                    scaleY = 0.25f;
            }
        }

        private void ThirdSpeedMenuItem_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                playSpeed = 0.3f;
                float freq = 0.0f;

                // Setting frequency back to normal first
                fmodChannel.setFrequency(originalFreq);

                fmodChannel.getFrequency(ref freq);
                fmodChannel.setFrequency(playSpeed * freq);
            }
        }

        private void HalfSpeedMenuItem_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                playSpeed = 0.5f;
                float freq = 0.0f;

                // Setting frequency back to normal first
                fmodChannel.setFrequency(originalFreq);

                fmodChannel.getFrequency(ref freq);

                fmodChannel.setFrequency(playSpeed * freq);
            }
        }

        private void NormalSpeedMenuItem_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                playSpeed = 1.0f;
                fmodChannel.setFrequency(originalFreq);
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

            PlayButton_Click(null, null);
            PauseButton_Click(null, null);

            fmodChannel.getFrequency(ref originalFreq);
        }

        private void GetWaveform()
        {

        }

        //*****************ICON SELECTION EVENTS***************//

        private void WKeyPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                WKeyPictureBox.BackColor = Color.Crimson;
                MouseAddBeat.KeyPress = 'w';
                MouseAddBeat.Image = szWKeyImage;
                MouseAddBeat.Completion = BEATIS.KEY;
                MouseAddBeat.TextureIndex = KeyW;               
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
            }

        }

        private void AKeyPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                AKeyPictureBox.BackColor = Color.Crimson;
                MouseAddBeat.KeyPress = 'a';
                MouseAddBeat.Image = szAKeyImage;
                MouseAddBeat.Completion = BEATIS.KEY;
                MouseAddBeat.TextureIndex = KeyA;               
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
                
            }
        }

        private void SKeyPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                SKeyPictureBox.BackColor = Color.Crimson;
                MouseAddBeat.KeyPress = 's';
                MouseAddBeat.Image = szSKeyImage;
                MouseAddBeat.Completion = BEATIS.KEY;
                MouseAddBeat.TextureIndex = KeyS;
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
            }
        }

        private void DKeyPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                DKeyPictureBox.BackColor = Color.Crimson;
                MouseAddBeat.KeyPress = 'd';
                MouseAddBeat.Image = szDKeyImage;
                MouseAddBeat.Completion = BEATIS.KEY;
                MouseAddBeat.TextureIndex = KeyD;
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
            }
        }

        private void UpLeftPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                UpLeftPictureBox.BackColor = Color.CornflowerBlue;
                MouseAddBeat.Direction = "leftup";
                MouseAddBeat.Completion = BEATIS.ARROW;
                MouseAddBeat.ArrowTextureIndex = ArrowUpLeft;
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
            }
        }

        private void LeftPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                LeftPictureBox.BackColor = Color.CornflowerBlue;
                MouseAddBeat.Direction = "left";
                MouseAddBeat.Completion = BEATIS.ARROW;
                MouseAddBeat.ArrowTextureIndex = ArrowLeft;
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
            }
        }

        private void DownLeftPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                DownLeftPictureBox.BackColor = Color.CornflowerBlue;
                MouseAddBeat.Direction = "leftdown";
                MouseAddBeat.Completion = BEATIS.ARROW;
                MouseAddBeat.ArrowTextureIndex = ArrowDownLeft;
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
            }
        }

        private void DownPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                DownPictureBox.BackColor = Color.CornflowerBlue;
                MouseAddBeat.Direction = "down";
                MouseAddBeat.Completion = BEATIS.ARROW;
                MouseAddBeat.ArrowTextureIndex = ArrowDown;
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
            }
        }

        private void UpPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                UpPictureBox.BackColor = Color.CornflowerBlue;
                MouseAddBeat.Direction = "up";
                MouseAddBeat.Completion = BEATIS.ARROW;
                MouseAddBeat.ArrowTextureIndex = ArrowUp;
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
            }
        }

        private void UpRightPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                UpRightPictureBox.BackColor = Color.CornflowerBlue;
                MouseAddBeat.Direction = "rightup";
                MouseAddBeat.Completion = BEATIS.ARROW;
                MouseAddBeat.ArrowTextureIndex = ArrowUpRight;
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
            }
        }

        private void RightPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                RightPictureBox.BackColor = Color.CornflowerBlue;
                MouseAddBeat.Direction = "right";
                MouseAddBeat.Completion = BEATIS.ARROW;
                MouseAddBeat.ArrowTextureIndex = ArrowRight;
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
            }
        }

        private void DownRightPictureBox_Click(object sender, EventArgs e)
        {
            if (fmodChannel != null)
            {
                ResetButtonBackgrounds();

                DownRightPictureBox.BackColor = Color.CornflowerBlue;
                MouseAddBeat.Direction = "rightdown";
                MouseAddBeat.Completion = BEATIS.ARROW;
                MouseAddBeat.ArrowTextureIndex = ArrowDownRight;
                MouseAddBeat.Width = 32;
                MouseAddBeat.Height = 32;
            }
        }

        private void ClearSelectionButton_Click(object sender, EventArgs e)
        {
            ResetButtonBackgrounds();
            SelectedBeats.Clear();
            MouseAddBeat = new Beat();
        }

        private void DeleteSelectionButton_Click(object sender, EventArgs e)
        {
            // Only removing a single note
            if (SelectedBeats.Count > 0)
            {
                List<Beat> toRemove = new List<Beat>();

                // Getting beats to remove (since removing at an index will invalidate selected beats)
                for (int i = 0; i < SelectedBeats.Count; ++i)
                {
                    toRemove.Add( listBeats[SelectedBeats[i]]);                    
                }

                // Now Removing specific items
                for (int i = 0; i < toRemove.Count; ++i)
                {
                    listBeats.Remove(toRemove[i]);
                }
            }

            nMouseClickedIndex = -1;

            SelectedBeats.Clear();

            bListChanged = true;
        }

        private void EditEventButton_Click(object sender, EventArgs e)
        {
            if (setEventWindow == null)
            {
                setEventWindow = new SetEvent();
                setEventWindow.FormClosed += new FormClosedEventHandler(setEventWindow_FormClosed);
                setEventWindow.EventName = listBeats[nMouseClickedIndex].Event;
                setEventWindow.ShowDialog();
            }
        }

        private void EasyButton_Click(object sender, EventArgs e)
        {
            szDifficulty = "easy";

            // Difficulty label value
            DifficultyValueLabel.Text = szDifficulty;

            for (int i = 0; i < SelectedBeats.Count; ++i)
                listBeats[SelectedBeats[i]].Difficulty = szDifficulty;
        }

        private void NormalButton_Click(object sender, EventArgs e)
        {
            szDifficulty = "normal";

            // Difficulty label value
            DifficultyValueLabel.Text = szDifficulty;

            for (int i = 0; i < SelectedBeats.Count; ++i)
                listBeats[SelectedBeats[i]].Difficulty = szDifficulty;
        }

        private void HardButton_Click(object sender, EventArgs e)
        {
            szDifficulty = "hard";

            // Difficulty label value
            DifficultyValueLabel.Text = szDifficulty;

            for (int i = 0; i < SelectedBeats.Count; ++i)
                listBeats[SelectedBeats[i]].Difficulty = szDifficulty;
        }     
        
    }
}
