namespace DeadCode.WME.StringTableMgr
{
	partial class FormAbout
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormAbout));
			this.LblProgram = new System.Windows.Forms.Label();
			this.LblVersion = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.BtnOK = new System.Windows.Forms.Button();
			this.LblLink = new System.Windows.Forms.LinkLabel();
			this.PicLogo = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.PicLogo)).BeginInit();
			this.SuspendLayout();
			// 
			// LblProgram
			// 
			this.LblProgram.AutoSize = true;
			this.LblProgram.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
			this.LblProgram.Location = new System.Drawing.Point(12, 9);
			this.LblProgram.Name = "LblProgram";
			this.LblProgram.Size = new System.Drawing.Size(129, 13);
			this.LblProgram.TabIndex = 1;
			this.LblProgram.Text = "String Table Manager";
			// 
			// LblVersion
			// 
			this.LblVersion.AutoSize = true;
			this.LblVersion.Location = new System.Drawing.Point(12, 43);
			this.LblVersion.Name = "LblVersion";
			this.LblVersion.Size = new System.Drawing.Size(45, 13);
			this.LblVersion.TabIndex = 3;
			this.LblVersion.Text = "Version:";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 26);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(121, 13);
			this.label1.TabIndex = 2;
			this.label1.Text = "Wintermute Engine Tool";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(12, 69);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(195, 13);
			this.label2.TabIndex = 4;
			this.label2.Text = "Copyright (c) Dead:Code Software 2013";
			// 
			// BtnOK
			// 
			this.BtnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOK.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnOK.Location = new System.Drawing.Point(163, 187);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(75, 23);
			this.BtnOK.TabIndex = 0;
			this.BtnOK.Text = "OK";
			this.BtnOK.UseVisualStyleBackColor = true;
			// 
			// LblLink
			// 
			this.LblLink.AutoSize = true;
			this.LblLink.Location = new System.Drawing.Point(12, 89);
			this.LblLink.Name = "LblLink";
			this.LblLink.Size = new System.Drawing.Size(169, 13);
			this.LblLink.TabIndex = 5;
			this.LblLink.TabStop = true;
			this.LblLink.Text = "http://www.wintermute-engine.org";
			this.LblLink.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.OnLinkClicked);
			// 
			// PicLogo
			// 
			this.PicLogo.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.PicLogo.Image = ((System.Drawing.Image)(resources.GetObject("PicLogo.Image")));
			this.PicLogo.Location = new System.Drawing.Point(12, 117);
			this.PicLogo.Name = "PicLogo";
			this.PicLogo.Size = new System.Drawing.Size(100, 93);
			this.PicLogo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.PicLogo.TabIndex = 6;
			this.PicLogo.TabStop = false;
			// 
			// FormAbout
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.BtnOK;
			this.ClientSize = new System.Drawing.Size(250, 222);
			this.Controls.Add(this.PicLogo);
			this.Controls.Add(this.LblLink);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.LblVersion);
			this.Controls.Add(this.LblProgram);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormAbout";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "About";
			this.Load += new System.EventHandler(this.OnLoad);
			((System.ComponentModel.ISupportInitialize)(this.PicLogo)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label LblProgram;
		private System.Windows.Forms.Label LblVersion;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.LinkLabel LblLink;
		private System.Windows.Forms.PictureBox PicLogo;
	}
}