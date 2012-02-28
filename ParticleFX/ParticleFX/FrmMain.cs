// System References
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Image = System.Drawing.Image;
using Bitmap = System.Drawing.Bitmap;
using GdiGraphics = System.Drawing.Graphics;

// XNA References
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System.IO;
using System.Threading;
using System.Xml;
using System.Xml.Linq;

/***************************************************************
 * BufferedPanel.cs
 ***************************************************************
 *  Author:         Jeffery Sventora
 *  Created:        2/7/2012
 *  Last Edit:      2/14/2012
 ***************************************************************
 *  Description:    
 ***************************************************************
 * This file is a control that inherits Panel, the only 
 * difference is that this panel features double buffering.
 ***************************************************************/

namespace ParticleFX
{
    public partial class FrmMain : Form
    {
        // Data Fields
        public GraphicsDevice Device;
        SpriteBatch Batch;
        RenderTarget2D RenderTarget;
        PerformanceCounter GameTimer;
        public bool InEditor = true;
        bool Playing = true;

        // Particle Data
        List<ParticleEmitter> Emitters;

        // Mouse Cursor
        ParticleTexture CursorTexture;
        Vector2 Position = Vector2.Zero;
        Color CursorColor = Color.White;
        new bool MouseDown = false;

        // Other Forms
        FrmEmitter ToolEmitter;

        // Constructor
        public FrmMain()
        {
            InitializeComponent();
            InitializeXNA();
          
            // Load any images
            CursorTexture = new ParticleTexture("Cursor.png", Device);

            // Initialize data
            GameTimer = new PerformanceCounter();
            Emitters = new List<ParticleEmitter>();

            ToolEmitter = new FrmEmitter();
            ToolEmitter.Owner = this;

            // Create the Emitter Tool
            Point CenterScreen = new Point(Screen.PrimaryScreen.Bounds.Width >> 1, Screen.PrimaryScreen.Bounds.Height >> 1);

            Left = CenterScreen.X - (Width >> 1) - (ToolEmitter.Width >> 1) - 2;
            Top = CenterScreen.Y - (Height >> 1);
            ToolEmitter.Left = Right + 2;
            ToolEmitter.Top = CenterScreen.Y - (ToolEmitter.Height >> 1);

            ToolEmitter.Show();
        }

        // XNA Initialization
        private void InitializeXNA()
        {
            PresentationParameters PresentParams = new PresentationParameters();
            PresentParams.BackBufferFormat = SurfaceFormat.Color;
            PresentParams.BackBufferWidth = PnlSurface.Width;
            PresentParams.BackBufferHeight = PnlSurface.Height;
            PresentParams.DepthStencilFormat = DepthFormat.None;
            PresentParams.DisplayOrientation = DisplayOrientation.Default;
            PresentParams.DeviceWindowHandle = PnlSurface.Handle;
            PresentParams.IsFullScreen = false;
            PresentParams.MultiSampleCount = 0;
            PresentParams.PresentationInterval = PresentInterval.Immediate;
            PresentParams.RenderTargetUsage = RenderTargetUsage.DiscardContents;

            // Try to initialize the Device
            try
            {
                Device = new GraphicsDevice(GraphicsAdapter.DefaultAdapter, GraphicsProfile.HiDef, PresentParams);
            }
            catch
            {
                MessageBox.Show("Failed to Initialize the Device!", "XNA Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            // Try to initialize the SpriteBatch
            try
            {
                Batch = new SpriteBatch(Device);
            }
            catch
            {
                MessageBox.Show("Failed to Initialize SpriteBatch!", "XNA Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            // Set up our Render Target
            RenderTarget = new RenderTarget2D(Device, PnlSurface.Width, PnlSurface.Height);
        }

        // Render/Update Logic
        public void UpdateFunction()
        {
            /*********************************************
            * UPDATE LOGIC HERE
            *********************************************/
            GameTimer.Update(LblFrames);

            int ParticleCount = 0;
            if (Playing)
            {
                for (int i = 0; i < Emitters.Count; ++i)
                {
                    Emitters[i].Update(GameTimer);
                    ParticleCount += Emitters[i].ParticleCountRendering;
                }
            }
            LblParticleCount.Text = "Count: " + ParticleCount.ToString();
            /*********************************************/
        }
        public void RenderFunction()
        {
            if (Device != null)
            {
                // Clear the surface
                Device.SetRenderTarget(RenderTarget);
                Device.Clear(Color.Black);

                /*********************************************
                * DRAWING LOGIC HERE
                *********************************************/
                for (int i = 0; i < Emitters.Count; ++i)
                    Emitters[i].Render(Batch);
                /*********************************************/

                Device.SetRenderTarget(null);
                Device.Clear(Color.Black);

                Batch.Begin();
                Batch.Draw(RenderTarget, Vector2.Zero, Color.White);
                Batch.Draw(CursorTexture.Texture, Position, Color.White);
                Batch.End();

                // Present all of the changes!
                Device.Present();
                Thread.Sleep(1);
                GameTimer.AddFrame();
            }
        }

        // Save & Load
        void SaveFunction()
        {
            SaveFileDialog SFD = new SaveFileDialog();
            SFD.Filter = "XML File (*.xml)|*.xml";

            if (SFD.ShowDialog() == DialogResult.OK)
            {
                using (XmlWriter X = XmlWriter.Create(SFD.FileName))
                {
                    // Document Header
                    X.WriteStartDocument();
                    X.WriteStartElement("EffectEmitters");
                    X.WriteStartAttribute("Repeat");
                    {
                        X.WriteValue(ChkRepeat.Checked);
                    }
                    X.WriteEndAttribute();

                    foreach (ParticleEmitter M in Emitters)
                    {
                        // New Emitter
                        X.WriteStartElement("Emitter");
                        {
                            // Emitter Name
                            X.WriteStartAttribute("Name");
                            {
                                X.WriteValue(M.Name);
                            }
                            X.WriteEndAttribute();

                            // Emitter Life Span
                            X.WriteElementString("Life", M.LifeSpan.ToString());
                            // Particle Duration Min.
                            X.WriteElementString("ParticleDurationMin", M.ParticleDuration.ToString());
                            // Particle Duration Max.
                            X.WriteElementString("ParticleDurationMax", M.ParticleDurationMax.ToString());
                            // Particle Spawn Rate
                            X.WriteElementString("SpawnRate", M.SpawnRate.ToString());
                            // Maximum Number of Particles
                            X.WriteElementString("MaxParticles", M.MaxParticles.ToString());
                            // Number of Particles to Spit
                            X.WriteElementString("NumToSpit", M.NumParticlesToSpit.ToString());

                            // Particle Gravity
                            #region EmitterGravity Values
                            X.WriteStartElement("Gravity");
                            {
                                // X Location
                                X.WriteStartAttribute("X");
                                {
                                    X.WriteValue((M.Gravity.X - M.SpawnRange.X).ToString());
                                }
                                X.WriteEndAttribute();
                                // Y Location
                                X.WriteStartAttribute("Y");
                                {
                                    X.WriteValue((M.Gravity.Y - M.SpawnRange.Y).ToString());
                                }
                                X.WriteEndAttribute();
                                // Pull Factor
                                X.WriteStartAttribute("Pull");
                                {
                                    X.WriteValue(M.GravitationalPull);
                                }
                                X.WriteEndAttribute();
                            }
                            X.WriteEndElement();
                            #endregion

                            // Emitter Spawn Range
                            #region EmitterSpawnRange Values
                            X.WriteStartElement("Range");
                            {
                                // Emitter Range Width
                                X.WriteStartAttribute("Width");
                                {
                                    X.WriteValue(M.SpawnRange.Width.ToString());
                                }
                                X.WriteEndAttribute();
                                // Emitter Range Height
                                X.WriteStartAttribute("Height");
                                {
                                    X.WriteValue(M.SpawnRange.Height.ToString());
                                }
                                X.WriteEndAttribute();
                            }
                            X.WriteEndElement();
                            #endregion

                            // Start Color
                            #region StartColor Values
                            X.WriteStartElement("StartColor");
                            {
                                // Start Color Red
                                X.WriteStartAttribute("Red");
                                {
                                    X.WriteValue(M.StartColor.R.ToString());
                                }
                                X.WriteEndAttribute();
                                // Start Color Green
                                X.WriteStartAttribute("Green");
                                {
                                    X.WriteValue(M.StartColor.G.ToString());
                                }
                                X.WriteEndAttribute();
                                // Start Color Blue
                                X.WriteStartAttribute("Blue");
                                {
                                    X.WriteValue(M.StartColor.B.ToString());
                                }
                                X.WriteEndAttribute();
                                // Start Color Alpha
                                X.WriteStartAttribute("Alpha");
                                {
                                    X.WriteValue(M.StartColor.A.ToString());
                                }
                                X.WriteEndAttribute();
                            }
                            X.WriteEndElement();
                            #endregion

                            // End Color
                            #region EndColor Values
                            X.WriteStartElement("EndColor");
                            {
                                // End Color Red
                                X.WriteStartAttribute("Red");
                                {
                                    X.WriteValue(M.EndColor.R.ToString());
                                }
                                X.WriteEndAttribute();
                                // End Color Green
                                X.WriteStartAttribute("Green");
                                {
                                    X.WriteValue(M.EndColor.G.ToString());
                                }
                                X.WriteEndAttribute();
                                // End Color Blue
                                X.WriteStartAttribute("Blue");
                                {
                                    X.WriteValue(M.EndColor.B.ToString());
                                }
                                X.WriteEndAttribute();
                                // End Color Alpha
                                X.WriteStartAttribute("Alpha");
                                {
                                    X.WriteValue(M.EndColor.A.ToString());
                                }
                                X.WriteEndAttribute();
                            }
                            X.WriteEndElement();
                            #endregion

                            // Source/Destination Blends
                            X.WriteElementString("SourceBlend", M.SourceBlend.ToString());
                            X.WriteElementString("DestinationBlend", M.DestinationBlend.ToString());

                            // Particle Start Velocity
                            #region ParticleStartVelocity Values
                            X.WriteStartElement("StartVelocity");
                            {
                                // X Velocity
                                X.WriteStartAttribute("X");
                                {
                                    X.WriteValue(M.StartVelocity.X.ToString());
                                }
                                X.WriteEndAttribute();
                                // Y Velocity
                                X.WriteStartAttribute("Y");
                                {
                                    X.WriteValue(M.StartVelocity.Y.ToString());
                                }
                                X.WriteEndAttribute();
                                // Random X
                                X.WriteStartAttribute("RandX");
                                {
                                    X.WriteValue(M.RandomStartVelocityX);
                                }
                                X.WriteEndAttribute();
                                // Random Y
                                X.WriteStartAttribute("RandY");
                                {
                                    X.WriteValue(M.RandomStartVelocityY);
                                }
                                X.WriteEndAttribute();
                            }
                            X.WriteEndElement();
                            #endregion

                            // Particle End Velocity
                            #region ParticleEndVelocity Values
                            X.WriteStartElement("EndVelocity");
                            {
                                // X Velocity
                                X.WriteStartAttribute("X");
                                {
                                    X.WriteValue(M.EndVelocity.X.ToString());
                                }
                                X.WriteEndAttribute();
                                //Y Velocity
                                X.WriteStartAttribute("Y");
                                {
                                    X.WriteValue(M.EndVelocity.Y.ToString());
                                }
                                X.WriteEndAttribute();
                                // Random X
                                X.WriteStartAttribute("RandX");
                                {
                                    X.WriteValue(M.RandomEndVelocityX);
                                }
                                X.WriteEndAttribute();
                                // Random Y
                                X.WriteStartAttribute("RandY");
                                {
                                    X.WriteValue(M.RandomEndVelocityY);
                                }
                            }
                            X.WriteEndElement();
                            #endregion

                            // Particle Start & End Scale
                            #region ParticleStartEndScales Value
                            X.WriteStartElement("Scale");
                            {
                                // Start Value
                                X.WriteStartAttribute("Start");
                                {
                                    X.WriteValue(M.StartScale.ToString());
                                }
                                X.WriteEndAttribute();
                                // End Value
                                X.WriteStartAttribute("End");
                                {
                                    X.WriteValue(M.EndScale.ToString());
                                }
                                X.WriteEndAttribute();
                            }
                            X.WriteEndElement();
                            #endregion

                            // Particle Start & End Rotation
                            #region ParticleStartEndRotations Value
                            X.WriteStartElement("Rotation");
                            {
                                // Start Rotation
                                X.WriteStartAttribute("Start");
                                {
                                    X.WriteValue(M.StartRotation.ToString());
                                }
                                X.WriteEndAttribute();
                                // End Rotation
                                X.WriteStartAttribute("End");
                                {
                                    X.WriteValue(M.EndRotation.ToString());
                                }
                                X.WriteEndAttribute();
                            }
                            X.WriteEndElement();
                            #endregion

                            // Write out the textures
                            #region EmitterTextures Files
                            X.WriteStartElement("Textures");
                            {
                                foreach (ParticleTexture T in M.ParticleTextures)
                                {
                                    // Start Writing a New Texture File
                                    X.WriteStartElement("Texture");
                                    {
                                        // Name of the File
                                        X.WriteStartAttribute("File");
                                        {
                                            X.WriteValue(T.Name);
                                        }
                                        X.WriteEndAttribute();
                                    }
                                    X.WriteEndElement();
                                }
                            }
                            X.WriteEndElement();
                            #endregion
                        }
                        X.WriteEndElement();
                    }

                    X.WriteEndElement();
                    X.WriteEndDocument();
                }
            }

            SFD.Dispose();
        }
        void LoadFunction()
        {
            OpenFileDialog OFD = new OpenFileDialog();
            OFD.Filter = "XML File (*.xml)|*.xml";

            if (OFD.ShowDialog() == DialogResult.OK)
            {
                XElement Root = XElement.Load(OFD.FileName);
                IEnumerable<XElement> EM = Root.Elements("Emitter");
                XAttribute RA = Root.Attribute("Repeat");
                ChkRepeat.Checked = Convert.ToBoolean(RA.Value);

                if (EM != null)
                {
                    Emitters.Clear();
                    int Last = -1;

                    foreach (XElement X in EM)
                    {
                        XAttribute A;
                        XElement E;
                        Last = Emitters.Count;
                        Emitters.Add(new ParticleEmitter());

                        // Name
                        A = X.Attribute("Name");
                        Emitters[Last].Name = Convert.ToString(A.Value);

                        // Particle Duration Min
                        E = X.Element("ParticleDurationMin");
                        Emitters[Last].ParticleDuration = Convert.ToSingle(E.Value);

                        // Particle Duration Max
                        E = X.Element("ParticleDurationMax");
                        Emitters[Last].ParticleDurationMax = Convert.ToSingle(E.Value);

                        // Spawn Rate
                        E = X.Element("SpawnRate");
                        Emitters[Last].SpawnRate = Convert.ToSingle(E.Value);

                        // Max Particles
                        E = X.Element("MaxParticles");
                        Emitters[Last].MaxParticles = Convert.ToInt16(E.Value);

                        // Num. to Spit
                        E = X.Element("NumToSpit");
                        Emitters[Last].NumParticlesToSpit = Convert.ToInt16(E.Value);

                        // Gravity
                        E = X.Element("Gravity");
                        A = E.Attribute("X");
                        Emitters[Last].Gravity.X = Convert.ToSingle(A.Value);
                        A = E.Attribute("Y");
                        Emitters[Last].Gravity.Y = Convert.ToSingle(A.Value);
                        A = E.Attribute("Pull");
                        Emitters[Last].GravitationalPull = Convert.ToSingle(A.Value);

                        // Spawn Range
                        E = X.Element("Range");
                        A = E.Attribute("Width");
                        Emitters[Last].SpawnRange.Width = Convert.ToInt32(A.Value);
                        A = E.Attribute("Height");
                        Emitters[Last].SpawnRange.Height = Convert.ToInt32(A.Value);

                        // Start Color
                        E = X.Element("StartColor");
                        A = E.Attribute("Red");
                        Emitters[Last].StartColor.R = Convert.ToByte(A.Value);
                        A = E.Attribute("Green");
                        Emitters[Last].StartColor.G = Convert.ToByte(A.Value);
                        A = E.Attribute("Blue");
                        Emitters[Last].StartColor.B = Convert.ToByte(A.Value);
                        A = E.Attribute("Alpha");
                        Emitters[Last].StartColor.A = Convert.ToByte(A.Value);

                        // End Color
                        E = X.Element("EndColor");
                        A = E.Attribute("Red");
                        Emitters[Last].EndColor.R = Convert.ToByte(A.Value);
                        A = E.Attribute("Green");
                        Emitters[Last].EndColor.G = Convert.ToByte(A.Value);
                        A = E.Attribute("Blue");
                        Emitters[Last].EndColor.B = Convert.ToByte(A.Value);
                        A = E.Attribute("Alpha");
                        Emitters[Last].EndColor.A = Convert.ToByte(A.Value);

                        // Source/Destination Blends
                        E = X.Element("SourceBlend");
                        Emitters[Last].SourceBlend = (Blend)Enum.Parse(typeof(Blend), E.Value);
                        E = X.Element("DestinationBlend");
                        Emitters[Last].DestinationBlend = (Blend)Enum.Parse(typeof(Blend), E.Value);

                        // Start Velocity
                        E = X.Element("StartVelocity");
                        A = E.Attribute("X");
                        Emitters[Last].StartVelocity.X = Convert.ToSingle(A.Value);
                        A = E.Attribute("Y");
                        Emitters[Last].StartVelocity.Y = Convert.ToSingle(A.Value);
                        A = E.Attribute("RandX");
                        Emitters[Last].RandomStartVelocityX = Convert.ToBoolean(A.Value);
                        A = E.Attribute("RandY");
                        Emitters[Last].RandomStartVelocityY = Convert.ToBoolean(A.Value);

                        // End Velocity
                        E = X.Element("EndVelocity");
                        A = E.Attribute("X");
                        Emitters[Last].EndVelocity.X = Convert.ToSingle(A.Value);
                        A = E.Attribute("Y");
                        Emitters[Last].EndVelocity.Y = Convert.ToSingle(A.Value);
                        A = E.Attribute("RandX");
                        Emitters[Last].RandomEndVelocityX = Convert.ToBoolean(A.Value);
                        A = E.Attribute("RandY");
                        Emitters[Last].RandomEndVelocityY = Convert.ToBoolean(A.Value);

                        // Start & End Scale
                        E = X.Element("Scale");
                        A = E.Attribute("Start");
                        Emitters[Last].StartScale = Convert.ToSingle(A.Value);
                        A = E.Attribute("End");
                        Emitters[Last].EndScale = Convert.ToSingle(A.Value);

                        // Start & End Rotation
                        E = X.Element("Rotation");
                        A = E.Attribute("Start");
                        Emitters[Last].StartRotation = Convert.ToSingle(A.Value);
                        A = E.Attribute("End");
                        Emitters[Last].EndRotation = Convert.ToSingle(A.Value);

                        // Textures
                        IEnumerable<XElement> ET = X.Elements("Textures");
                        foreach (XElement T in ET)
                        {
                            E = T.Element("Texture");
                            string FileName = E.Attribute("File").Value;

                            foreach (string F in ToolEmitter.TextureNames)
                                if (FileName == F)
                                    Emitters[Last].ParticleTextures.Add(new ParticleTexture(ToolEmitter.RelativePath + '\\' + F, Device));
                        }
                    }

                    LstEmitters.DataSource = null;
                    LstEmitters.DataSource = Emitters;
                }
            }

            OFD.Dispose();
        }

        // Unload all resources
        private void FrmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Turn off the rendering
            InEditor = false;
        }
        private void FrmMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            RenderTarget.Dispose();
            RenderTarget = null;

            Batch.Dispose();
            Batch = null;

            Device.Dispose();
            Device = null;

            GC.Collect();
        }

        // Follow Mouse
        private void PnlSurface_MouseMove(object sender, MouseEventArgs e)
        {
            Position = new Vector2(
                (float)e.X,
                (float)e.Y);

            if (MouseDown)
            {
                if (ToolEmitter.Emitter != null)
                {
                    ToolEmitter.Emitter.SpawnRange.X = e.X;
                    ToolEmitter.Emitter.SpawnRange.Y = e.Y;
                }
            }
        }
        private void BtnScreen_Click(object sender, EventArgs e)
        {
            FileStream FStream = new FileStream("Screen.png", FileMode.Create);
            RenderTarget.SaveAsPng(FStream, PnlSurface.Width, PnlSurface.Height);
            FStream.Dispose();
            FStream = null;
            GC.Collect();
        }
        private void PnlSurface_MouseLeave(object sender, EventArgs e)
        {
            Cursor.Show();
        }
        private void PnlSurface_MouseEnter(object sender, EventArgs e)
        {
            Cursor.Hide();
        }
        private void BtnNewEmitter_Click(object sender, EventArgs e)
        {
            ParticleEmitter New = new ParticleEmitter();
            New.Name = TxtEmitterName.Text;
            Emitters.Add(New);

            LstEmitters.DataSource = null;
            LstEmitters.DataSource = Emitters;
        }
        private void BtnRemoveSelected_Click(object sender, EventArgs e)
        {
            int Index = LstEmitters.SelectedIndex;

            if (Index > -1 && Index < Emitters.Count)
            {
                Emitters.RemoveAt(Index);
                LstEmitters.DataSource = null;
                LstEmitters.DataSource = Emitters;
                ToolEmitter.NewEmitter(null);
            }
        }
        private void BtnUpdateSelected_Click(object sender, EventArgs e)
        {
            int Index = LstEmitters.SelectedIndex;

            if (Index > -1 && Index < Emitters.Count)
            {
                Emitters[Index].Name = TxtEmitterName.Text;
                LstEmitters.DataSource = null;
                LstEmitters.DataSource = Emitters;
                ToolEmitter.NewEmitter(Emitters[Index]);
            }
        }
        private void BtnUp_Click(object sender, EventArgs e)
        {
            int Index = LstEmitters.SelectedIndex;

            if (Index > 0 && Index < Emitters.Count)
            {
                ParticleEmitter ToSwap = Emitters[Index];
                Emitters.RemoveAt(Index);
                Emitters.Insert(Index - 1, ToSwap);
                LstEmitters.DataSource = null;
                LstEmitters.DataSource = Emitters;
                LstEmitters.SelectedIndex -= 1;
            }
        }
        private void BtnDown_Click(object sender, EventArgs e)
        {
            int Index = LstEmitters.SelectedIndex;

            if (Index > -1 && Index < (Emitters.Count - 1))
            {
                ParticleEmitter ToSwap = Emitters[Index];
                Emitters.RemoveAt(Index);
                Emitters.Insert(Index + 1, ToSwap);
                LstEmitters.DataSource = null;
                LstEmitters.DataSource = Emitters;
                LstEmitters.SelectedIndex += 1;
            }
        }
        private void LstEmitters_SelectedIndexChanged(object sender, EventArgs e)
        {
            int Index = LstEmitters.SelectedIndex;

            if(Index > -1 && Index < Emitters.Count)
                ToolEmitter.NewEmitter(Emitters[LstEmitters.SelectedIndex]);
        }
        private void PnlSurface_MouseDown(object sender, MouseEventArgs e)
        {
            MouseDown = true;
            if (ToolEmitter.Emitter != null)
            {
                ToolEmitter.Emitter.SpawnRange.X = e.X;
                ToolEmitter.Emitter.SpawnRange.Y = e.Y;
            }
        }
        private void PnlSurface_MouseUp(object sender, MouseEventArgs e)
        {
            MouseDown = false;
            ToolEmitter.EmitterMouseUp();
        }

        // Menu Events
        private void MnuSave_Click(object sender, EventArgs e)
        {
            SaveFunction();
        }
        private void MnuOpen_Click(object sender, EventArgs e)
        {
            LoadFunction();
        }

        // Play & Pause
        private void BtnPlay_Click(object sender, EventArgs e)
        {
            Playing = true;
        }
        private void BtnStop_Click(object sender, EventArgs e)
        {
            Playing = false;
        }
        private void MnuTogglePlaying_Click(object sender, EventArgs e)
        {
            Playing = !Playing;
        }

        // Take Screen shot
        private void BtnScreenShot_Click(object sender, EventArgs e)
        {
            SaveFileDialog SDG = new SaveFileDialog();
            SDG.Filter = "Portable Network Graphic (*.png)|*.png";

            if (SDG.ShowDialog() == DialogResult.OK)
            {
                FileStream FStream = new FileStream(SDG.FileName, FileMode.Create);
                RenderTarget.SaveAsPng(FStream, PnlSurface.Width, PnlSurface.Height);
                FStream.Dispose();
            }

            SDG.Dispose();
        }
    }
}