namespace BeatMaker
{
    partial class BeatMaker
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(BeatMaker));
            this.MainMenuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewHelpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainStatusStrip = new System.Windows.Forms.StatusStrip();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.IconGroup = new System.Windows.Forms.GroupBox();
            this.DKeyLabel = new System.Windows.Forms.Label();
            this.DKeyPictureBox = new System.Windows.Forms.PictureBox();
            this.SKeyLabel = new System.Windows.Forms.Label();
            this.SKeyPictureBox = new System.Windows.Forms.PictureBox();
            this.AKeyLabel = new System.Windows.Forms.Label();
            this.AKeyPictureBox = new System.Windows.Forms.PictureBox();
            this.WKeyLabel = new System.Windows.Forms.Label();
            this.WKeyPictureBox = new System.Windows.Forms.PictureBox();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripSplitButton1 = new System.Windows.Forms.ToolStripSplitButton();
            this.loadWImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadAImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadSImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadDImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ModeGroup = new System.Windows.Forms.GroupBox();
            this.MainMenuStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.IconGroup.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DKeyPictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SKeyPictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.AKeyPictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.WKeyPictureBox)).BeginInit();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // MainMenuStrip
            // 
            this.MainMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.optionsToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.MainMenuStrip.Location = new System.Drawing.Point(0, 0);
            this.MainMenuStrip.Name = "MainMenuStrip";
            this.MainMenuStrip.Size = new System.Drawing.Size(1244, 24);
            this.MainMenuStrip.TabIndex = 0;
            this.MainMenuStrip.Text = "MainMenuStrip";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(103, 22);
            this.newToolStripMenuItem.Text = "&New";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(103, 22);
            this.openToolStripMenuItem.Text = "&Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(103, 22);
            this.saveToolStripMenuItem.Text = "&Save";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(103, 22);
            this.exitToolStripMenuItem.Text = "&Exit";
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(61, 20);
            this.optionsToolStripMenuItem.Text = "&Options";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.viewHelpToolStripMenuItem,
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // viewHelpToolStripMenuItem
            // 
            this.viewHelpToolStripMenuItem.Name = "viewHelpToolStripMenuItem";
            this.viewHelpToolStripMenuItem.Size = new System.Drawing.Size(127, 22);
            this.viewHelpToolStripMenuItem.Text = "&View Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(127, 22);
            this.aboutToolStripMenuItem.Text = "&About";
            // 
            // MainStatusStrip
            // 
            this.MainStatusStrip.Location = new System.Drawing.Point(0, 607);
            this.MainStatusStrip.Name = "MainStatusStrip";
            this.MainStatusStrip.Size = new System.Drawing.Size(1244, 22);
            this.MainStatusStrip.TabIndex = 1;
            this.MainStatusStrip.Text = "MainStatusStrip";
            // 
            // splitContainer1
            // 
            this.splitContainer1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 24);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.IconGroup);
            this.splitContainer1.Panel1.Controls.Add(this.ModeGroup);
            this.splitContainer1.Size = new System.Drawing.Size(1244, 583);
            this.splitContainer1.SplitterDistance = 302;
            this.splitContainer1.TabIndex = 2;
            // 
            // IconGroup
            // 
            this.IconGroup.Controls.Add(this.DKeyLabel);
            this.IconGroup.Controls.Add(this.DKeyPictureBox);
            this.IconGroup.Controls.Add(this.SKeyLabel);
            this.IconGroup.Controls.Add(this.SKeyPictureBox);
            this.IconGroup.Controls.Add(this.AKeyLabel);
            this.IconGroup.Controls.Add(this.AKeyPictureBox);
            this.IconGroup.Controls.Add(this.WKeyLabel);
            this.IconGroup.Controls.Add(this.WKeyPictureBox);
            this.IconGroup.Controls.Add(this.toolStrip1);
            this.IconGroup.Location = new System.Drawing.Point(11, 254);
            this.IconGroup.Name = "IconGroup";
            this.IconGroup.Size = new System.Drawing.Size(284, 322);
            this.IconGroup.TabIndex = 1;
            this.IconGroup.TabStop = false;
            this.IconGroup.Text = "Icon Selection";
            // 
            // DKeyLabel
            // 
            this.DKeyLabel.AutoSize = true;
            this.DKeyLabel.Location = new System.Drawing.Point(195, 118);
            this.DKeyLabel.Name = "DKeyLabel";
            this.DKeyLabel.Size = new System.Drawing.Size(15, 13);
            this.DKeyLabel.TabIndex = 8;
            this.DKeyLabel.Text = "D";
            // 
            // DKeyPictureBox
            // 
            this.DKeyPictureBox.BackgroundImage = global::BeatMaker.Properties.Resources.lighting32;
            this.DKeyPictureBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.DKeyPictureBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.DKeyPictureBox.InitialImage = null;
            this.DKeyPictureBox.Location = new System.Drawing.Point(182, 134);
            this.DKeyPictureBox.Name = "DKeyPictureBox";
            this.DKeyPictureBox.Size = new System.Drawing.Size(40, 40);
            this.DKeyPictureBox.TabIndex = 7;
            this.DKeyPictureBox.TabStop = false;
            // 
            // SKeyLabel
            // 
            this.SKeyLabel.AutoSize = true;
            this.SKeyLabel.Location = new System.Drawing.Point(121, 118);
            this.SKeyLabel.Name = "SKeyLabel";
            this.SKeyLabel.Size = new System.Drawing.Size(14, 13);
            this.SKeyLabel.TabIndex = 6;
            this.SKeyLabel.Text = "S";
            // 
            // SKeyPictureBox
            // 
            this.SKeyPictureBox.BackgroundImage = global::BeatMaker.Properties.Resources.heart32;
            this.SKeyPictureBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.SKeyPictureBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.SKeyPictureBox.InitialImage = null;
            this.SKeyPictureBox.Location = new System.Drawing.Point(110, 134);
            this.SKeyPictureBox.Name = "SKeyPictureBox";
            this.SKeyPictureBox.Size = new System.Drawing.Size(40, 40);
            this.SKeyPictureBox.TabIndex = 5;
            this.SKeyPictureBox.TabStop = false;
            // 
            // AKeyLabel
            // 
            this.AKeyLabel.AutoSize = true;
            this.AKeyLabel.Location = new System.Drawing.Point(54, 118);
            this.AKeyLabel.Name = "AKeyLabel";
            this.AKeyLabel.Size = new System.Drawing.Size(14, 13);
            this.AKeyLabel.TabIndex = 4;
            this.AKeyLabel.Text = "A";
            // 
            // AKeyPictureBox
            // 
            this.AKeyPictureBox.BackgroundImage = global::BeatMaker.Properties.Resources.fire32;
            this.AKeyPictureBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.AKeyPictureBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.AKeyPictureBox.InitialImage = null;
            this.AKeyPictureBox.Location = new System.Drawing.Point(40, 134);
            this.AKeyPictureBox.Name = "AKeyPictureBox";
            this.AKeyPictureBox.Size = new System.Drawing.Size(40, 40);
            this.AKeyPictureBox.TabIndex = 3;
            this.AKeyPictureBox.TabStop = false;
            // 
            // WKeyLabel
            // 
            this.WKeyLabel.AutoSize = true;
            this.WKeyLabel.Location = new System.Drawing.Point(121, 49);
            this.WKeyLabel.Name = "WKeyLabel";
            this.WKeyLabel.Size = new System.Drawing.Size(18, 13);
            this.WKeyLabel.TabIndex = 2;
            this.WKeyLabel.Text = "W";
            // 
            // WKeyPictureBox
            // 
            this.WKeyPictureBox.BackgroundImage = global::BeatMaker.Properties.Resources.airplane32;
            this.WKeyPictureBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.WKeyPictureBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.WKeyPictureBox.InitialImage = null;
            this.WKeyPictureBox.Location = new System.Drawing.Point(110, 65);
            this.WKeyPictureBox.Name = "WKeyPictureBox";
            this.WKeyPictureBox.Size = new System.Drawing.Size(40, 40);
            this.WKeyPictureBox.TabIndex = 1;
            this.WKeyPictureBox.TabStop = false;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripSplitButton1});
            this.toolStrip1.Location = new System.Drawing.Point(3, 16);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(278, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripSplitButton1
            // 
            this.toolStripSplitButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripSplitButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadWImageToolStripMenuItem,
            this.loadAImageToolStripMenuItem,
            this.loadSImageToolStripMenuItem,
            this.loadDImageToolStripMenuItem});
            this.toolStripSplitButton1.Image = global::BeatMaker.Properties.Resources.import24;
            this.toolStripSplitButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSplitButton1.Name = "toolStripSplitButton1";
            this.toolStripSplitButton1.Size = new System.Drawing.Size(32, 22);
            this.toolStripSplitButton1.Text = "toolStripSplitButton1";
            // 
            // loadWImageToolStripMenuItem
            // 
            this.loadWImageToolStripMenuItem.Name = "loadWImageToolStripMenuItem";
            this.loadWImageToolStripMenuItem.Size = new System.Drawing.Size(156, 22);
            this.loadWImageToolStripMenuItem.Text = "Load \'W\' Image";
            // 
            // loadAImageToolStripMenuItem
            // 
            this.loadAImageToolStripMenuItem.Name = "loadAImageToolStripMenuItem";
            this.loadAImageToolStripMenuItem.Size = new System.Drawing.Size(156, 22);
            this.loadAImageToolStripMenuItem.Text = "Load \'A\' Image";
            // 
            // loadSImageToolStripMenuItem
            // 
            this.loadSImageToolStripMenuItem.Name = "loadSImageToolStripMenuItem";
            this.loadSImageToolStripMenuItem.Size = new System.Drawing.Size(156, 22);
            this.loadSImageToolStripMenuItem.Text = "Load \'S\' Image";
            // 
            // loadDImageToolStripMenuItem
            // 
            this.loadDImageToolStripMenuItem.Name = "loadDImageToolStripMenuItem";
            this.loadDImageToolStripMenuItem.Size = new System.Drawing.Size(156, 22);
            this.loadDImageToolStripMenuItem.Text = "Load \'D\' Image";
            // 
            // ModeGroup
            // 
            this.ModeGroup.Location = new System.Drawing.Point(11, 4);
            this.ModeGroup.Name = "ModeGroup";
            this.ModeGroup.Size = new System.Drawing.Size(284, 243);
            this.ModeGroup.TabIndex = 0;
            this.ModeGroup.TabStop = false;
            this.ModeGroup.Text = "Modes";
            // 
            // BeatMaker
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1244, 629);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.MainStatusStrip);
            this.Controls.Add(this.MainMenuStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "BeatMaker";
            this.Text = "BeatMaker";
            this.MainMenuStrip.ResumeLayout(false);
            this.MainMenuStrip.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.IconGroup.ResumeLayout(false);
            this.IconGroup.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DKeyPictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SKeyPictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.AKeyPictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.WKeyPictureBox)).EndInit();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip MainMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewHelpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.StatusStrip MainStatusStrip;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.GroupBox IconGroup;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripSplitButton toolStripSplitButton1;
        private System.Windows.Forms.ToolStripMenuItem loadWImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadAImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadSImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadDImageToolStripMenuItem;
        private System.Windows.Forms.GroupBox ModeGroup;
        private System.Windows.Forms.PictureBox WKeyPictureBox;
        private System.Windows.Forms.Label AKeyLabel;
        private System.Windows.Forms.PictureBox AKeyPictureBox;
        private System.Windows.Forms.Label WKeyLabel;
        private System.Windows.Forms.Label DKeyLabel;
        private System.Windows.Forms.PictureBox DKeyPictureBox;
        private System.Windows.Forms.Label SKeyLabel;
        private System.Windows.Forms.PictureBox SKeyPictureBox;
    }
}

