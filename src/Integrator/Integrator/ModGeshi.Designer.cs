namespace Integrator
{
	partial class ModGeshi
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.label1 = new System.Windows.Forms.Label();
			this.linkLabel1 = new System.Windows.Forms.LinkLabel();
			this.BtnBrowseDir = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.TxtOutputDir = new System.Windows.Forms.TextBox();
			this.BtnGenerate = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(3, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(350, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "GeSHi is a generic syntax highlighter for PHP. More info can be found at:";
			// 
			// linkLabel1
			// 
			this.linkLabel1.AutoSize = true;
			this.linkLabel1.Location = new System.Drawing.Point(359, 9);
			this.linkLabel1.Name = "linkLabel1";
			this.linkLabel1.Size = new System.Drawing.Size(77, 13);
			this.linkLabel1.TabIndex = 1;
			this.linkLabel1.TabStop = true;
			this.linkLabel1.Text = "www.geshi.org";
			this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.OnDownloadLink);
			// 
			// BtnBrowseDir
			// 
			this.BtnBrowseDir.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnBrowseDir.Location = new System.Drawing.Point(430, 55);
			this.BtnBrowseDir.Name = "BtnBrowseDir";
			this.BtnBrowseDir.Size = new System.Drawing.Size(25, 23);
			this.BtnBrowseDir.TabIndex = 28;
			this.BtnBrowseDir.Text = "...";
			this.BtnBrowseDir.UseVisualStyleBackColor = true;
			this.BtnBrowseDir.Click += new System.EventHandler(this.OnBrowseDir);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(3, 41);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(85, 13);
			this.label2.TabIndex = 26;
			this.label2.Text = "Output directory:";
			// 
			// TxtOutputDir
			// 
			this.TxtOutputDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TxtOutputDir.Location = new System.Drawing.Point(6, 57);
			this.TxtOutputDir.Name = "TxtOutputDir";
			this.TxtOutputDir.Size = new System.Drawing.Size(418, 20);
			this.TxtOutputDir.TabIndex = 27;
			// 
			// BtnGenerate
			// 
			this.BtnGenerate.Location = new System.Drawing.Point(6, 83);
			this.BtnGenerate.Name = "BtnGenerate";
			this.BtnGenerate.Size = new System.Drawing.Size(207, 23);
			this.BtnGenerate.TabIndex = 29;
			this.BtnGenerate.Text = "Generate GeSHi syntax file";
			this.BtnGenerate.UseVisualStyleBackColor = true;
			this.BtnGenerate.Click += new System.EventHandler(this.OnGenerate);
			// 
			// ModGeshi
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.BtnGenerate);
			this.Controls.Add(this.BtnBrowseDir);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.TxtOutputDir);
			this.Controls.Add(this.linkLabel1);
			this.Controls.Add(this.label1);
			this.Name = "ModGeshi";
			this.Size = new System.Drawing.Size(458, 305);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.LinkLabel linkLabel1;
		private System.Windows.Forms.Button BtnBrowseDir;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox TxtOutputDir;
		private System.Windows.Forms.Button BtnGenerate;
	}
}
