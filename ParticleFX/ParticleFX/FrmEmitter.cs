// System References
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

using Bitmap = System.Drawing.Bitmap;
using Image = System.Drawing.Image;
using SysRect = System.Drawing.Rectangle;
using SysColor = System.Drawing.Color;

// XNA References
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

/***************************************************************
 * FrmEmitter.cs
 ***************************************************************
 *  Author:         Jeffery Sventora
 *  Created:        2/7/2012
 *  Last Edit:      2/14/2012
 ***************************************************************
 *  Description:    
 ***************************************************************
 * Tools for handling emitters
 ***************************************************************/

namespace ParticleFX
{
    public partial class FrmEmitter : Form
    {
        // Data Members
        public ParticleEmitter Emitter;
        public string RelativePath;
        int[] CustomColors;

        // Particle Image Members
        Bitmap ParticleImage;
        SysRect DestRect;

        // List Members
        List<Blend> SourceBlends;
        List<Blend> DestinationBlends;

        // Public Properties
        public System.Windows.Forms.ListBox.ObjectCollection TextureNames
        {
            get { return LstWorkDirFiles.Items; }
        }

        // Constructor
        public FrmEmitter()
        {
            InitializeComponent();

            // Handle our Relative Path
            if (Properties.Settings.Default.RelativePath == "")
            {
                FindValidWorkDirectory();
                InitializeWorkDirectory();
            }
            else
            {
                RelativePath = Properties.Settings.Default.RelativePath;

                if (Directory.Exists(RelativePath))
                {
                    InitializeWorkDirectory();
                }
                else
                {
                    FindValidWorkDirectory();
                    InitializeWorkDirectory();
                }
            }

            // Set up Controls
            SourceBlends = Enum.GetValues(typeof(Blend)).Cast<Blend>().ToList();
            DestinationBlends = Enum.GetValues(typeof(Blend)).Cast<Blend>().ToList();
            CmbSourceBlends.DataSource = SourceBlends;
            CmbDestBlends.DataSource = DestinationBlends;

            // Custom colors for Color Picking
            CustomColors = new int[16];
            for (int i = 0; i < 16; ++i)
                CustomColors[i] = 0xFFFFFF;
        }

        // Working Directory Logic
        void FindValidWorkDirectory()
        {
            FolderBrowserDialog FBD = new FolderBrowserDialog();

            if (FBD.ShowDialog() == DialogResult.OK)
            {
                RelativePath = FBD.SelectedPath;
                Properties.Settings.Default.RelativePath = RelativePath;
                Properties.Settings.Default.Save();
            }
            else
            {
                FBD.Dispose();
                FBD = null;
                GC.Collect();
                FindValidWorkDirectory();
            }
        }
        void InitializeWorkDirectory()
        {
            string[] ValidImageFiles = Directory.GetFiles(RelativePath);

            LstWorkDirFiles.Items.Clear();

            foreach (string File in ValidImageFiles)
            {
                if (Path.GetExtension(File) == ".png")
                    LstWorkDirFiles.Items.Add(Path.GetFileName(File));
            }
        }

        // Change the Emitter to edit
        public void NewEmitter(ParticleEmitter NewEmitter)
        {
            Emitter = NewEmitter;

            if (Emitter != null)
            {
                LblEmitterName.Text = "Selected Emitter: " + Emitter.Name;

                LstEmitterTextures.DataSource = null;
                LstEmitterTextures.DataSource = Emitter.ParticleTextures;

                NudStartAlpha.Value = Emitter.StartColor.A;
                NudEndAlpha.Value = Emitter.EndColor.A;
                PnlStartColor.BackColor = SysColor.FromArgb(Emitter.StartColor.R, Emitter.StartColor.G, Emitter.StartColor.B);
                PnlEndColor.BackColor = SysColor.FromArgb(Emitter.EndColor.R, Emitter.EndColor.G, Emitter.EndColor.B);
                CmbSourceBlends.SelectedIndex = (int)Emitter.SourceBlend;
                CmbDestBlends.SelectedIndex = (int)Emitter.DestinationBlend;
                NudStartRotation.Value = (decimal)Emitter.StartRotation;
                NudEndRotation.Value = (decimal)Emitter.EndRotation;
                NudStartScale.Value = (decimal)Emitter.StartScale;
                NudEndScale.Value = (decimal)Emitter.EndScale;
                NudNumToSpit.Value = Emitter.NumParticlesToSpit;
                NudLifeDuration.Value = (decimal)Emitter.LifeSpan;
                NudMaxParticles.Value = Emitter.MaxParticles;
                NudStartVelX.Value = (decimal)Emitter.StartVelocity.X;
                NudStartVelY.Value = (decimal)Emitter.StartVelocity.Y;
                NudEndVelX.Value = (decimal)Emitter.EndVelocity.X;
                NudEndVelY.Value = (decimal)Emitter.EndVelocity.Y;
                NudSpawnRate.Value = (decimal)Emitter.SpawnRate;
                NudRangeX.Value = (decimal)Emitter.SpawnRange.X;
                NudRangeY.Value = (decimal)Emitter.SpawnRange.Y;
                NudRangeWidth.Value = (decimal)Emitter.SpawnRange.Width;
                NudRangeHeight.Value = (decimal)Emitter.SpawnRange.Height;
                NudGravityX.Value = (decimal)Emitter.Gravity.X;
                NudGravityY.Value = (decimal)Emitter.Gravity.Y;
                NudDuration.Value = (decimal)Emitter.ParticleDuration;
                NudMaxParticleDuration.Value = (decimal)Emitter.ParticleDurationMax;
                NudGravitationalPull.Value = (decimal)Emitter.GravitationalPull;
                ChkRandVelocityX.Checked = Emitter.RandomStartVelocityX;
                ChkRandVelocityY.Checked = Emitter.RandomStartVelocityY;
                ChkRandomEndVelX.Checked = Emitter.RandomEndVelocityX;
                ChkRandomEndVelY.Checked = Emitter.RandomEndVelocityY;
            }
            else
            {
                LblEmitterName.Text = "Selected Emitter: None";

                LstEmitterTextures.DataSource = null;

                NudStartAlpha.Value = 255;
                NudEndAlpha.Value = 0;
                PnlStartColor.BackColor = SysColor.White;
                PnlEndColor.BackColor = SysColor.Black;
                CmbSourceBlends.SelectedIndex = (int)Blend.SourceAlpha;
                CmbDestBlends.SelectedIndex = (int)Blend.One;
                NudStartRotation.Value = 0;
                NudEndRotation.Value = 0;
                NudStartScale.Value = 1;
                NudEndScale.Value = 0;
                NudNumToSpit.Value = 50;
                NudLifeDuration.Value = 5;
                NudMaxParticles.Value = 100;
                NudStartVelX.Value = 0;
                NudStartVelY.Value = 0;
                NudEndVelX.Value = 0;
                NudEndVelY.Value = 0;
                NudSpawnRate.Value = (decimal)0.5f;
                NudRangeX.Value = 248;
                NudRangeY.Value = 248;
                NudRangeWidth.Value = 32;
                NudRangeHeight.Value = 32;
                NudGravityX.Value = 256;
                NudGravityY.Value = 256;
                NudDuration.Value = (decimal)0.5f;
                NudGravitationalPull.Value = 0;
                ChkRandVelocityX.Checked = false;
                ChkRandVelocityY.Checked = false;
                ChkRandomEndVelX.Checked = false;
                ChkRandomEndVelY.Checked = false;
            }
        }
        public void EmitterMouseUp()
        {
            if (Emitter != null)
            {
                NudRangeX.Value = (decimal)Emitter.SpawnRange.X;
                NudRangeY.Value = (decimal)Emitter.SpawnRange.Y;
                NudGravityX.Value = (decimal)Emitter.Gravity.X;
                NudGravityY.Value = (decimal)Emitter.Gravity.Y;
                NudGravitationalPull.Value = (decimal)Emitter.GravitationalPull;
            }
        }

        void Randomize()
        {
            Random Rand = new Random();

            NudStartAlpha.Value = Rand.Next(0, 255);
            NudEndAlpha.Value = Rand.Next(0, 255);

            SysColor RandStart = SysColor.FromArgb(Rand.Next(0, 255), Rand.Next(0, 255), Rand.Next(0, 255));
            SysColor RandEnd = SysColor.FromArgb(Rand.Next(0, 255), Rand.Next(0, 255), Rand.Next(0, 255));

            PnlStartColor.BackColor = RandStart;
            PnlEndColor.BackColor = RandEnd;

            if (Emitter != null)
            {
                Emitter.StartColor = new Color(RandStart.R, RandStart.G, RandStart.B, (int)NudStartAlpha.Value);
                Emitter.EndColor = new Color(RandEnd.R, RandEnd.G, RandEnd.B, (int)NudEndAlpha.Value);
            }

            NudStartRotation.Value = Rand.Next(0, 6);
            NudEndRotation.Value = Rand.Next(0, 6);
            NudStartScale.Value = Rand.Next(0, 5);
            NudEndScale.Value = Rand.Next(0, 5);
            NudNumToSpit.Value = Rand.Next(1, 25);
            NudLifeDuration.Value = Rand.Next(0, 10);
            NudMaxParticles.Value = Rand.Next(50, 500);
            NudStartVelX.Value = Rand.Next(-200, 200);
            NudStartVelY.Value = Rand.Next(-200, 200);
            NudEndVelX.Value = Rand.Next(-200, 200);
            NudEndVelY.Value = Rand.Next(-200, 200);
            NudSpawnRate.Value = (decimal)Rand.NextDouble();
            NudRangeWidth.Value = Rand.Next(1, 200);
            NudRangeHeight.Value = Rand.Next(1, 200);
            NudRangeX.Value = 256 - (NudRangeWidth.Value / 2);
            NudRangeY.Value = 256 - (NudRangeHeight.Value / 2);
            NudGravityX.Value = (decimal)(Rand.Next(0, 512));
            NudGravityY.Value = (decimal)(Rand.Next(0, 512));
            NudDuration.Value = (decimal)Rand.NextDouble();
            NudMaxParticleDuration.Value = NudDuration.Value + (decimal)Rand.NextDouble() + (decimal)Rand.Next(0, 2);
            NudGravitationalPull.Value = (decimal)((float)Rand.Next(0, 1000) + (float)Rand.NextDouble());

            int RndStartVel = Rand.Next(0, 2);

            if (RndStartVel == 0)
                ChkRandVelocityX.Checked = false;
            else
                ChkRandVelocityX.Checked = true;

            RndStartVel = Rand.Next(0, 2);

            if (RndStartVel == 0)
                ChkRandVelocityY.Checked = false;
            else
                ChkRandVelocityY.Checked = true;

            RndStartVel = Rand.Next(0, 2);

            if (RndStartVel == 0)
                ChkRandomEndVelX.Checked = false;
            else
                ChkRandomEndVelX.Checked = true;

            RndStartVel = Rand.Next(0, 2);

            if (RndStartVel == 0)
                ChkRandomEndVelY.Checked = false;
            else
                ChkRandomEndVelY.Checked = true;

            if (Emitter != null)
            {
                LblEmitterName.Text = "Selected Emitter: " + Emitter.Name;

                LstEmitterTextures.DataSource = null;
                LstEmitterTextures.DataSource = Emitter.ParticleTextures;

                NudStartAlpha.Value = Emitter.StartColor.A;
                NudEndAlpha.Value = Emitter.EndColor.A;
                PnlStartColor.BackColor = SysColor.FromArgb(Emitter.StartColor.R, Emitter.StartColor.G, Emitter.StartColor.B);
                PnlEndColor.BackColor = SysColor.FromArgb(Emitter.EndColor.R, Emitter.EndColor.G, Emitter.EndColor.B);
                CmbSourceBlends.SelectedIndex = (int)Emitter.SourceBlend;
                CmbDestBlends.SelectedIndex = (int)Emitter.DestinationBlend;
                NudStartRotation.Value = (decimal)Emitter.StartRotation;
                NudEndRotation.Value = (decimal)Emitter.EndRotation;
                NudStartScale.Value = (decimal)Emitter.StartScale;
                NudEndScale.Value = (decimal)Emitter.EndScale;
                NudNumToSpit.Value = Emitter.NumParticlesToSpit;
                NudLifeDuration.Value = (decimal)Emitter.LifeSpan;
                NudMaxParticles.Value = Emitter.MaxParticles;
                NudStartVelX.Value = (decimal)Emitter.StartVelocity.X;
                NudStartVelY.Value = (decimal)Emitter.StartVelocity.Y;
                NudEndVelX.Value = (decimal)Emitter.EndVelocity.X;
                NudEndVelY.Value = (decimal)Emitter.EndVelocity.Y;
                NudSpawnRate.Value = (decimal)Emitter.SpawnRate;
                NudRangeX.Value = (decimal)Emitter.SpawnRange.X;
                NudRangeY.Value = (decimal)Emitter.SpawnRange.Y;
                NudRangeWidth.Value = (decimal)Emitter.SpawnRange.Width;
                NudRangeHeight.Value = (decimal)Emitter.SpawnRange.Height;
                NudGravityX.Value = (decimal)Emitter.Gravity.X;
                NudGravityY.Value = (decimal)Emitter.Gravity.Y;
                NudDuration.Value = (decimal)Emitter.ParticleDuration;
                NudMaxParticleDuration.Value = (decimal)Emitter.ParticleDurationMax;
                NudGravitationalPull.Value = (decimal)Emitter.GravitationalPull;
                ChkRandVelocityX.Checked = Emitter.RandomStartVelocityX;
                ChkRandVelocityY.Checked = Emitter.RandomStartVelocityY;
                ChkRandomEndVelX.Checked = Emitter.RandomEndVelocityX;
                ChkRandomEndVelY.Checked = Emitter.RandomEndVelocityY;
            }
        }

        // Paint the Selected Particle
        private void PnlParticle_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.Clear(SysColor.Black);

            if (ParticleImage != null)
                e.Graphics.DrawImage(ParticleImage, DestRect);
        }

        // Select Different Particle for viewing
        private void LstWorkDirFiles_SelectedIndexChanged(object sender, EventArgs e)
        {
            string ImageFile = RelativePath + '\\' + LstWorkDirFiles.Items[LstWorkDirFiles.SelectedIndex];
            ParticleImage = (Bitmap)Image.FromFile(ImageFile);

            int WidthOff = 0;
            int HeightOff = 0;
            int ImgWidth = ParticleImage.Width;
            int ImgHeight = ParticleImage.Height;

            if (ParticleImage.Width > PnlParticle.Width)
                WidthOff = ParticleImage.Width - PnlParticle.Width;

            if (ParticleImage.Height > PnlParticle.Height)
                HeightOff = ParticleImage.Height - PnlParticle.Height;

            if (WidthOff > 0 || HeightOff > 0)
            {
                float Ratio = 0.0f;

                if (WidthOff > HeightOff)
                    Ratio = ((float)PnlParticle.Width) / ((float)ParticleImage.Width);
                else
                    Ratio = ((float)PnlParticle.Height) / ((float)ParticleImage.Height);

                if (Ratio > 0.0f)
                {
                    ImgWidth = (int)(Ratio * (float)ParticleImage.Width);
                    ImgHeight = (int)(Ratio * (float)ParticleImage.Height);
                }
            }

            DestRect = new SysRect(0, 0, ImgWidth, ImgHeight);

            PnlParticle.Invalidate();
        }

        // Control Events
        private void BtnChangeWorkDir_Click(object sender, EventArgs e)
        {
            FindValidWorkDirectory();
            InitializeWorkDirectory();
        }
        private void PnlParticle_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (ParticleImage != null)
            {
                string ImageFile = RelativePath + '\\' + LstWorkDirFiles.Items[LstWorkDirFiles.SelectedIndex];

                Process p = new Process();

                p.StartInfo.FileName = "rundll32.exe";
                p.StartInfo.Arguments = @"C:\WINDOWS\System32\shimgvw.dll,ImageView_Fullscreen " + ImageFile;
                p.Start();
            }
        }
        private void PnlParticle_MouseEnter(object sender, EventArgs e)
        {
            LblViewImage.Visible = true;
        }
        private void PnlParticle_MouseLeave(object sender, EventArgs e)
        {
            LblViewImage.Visible = false;
        }
        private void BtnStartColor_Click(object sender, EventArgs e)
        {
            ColorDialog CD = new ColorDialog();

            CD.CustomColors = CustomColors;

            if(CD.ShowDialog() == DialogResult.OK)
                PnlStartColor.BackColor = CD.Color;

            CustomColors = (int[])CD.CustomColors.Clone();

            CD.Dispose();
            CD = null;
            GC.Collect();

            if (Emitter != null)
            {
                Emitter.StartColor = new Color(PnlStartColor.BackColor.R, PnlStartColor.BackColor.G, PnlStartColor.BackColor.B, (int)NudStartAlpha.Value);
            }
        }
        private void BtnEndColor_Click(object sender, EventArgs e)
        {
            ColorDialog CD = new ColorDialog();

            CD.CustomColors = CustomColors;

            if (CD.ShowDialog() == DialogResult.OK)
                PnlEndColor.BackColor = CD.Color;

            CustomColors = (int[])CD.CustomColors.Clone();

            CD.Dispose();
            CD = null;
            GC.Collect();

            if(Emitter != null)
                Emitter.EndColor = new Color(PnlEndColor.BackColor.R, PnlEndColor.BackColor.G, PnlEndColor.BackColor.B, (int)NudEndAlpha.Value);
        }
        private void BtnImageToEmitter_Click(object sender, EventArgs e)
        {
            int ImgIndex = LstWorkDirFiles.SelectedIndex;

            if (Emitter != null)
            {
                if (ImgIndex > -1 && ImgIndex < LstWorkDirFiles.Items.Count)
                {
                    string ImageFile = RelativePath + '\\' + LstWorkDirFiles.Items[ImgIndex];
                    Emitter.ParticleTextures.Add(new ParticleTexture(ImageFile, ((FrmMain)Owner).Device));
                    LstEmitterTextures.DataSource = null;
                    LstEmitterTextures.DataSource = Emitter.ParticleTextures;
                }
            }
        }
        private void CmbSourceBlends_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
            {
                int Index = CmbSourceBlends.SelectedIndex;

                if (Index > -1 && Index < SourceBlends.Count)
                    Emitter.SourceBlend = SourceBlends[Index];
            }
        }
        private void CmbDestBlends_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
            {
                int Index = CmbDestBlends.SelectedIndex;

                if (Index > -1 && Index < SourceBlends.Count)
                    Emitter.DestinationBlend = DestinationBlends[Index];
            }
        }
        private void NudStartScale_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.StartScale = (float)NudStartScale.Value;
        }
        private void NudEndScale_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.EndScale = (float)NudEndScale.Value;
        }
        private void NudMaxParticles_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
            {
                Emitter.MaxParticles = (short)NudMaxParticles.Value;
                Emitter.AliveParticles.Clear();
                Emitter.DeadParticles.Clear();

                for (int i = 0; i < Emitter.MaxParticles; ++i)
                    Emitter.DeadParticles.Add(new Particle());
            }
        }
        private void NudNumToSpit_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.NumParticlesToSpit = (short)NudNumToSpit.Value;
        }
        private void NudStartRotation_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.StartRotation = MathHelper.ToRadians(((float)NudStartRotation.Value));
        }
        private void NudEndRotation_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.EndRotation = MathHelper.ToRadians((float)NudEndRotation.Value);
        }
        private void NudStartVelX_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.StartVelocity.X = (float)NudStartVelX.Value;
        }
        private void NudStartVelY_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.StartVelocity.Y = (float)NudStartVelY.Value;
        }
        private void NudEndVelX_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.EndVelocity.X = (float)NudEndVelX.Value;
        }
        private void NudEndVelY_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.EndVelocity.Y = (float)NudEndVelY.Value;
        }
        private void NudSpawnRate_ValueChanged(object sender, EventArgs e)
        {
            if(Emitter != null)
                Emitter.SpawnRate = (float)NudSpawnRate.Value;
        }
        private void NudRangeX_ValueChanged(object sender, EventArgs e)
        {
            if(Emitter != null)
                Emitter.SpawnRange.X = (int)NudRangeX.Value;
        }
        private void NudRangeY_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.SpawnRange.Y = (int)NudRangeY.Value;
        }
        private void NudRangeWidth_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.SpawnRange.Width = (int)NudRangeWidth.Value;
        }
        private void NudRangeHeight_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.SpawnRange.Height = (int)NudRangeHeight.Value;
        }
        private void NudGravityX_ValueChanged(object sender, EventArgs e)
        {
            if(Emitter != null)
                Emitter.Gravity.X = (float)NudGravityX.Value;
        }
        private void NudGravityY_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.Gravity.Y = (float)NudGravityY.Value;
        }
        private void NudDuration_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
            {
                if (Emitter.ParticleDurationMax < (float)NudDuration.Value)
                    Emitter.ParticleDurationMax = (float)NudDuration.Value;

                Emitter.ParticleDuration = (float)NudDuration.Value;
            }

            NudMaxParticleDuration.Minimum = NudDuration.Value;
        }
        private void BtnRandomize_Click(object sender, EventArgs e)
        {
            Randomize();
        }
        private void BtnRemoveTexture_Click(object sender, EventArgs e)
        {
            int Index = LstEmitterTextures.SelectedIndex;

            if (Index > -1 && Index < Emitter.ParticleTextures.Count)
            {
                Emitter.ParticleTextures.RemoveAt(Index);
                LstEmitterTextures.DataSource = null;
                LstEmitterTextures.DataSource = Emitter.ParticleTextures;
            }
        }
        private void NudGravitationalPull_ValueChanged(object sender, EventArgs e)
        {
            if(Emitter != null)
                Emitter.GravitationalPull = (float)NudGravitationalPull.Value;
        }
        private void ChkRandVelocityX_CheckedChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.RandomStartVelocityX = ChkRandVelocityX.Checked;
        }
        private void ChkRandVelocityY_CheckedChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.RandomStartVelocityY = ChkRandVelocityY.Checked;
        }
        private void ChkRandomEndVelX_CheckedChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
            {
                Random Rand = new Random();
                Emitter.EndVelocity.X = (float)Rand.Next(-300, 300) + (float)Rand.NextDouble();
            }
        }
        private void ChkRandomEndVelY_CheckedChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
            {
                Random Rand = new Random();
                Emitter.EndVelocity.X = (float)Rand.Next(-300, 300) + (float)Rand.NextDouble();
            }
        }

        // Random Tools
        private void TsRandMaxParticles_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudMaxParticles.Value = Rand.Next((int)NudMaxParticles.Minimum, (int)NudMaxParticles.Maximum);

            if(Emitter != null)
                Emitter.MaxParticles = (short)NudMaxParticles.Value;
        }
        private void TsRandLifeDuration_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudLifeDuration.Value = Rand.Next((int)NudLifeDuration.Minimum, (int)NudLifeDuration.Maximum);

            if (Emitter != null)
                Emitter.LifeSpan = (short)NudLifeDuration.Value;
        }
        private void TsRandNumToSpit_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudNumToSpit.Value = Rand.Next((int)NudNumToSpit.Minimum, (int)NudNumToSpit.Maximum);

            if (Emitter != null)
                Emitter.NumParticlesToSpit = (short)NudNumToSpit.Value;
        }
        private void TsRandSpawnRate_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudSpawnRate.Value = Rand.Next((int)NudSpawnRate.Minimum, (int)NudSpawnRate.Maximum);

            if (Emitter != null)
                Emitter.SpawnRate = (short)NudSpawnRate.Value;
        }
        private void TsRandParticleDurationMin_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudDuration.Value = Rand.Next((int)NudDuration.Minimum, (int)NudDuration.Maximum);

            if (Emitter != null)
                Emitter.ParticleDuration = (short)NudDuration.Value;
        }
        private void TsRandParticleDurationMax_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudMaxParticleDuration.Value = Rand.Next((int)NudMaxParticleDuration.Minimum, (int)NudMaxParticleDuration.Maximum);

            if (Emitter != null)
                Emitter.ParticleDurationMax = (short)NudMaxParticleDuration.Value;
        }
        private void TsRandStartColor_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            SysColor RandStart = SysColor.FromArgb(Rand.Next(0, 255), Rand.Next(0, 255), Rand.Next(0, 255));

            PnlStartColor.BackColor = RandStart;

            if (Emitter != null)
                Emitter.StartColor = new Color(RandStart.R, RandStart.G, RandStart.B, (int)NudStartAlpha.Value);
        }
        private void TsRandEndColor_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            SysColor RandEnd = SysColor.FromArgb(Rand.Next(0, 255), Rand.Next(0, 255), Rand.Next(0, 255));

            PnlEndColor.BackColor = RandEnd;

            if (Emitter != null)
                Emitter.EndColor = new Color(RandEnd.R, RandEnd.G, RandEnd.B, (int)NudEndAlpha.Value);
        }
        private void TsRandStartAlpha_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudStartAlpha.Value = Rand.Next((int)NudStartAlpha.Minimum, (int)NudStartAlpha.Maximum);

            if (Emitter != null)
                Emitter.StartColor.A = (byte)NudStartAlpha.Value;
        }
        private void TsRandEndAlpha_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudEndAlpha.Value = Rand.Next((int)NudEndAlpha.Minimum, (int)NudEndAlpha.Maximum);

            if (Emitter != null)
                Emitter.EndColor.A = (byte)NudEndAlpha.Value;
        }
        private void TsRandStartScale_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudStartScale.Value = Rand.Next(0, 10);

            if (Emitter != null)
                Emitter.StartScale = (float)NudStartScale.Value;
        }
        private void TsRandEndScale_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudEndScale.Value = Rand.Next(0, 10);

            if (Emitter != null)
                Emitter.EndScale = (float)NudEndScale.Value;
        }
        private void TsRandStartRotation_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudStartRotation.Value = Rand.Next((int)NudStartRotation.Minimum, (int)NudStartRotation.Maximum);

            if (Emitter != null)
                Emitter.StartRotation = (float)NudStartRotation.Value;
        }
        private void TsRandEndRotation_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudEndRotation.Value = Rand.Next((int)NudEndRotation.Minimum, (int)NudEndRotation.Maximum);

            if (Emitter != null)
                Emitter.EndRotation = (float)NudEndRotation.Value;
        }
        private void TsRandSpawnRange_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudRangeX.Value = Rand.Next(0, 400);
            NudRangeY.Value = Rand.Next(0, 400);
            NudRangeWidth.Value = Rand.Next(0, 512);
            NudRangeHeight.Value = Rand.Next(0, 512);

            if (Emitter != null)
                Emitter.SpawnRange = new Rectangle((int)NudRangeX.Value, (int)NudRangeY.Value, (int)NudRangeWidth.Value, (int)NudRangeHeight.Value);
        }
        private void TsRandGravityWell_Click(object sender, EventArgs e)
        {
            Random Rand = new Random();
            NudGravityX.Value = Rand.Next(0, 512);
            NudGravityY.Value = Rand.Next(0, 512);
            NudGravitationalPull.Value = (decimal)Rand.Next((int)NudGravitationalPull.Minimum, (int)NudGravitationalPull.Maximum - 1) + (decimal)Rand.NextDouble();

            if (Emitter != null)
            {
                Emitter.Gravity = new Vector2((float)NudGravityX.Value, (float)NudGravityY.Value);
                Emitter.GravitationalPull = (float)NudGravitationalPull.Value;
            }
        }
        private void NudMaxParticleDuration_ValueChanged(object sender, EventArgs e)
        {
            if(Emitter != null)
                Emitter.ParticleDurationMax = (float)NudMaxParticleDuration.Value;
        }

        private void NudLifeDuration_ValueChanged(object sender, EventArgs e)
        {
            if (Emitter != null)
                Emitter.LifeSpan = (float)NudLifeDuration.Value;
        }
    }
}