namespace DeadCode.WME.SpriteOptimizer
{
	partial class FormMain
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
			this.label1 = new System.Windows.Forms.Label();
			this.TbProject = new System.Windows.Forms.TextBox();
			this.BtnProcess = new System.Windows.Forms.Button();
			this.TbLog = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 15);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(43, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Project:";
			// 
			// TbProject
			// 
			this.TbProject.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TbProject.Location = new System.Drawing.Point(61, 12);
			this.TbProject.Name = "TbProject";
			this.TbProject.Size = new System.Drawing.Size(380, 20);
			this.TbProject.TabIndex = 1;
			this.TbProject.TextChanged += new System.EventHandler(this.OnProjectChanged);
			// 
			// BtnProcess
			// 
			this.BtnProcess.Location = new System.Drawing.Point(61, 38);
			this.BtnProcess.Name = "BtnProcess";
			this.BtnProcess.Size = new System.Drawing.Size(132, 23);
			this.BtnProcess.TabIndex = 2;
			this.BtnProcess.Text = "Process...";
			this.BtnProcess.UseVisualStyleBackColor = true;
			this.BtnProcess.Click += new System.EventHandler(this.OnProcess);
			// 
			// TbLog
			// 
			this.TbLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TbLog.Location = new System.Drawing.Point(12, 67);
			this.TbLog.Multiline = true;
			this.TbLog.Name = "TbLog";
			this.TbLog.ReadOnly = true;
			this.TbLog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.TbLog.Size = new System.Drawing.Size(429, 319);
			this.TbLog.TabIndex = 3;
			// 
			// FormMain
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(453, 398);
			this.Controls.Add(this.TbLog);
			this.Controls.Add(this.BtnProcess);
			this.Controls.Add(this.TbProject);
			this.Controls.Add(this.label1);
			this.Name = "FormMain";
			this.Text = "WME Sprite Optimizer";
			this.Load += new System.EventHandler(this.OnLoad);
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnFormClosed);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox TbProject;
		private System.Windows.Forms.Button BtnProcess;
		private System.Windows.Forms.TextBox TbLog;
	}
}

