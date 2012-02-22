namespace ParticleFX
{
    partial class FrmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmMain));
            this.MnuMain = new System.Windows.Forms.MenuStrip();
            this.MnuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.MnuOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.MnuSave = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.MnuExit = new System.Windows.Forms.ToolStripMenuItem();
            this.MnuOptions = new System.Windows.Forms.ToolStripMenuItem();
            this.MnuTogglePlaying = new System.Windows.Forms.ToolStripMenuItem();
            this.PnlBorder = new System.Windows.Forms.Panel();
            this.PnlSurface = new ParticleFX.BufferedPanel();
            this.StatusMain = new System.Windows.Forms.StatusStrip();
            this.LblFrames = new System.Windows.Forms.ToolStripStatusLabel();
            this.LblParticleCount = new System.Windows.Forms.ToolStripStatusLabel();
            this.LstEmitters = new System.Windows.Forms.ListBox();
            this.BtnNewEmitter = new System.Windows.Forms.Button();
            this.TxtEmitterName = new System.Windows.Forms.TextBox();
            this.BtnRemoveSelected = new System.Windows.Forms.Button();
            this.BtnUpdateSelected = new System.Windows.Forms.Button();
            this.BtnUp = new System.Windows.Forms.Button();
            this.BtnDown = new System.Windows.Forms.Button();
            this.BtnPlay = new System.Windows.Forms.Button();
            this.BtnStop = new System.Windows.Forms.Button();
            this.BtnScreenShot = new System.Windows.Forms.Button();
            this.ChkRepeat = new System.Windows.Forms.CheckBox();
            this.MnuMain.SuspendLayout();
            this.PnlBorder.SuspendLayout();
            this.StatusMain.SuspendLayout();
            this.SuspendLayout();
            // 
            // MnuMain
            // 
            this.MnuMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MnuFile,
            this.MnuOptions});
            this.MnuMain.Location = new System.Drawing.Point(0, 0);
            this.MnuMain.Name = "MnuMain";
            this.MnuMain.Size = new System.Drawing.Size(759, 24);
            this.MnuMain.TabIndex = 0;
            this.MnuMain.Text = "Main menu";
            // 
            // MnuFile
            // 
            this.MnuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MnuOpen,
            this.MnuSave,
            this.toolStripSeparator1,
            this.MnuExit});
            this.MnuFile.Name = "MnuFile";
            this.MnuFile.Size = new System.Drawing.Size(37, 20);
            this.MnuFile.Text = "&File";
            // 
            // MnuOpen
            // 
            this.MnuOpen.Image = ((System.Drawing.Image)(resources.GetObject("MnuOpen.Image")));
            this.MnuOpen.Name = "MnuOpen";
            this.MnuOpen.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.MnuOpen.Size = new System.Drawing.Size(221, 22);
            this.MnuOpen.Text = "Open Particle Effect";
            this.MnuOpen.Click += new System.EventHandler(this.MnuOpen_Click);
            // 
            // MnuSave
            // 
            this.MnuSave.Image = ((System.Drawing.Image)(resources.GetObject("MnuSave.Image")));
            this.MnuSave.Name = "MnuSave";
            this.MnuSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.MnuSave.Size = new System.Drawing.Size(221, 22);
            this.MnuSave.Text = "Save Particle Effect";
            this.MnuSave.Click += new System.EventHandler(this.MnuSave_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(218, 6);
            // 
            // MnuExit
            // 
            this.MnuExit.Image = ((System.Drawing.Image)(resources.GetObject("MnuExit.Image")));
            this.MnuExit.Name = "MnuExit";
            this.MnuExit.Size = new System.Drawing.Size(221, 22);
            this.MnuExit.Text = "Exit Program";
            // 
            // MnuOptions
            // 
            this.MnuOptions.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MnuTogglePlaying});
            this.MnuOptions.Name = "MnuOptions";
            this.MnuOptions.Size = new System.Drawing.Size(61, 20);
            this.MnuOptions.Text = "&Options";
            // 
            // MnuTogglePlaying
            // 
            this.MnuTogglePlaying.Name = "MnuTogglePlaying";
            this.MnuTogglePlaying.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
            this.MnuTogglePlaying.Size = new System.Drawing.Size(194, 22);
            this.MnuTogglePlaying.Text = "Toggle Playing";
            this.MnuTogglePlaying.Click += new System.EventHandler(this.MnuTogglePlaying_Click);
            // 
            // PnlBorder
            // 
            this.PnlBorder.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PnlBorder.Controls.Add(this.PnlSurface);
            this.PnlBorder.Location = new System.Drawing.Point(12, 27);
            this.PnlBorder.Name = "PnlBorder";
            this.PnlBorder.Size = new System.Drawing.Size(520, 520);
            this.PnlBorder.TabIndex = 1;
            // 
            // PnlSurface
            // 
            this.PnlSurface.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PnlSurface.Location = new System.Drawing.Point(3, 3);
            this.PnlSurface.Name = "PnlSurface";
            this.PnlSurface.Size = new System.Drawing.Size(512, 512);
            this.PnlSurface.TabIndex = 0;
            this.PnlSurface.MouseDown += new System.Windows.Forms.MouseEventHandler(this.PnlSurface_MouseDown);
            this.PnlSurface.MouseEnter += new System.EventHandler(this.PnlSurface_MouseEnter);
            this.PnlSurface.MouseLeave += new System.EventHandler(this.PnlSurface_MouseLeave);
            this.PnlSurface.MouseMove += new System.Windows.Forms.MouseEventHandler(this.PnlSurface_MouseMove);
            this.PnlSurface.MouseUp += new System.Windows.Forms.MouseEventHandler(this.PnlSurface_MouseUp);
            // 
            // StatusMain
            // 
            this.StatusMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.LblFrames,
            this.LblParticleCount});
            this.StatusMain.Location = new System.Drawing.Point(0, 562);
            this.StatusMain.Name = "StatusMain";
            this.StatusMain.Size = new System.Drawing.Size(759, 22);
            this.StatusMain.TabIndex = 2;
            this.StatusMain.Text = "statusStrip1";
            // 
            // LblFrames
            // 
            this.LblFrames.Name = "LblFrames";
            this.LblFrames.Size = new System.Drawing.Size(120, 17);
            this.LblFrames.Text = "Frames: Calculating...";
            // 
            // LblParticleCount
            // 
            this.LblParticleCount.Name = "LblParticleCount";
            this.LblParticleCount.Size = new System.Drawing.Size(88, 17);
            this.LblParticleCount.Text = "Particle Count: ";
            // 
            // LstEmitters
            // 
            this.LstEmitters.FormattingEnabled = true;
            this.LstEmitters.Location = new System.Drawing.Point(539, 28);
            this.LstEmitters.Name = "LstEmitters";
            this.LstEmitters.Size = new System.Drawing.Size(150, 95);
            this.LstEmitters.TabIndex = 3;
            this.LstEmitters.SelectedIndexChanged += new System.EventHandler(this.LstEmitters_SelectedIndexChanged);
            // 
            // BtnNewEmitter
            // 
            this.BtnNewEmitter.Location = new System.Drawing.Point(539, 155);
            this.BtnNewEmitter.Name = "BtnNewEmitter";
            this.BtnNewEmitter.Size = new System.Drawing.Size(208, 23);
            this.BtnNewEmitter.TabIndex = 4;
            this.BtnNewEmitter.Text = "Add New Emitter";
            this.BtnNewEmitter.UseVisualStyleBackColor = true;
            this.BtnNewEmitter.Click += new System.EventHandler(this.BtnNewEmitter_Click);
            // 
            // TxtEmitterName
            // 
            this.TxtEmitterName.Location = new System.Drawing.Point(539, 129);
            this.TxtEmitterName.MaxLength = 16;
            this.TxtEmitterName.Name = "TxtEmitterName";
            this.TxtEmitterName.Size = new System.Drawing.Size(208, 20);
            this.TxtEmitterName.TabIndex = 5;
            this.TxtEmitterName.Text = "Emitter name";
            // 
            // BtnRemoveSelected
            // 
            this.BtnRemoveSelected.Location = new System.Drawing.Point(539, 184);
            this.BtnRemoveSelected.Name = "BtnRemoveSelected";
            this.BtnRemoveSelected.Size = new System.Drawing.Size(208, 23);
            this.BtnRemoveSelected.TabIndex = 6;
            this.BtnRemoveSelected.Text = "Remove Selected";
            this.BtnRemoveSelected.UseVisualStyleBackColor = true;
            this.BtnRemoveSelected.Click += new System.EventHandler(this.BtnRemoveSelected_Click);
            // 
            // BtnUpdateSelected
            // 
            this.BtnUpdateSelected.Location = new System.Drawing.Point(539, 213);
            this.BtnUpdateSelected.Name = "BtnUpdateSelected";
            this.BtnUpdateSelected.Size = new System.Drawing.Size(208, 23);
            this.BtnUpdateSelected.TabIndex = 7;
            this.BtnUpdateSelected.Text = "Update Name";
            this.BtnUpdateSelected.UseVisualStyleBackColor = true;
            this.BtnUpdateSelected.Click += new System.EventHandler(this.BtnUpdateSelected_Click);
            // 
            // BtnUp
            // 
            this.BtnUp.Image = ((System.Drawing.Image)(resources.GetObject("BtnUp.Image")));
            this.BtnUp.Location = new System.Drawing.Point(695, 27);
            this.BtnUp.Name = "BtnUp";
            this.BtnUp.Size = new System.Drawing.Size(52, 47);
            this.BtnUp.TabIndex = 8;
            this.BtnUp.UseVisualStyleBackColor = true;
            this.BtnUp.Click += new System.EventHandler(this.BtnUp_Click);
            // 
            // BtnDown
            // 
            this.BtnDown.Image = ((System.Drawing.Image)(resources.GetObject("BtnDown.Image")));
            this.BtnDown.Location = new System.Drawing.Point(695, 76);
            this.BtnDown.Name = "BtnDown";
            this.BtnDown.Size = new System.Drawing.Size(52, 47);
            this.BtnDown.TabIndex = 9;
            this.BtnDown.UseVisualStyleBackColor = true;
            this.BtnDown.Click += new System.EventHandler(this.BtnDown_Click);
            // 
            // BtnPlay
            // 
            this.BtnPlay.Image = ((System.Drawing.Image)(resources.GetObject("BtnPlay.Image")));
            this.BtnPlay.Location = new System.Drawing.Point(539, 304);
            this.BtnPlay.Name = "BtnPlay";
            this.BtnPlay.Size = new System.Drawing.Size(208, 78);
            this.BtnPlay.TabIndex = 10;
            this.BtnPlay.UseVisualStyleBackColor = true;
            this.BtnPlay.Click += new System.EventHandler(this.BtnPlay_Click);
            // 
            // BtnStop
            // 
            this.BtnStop.Image = ((System.Drawing.Image)(resources.GetObject("BtnStop.Image")));
            this.BtnStop.Location = new System.Drawing.Point(539, 388);
            this.BtnStop.Name = "BtnStop";
            this.BtnStop.Size = new System.Drawing.Size(208, 78);
            this.BtnStop.TabIndex = 11;
            this.BtnStop.UseVisualStyleBackColor = true;
            this.BtnStop.Click += new System.EventHandler(this.BtnStop_Click);
            // 
            // BtnScreenShot
            // 
            this.BtnScreenShot.Image = ((System.Drawing.Image)(resources.GetObject("BtnScreenShot.Image")));
            this.BtnScreenShot.Location = new System.Drawing.Point(539, 472);
            this.BtnScreenShot.Name = "BtnScreenShot";
            this.BtnScreenShot.Size = new System.Drawing.Size(208, 78);
            this.BtnScreenShot.TabIndex = 12;
            this.BtnScreenShot.UseVisualStyleBackColor = true;
            this.BtnScreenShot.Click += new System.EventHandler(this.BtnScreenShot_Click);
            // 
            // ChkRepeat
            // 
            this.ChkRepeat.AutoSize = true;
            this.ChkRepeat.Location = new System.Drawing.Point(539, 243);
            this.ChkRepeat.Name = "ChkRepeat";
            this.ChkRepeat.Size = new System.Drawing.Size(67, 17);
            this.ChkRepeat.TabIndex = 13;
            this.ChkRepeat.Text = "Repeat?";
            this.ChkRepeat.UseVisualStyleBackColor = true;
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(759, 584);
            this.Controls.Add(this.ChkRepeat);
            this.Controls.Add(this.BtnScreenShot);
            this.Controls.Add(this.BtnStop);
            this.Controls.Add(this.BtnPlay);
            this.Controls.Add(this.BtnDown);
            this.Controls.Add(this.BtnUp);
            this.Controls.Add(this.BtnUpdateSelected);
            this.Controls.Add(this.BtnRemoveSelected);
            this.Controls.Add(this.TxtEmitterName);
            this.Controls.Add(this.BtnNewEmitter);
            this.Controls.Add(this.LstEmitters);
            this.Controls.Add(this.StatusMain);
            this.Controls.Add(this.PnlBorder);
            this.Controls.Add(this.MnuMain);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MainMenuStrip = this.MnuMain;
            this.MaximizeBox = false;
            this.Name = "FrmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "ParticleFX";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FrmMain_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FrmMain_FormClosed);
            this.MnuMain.ResumeLayout(false);
            this.MnuMain.PerformLayout();
            this.PnlBorder.ResumeLayout(false);
            this.StatusMain.ResumeLayout(false);
            this.StatusMain.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip MnuMain;
        private System.Windows.Forms.Panel PnlBorder;
        private BufferedPanel PnlSurface;
        private System.Windows.Forms.StatusStrip StatusMain;
        private System.Windows.Forms.ToolStripStatusLabel LblFrames;
        private System.Windows.Forms.ToolStripMenuItem MnuFile;
        private System.Windows.Forms.ToolStripMenuItem MnuOpen;
        private System.Windows.Forms.ToolStripMenuItem MnuSave;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem MnuExit;
        private System.Windows.Forms.ToolStripStatusLabel LblParticleCount;
        private System.Windows.Forms.ListBox LstEmitters;
        private System.Windows.Forms.Button BtnNewEmitter;
        private System.Windows.Forms.TextBox TxtEmitterName;
        private System.Windows.Forms.Button BtnRemoveSelected;
        private System.Windows.Forms.Button BtnUpdateSelected;
        private System.Windows.Forms.Button BtnUp;
        private System.Windows.Forms.Button BtnDown;
        private System.Windows.Forms.Button BtnPlay;
        private System.Windows.Forms.Button BtnStop;
        private System.Windows.Forms.ToolStripMenuItem MnuOptions;
        private System.Windows.Forms.ToolStripMenuItem MnuTogglePlaying;
        private System.Windows.Forms.Button BtnScreenShot;
        private System.Windows.Forms.CheckBox ChkRepeat;
    }
}

