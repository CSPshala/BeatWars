namespace BeatMaker
{
    partial class SetTitle
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
            this.SongTitleLabel = new System.Windows.Forms.Label();
            this.SongTitleTextBox = new System.Windows.Forms.TextBox();
            this.OKButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // SongTitleLabel
            // 
            this.SongTitleLabel.AutoSize = true;
            this.SongTitleLabel.Location = new System.Drawing.Point(12, 9);
            this.SongTitleLabel.Name = "SongTitleLabel";
            this.SongTitleLabel.Size = new System.Drawing.Size(58, 13);
            this.SongTitleLabel.TabIndex = 0;
            this.SongTitleLabel.Text = "Song Title:";
            // 
            // SongTitleTextBox
            // 
            this.SongTitleTextBox.Location = new System.Drawing.Point(77, 9);
            this.SongTitleTextBox.Name = "SongTitleTextBox";
            this.SongTitleTextBox.Size = new System.Drawing.Size(260, 20);
            this.SongTitleTextBox.TabIndex = 1;
            // 
            // OKButton
            // 
            this.OKButton.Location = new System.Drawing.Point(258, 35);
            this.OKButton.Name = "OKButton";
            this.OKButton.Size = new System.Drawing.Size(78, 33);
            this.OKButton.TabIndex = 2;
            this.OKButton.Text = "OK";
            this.OKButton.UseVisualStyleBackColor = true;
            this.OKButton.Click += new System.EventHandler(this.OKButton_Click);
            // 
            // SetTitle
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(357, 80);
            this.Controls.Add(this.OKButton);
            this.Controls.Add(this.SongTitleTextBox);
            this.Controls.Add(this.SongTitleLabel);
            this.Name = "SetTitle";
            this.Text = "Set Song Title";
            this.Shown += new System.EventHandler(this.SetTitle_Shown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label SongTitleLabel;
        private System.Windows.Forms.TextBox SongTitleTextBox;
        private System.Windows.Forms.Button OKButton;
    }
}