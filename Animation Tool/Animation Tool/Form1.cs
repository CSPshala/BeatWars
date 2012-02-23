using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

// Adding DirectX
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

//XML
using System.Xml.Linq;

namespace Animation_Tool
{
    public partial class Form1 : Form
    {

        SGP.ManagedTextureManager TextureManager;
        SGP.ManagedDirect3D Direct3D;
        string texture;
        int nID = -1;
        int nAnchorID = -1;
        bool bFrame = true;
        bool bCollision = false;
        bool bAnchorPoint = false;
        public bool InEditor = true;
        int currFrame = 0;
        int MaxFrame = 0;


        List<Animations> lAnimations = new List<Animations>();
        
        int dX = -1;
        int dY = -1;

        int cX = -1;
        int cY = -1;

        int anchorX = -1;
        int anchorY = -1;

        int cWidth = -1;
        int cHeight = -1;

        int dWidth = -1;
        int dHeight = -1;

        float fDuration = 250;

        bool bPlay = false;
        bool bReset = false;

        float TimeWaited = 0;

        Timer timer = new Timer();

        public Form1()
        {
            InitializeComponent();
            this.DoubleBuffered = true;
            Direct3D = SGP.ManagedDirect3D.Instance;
            Direct3D.InitManagedDirect3D(panel1);
            Direct3D.InitManagedDirect3D(panel2);

            TextureManager = SGP.ManagedTextureManager.Instance;
            TextureManager.InitManagedTextureManager(Direct3D.Device, Direct3D.Sprite);

            timer.InitTimer();

            nAnchorID = TextureManager.LoadTexture("Resources/AnchorPoint.png", 0);

        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            bFrame = true;
            bCollision = false;
            bAnchorPoint = false;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            bFrame = false;
            bCollision = true;
            bAnchorPoint = false;
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            bFrame = false;
            bCollision = false;
            bAnchorPoint = true;
        }

        private void openImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                nID = TextureManager.LoadTexture( dlg.FileName.ToString(),0 );

                Size nSize = new Size(TextureManager.GetTextureWidth(nID),TextureManager.GetTextureHeight(nID));

                texture = dlg.FileName;
                panel1.Size = nSize;
            }
        }
        
        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {
            if (bFrame)
            {
                if (dX > -1)
                    dX = (int)numericUpDown2.Value; 

                if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                {
                    lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawX = dX;
                }

            }
            else if (bCollision)
            {
                if (cX > -1)
                    cX = (int)numericUpDown2.Value;

                if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                {
                    lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].CollisionX = cX;
                }
            }
            
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            if (bFrame)
            {
                if( dY > -1)
                dY = (int)numericUpDown1.Value;

                if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                {
                    lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawY = dY;
                }
            }
            else if (bCollision)
            {
                if( cY > -1)
                cY = (int)numericUpDown1.Value;

                if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                {
                    lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].CollisionY = cY;
                }
            }
        }

        private void numericUpDown3_ValueChanged(object sender, EventArgs e)
        {
            if (bFrame)
            {
                if( dWidth > -1 )
                dWidth = (int)numericUpDown3.Value;

                if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                {
                    lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawWidth = dWidth;
                }
            }
            else if (bCollision)
            {
                if( cWidth > -1 )
                cWidth = (int)numericUpDown3.Value;

                if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                {
                    lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].CollisionWidth = cWidth;
                }
            }
        }

        private void numericUpDown4_ValueChanged(object sender, EventArgs e)
        {
            if( bFrame )
            {
                if( dHeight > -1)
                dHeight = (int)numericUpDown4.Value;

                if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                {
                    lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawHeight = dHeight;
                }
            }
            else if (bCollision)
            {
                if( cHeight > -1)
                cHeight = (int)numericUpDown4.Value;

                if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                {
                    lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].CollisionHeight = cHeight;
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Animations anim = new Animations();

            lAnimations.Add(anim);

            listBox1.Items.Clear();
            listBox1.SelectedIndex = listBox1.Items.Count - 1;

            int num = 0;

            foreach (Animations curranim in lAnimations)
            {
                curranim.Ordernumber = num;
                listBox1.Items.Add(curranim);
                num += 1;
            }

            listBox2.ClearSelected();
            listBox2.Items.Clear();
            listBox1.Invalidate();
            listBox2.Invalidate();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if(listBox1.SelectedIndex > -1)
            {
                if (textBox2.Text == "")
                {
                    if (MessageBox.Show("The Animation has no name. Please name the Animation.") == DialogResult.OK)
                    {

                    }
                }
                else
                {
                    Frame newFrame = new Frame();

                    if (lAnimations[listBox1.SelectedIndex].lFrames == null)
                        lAnimations[listBox1.SelectedIndex].lFrames = new List<Frame>();


                    lAnimations[listBox1.SelectedIndex].lFrames.Add(newFrame);


                    listBox2.Items.Clear();

                    int num = 0;

                    foreach (Frame f in lAnimations[listBox1.SelectedIndex].lFrames)
                    {
                        f.FrameNumber = num;
                        f.Duration = fDuration;
                        listBox2.Items.Add(f);
                        num += 1;
                    }

                    listBox2.SelectedIndex = listBox2.Items.Count - 1;
                    listBox2.Invalidate();
                }
            }

        }

        public void Render()
        {


            if (Direct3D.ChangeDisplayParam(panel1))
            {
                Direct3D.Clear(Color.CornflowerBlue);
                Direct3D.DeviceBegin();
                Direct3D.SpriteBegin();

                Point offset = new Point(0, 0);

                offset.X += panel1.AutoScrollPosition.X;
                offset.Y += panel1.AutoScrollPosition.Y;

                if (nID != -1)
                    TextureManager.Draw(nID, offset.X, offset.Y, 1.0f, 1.0f, Rectangle.Empty, 0, 0, 0, Color.FromArgb(255, 255, 255, 255).ToArgb());

                Direct3D.Sprite.Flush();

                if (dX > -1 && dY > -1 && dWidth > -1 && dHeight > -1)
                {
                    Direct3D.DrawLine(dX + offset.X, dY + offset.Y, (dX + offset.X) + dWidth, dY + offset.Y, Color.Black, 0.5f);
                    Direct3D.DrawLine(dX + offset.X, dY + offset.Y, dX + offset.X, (dY + offset.Y) + dHeight, Color.Black, 0.5f);
                    Direct3D.DrawLine(dX + offset.X, (dY + offset.Y) + dHeight, (dX + offset.X) + dWidth, (dY + offset.Y) + dHeight, Color.Black, 0.5f);
                    Direct3D.DrawLine(dX + offset.X + dWidth, dY + offset.Y, (dX + offset.X) + dWidth, (dY + offset.Y) + dHeight, Color.Black, 0.5f);
                }

                if (cX > -1 && cY > -1 && cWidth > -1 && cHeight > -1)
                {
                    Direct3D.DrawLine(cX + offset.X, cY + offset.Y, (cX + offset.X) + cWidth, cY + offset.Y, Color.Red, 0.5f);
                    Direct3D.DrawLine(cX + offset.X, cY + offset.Y, cX + offset.X, (cY + offset.Y) + cHeight, Color.Red, 0.5f);
                    Direct3D.DrawLine(cX + offset.X, (cY + offset.Y) + cHeight, (cX + offset.X) + cWidth, (cY + offset.Y) + cHeight, Color.Red, 0.5f);
                    Direct3D.DrawLine(cX + offset.X + cWidth, cY + offset.Y, (cX + offset.X) + cWidth, (cY + offset.Y) + cHeight, Color.Red, 0.5f);
                }

                if (anchorX > -1 && anchorY > -1)
                {
                    TextureManager.Draw(nAnchorID, anchorX + offset.X, anchorY + offset.Y, 1.0f, 1.0f, Rectangle.Empty, 0, 0, 0, Color.FromArgb(255, 255, 255, 255).ToArgb());
                }


                Direct3D.SpriteEnd();
                Direct3D.DeviceEnd();
                Direct3D.Present();
            }

            if (Direct3D.ChangeDisplayParam(panel2))
            {

                Direct3D.Clear(Color.CornflowerBlue);
                Direct3D.DeviceBegin();
                Direct3D.SpriteBegin();

                if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                {
                    
                    if (nID != -1)
                    {
                        if (bPlay)
                        {

                            Rectangle currRect = new Rectangle(lAnimations[listBox1.SelectedIndex].lFrames[currFrame].DrawX,
                                                                lAnimations[listBox1.SelectedIndex].lFrames[currFrame].DrawY,
                                                                lAnimations[listBox1.SelectedIndex].lFrames[currFrame].DrawWidth,
                                                                lAnimations[listBox1.SelectedIndex].lFrames[currFrame].DrawHeight);

                            TextureManager.Draw(nID, (panel2.Width / 2) - (lAnimations[listBox1.SelectedIndex].lFrames[currFrame].AnchorX - lAnimations[listBox1.SelectedIndex].lFrames[currFrame].DrawX),
                                (panel2.Height / 2) - (lAnimations[listBox1.SelectedIndex].lFrames[currFrame].AnchorY - lAnimations[listBox1.SelectedIndex].lFrames[currFrame].DrawY),
                                1.0f, 1.0f, currRect, 0, 0, 0, Color.FromArgb(255, 255, 255, 255).ToArgb());
                            
                        }
                        else
                        {
                            if( listBox2.SelectedIndex < listBox2.Items.Count)
                            {
                            Rectangle currRect = new Rectangle(lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawX,
                                                                lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawY,
                                                                lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawWidth,
                                                                lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawHeight);

                            TextureManager.Draw(nID, (panel2.Width / 2) - (lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].AnchorX - lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawX),
                                (panel2.Height / 2) - (lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].AnchorY - lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawY),
                                1.0f, 1.0f, currRect, 0, 0, 0, Color.FromArgb(255, 255, 255, 255).ToArgb());
                            }

                        }
                    }


                    Direct3D.SpriteEnd();
                    Direct3D.DeviceEnd();
                    Direct3D.Present();
                }
            }
        }

        public void UpdateTime()
        {
            timer.Update();

            if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
            {
                MaxFrame = listBox2.Items.Count - 1;

                if (bPlay)
                {
                    if (lAnimations[listBox1.SelectedIndex].lFrames.Count() > 0)
                    {
                        TimeWaited += (timer.DeltaTime.Milliseconds);


                        if (TimeWaited > lAnimations[listBox1.SelectedIndex].lFrames[currFrame].Duration)
                        {
                            TimeWaited -= lAnimations[listBox1.SelectedIndex].lFrames[currFrame].Duration;

                            currFrame += 1;

                            if( currFrame > MaxFrame )
                            {
                                currFrame = 0;
                                TimeWaited = 0;
                            }
                        }

                    }
                }
                else if (bReset)
                {
                    currFrame = 0;
                    TimeWaited = 0;
                }

            }


        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            InEditor = false;
            Direct3D.Shutdown();
            TextureManager.ShutdownManagedTextureManager();
        }

        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (bFrame)
                {
                    dX = e.Location.X;
                    numericUpDown2.Value = dX;

                    dY = e.Location.Y;
                    numericUpDown1.Value = dY;

                    dHeight = 0;
                    dWidth = 0;


                    if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                    {
                        lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawX = dX;
                        lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawY = dY;
                    }
                }
                else if (bCollision)
                {
                    cX = e.Location.X;
                    numericUpDown2.Value = cX;

                    cY = e.Location.Y;
                    numericUpDown1.Value = cY;

                    cHeight = 0;
                    cWidth = 0;

                    if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                    {
                        lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].CollisionX = cX;
                        lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].CollisionY = cY;
                    }
                }
                else if (bAnchorPoint)
                {
                    anchorX = e.Location.X;
                    numericUpDown6.Value = anchorX;

                    anchorY = e.Location.Y;
                    numericUpDown5.Value = anchorY;

                    if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                    {
                        lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].AnchorX = anchorX;
                        lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].AnchorY = anchorY;
                    }
                }
            }
        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (bFrame)
                {
                    if (e.Location.X > dX)
                    {
                        dWidth = e.Location.X - dX;
                        numericUpDown3.Value = dWidth;
                    }

                    if (e.Location.Y > dY)
                    {
                        dHeight = e.Location.Y - dY;
                        numericUpDown4.Value = dHeight;
                    }
                }
                else if (bCollision)
                {
                    if (e.Location.X > cX)
                    {
                        cWidth = e.Location.X - cX;
                        numericUpDown3.Value = cWidth;
                    }

                    if (e.Location.Y > cY)
                    {
                        cHeight = e.Location.Y - cY;
                        numericUpDown4.Value = cHeight;
                    }
                }
                else if (bAnchorPoint)
                {
                    anchorX = e.Location.X;
                    numericUpDown6.Value = anchorX;

                    anchorY = e.Location.Y;
                    numericUpDown5.Value = anchorY;
                }

            }
        }

        private void panel3_Scroll(object sender, ScrollEventArgs e)
        {
            Render();
        }

        private void numericUpDown6_ValueChanged(object sender, EventArgs e)
        {
                anchorX = (int)numericUpDown6.Value;

                if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                {
                    lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].AnchorX = anchorX;
                }
        }

        private void numericUpDown5_ValueChanged(object sender, EventArgs e)
        {
                anchorY = (int)numericUpDown5.Value;

                if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
                {
                    lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].AnchorY = anchorY;
                }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex > -1)
            {
                if (lAnimations[listBox1.SelectedIndex].lFrames == null)
                    lAnimations[listBox1.SelectedIndex].lFrames = new List<Frame>();

                textBox2.Text = lAnimations[listBox1.SelectedIndex].Name;

                listBox2.Items.Clear();

                int num = 0;

                foreach (Frame f in lAnimations[listBox1.SelectedIndex].lFrames)
                {
                    f.FrameNumber = num;
                    listBox2.Items.Add(f);
                    num += 1;
                }

                listBox2.SelectedIndex = listBox2.Items.Count - 1;
                listBox2.Invalidate();
            }
        }

        private void listBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
            {
                dX = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawX;
                dY = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawY;
                dWidth = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawWidth;
                dHeight = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].DrawHeight;

                cX = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].CollisionX;
                cY = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].CollisionY;
                cWidth = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].CollisionWidth;
                cHeight = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].CollisionHeight;

                anchorX = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].AnchorX;
                anchorY = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].AnchorY;

                fDuration = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].Duration;

                textBox1.Text = lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].TriggerEvent;
                numericUpDown6.Value = anchorX;
                numericUpDown5.Value = anchorY;
                numericUpDown7.Value = (decimal)fDuration;

                if (bFrame)
                {
                    numericUpDown1.Value = dY;
                    numericUpDown2.Value = dX;
                    numericUpDown3.Value = dWidth;
                    numericUpDown4.Value = dHeight;
                    numericUpDown6.Value = anchorX;
                    numericUpDown5.Value = anchorY;
                }
                else if (bCollision)
                {
                    numericUpDown1.Value = cY;
                    numericUpDown2.Value = cX;
                    numericUpDown3.Value = cWidth;
                    numericUpDown4.Value = cHeight;
                    numericUpDown6.Value = anchorX;
                    numericUpDown5.Value = anchorY;
                    numericUpDown7.Value = (decimal)fDuration;
                }

            }
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex > -1)
            {
                lAnimations[listBox1.SelectedIndex].Name = textBox2.Text;
            }
            listBox1.Invalidate();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
            {
                lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].TriggerEvent = textBox1.Text;
            }
        }

        private void numericUpDown7_ValueChanged(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
            {
                lAnimations[listBox1.SelectedIndex].lFrames[listBox2.SelectedIndex].Duration = (int)numericUpDown7.Value;
            }
        }

        private void button11_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
            {
                if (listBox2.SelectedIndex + 1 >= listBox2.Items.Count)
                    listBox2.SelectedIndex = 0;
                else
                    listBox2.SelectedIndex += 1;

            }
        }

        private void button10_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
            {
                // prev frame
                if (listBox2.SelectedIndex - 1 < 0)
                    listBox2.SelectedIndex = listBox2.Items.Count - 1;
                else
                    listBox2.SelectedIndex -= 1;

                


            }
        }

        private void button8_Click(object sender, EventArgs e)
        {
            //Play
            bPlay = true;
            bReset = false;
        }

        private void button9_Click(object sender, EventArgs e)
        {
            //Stop
            bPlay = false;
            bReset = false;
        }

        private void button12_Click(object sender, EventArgs e)
        {
            //Reset
            bPlay = false;
            bReset = true;
        }

        private void saveXMLToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "XML | *.xml;*.XML";
            dlg.Title = "Save Current Animations";
            dlg.DefaultExt = "xml";
            dlg.FilterIndex = 2;
            dlg.AddExtension = true;

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                XElement xRoot = new XElement("Animations");

                for (int i = 0; i < lAnimations.Count; ++i)
                {
                    XElement xAnim = new XElement("Animation");

                    XAttribute xIsLooping = new XAttribute("IsLooping",0);
                    xAnim.Add(xIsLooping);

                    XAttribute xFileName = new XAttribute("File", texture);
                    xAnim.Add(xFileName);
                    
                    XAttribute xName = new XAttribute("Name", lAnimations[i].Name);
                    xAnim.Add(xName);


                    for (int j = 0; j < lAnimations[i].lFrames.Count; ++j)
                    {
                        XElement xFrame = new XElement("Frames");

                        //collision

                        //framey
                        XAttribute xCollisionY = new XAttribute("CollisionFrameY", lAnimations[i].lFrames[j].CollisionY);
                        xFrame.Add(xCollisionY);

                        //framex
                        XAttribute xCollisionX = new XAttribute("CollisionFrameX", lAnimations[i].lFrames[j].CollisionX);
                        xFrame.Add(xCollisionX);

                        //draw width
                        XAttribute xCollisionWidth = new XAttribute("CollisionWidth", lAnimations[i].lFrames[j].CollisionWidth);
                        xFrame.Add(xCollisionWidth);

                        //draw height
                        XAttribute xCollisionHeight = new XAttribute("CollisionHeight", lAnimations[i].lFrames[j].CollisionHeight);
                        xFrame.Add(xCollisionHeight);

                        if (lAnimations[i].lFrames[j].TriggerEvent != "")
                        {
                            XAttribute xEvent = new XAttribute("Event", lAnimations[i].lFrames[j].TriggerEvent);
                            xFrame.Add(xEvent);
                        }
                        else
                        {
                            string st = "";

                            XAttribute xEvent = new XAttribute("Event", st);
                            xFrame.Add(xEvent);

                        }

                        //draw height
                        XAttribute xDrawHeight = new XAttribute("Height", lAnimations[i].lFrames[j].DrawHeight);
                        xFrame.Add(xDrawHeight);
                       
                        //draw width
                        XAttribute xDrawWidth = new XAttribute("Width",lAnimations[i].lFrames[j].DrawWidth);
                        xFrame.Add(xDrawWidth);
                        
                        //anchorx
                        XAttribute xAnchorX = new XAttribute("AnchorX", lAnimations[i].lFrames[j].AnchorX - lAnimations[i].lFrames[j].DrawX);
                        xFrame.Add(xAnchorX);
                       
                        //anchory
                        XAttribute xAnchorY = new XAttribute("AnchorY",lAnimations[i].lFrames[j].AnchorY - lAnimations[i].lFrames[j].DrawY);
                        xFrame.Add(xAnchorY);
                       
                        //framex
                        XAttribute xDrawX = new XAttribute("FrameX",lAnimations[i].lFrames[j].DrawX);
                        xFrame.Add(xDrawX);
                       
                        //framey
                        XAttribute xDrawY = new XAttribute("FrameY",lAnimations[i].lFrames[j].DrawY);
                        xFrame.Add(xDrawY);
                       
                        //duration
                        float fDuration = lAnimations[i].lFrames[j].Duration / 1000;
                        XAttribute xDuration = new XAttribute("Duration", fDuration);
                        xFrame.Add(xDuration);
                        xAnim.Add(xFrame);
                    }

                    xRoot.Add(xAnim);
                }

                xRoot.Save(dlg.FileName);
                GC.Collect();
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex > -1 && listBox2.SelectedIndex > -1)
            {
                if (listBox2.Items.Count > 1)
                {
                    lAnimations[listBox1.SelectedIndex].lFrames.RemoveAt(listBox2.SelectedIndex);
                    listBox2.Items.RemoveAt(listBox2.SelectedIndex);
                    if (listBox2.SelectedIndex - 1 < 0)
                    {
                        listBox2.SelectedIndex = 0;
                    }
                }
                else
                {
                    lAnimations[listBox1.SelectedIndex].lFrames.Clear();
                    listBox2.Items.Clear();
                }

                listBox2.Invalidate();
            }
        }

        //remove animations
        private void button4_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex > -1)
            {
                lAnimations[listBox1.SelectedIndex].lFrames.Clear();

                if (listBox1.Items.Count > 1)
                {
                    lAnimations.RemoveAt(listBox1.SelectedIndex);
                    listBox1.Items.RemoveAt(listBox1.SelectedIndex);

                    if (listBox1.SelectedIndex - 1 < 0)
                    {
                        listBox1.SelectedIndex = 0;
                    }
                }
                else
                {
                    lAnimations.Clear();
                    listBox1.Items.Clear();
                    listBox2.Items.Clear();
                }


                listBox1.Invalidate();
                listBox2.Invalidate();
            }
        }

        private void laodAnimationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "All Files(*.*)|*.*|XML Files(*.xml)|*.xml";
            dlg.FilterIndex = 2;
            dlg.DefaultExt = "xml";

            if( dlg.ShowDialog() == DialogResult.OK)
            {

                XElement root = XElement.Load(dlg.FileName);

                IEnumerable<XElement> animations = root.Elements("Animation");


                if (animations != null)
                {
                    lAnimations.Clear();
                    int last = -1;

                    foreach (XElement X in animations)
                    {
                        XAttribute A;
                        Animations anim = new Animations();
                        last = lAnimations.Count;

                        //name
                        A = X.Attribute("Name");
                        anim.Name = Convert.ToString(A.Value);

                        //file
                        A = X.Attribute("File");
                        texture = Convert.ToString(A.Value);

                        nID = TextureManager.LoadTexture(texture,0);
                        if (nID == -1)
                        {
                            OpenFileDialog opdlg = new OpenFileDialog();

                            if (DialogResult.OK == opdlg.ShowDialog())
                            {
                                nID = TextureManager.LoadTexture(opdlg.FileName.ToString(), 0);

                                Size nSize = new Size(TextureManager.GetTextureWidth(nID), TextureManager.GetTextureHeight(nID));

                                texture = opdlg.FileName;

                                panel1.Size = nSize;
                            }
                        }


                        anim.Ordernumber = last;

                        IEnumerable<XElement> frames = X.Elements("Frames");


                        if (frames != null)
                        {
                            int framenumber = -1;

                            foreach (XElement Z in frames)
                            {
                                if (anim.lFrames == null)
                                {
                                    anim.lFrames = new List<Frame>();
                                }

                                framenumber += 1;
                                XAttribute B;
                                Frame frame = new Frame();

                                frame.FrameNumber = framenumber;

                                //Duration
                                B = Z.Attribute("Duration");
                                frame.Duration = ((float)Convert.ToDecimal(B.Value)) * 1000;

                                fDuration = frame.Duration;

                                //FrameY
                                B = Z.Attribute("FrameY");
                                frame.DrawY = Convert.ToInt32(B.Value);
                                dY = frame.DrawY;

                                //FrameX
                                B = Z.Attribute("FrameX");
                                frame.DrawX = Convert.ToInt32(B.Value);
                                dX = frame.DrawX;

                                //AnchorY
                                B = Z.Attribute("AnchorY");
                                frame.AnchorY = Convert.ToInt32(B.Value) + frame.DrawY;
                                anchorY = frame.AnchorY + frame.DrawY;

                                //AnchorX
                                B = Z.Attribute("AnchorX");
                                frame.AnchorX = Convert.ToInt32(B.Value) + frame.DrawX;
                                anchorX = frame.AnchorX + frame.DrawX;

                                //Width
                                B = Z.Attribute("Width");
                                frame.DrawWidth = Convert.ToInt32(B.Value);
                                dWidth = frame.DrawWidth;

                                //Height
                                B = Z.Attribute("Height");
                                frame.DrawHeight = Convert.ToInt32(B.Value);
                                dHeight = frame.DrawHeight;

                                //Event
                                B = Z.Attribute("Event");
                                frame.TriggerEvent = Convert.ToString(B.Value);
                                textBox1.Text = frame.TriggerEvent;

                                //CollisionHeigh
                                B = Z.Attribute("CollisionHeight");
                                frame.CollisionHeight = Convert.ToInt32(B.Value);
                                cHeight = frame.CollisionHeight;

                                //CollisionWidth
                                B = Z.Attribute("CollisionWidth");
                                frame.CollisionWidth = Convert.ToInt32(B.Value);
                                cWidth = frame.CollisionWidth;

                                //CollisionFrameX
                                B = Z.Attribute("CollisionFrameX");
                                frame.CollisionX = Convert.ToInt32(B.Value);
                                cX = frame.CollisionX;

                                //CollisionFrameY
                                B = Z.Attribute("CollisionFrameY");
                                frame.CollisionY = Convert.ToInt32(B.Value);
                                cY = frame.CollisionY;

                                anim.lFrames.Add(frame);
                                listBox2.Items.Add(anim.lFrames[framenumber]);

                            }

                        }
                            lAnimations.Add(anim);
                            listBox1.Items.Add(lAnimations[last]);


                    }
                    
                }




            }
        }

    }
}
